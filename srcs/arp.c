#include "ft_malcolm.h"

void	print_ehthdr(struct ethhdr *hdr)
{
	printf("Ethernet header:\n");
	printf("\tdestination: ");
	PRINT_MAC(hdr->h_dest);
	printf("\tsource: ");
	PRINT_MAC(hdr->h_source);
	printf("\ttype 0x%04x\n", ntohs(hdr->h_proto));
}

void	print_arphdr(struct arphdr *hdr)
{
	printf("ARP:\n");
	printf("\thardware type: %d\n", ntohs(hdr->ar_hrd));
	printf("\tprotocol type: 0x%04x\n", ntohs(hdr->ar_pro));
	printf("\thardware size: %d\n", hdr->ar_hln);
	printf("\tprotocol size: %d\n", hdr->ar_pln);
	printf("\top code: %d\n", ntohs(hdr->ar_op));
}

int	listen_arp(t_malcolm *m)
{
	t_arp_pkt	*pkt;
	u_int8_t	buf[sizeof(struct ethhdr) + sizeof(t_arp_pkt)];

	ft_memset(buf, 0, sizeof(buf));
	printf("Listening For Incoming ARP\n");
	while (ft_memcmp(buf, "\xff\xff\xff\xff\xff\xff", 6))
	{
		if (recvfrom(m->sock, &buf, sizeof(buf), 0, NULL, NULL) <= 0)
			return (printf("Error: Cannot Listen ARP Broadcast\n"));
	}
	pkt = (void *)buf + sizeof(struct ethhdr);
	printf("ARP Broadcast Captured\n");
	if (m->verbose)
	{
		print_ehthdr((void *)buf);
		print_arphdr(&pkt->arp_hdr);
	}
	printf("\tMAC addr of request: ");
	PRINT_MAC(pkt->sender_mac);
	printf("\tIP addr of request: ");
	PRINT_IP(pkt->sender_ip);
	if (m->verbose)
	{
		printf("\tMAC addr of target: ");
		PRINT_MAC(pkt->target_mac);
		printf("\tIP addr of target: ");
		PRINT_IP(pkt->target_ip);
	}
	return (0);
}

void	init_packet(t_malcolm *m, struct ethhdr *eth_hdr, t_arp_pkt *pkt)
{
	ft_memcpy(eth_hdr->h_dest, m->target.arp_mac, sizeof(m->target.arp_mac));
	ft_memcpy(eth_hdr->h_source, m->source.arp_mac, sizeof(m->source.arp_mac));
	eth_hdr->h_proto = htons(ETH_P_ARP);
	pkt->arp_hdr.ar_hrd = htons(1);
	pkt->arp_hdr.ar_pro = htons(ETH_P_IP);
	pkt->arp_hdr.ar_hln = MAC_LEN;
	pkt->arp_hdr.ar_pln = IP_LEN;
	pkt->arp_hdr.ar_op = htons(ARPOP_REPLY);
	ft_memcpy(pkt->sender_mac, m->source.arp_mac, sizeof(m->source.arp_mac));
	ft_memcpy(pkt->sender_ip, m->source.arp_ip, sizeof(m->source.arp_ip));
	ft_memcpy(pkt->target_mac, m->target.arp_mac, sizeof(m->target.arp_mac));
	ft_memcpy(pkt->target_ip, m->target.arp_ip, sizeof(m->target.arp_ip));
}

int	send_arp(t_malcolm *m)
{
	u_int8_t		buf[sizeof(struct ethhdr) + sizeof(t_arp_pkt) + PADDING];
	struct ethhdr	*eth_hdr;
	t_arp_pkt		*pkt;
	
	ft_memset(buf, 0, sizeof(buf));
	eth_hdr = (void *)buf;
	pkt = (void *)buf + sizeof(struct ethhdr);
	init_packet(m, eth_hdr, pkt);
	printf("Sending ARP Packet...\n");
	if (m->verbose)
	{
		print_ehthdr(eth_hdr);
		print_arphdr(&pkt->arp_hdr);
		printf("\tMAC addr of request: ");
		PRINT_MAC(pkt->sender_mac);
		printf("\tIP addr of request: ");
		PRINT_IP(pkt->sender_ip);
		printf("\tMAC addr of target: ");
		PRINT_MAC(pkt->target_mac);
		printf("\tIP addr of target: ");
		PRINT_IP(pkt->target_ip);
	}
	if (sendto(m->sock, &buf, sizeof(buf), 0, (struct sockaddr *)&m->sock_addr, sizeof(m->sock_addr)) <= 0)
		return (printf("Error: Failed to Send Packet"));
	printf("Sent ARP Packet\n");
	return (0);
}
