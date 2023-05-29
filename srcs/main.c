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

    
    char    *file = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, file_handler.fd, 0);
    Elf32_Ehdr *header = (Elf32_Ehdr *)file;
    Elf32_Shdr *sections = (Elf32_Shdr *)((char *)file + header->e_shoff);
    for (int i = 0; i < header->e_shnum; i++) {
        if (sections[i].sh_type == SHT_SYMTAB) {
            Elf32_Sym *symtab = (Elf32_Sym *)((char *)file + sections[i].sh_offset);
            printf("%d\n", symtab->st_name);
            break; 
        }
    }


}