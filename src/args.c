# include "defines.h"
# include "utils.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <arpa/inet.h>		/* inet_pton */
# include <sys/types.h>		/* getaddrinfo */
# include <sys/socket.h>	/* getaddrinfo */
# include <netdb.h>			/* getaddrinfo */

void	exit_usage(void);
void	host_resval(const char *host);
void	hostname_resolution(const char *hostname);

extern t_traceroute	g_data;

void	arg_parser(int argc, char **argv)
{
	if (argc < 2)
		exit_usage();

	for (int i = 0; i < argc; ++i)
	{
		if (ft_strcmp(argv[i], "--help") == 0)
			exit_usage();
	}

	host_resval(argv[1]);
}

void	exit_usage(void)
{
	print_usage();
	exit(EXIT_SUCCESS);
}

void	host_resval(const char *host)
{
	struct sockaddr_in	sa;
	int					res, len;

	res = inet_pton(AF_INET, host, &(sa.sin_addr));
	if (res == 1)
		ft_memcpy(&g_data.host_sa, &sa, sizeof(struct sockaddr_in));
	else
		hostname_resolution(host);

	len = ft_strlen(host);
	g_data.hostname =  (char *)malloc((len + 1) * sizeof(char));
	if (g_data.hostname == NULL)
		exit_failure("error on allocation memory");

	ft_strncpy(g_data.hostname, host, len);
	g_data.hostname[len] = '\0';
}

void	hostname_resolution(const char *hostname)
{
	struct addrinfo	hints, *result;
	int				errcode;

	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	errcode = getaddrinfo(hostname, NULL, &hints, &result);
	if (errcode != 0)
		exit_failure("ft_traceroute: unknown host");

	ft_memcpy(&g_data.host_sa, result->ai_addr, sizeof(struct sockaddr_in));

	freeaddrinfo(result);
}
