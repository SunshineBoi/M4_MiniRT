#include "mrt.h"

/*
Reads "x,y,z" into *dest.
Returns 0 if str does not have exactly 2 commas and 3 numbers.
*/
int	parse_vector(char *str, t_vec *dest)
{
	char	**xyz;
	int		ok;

	if (count_commas(str) != 2)
		return (0);
	xyz = ft_split_set(str, ",");
	ok = (count_lst(xyz) == 3
			&& parse_double(xyz[0], &dest->x)
			&& parse_double(xyz[1], &dest->y)
			&& parse_double(xyz[2], &dest->z));
	free_list(xyz);
	return (ok);
}

/*
Returns 1 if vec has a length of 1 (tolerance 0.001)
and each part is in -1 to 1, else 0.
*/
int	is_unit_vector(t_vec vec)
{
	return (fabs(vec_length(vec) - 1) <= 0.001
		&& is_in_range(vec.x, -1, 1)
		&& is_in_range(vec.y, -1, 1)
		&& is_in_range(vec.z, -1, 1));
}
