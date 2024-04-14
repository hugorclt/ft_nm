#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*result;
	int		size;

	size = ft_strlen(s1);
	result = malloc(sizeof(char) * size + 1);
	if (result == NULL)
		return (NULL);
	result[size] = '\0';
	while (size--)
		result[size] = s1[size];
	return (result);
}
