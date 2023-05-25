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

    if (open_file(av[1], &file_handler))
        return (EXIT_FAILURE);
    
    
}