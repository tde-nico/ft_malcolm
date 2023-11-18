#include "ft_malcolm.h"

struct addrinfo	*get_info(char *hostname)
{
	int				res;
	struct addrinfo	hints;
	struct addrinfo	*info;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	hints.ai_flags = AI_CANONNAME;
	res = getaddrinfo(hostname, NULL, &hints, &info);
	if (res)
	{
		printf("Error: %s -> %s\n", hostname, gai_strerror(res));
		return (NULL);
	}
	return (info);
}

struct addrinfo	*get_socket(int *sock, struct addrinfo *info)
{
	struct addrinfo	*tmp;

	tmp = info;
	while (tmp)
	{
		*sock = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (*sock)
			break ;
		tmp = tmp->ai_next;
	}
	if (*sock < 0 || !tmp)
	{
		printf("Error: cannot open socket\n");
		freeaddrinfo(info);
		return (NULL);
	}
	return (tmp);
}

int	resolve_address(int *sock, struct addrinfo *tmp, char *ip)
{
	struct timeval			timeout;
	int						opt;
	struct sockaddr_storage	addr;

	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	opt = 1;
	if (setsockopt(*sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)))
		return (printf("Error: setsockopt failure\n"));
	if (setsockopt(*sock, SOL_IP, IP_RECVERR, &opt, sizeof(opt)))
		return (printf("Error: setsockopt failure\n"));
	ft_memcpy(&addr, tmp->ai_addr, tmp->ai_addrlen);
	inet_ntop(addr.ss_family, (void *)&((struct sockaddr_in *)&addr)->sin_addr, ip, INET_ADDRSTRLEN);
	return (0);
}

int	resolve_hostname(char *hostname, char *ip)
{
	struct addrinfo	*info;
	struct addrinfo	*tmp;
	int				sock;

	info = get_info(hostname);
	if (!info)
		return (1);
	tmp = get_socket(&sock, info);
	if (!tmp)
		return (1);
	if (resolve_address(&sock, tmp, ip))
	{
		freeaddrinfo(info);
		close(sock);
		return (1);
	}
	freeaddrinfo(info);
	close(sock);
	printf("Resolve: %s to %s\n", hostname, ip);
	return (0);
}
