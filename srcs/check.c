#include "nm.h"

int check_magic_number(char *data)
{
	if (data[0] != 0x7f)
		return (-1);
	else if (data[1] != 'E')
		return (-1);
	else if (data[2] != 'L')
		return (-1);
	else if (data[3] != 'F')
		return (-1);
	return (0);
}

int check_architecture(char *data)
{
	int arch_type = data[4];

	if (arch_type == 0) {
		printf("Error: architecture not recognized\n");
		return (-1);
	}
	else if (arch_type == 1)
		return (BITS_32);
	return (BITS_64);
}
