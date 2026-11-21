#include "mrt.h"

/*
Returns 1 if c is an ASCII letter (A-Z or a-z), else 0.
*/
int	ft_isalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

/*
Returns 1 if c is an ASCII digit (0-9), else 0.
*/
int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

/*
Returns 1 if c is one of the characters in set, else 0.
Example: match_delim('\t', " \t\r") returns 1.
*/
int	match_delim(char c, char *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}
