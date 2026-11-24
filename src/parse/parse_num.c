#include "mrt.h"

/*
Reads an optional '+' or '-' at str[*i] and moves *i past it.
Returns -1 for '-', else 1.
*/
static double	read_sign(char *str, int *i)
{
	double	sign;

	sign = 1;
	if (str[*i] == '-')
		sign = -1;
	if (str[*i] == '-' || str[*i] == '+')
		(*i)++;
	return (sign);
}

/*
Reads str as a decimal number into *dest.
Accepts an optional sign, digits, then optionally a dot and digits.
Returns 0 for any other form, or if the number is too big (inf).
The inf check protects positions and sizes, which have no range check.
*/
int	parse_double(char *str, double *dest)
{
	double	sign;
	double	div;
	int		i;

	i = 0;
	sign = read_sign(str, &i);
	if (!ft_isdigit(str[i]))
		return (0);
	*dest = 0;
	while (ft_isdigit(str[i]))
		*dest = *dest * 10 + (str[i++] - '0');
	if (str[i] == '.' && !ft_isdigit(str[++i]))
		return (0);
	div = 10;
	while (ft_isdigit(str[i]))
	{
		*dest += (str[i++] - '0') / div;
		div *= 10;
	}
	if (str[i] != '\0' || isinf(*dest))
		return (0);
	*dest *= sign;
	return (1);
}

/*
Reads str as one colour part into *dest: digits only, 0 to 255.
Returns 0 for an empty string, any other character, or a value over 255.
*/
int	parse_byte(char *str, int *dest)
{
	int	i;

	if (!str[0])
		return (0);
	*dest = 0;
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		*dest = *dest * 10 + (str[i] - '0');
		if (*dest > 255)
			return (0);
		i++;
	}
	return (1);
}

/*
Returns 1 if n is in the closed range lb to ub, else 0.
*/
int	is_in_range(double n, double lb, double ub)
{
	return (n >= lb && n <= ub);
}
