#include "ft_malcolm.h"

int	usage(void **flags)
{
	size_t	i;

	printf("Usage: ./ft_malcolm [flags]\n");
	i = 0;
	while (flags[i])
	{
		printf("\t%s\t%s\n", (char*)flags[i], (char*)flags[i + 2]);
		i += 3;
	}
	return (1);
}

int	parse_flags(int argc, char **argv, void **f, t_malcolm *m)
{
	int	(*fun)(t_malcolm *m, char **args, int *i);
	int	i;
	int	j;
	int	flag;

	i = -1;
	while (++i < argc)
	{
		j = -1;
		flag = 1;
		while (f[++j * 3])
		{
			if (!ft_strncmp(f[j * 3], argv[i], ft_strlen(f[j * 3]) + 1))
			{
				fun = f[(j * 3) + 1];
				if (fun(m, &argv[i], &i))
					return (1);
				flag = 0;
				break ;
			}
		}
		if (flag && parse_data(argv, i, m))
			return (1);
	}
	return (0);
}

int	print_usage(t_malcolm *m, char **args, int *i)
{
	(void)args;
	(void)i;
	return (usage(m->flags));
}

int	enable_verbose(t_malcolm *m, char **args, int *i)
{
	(void)args;
	(void)i;
	m->verbose = 1;
	return (0);
}

int	parse_args(int argc, char **argv, t_malcolm *m)
{
	void	*flags[] = {
		"-h", &print_usage, "prints the usage",
		"-v", &enable_verbose, "enable verbose",
		NULL, NULL, NULL,
	};

	ft_memset(m, 0, sizeof(m));
	m->flags = flags;
	if (argc < 4)
		return (usage(flags));
	if (parse_flags(argc, argv, flags, m))
		return (1);
	if (m->verbose)
	{
		printf("Source IP: ");
		PRINT_IP(m->source.arp_ip);
		printf("Source MAC: ");
		PRINT_MAC(m->source.arp_mac);
		printf("Target IP: ");
		PRINT_IP(m->target.arp_ip);
		printf("Target MAC: ");
		PRINT_MAC(m->target.arp_mac);
	}
	return (0);
}


