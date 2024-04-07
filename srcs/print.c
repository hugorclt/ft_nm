#include "nm.h"

void    print_structure(t_sym_data *data, size_t len) {
    for (size_t i = 0; i < len; i++)
        printf("%016lx %c %s\n", data[i].value, data[i].symbol, data[i].name);
}
