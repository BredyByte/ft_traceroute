# include "defines.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

extern t_traceroute	g_data;

size_t	ft_strlen(const char *str)
{
	const char	*s;

	for (s = str; *s; ++s)
		;

	return (s - str);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	size_t			i;

	d = (unsigned char *) dest;
	s = (unsigned char *) src;
	i = 0;

	if (!n || (!dest && !src))
		return dest;

	for (; i < n; ++i)
		d[i] = s[i];

	return dest;
}

void	*ft_memset(void *s, int c, size_t n)
{
	char	*p = s;
	size_t	i = 0;

	for (; i < n; ++i)
		p[i] = c;

	return s;
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t i = 0;

	for (; i < n; ++i)
		dest[i] = src[i];

	return dest;
}

void	exit_failure(char *str)
{
	if (!str)
		fprintf(stderr, "Fatal error\n");
	else
		fprintf(stderr, "%s\n", str);

	if (g_data.hostname != NULL)
		free(g_data.hostname);

	if (g_data.udp_sockfd)
		close(g_data.udp_sockfd);

	if (g_data.icmp_sockfd)
		close(g_data.icmp_sockfd);

	exit(EXIT_FAILURE);
}

void	print_usage(void)
{
	printf("Usage: \n");
	printf("\tft_tracerote:\thost (IPv4 10.3.1.14 or FQDM example.com)\n");
	printf("Arguments: \n");
	printf("+\thost\t\tThe host to traceroute to.\n");
	printf("\t\t\tIf --help is not used, it is assumed\n");
	printf("\t\t\tthat host is always the first argument passed.\n");
}

void	print_gdata(void)
{
	printf("g_data struct params:\n");
	printf("\thost_sa:\t%s\n", inet_ntoa(g_data.host_sa.sin_addr));
	printf("\thostname:\t%s\n", (g_data.hostname != NULL) ? g_data.hostname : "null");
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i = 0;

	while (s1[i] == s2[i])
	{
		if (s1[i] == '\0' || s2[i] == '\0')
			break;

		++i;
	}

	if (s1[i] == '\0' && s2[i] == '\0')
		return 0;

	return (s1[i] - s2[i]);
}

void	g_setdefs(void)
{
	g_data.hostname = NULL;
	ft_memset(&g_data.host_sa, 0, sizeof(struct sockaddr_in));
	g_data.udp_sockfd = -1;
	g_data.icmp_sockfd = -1;
	g_data.udp_port = 0;
	g_data.curttl = 0;
}
