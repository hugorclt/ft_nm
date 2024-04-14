#pragma once

#include <unistd.h>
#include <stdlib.h>

char	*ft_strdup(const char *s1);
int	ft_strlen(const char *s);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_utoa_base(unsigned long num, char *base, unsigned int base_size);
