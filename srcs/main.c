#include "ft_malcolm.h"

int	init_malcolm(t_malcolm *m)
{
	m->sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (m->sock < 0)
		return (printf("Error: cannot open socket\n"));
	if (acquire_iface(m))
		return (1);
	m->sock_addr.sll_family = AF_PACKET;
	m->sock_addr.sll_protocol = htons(ETH_P_ARP);
	m->sock_addr.sll_ifindex = m->ifindex;
	m->sock_addr.sll_hatype = htons(ARPHRD_ETHER);
	m->sock_addr.sll_pkttype = 0;
	m->sock_addr.sll_halen = MAC_LEN;
	ft_memcpy(m->sock_addr.sll_addr, m->target.arp_mac, sizeof(m->target.mac));
	return (0);
}

int	main(int argc, char **argv)
{
	t_malcolm	m;

	if (getuid() != 0)
	{
		printf("Error: Permission Denied (please use sudo)\n");
		return (1);
	}
	if (parse_args(argc - 1, &argv[1], &m))
		return (1);
	if (init_malcolm(&m))
		return (1);
	if (listen_arp(&m))
		return (1);
	send_arp(&m);
	close(m.sock);
	printf("Exiting program...\n");
	return (0);
}
