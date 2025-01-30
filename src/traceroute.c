# include "defines.h"
# include "utils.h"
# include <sys/socket.h> /* socket() */

void icmp_sock_create(int *sock);
void udp_sock_create(int *sock);

extern t_traceroute g_data;

void	traceroute_lifecycle(void)
{
	icmp_sock_create(&g_data.icmp_sockfd);
	udp_sock_create(&g_data.udp_sockfd);

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
