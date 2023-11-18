#include "ft_malcolm.h"

u_int8_t	hex2byte(char *hex)
{
	u_int8_t	ret;
	u_int8_t	nibble;
	int			i;

	ret = 0;
	i = 0;
	while (IS_HEX(hex[i]))
	{
		nibble = hex[i];
		if (nibble >= 0x30 && nibble <= 0x39)
			nibble -= 0x30;
		else if (nibble >= 0x41 && nibble <= 0x46)
			nibble -= 0x41 - 10;
		else if (nibble >= 0x61 && nibble <= 0x66)
			nibble -= 0x61 - 10;
		ret = (ret << 4) | (nibble & 0xf);
		if (++i >= 2)
			break ;
	}
	return (ret);
}
