# include "defines.h"
# include "utils.h"
# include <sys/socket.h> /* socket() */

# define MAXTTL		30
# define STARTPORT	33434

void icmp_sock_create(int *sock);
void udp_sock_create(int *sock);

extern t_traceroute g_data;

void	traceroute_lifecycle(void)
{
	g_data.udp_port = STARTPORT;
	g_data.curttl = 1;

	icmp_sock_create(&g_data.icmp_sockfd);
	udp_sock_create(&g_data.udp_sockfd);

	g_data.host_sa.sin_family = AF_INET;
	g_data.host_sa.sin_port = htons(g_data.udp_port++);
}

/* create icmp sock for receive.
	ICMP Time Exceeded. or ICMP Port Unreachable.*/
void icmp_sock_create(int *sock)
{
	if ((*sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
    {
        perror("socket");
		exit_failure(NULL);
    }
}

/* create upd sock for sendto() */
void udp_sock_create(int *sock)
{
	if ((*sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        perror("socket");
		exit_failure(NULL);
    }
}
