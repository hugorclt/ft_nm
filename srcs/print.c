#include "nm.h"

void    print_structure(t_sym_data *data, size_t len) {
	for (size_t i = 0; i < len; i++)
	{
		char	*value = ft_utoa_base(data[i].value, "0123456789abcdef", 16);
		for (int j = 0; j < 16 - ft_strlen(value); j++)
		{
			write(1, "0", 1);
		}
		write(1, value, ft_strlen(value));
		write(1, " ", 1);
		write(1, &data[i].symbol, 1);
		write(1, " ", 1);
		write(1, data[i].name, ft_strlen(data[i].name));
		write(1, "\n", 1);
		free(value);
	}
}
