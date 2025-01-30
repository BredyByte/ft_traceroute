#ifndef UTILS_H
# define UTILS_H
# include <stdio.h>

void	exit_failure(char *str);
void	print_usage(void);
int		ft_strcmp(const char *s1, const char *s2);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);

#endif
