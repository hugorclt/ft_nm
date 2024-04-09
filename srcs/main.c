#include "nm.h"

void    swap_data(t_sym_data *data, size_t idx1, size_t idx2) {
    t_sym_data  *temp = malloc(sizeof(t_sym_data) * 1000);
    temp->name =malloc(sizeof(char) * 1000);

    memcpy(temp, data + idx1, sizeof(data[idx1]));
    memcpy(data + idx1, data + idx2,sizeof(data[idx1]));
    memcpy(data + idx2, temp,sizeof(data[idx1]));
}

void    sort_data(t_sym_data *data, size_t len) {
    for (size_t i =0; i < len; i++) {
        for (size_t j = 0; j < len - 1 - i; j++) {
            if (strcmp(data[j].name, data[j + 1].name) > 0) {
                swap_data(data, j, j + 1);
            }
        }
    }
}

// type to do: 'G' 'g' 'i' 'N' 'p' 'S' 's' '-'
char	find_symbols(Elf64_Sym symtab, Elf64_Shdr *sections)
{
	char c;

	if (ELF64_ST_BIND(symtab.st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(symtab.st_info) == STB_WEAK)
	{
		c = 'W';
		if (symtab.st_shndx == SHN_UNDEF)
			c = 'w';
	}
	else if (ELF64_ST_BIND(symtab.st_info) == STB_WEAK && ELF64_ST_TYPE(symtab.st_info) == STT_OBJECT)
	{
		c = 'V';
		if (symtab.st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (symtab.st_shndx == SHN_UNDEF)
		c = 'U';
	else if (symtab.st_shndx == SHN_ABS)
		c = 'A';
	else if (symtab.st_shndx == SHN_COMMON)
		c = 'C';
	else if (sections[symtab.st_shndx].sh_type == SHT_PROGBITS
			&& sections[symtab.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'D';
	else if (sections[symtab.st_shndx].sh_type == SHT_NOBITS
			&& sections[symtab.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'B';
	else if (sections[symtab.st_shndx].sh_type == SHT_PROGBITS
			&& sections[symtab.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';
	else if (sections[symtab.st_shndx].sh_type == SHT_PROGBITS
			&& sections[symtab.st_shndx].sh_flags == (SHF_ALLOC))
		c = 'R';
	else
	{
		c = '?';
	}
	
	if (c != '?' && ELF64_ST_BIND(symtab.st_info) == STB_LOCAL) // local symbol are in lowercase
		c += 32;

	return (c);

}

int main(int ac, char **av) {
    if (ac != 2)
	{
		printf("Error: usage %s {file_to_load}\n", av[0]);
		return (EXIT_FAILURE);
	}
	int fd = open_file(av[1]);
	if (fd == -1)
		return (EXIT_FAILURE);

	int file_size = get_file_size(fd);
	if (file_size == -1)
		return (EXIT_FAILURE);

	char    *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (data == MAP_FAILED)
		return (EXIT_FAILURE);

	if (check_magic_number(data) == -1)
	{
		printf("Error: magic number error, not an elf file\n");
		munmap(data, file_size);
		return (EXIT_FAILURE);
	}

	int architecture_type = check_architecture(data);
	if (architecture_type == -1)
	{
		munmap(data, file_size);
		return (EXIT_FAILURE);
	}

	t_sym_data      *sym_data = NULL; 
    Elf64_Ehdr *elf = (Elf64_Ehdr *)data;
    Elf64_Shdr *sections = (Elf64_Shdr *)((char *)data + elf->e_shoff);


    int len = 0;
    for (int i = 0; i < elf->e_shnum; i++) {
        if (sections[i].sh_type == SHT_SYMTAB) {
            Elf64_Sym *symtab = (Elf64_Sym *)((char *)data + sections[i].sh_offset);
            int symbol_num = sections[i].sh_size / sections[i].sh_entsize;
            char *symbol_names = (char *)(data + sections[sections[i].sh_link].sh_offset);

		 	sym_data = malloc(sizeof(t_sym_data) * symbol_num);
			for (int j=0; j<symbol_num; j++) {
                if (symtab[j].st_name) {
                    sym_data[len].value = symtab[j].st_value;
                    sym_data[len].symbol = find_symbols(symtab[j], sections);
                    sym_data[len].name = strdup(symbol_names + symtab[j].st_name);
                    len++;
                }
            }
        }
    }

    sort_data(sym_data, len);
    print_structure(sym_data, len);
}
