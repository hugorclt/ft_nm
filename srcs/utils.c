#include "nm.h"

int open_file(char *filename)
{
	int	fd = open(filename, O_RDONLY);
    if (fd < 0)
	{
		printf("Error: file opening failed\n");
        return (-1);
	}

	return (fd);
}

int	get_file_size(int fd)
{
    struct stat sb;
    if (fstat(fd, &sb) == -1)
	{
		printf("Error: can't get file size\n");
        return (-1);
	}

	return (sb.st_size);
}
