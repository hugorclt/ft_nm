#include "nm.h"

bool    open_file(char *filename, t_file_handler *file_handler) {
    file_handler->fd = open(filename, 0);
    if (file_handler->fd < 0) 
        return (false);
    return (true);
}

int main(int ac, char **av) {
    if (ac != 2)
        return (EXIT_FAILURE);
    
    t_file_handler  file_handler;

    if (!open_file(av[1], &file_handler))
        return (EXIT_FAILURE);

    struct stat sb;

    if (fstat(file_handler.fd, &sb) == -1) {
        return (EXIT_FAILURE);
    }

    
    char    *data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, file_handler.fd, 0);
    Elf64_Ehdr *elf = (Elf64_Ehdr *)data;
    Elf64_Shdr *sections = (Elf64_Shdr *)((char *)data + elf->e_shoff);
    for (int i = 0; i < elf->e_shnum; i++) {
        if (sections[i].sh_type == SHT_SYMTAB) {
            Elf64_Sym *symtab = (Elf64_Sym *)((char *)data + sections[i].sh_offset);
            for (int j = 0; j < 10; j++) {
                printf("%c\n", symtab[j].st_other);
            }
            break; 
        }
    }

}
