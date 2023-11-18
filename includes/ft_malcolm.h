#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <ifaddrs.h>
# include <net/if.h>
# include <net/if_arp.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <linux/if_ether.h>
# include <linux/if_packet.h>

// bonus (resolve.c)
# include <netdb.h>
# include <linux/icmp.h>
/*
	gai_strerror(1)
	getaddrinfo(4)
	freeaddrinfo(1)
	inet_ntop(4)
*/

# define IP_LEN 4
# define MAC_LEN 6
# define MAC_STR_LEN 17
# define PADDING 18
# define PACKETSIZE 56

# define IS_HEX(byte) ((byte >= 0x30 && byte <= 0x39) || (byte >= 0x41 && byte <= 0x46) || (byte >= 0x61 && byte <= 0x66))
# define PRINT_MAC(mac) printf("%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5])
# define PRINT_IP(ip) printf("%d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3])

typedef struct s_arp_pkt
{
	struct arphdr	arp_hdr;
	u_int8_t		sender_mac[MAC_LEN];
	u_int8_t		sender_ip[IP_LEN];
	u_int8_t		target_mac[MAC_LEN];
	u_int8_t		target_ip[IP_LEN];
}	t_arp_pkt;

typedef struct s_addr
{
	char		*ip;
	char		*mac;
	in_addr_t	inet_ip;
	u_int8_t	arp_ip[IP_LEN];
	u_int8_t	arp_mac[MAC_LEN];
}	t_addr;

typedef struct s_ping
{
	char				canonname[NI_MAXHOST];
	int					sock;
	struct sockaddr_in	sock_addr;
}	t_ping;

typedef struct s_malcolm
{
	t_addr				source;
	t_addr				target;
	void				**flags;
	int					verbose;
	int					sock;
	int					ifindex;
	struct sockaddr_ll	sock_addr;
	u_int8_t			arp_ip[IP_LEN];
	u_int8_t			arp_mac[MAC_LEN];
}	t_malcolm;

// arp.c
int			listen_arp(t_malcolm *m);
int			send_arp(t_malcolm *m);

// iface.c
int			acquire_iface(t_malcolm *m);

// libft.c
size_t		ft_strlen(const char *str);
int			ft_strncmp(const char *s1, const char *s2, unsigned int n);
void		*ft_memset(void *b, int c, size_t len);
void		*ft_memcpy(void *dst, const void *src, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
long		ft_atol(const char *str);

// parse.c
int			parse_args(int argc, char **argv, t_malcolm *m);

// parse_data.c
int			parse_data(char **argv, int i, t_malcolm *m);

// resolve.c
int			resolve_hostname(char *hostname, char *ip);

// utils.c
u_int8_t	hex2byte(char *hex);

#endif
