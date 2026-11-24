#include "mrt.h"

/*
Returns the number of strings in a NULL-terminated array, for example
the field count of a split line. Returns 0 if lst is NULL.
*/
int	count_lst(char **lst)
{
	int	count;

	count = 0;
	if (!lst)
		return (0);
	while (lst[count])
		count++;
	return (count);
}

/*
Frees each string of a NULL-terminated array, then the array.
Accepts NULL. Use it on a full result of ft_split_set,
or on a partial one after a failed malloc (the next free slot must be NULL).
*/
void	free_list(char **lst)
{
	int	i;

	if (!lst)
		return ;
	i = 0;
	while (lst[i])
		free(lst[i++]);
	free(lst);
}

/*
Skips the separators at *str, then copies the next word.
Moves *str to the character after the word.
Returns NULL if malloc fails.
*/
static char	*next_word(char **str, char *set)
{
	char	*word;
	int		len;

	while (**str && match_delim(**str, set))
		(*str)++;
	len = 0;
	while ((*str)[len] && !match_delim((*str)[len], set))
		len++;
	word = ft_strndup(*str, len);
	*str += len;
	return (word);
}

/*
Splits str at each character of set into a NULL-terminated array.
Separators at the start, at the end and next to each other make no
empty string.
Returns NULL and frees the partial result if malloc fails.
*/
char	**ft_split_set(char *str, char *set)
{
	char	**lst;
	int		count;
	int		i;

	count = count_words(str, set);
	lst = malloc((count + 1) * sizeof(char *));
	if (!lst)
		return (p_syserr(), NULL);
	i = 0;
	while (i < count)
	{
		lst[i] = next_word(&str, set);
		if (!lst[i])
			return (free_list(lst), NULL);
		i++;
	}
	lst[i] = NULL;
	return (lst);
}
