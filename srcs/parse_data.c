#include "ft_malcolm.h"

int	parse_mac(char *mac, u_int8_t *addr_mac)
{
	int	i;
	int	j;

	if (ft_strlen(mac) != MAC_STR_LEN)
		return (printf("Error: Invalid MAC Address Length\n"));
	i = -1;
	while (++i < MAC_STR_LEN)
	{
		if (i % 3 == 2 && !(mac[i] == ':' || mac[i] == '-'))
			return (printf("Error: Invalid MAC Address Separator at char %d\n", i));
		else if (i % 3 != 2 && !IS_HEX(mac[i]))
			return (printf("Error: Invalid MAC Address HEX at char %d\n", i));
	}
	i = -1;
	j = 0;
	while (++i < MAC_LEN)
	{
		addr_mac[i] = hex2byte(&mac[j]);
		j += 3;
	}
	return (0);
}

int	parse_ip(char *ip, in_addr_t *addr_inet_ip, u_int8_t *addr_ip)
{
	size_t	i;
	size_t	j;
	long	converted_ip;

	*addr_inet_ip = inet_addr(ip);
	if (*addr_inet_ip == INADDR_NONE)
		return (printf("Error: Invalid IP Address\n"));
	i = 0;
	j = 0;
	while (i < ft_strlen(ip))
	{
		converted_ip = ft_atol(&ip[i]);
		addr_ip[j++] = converted_ip;
		while (ip[i] >= 0x30 && ip[i] <= 0x39)
			++i;
		if (ip[i])
			++i;
	}
	if (converted_ip != addr_ip[j - 1])
	{
		i = -1;
		while (++i < 4)
			addr_ip[i] = ((u_int8_t *)&converted_ip)[3 - i];
	}
	return (0);
}

int	parse_data(char **argv, int i, t_malcolm *m)
{
	static int	curr_arg = -1;
	u_int8_t	*addr;
	in_addr_t	*addr_inet;
	char		ip[INET_ADDRSTRLEN];

	if (++curr_arg % 2)
	{
		addr = m->target.arp_mac;
		if (curr_arg == 1)
		{
			m->source.mac = argv[i];
			addr = m->source.arp_mac;
		}
		else
			m->target.mac = argv[i];
		if (parse_mac(argv[i], addr))
			return (1);
	}
	else
	{
		addr_inet = &m->target.inet_ip;
		addr = m->target.arp_ip;
		if (resolve_hostname(argv[i], (char *)&ip))
			return (1);
		if (!curr_arg)
		{
			addr_inet = &m->source.inet_ip;
			addr = m->source.arp_ip;
			m->source.ip = ip;
		}
		else
			m->target.ip = ip;
		if (parse_ip(ip, addr_inet, addr))
			return (1);
	}
	return (0);
}
