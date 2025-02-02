# include "defines.h"
# include "utils.h"
# include <sys/socket.h>		/* socket(), setsockopt() */
# include <sys/select.h>		/* select() */
# include <netinet/ip_icmp.h>	/* struct icmphdr */
# include <sys/time.h>

# define MAXTTL			30
# define STARTPORT		33434
# define PACKET_SIZE	32
# define PROBES			3
# define RECVBUF_SIZE	512

extern t_traceroute g_data;

/* function prototypes */
void	icmp_sock_create(int *sock);
void	udp_sock_create(int *sock);
void	prep_udp_request(int ttl);
void	send_udp_request(struct timeval	send_times[PROBES]);
int		recv_icmp_responses(struct timeval	send_times[PROBES]);

void	traceroute_lifecycle(void)
{
	struct timeval	send_times[PROBES];
	ft_memset(send_times, 0, PROBES);

	g_data.udp_port = STARTPORT;
	g_data.curttl = 1;

	icmp_sock_create(&g_data.icmp_sockfd);
	udp_sock_create(&g_data.udp_sockfd);

	while (g_data.curttl <= MAXTTL)
	{
		prep_udp_request(g_data.curttl);
		send_udp_request(send_times);

		if (recv_icmp_responses(send_times))
			break ;

		g_data.udp_port++;
		g_data.curttl++;
	}
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
void	udp_sock_create(int *sock)
{
	if ((*sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        perror("socket");
		exit_failure(NULL);
    }
}

/* set ttl and prep UPD-request */
void	prep_udp_request(int ttl)
{
	if (setsockopt(g_data.udp_sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
	{
		perror("setsockopt (TTL)");
		exit_failure(NULL);
	}

	g_data.host_sa.sin_family = AF_INET;
	g_data.host_sa.sin_port = htons(g_data.udp_port);
}

void	send_udp_request(struct timeval	send_times[PROBES])
{
	char	packet[PACKET_SIZE];

	ft_memset(packet, 0x42, PACKET_SIZE);

	for (int i = 0; i < PROBES; ++i)
	{
		if (gettimeofday(&send_times[i], NULL) < 0)
        {
            perror("gettimeofday failed");
            exit_failure(NULL);
        }


		if (sendto(g_data.udp_sockfd, packet, PACKET_SIZE, 0,
			(struct sockaddr*)&g_data.host_sa, sizeof(g_data.host_sa)) < 0)
		{
			perror("sendto failed");
			exit_failure(NULL);
		}

		printf("Sent UDP packet to %s:%d\n",
			inet_ntoa(g_data.host_sa.sin_addr), ntohs(g_data.host_sa.sin_port));
	}
}

int	recv_icmp_responses(struct timeval	send_times[PROBES])
{
	fd_set				readfds;
	struct timeval		timeout;
	char				buffer[RECVBUF_SIZE];
	struct sockaddr_in	from;
	socklen_t			fromlen = sizeof(from);
	struct timeval		recv_time;
	double 				rtt;
	struct icmphdr		*icmp_hdr;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	FD_ZERO(&readfds);
	FD_SET(g_data.icmp_sockfd, &readfds);

	if (select(g_data.icmp_sockfd + 1, &readfds, NULL, NULL, &timeout) == 0)
	{
		printf(" * * * (timeout)\n");
		return 0;
	}

	for (int i = 0; i < PROBES; ++i)
	{
		if (recvfrom(g_data.icmp_sockfd, buffer, sizeof(buffer), 0,
				(struct sockaddr *)&from, &fromlen) < 0)
		{
			perror("recvfrom failed");
			continue ;
		}

		if (gettimeofday(&recv_time, NULL) < 0)
        {
            perror("gettimeofday failed");
            continue;
        }

		rtt = (recv_time.tv_sec - send_times[i].tv_sec) * 1000.0;
		rtt += (recv_time.tv_usec - send_times[i].tv_usec) / 1000.0;

		printf("Reply from %s: RTT=%.3f ms\n", inet_ntoa(from.sin_addr), rtt);

		icmp_hdr = (struct icmphdr *)(buffer + 20);
        if (icmp_hdr->type == ICMP_DEST_UNREACH && icmp_hdr->code == ICMP_PORT_UNREACH)
        {
            printf("Destination reached!\n");
            return 1;
        }
	}

	return 0;
}
