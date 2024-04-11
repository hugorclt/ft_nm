#include "nm.h"

int open_file(char *filename)
{
	int	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		return (-1);
	}

	return (fd);
}

int	get_file_size(int fd)
{
	struct stat sb;
	if (fstat(fd, &sb) == -1)
	{
		return (-1);
	}

	return (sb.st_size);
}

void	free_data(t_sym_data *data, int len)
{
	for (int i = 0; i < len; i++)
		free(data[i].name);
	free(data);
}
