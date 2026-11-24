#include "mrt.h"

size_t	ft_strlen(const char *str)
{
	size_t	count;

	if (!str)
		return (0);
	count = 0;
	while (str[count])
		count++;
	return (count);
}

/*
Compares s1 and s2 byte by byte.
Returns 0 if they are equal, a negative value if s1 comes first,
and a positive value if s2 comes first.
*/
int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/*
Returns a new string with the first n characters of str, ended by '\0'.
Copies fewer if str is shorter than n.
Returns NULL and prints the system reason if malloc fails.
*/
char	*ft_strndup(const char *str, size_t n)
{
	char	*dup;
	size_t	i;

	i = 0;
	while (i < n && str[i])
		i++;
	dup = malloc(i + 1);
	if (!dup)
		return (p_syserr(), NULL);
	ft_memcpy(dup, str, i);
	dup[i] = '\0';
	return (dup);
}

/*
Returns the number of ',' characters in str.
*/
int	count_commas(char *str)
{
	int	count;

	count = 0;
	while (*str)
	{
		if (*str == ',')
			count++;
		str++;
	}
	return (count);
}

/*
Counts the words in str. A word is a run of characters that are not in set.
Separators at the start, at the end and next to each other make no empty word.
Example: count_words("  sp 0,0,0  12 ", " ") returns 3.
*/
int	count_words(char *str, char *set)
{
	int	count;

	count = 0;
	while (*str)
	{
		while (*str && match_delim(*str, set))
			str++;
		if (*str)
			count++;
		while (*str && !match_delim(*str, set))
			str++;
	}
	return (count);
}
