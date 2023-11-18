#include "ft_malcolm.h"

void	find_iface(t_malcolm *m, struct ifaddrs *tmp, struct ifaddrs *temp, int *flag)
{
	int	i;
	
	i = 1;
	while (temp)
	{
		if (temp->ifa_addr && !ft_memcmp(temp->ifa_name, tmp->ifa_name, ft_strlen(tmp->ifa_name))
			&& temp->ifa_addr->sa_family == AF_PACKET)
		{
			printf("Found Available Interface %s\n", tmp->ifa_name);
			ft_memcpy(m->arp_ip, &((struct sockaddr_in *)tmp->ifa_addr)->sin_addr, IP_LEN);
			ft_memcpy(m->arp_mac, ((struct sockaddr_ll *)(temp->ifa_addr))->sll_addr, MAC_LEN);
			if (m->verbose)
			{
				printf("\tIP: ");
				PRINT_IP(m->arp_ip);
				printf("\tMAC: ");
				PRINT_MAC(m->arp_mac);
			}
			m->ifindex = i;
			*flag = 0;
			break ;
		}
		++i;
		temp = temp->ifa_next;
	}
}

int	check_iface(t_malcolm *m, struct ifaddrs *tmp, struct ifaddrs *addrs, int *flag)
{
	struct ifaddrs	*temp;
	u_int32_t		net_ip;
	u_int32_t		net_mask;

	net_ip = ((struct sockaddr_in *)tmp->ifa_addr)->sin_addr.s_addr;
	net_mask = ((struct sockaddr_in *)tmp->ifa_netmask)->sin_addr.s_addr;
	if ((net_ip & net_mask) == (m->source.inet_ip & net_mask)
		&& (net_ip & net_mask) == (m->target.inet_ip & net_mask))
	{
		temp = addrs;
		find_iface(m, tmp, temp, flag);
		if (temp)
			return (1);
		else if (m->verbose)
			printf("\t%s is not of type AF_PACKET\n", tmp->ifa_name);
	}
	else if (m->verbose)
	{
		if ((net_ip & net_mask) != (m->source.inet_ip & net_mask))
			printf("\t%s not in interface ip range\n", m->source.ip);
		if ((net_ip & net_mask) != (m->target.inet_ip & net_mask))
			printf("\t%s not in interface ip range\n", m->target.ip);
	}
	return (0);
}

int	acquire_iface(t_malcolm *m)
{
	struct ifaddrs	*addrs;
	struct ifaddrs	*tmp;
	int				flag;

	flag = 1;
	getifaddrs(&addrs);
	tmp = addrs;
	while (tmp)
	{
		if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET)
		{
			if (m->verbose)
				printf("Checking Interface: %s\n", tmp->ifa_name);
			if (check_iface(m, tmp, addrs, &flag))
				break ;
		}
		tmp = tmp->ifa_next;
	}
	freeifaddrs(addrs);
	if (flag)
		return (printf("Error: No Available Interface Found.\n"));
	return (0);
}
