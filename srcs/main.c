#include "nm.h"

typedef struct s_sym_data {
    long unsigned int    value;
    char        symbol;
    char        *name;
}   t_sym_data;

bool    open_file(char *filename, t_file_handler *file_handler) {
    file_handler->fd = open(filename, 0);
    if (file_handler->fd < 0) 
        return (false);
    return (true);
}

char            find_symbol(Elf64_Sym sym, Elf64_Shdr *shdr)
{
  char  c;

  if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
    c = 'u';
  else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK)
    {
      c = 'W';
      if (sym.st_shndx == SHN_UNDEF)
        c = 'w';
    }
  else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK && ELF64_ST_TYPE(sym.st_info) == STT_OBJECT)
    {
      c = 'V';
      if (sym.st_shndx == SHN_UNDEF)
        c = 'v';
    }
  else if (sym.st_shndx == SHN_UNDEF)
    c = 'U';
  else if (sym.st_shndx == SHN_ABS)
    c = 'A';
  else if (sym.st_shndx == SHN_COMMON)
    c = 'C';
  else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS
       && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
    c = 'B';
  else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
       && shdr[sym.st_shndx].sh_flags == SHF_ALLOC)
    c = 'R';
  else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
       && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
    c = 'D';
  else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
       && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
    c = 'T';
  else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
    c = 'D';
  else
    c = 'n';
  if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != 'n')
    c += 32;
  return c;
}

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

void    print_structure(t_sym_data *data, size_t len) {
    for (size_t i = 0; i < len; i++)
        printf("%016lx %c %s\n", data[i].value, data[i].symbol, data[i].name);
}

int main(int ac, char **av) {
    if (ac != 2)
        return (EXIT_FAILURE);
    
    t_file_handler  file_handler;
    t_sym_data      *sym_data = NULL;

    if (!open_file(av[1], &file_handler))
        return (EXIT_FAILURE);

    struct stat sb;

    if (fstat(file_handler.fd, &sb) == -1) {
        return (EXIT_FAILURE);
    }

    
    char    *data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, file_handler.fd, 0);
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
                    sym_data[len].symbol = find_symbol(symtab[j], sections);
                    sym_data[len].name = strdup(symbol_names + symtab[j].st_name);
                    len++;
                }
            }
        }
    }

    sort_data(sym_data, len);
    print_structure(sym_data, len);
}
