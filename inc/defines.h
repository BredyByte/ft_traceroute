#ifndef DEFINES_H
# define DEFINES_H

# include <arpa/inet.h>

typedef struct s_traceroute
{
	char				*hostname;
	struct sockaddr_in	host_sa;
	int					udp_sockfd;		/* send socket fd */
	int					icmp_sockfd;	/* recv socket fd */
	in_port_t			udp_port;
	uint8_t				curttl;
}	t_traceroute;


#endif
