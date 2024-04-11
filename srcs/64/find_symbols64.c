#include "nm.h"

char	find_symbols_type64(Elf64_Sym symtab, Elf64_Shdr *sections)
{
	char c;

	if (ELF64_ST_BIND(symtab.st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(symtab.st_info) == STT_GNU_IFUNC)
		c = 'i';
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
	else if (sections[symtab.st_shndx].sh_type == SHT_NOBITS
			&& sections[symtab.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE | SHF_IA_64_SHORT))
		c = 'S';
	else if (sections[symtab.st_shndx].sh_type == SHT_PROGBITS
			&& sections[symtab.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE | SHF_IA_64_SHORT))
		c = 'G';
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

void	find_symbols64(char *data)
{
	t_sym_data      *sym_data = NULL; 
	Elf64_Ehdr	*elf = (Elf64_Ehdr *)data;
	Elf64_Shdr	*sections = (Elf64_Shdr *)((char *)data + elf->e_shoff);
	Elf64_Shdr	*string_table_header = &sections[elf->e_shstrndx];
	char		*string_table = (char *)(data + string_table_header->sh_offset);

	int len = 0;
	for (int i = 0; i < elf->e_shnum; i++) {
		if (sections[i].sh_type == SHT_SYMTAB) {
			Elf64_Sym *symtab = (Elf64_Sym *)((char *)data + sections[i].sh_offset);
			int symbol_num = sections[i].sh_size / sections[i].sh_entsize;
			char *symbol_names = (char *)(data + sections[sections[i].sh_link].sh_offset);

			sym_data = malloc(sizeof(t_sym_data) * symbol_num);
			if (!sym_data)
				return ;
			for (int j=0; j<symbol_num; j++) {
				if (symtab[j].st_name) {
					if (symtab[j].st_shndx == 65521)
						continue ;
					sym_data[len].value = symtab[j].st_value;
					if (strncmp(".debug", string_table + sections[symtab[j].st_shndx].sh_name, 6) == 0 || strncmp(".group", string_table + sections[symtab[j].st_shndx].sh_name, 6) == 0)
						sym_data[len].symbol = 'N';
					else
						sym_data[len].symbol = find_symbols_type64(symtab[j], sections);
					sym_data[len].name = strdup(symbol_names + symtab[j].st_name);
					len++;
				}
			}
		}
	}
	sort_data(sym_data, len);
	print_structure(sym_data, len);
	free_data(sym_data, len);
}
