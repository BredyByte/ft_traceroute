#ifndef UTILS_H
# define UTILS_H
# include <stdio.h>

/* program termination */
void	exit_failure(char *str);

/* std lib functions */
int		ft_strcmp(const char *s1, const char *s2);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*ft_strncpy(char *dest, const char *src, size_t n);
size_t	ft_strlen(const char *str);

/* output functions */
void	print_usage(void);

#endif
