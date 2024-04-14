#include "libft.h"

char	*ft_utoa_base(unsigned long num, char *base, unsigned int base_size)
{
	char	buffer[64];
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (1)
	{
		buffer[i++] = base[num % base_size];
		if (num / base_size == 0)
			break ;
		num = num / base_size;
	}
	result = (char *)malloc(sizeof(char) * (i + 1));
	while (i > 0)
		result[j++] = buffer[--i];
	result[j] = '\0';
	return (result);
}
