#include "nm.h"

void    swap_data(t_sym_data *data, size_t idx1, size_t idx2) {
    t_sym_data  *temp = malloc(sizeof(t_sym_data));
    temp->name =malloc(sizeof(char) * strlen(data[idx1].name));

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

void	process_file(char *filename)
{
	int fd = open_file(filename != NULL ? filename : "a.out");
	if (fd == -1)
	{
		printf("Error: %s failed to open file\n", filename);
		return ; 
	}

	int file_size = get_file_size(fd);
	if (file_size == -1)
	{
		printf("Error: %s unable to retrieve file size\n", filename);
		close(fd);
		return ;
	}

	char    *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (data == MAP_FAILED)
	{
		printf("Error: %s memory mapping failed\n", filename);
		close(fd);
		return ;
	}

	if (check_magic_number(data) == -1)
	{
		printf("Error: %s magic number error, not an elf file\n", filename);
		munmap(data, file_size);
		close(fd);
		return ;
	}

	int architecture_type = check_architecture(data);
	if (architecture_type == -1)
	{
		printf("Error: %s architecture not recognized", filename);
		munmap(data, file_size);
		close(fd);
		return ;
	}
	
	if (architecture_type == BITS_64)
		find_symbols64(data);
	else
		find_symbols32(data);
	

	munmap(data, file_size);
	close(fd);
}

int main(int ac, char **av) {
	if (ac == 1)
	{
		process_file("a.out");
		return (EXIT_SUCCESS);
	}
	for (int i = 1; i < ac; i++)
	{
		printf("%s:\n", av[i]);
		process_file(av[i]);
	}
	return (EXIT_SUCCESS);
}

