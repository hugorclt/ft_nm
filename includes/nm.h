#pragma once

#include "libft.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stab.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <elf.h>
#include <string.h>

typedef struct s_sym_data {
    long unsigned int    value;
    char        symbol;
    char        *name;
}   t_sym_data;

# define BITS_64 1
# define BITS_32 0

int	get_file_size(int fd);
int open_file(char *filename);
void    print_structure(t_sym_data *data, size_t len);
int check_magic_number(char *data);
int check_architecture(char *data);
char	find_symbols_type64(Elf64_Sym symtab, Elf64_Shdr *sections);
char	find_symbols_type32(Elf32_Sym symtab, Elf32_Shdr *sections);
void	find_symbols64(char *data);
void	find_symbols32(char *data);
void    sort_data(t_sym_data *data, size_t len);
void	free_data(t_sym_data *data, int len);
