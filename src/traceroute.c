# include "defines.h"
# include "utils.h"
# include <sys/socket.h>			/* socket(), setsockopt() */
# include <sys/select.h>			/* select() */
# include <netinet/ip_icmp.h>		/* struct icmphdr */
# include <netinet/ip.h>			/* struct iphdr */
# include <sys/time.h>
# include <stdbool.h>
# include <unistd.h>

# define C_MAXTTL		30
# define STARTPORT		33434
# define PACKET_SIZE	32
# define PROBES			3
# define RECVBUF_SIZE	512
# define TIMEOUT_MICSEC 150000		/* 1 sec - 1000000 microsec */
# define RUNNING		1
# define STOPPED		0

extern t_traceroute g_data;

bool is_running;

/* function prototypes */
void	icmp_sock_create(int *sock);
void	udp_sock_create(int *sock);
void	prep_udp_request(int ttl);
void	send_udp_request(struct timeval	send_times[PROBES]);
void	recv_icmp_responses(struct timeval	send_times[PROBES]);
void	print_rtt(double rtt);
void	print_ttl(int ttl);
void	flush_icmp_socket(void);

void	traceroute_lifecycle(void)
{
	extern bool is_running;
	struct timeval	send_times[PROBES];

	ft_memset(send_times, 0, PROBES);

	is_running = RUNNING;
	g_data.udp_port = STARTPORT;
	g_data.curttl = 1;

	icmp_sock_create(&g_data.icmp_sockfd);
	udp_sock_create(&g_data.udp_sockfd);

	printf("traceroute to %s (%s), 30 hops max, 60 bytes packets\n",
		g_data.hostname, inet_ntoa(g_data.host_sa.sin_addr));

	while (g_data.curttl <= C_MAXTTL && is_running == RUNNING)
	{
		prep_udp_request(g_data.curttl);
		send_udp_request(send_times);
		recv_icmp_responses(send_times);
		flush_icmp_socket();

		g_data.udp_port++;
		g_data.curttl++;
	}

	if (g_data.icmp_sockfd)
		close(g_data.icmp_sockfd);
	if (g_data.udp_sockfd)
		close(g_data.udp_sockfd);
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
	}
}

void	recv_icmp_responses(struct timeval send_times[PROBES])
{
	extern bool			is_running;
	fd_set				readfds;
	struct timeval		timeout;
	char 				buffer[RECVBUF_SIZE] = {0};
	struct sockaddr_in 	from;
	socklen_t 			fromlen = sizeof(from);
	struct timeval 		recv_time;
	double 				rtt;
	struct icmphdr 		*icmp_hdr;
	int					res;
	char				last_ip[INET_ADDRSTRLEN] = {0};

	print_ttl(g_data.curttl);

    for (int i = 0; i < PROBES; ++i)
    {
        timeout.tv_sec = 0;
        timeout.tv_usec = TIMEOUT_MICSEC;

        FD_ZERO(&readfds);
        FD_SET(g_data.icmp_sockfd, &readfds);

		res = select(g_data.icmp_sockfd + 1, &readfds, NULL, NULL, &timeout);

		if (res == 0)
        {
			write(STDOUT_FILENO, "* ", 2);
            continue;
        }
		else if (res < 0)
		{
			write(STDERR_FILENO, "select failed", 15);
            continue;
		}

        if (recvfrom(g_data.icmp_sockfd, buffer, sizeof(buffer), 0,
        		(struct sockaddr *)&from, &fromlen) < 0)
        {
            perror("recvfrom failed");
            continue;
        }

        if (gettimeofday(&recv_time, NULL) < 0)
        {
            perror("gettimeofday failed");
            continue;
        }

        rtt = (recv_time.tv_sec - send_times[i].tv_sec) * 1000.0;
        rtt += (recv_time.tv_usec - send_times[i].tv_usec) / 1000.0;

        icmp_hdr = (struct icmphdr *)(buffer + 20);

		char *curr_ip = inet_ntoa(from.sin_addr);

		if (ft_strcmp(curr_ip, last_ip) != 0)
		{
			write(STDOUT_FILENO, curr_ip, ft_strlen(curr_ip));
			write(STDOUT_FILENO, "  ", 2);

			ft_strncpy(last_ip, curr_ip, sizeof(last_ip));
		}

		if (icmp_hdr->type == ICMP_TIME_EXCEEDED || icmp_hdr->type == ICMP_DEST_UNREACH)
            print_rtt(rtt);

		if (icmp_hdr->type == ICMP_DEST_UNREACH && icmp_hdr->code == ICMP_PORT_UNREACH)
			is_running = STOPPED;
    }

    write(STDOUT_FILENO, "\n", 1);
}

void	print_ttl(int ttl)
{
    char buffer[16];
    int len = snprintf(buffer, sizeof(buffer), " %d  ", ttl);
    write(STDOUT_FILENO, buffer, len);
}

void	print_rtt(double rtt)
{
    char buffer[32];
    int len = snprintf(buffer, sizeof(buffer), "%.3f ms   ", rtt);
    write(STDOUT_FILENO, buffer, len);
}

void	flush_icmp_socket(void)
{
    fd_set				readfds;
    struct timeval		timeout;
    char				buffer[RECVBUF_SIZE];
    struct sockaddr_in	from;
    socklen_t 			fromlen = sizeof(from);

    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;

    FD_ZERO(&readfds);
    FD_SET(g_data.icmp_sockfd, &readfds);

    while (select(g_data.icmp_sockfd + 1, &readfds, NULL, NULL, &timeout) > 0)
    {
        if (recvfrom(g_data.icmp_sockfd, buffer, sizeof(buffer), 0,
                     (struct sockaddr *)&from, &fromlen) < 0)
        {
            perror("recvfrom failed during flush");
            continue;
        }
    }
}
