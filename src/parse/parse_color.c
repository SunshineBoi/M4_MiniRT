#include "mrt.h"

/*
Reads "r,g,b" (each 0 to 255) into *dest as a colour in 0 to 1.
Returns 0 if str does not have exactly 2 commas and 3 valid parts.
*dest does not change on a failure.
*/
int	parse_color(char *str, t_color *dest)
{
	char	**rgb;
	int		ok;
	int		c[3];

	if (count_commas(str) != 2)
		return (0);
	rgb = ft_split_set(str, ",");
	ok = (count_lst(rgb) == 3
			&& parse_byte(rgb[0], &c[0])
			&& parse_byte(rgb[1], &c[1])
			&& parse_byte(rgb[2], &c[2]));
	free_list(rgb);
	if (ok)
		*dest = norm_rgb(c[0], c[1], c[2]);
	return (ok);
}
