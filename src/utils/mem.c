#include "mrt.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*dest_p;
	const unsigned char	*src_p;

	if (!dest || !src)
		return (NULL);

	dest_p = dest;
	src_p = src;
	while (n--)
		*dest_p++ = *src_p++;
	return (dest);
}

void	*ft_realloc(void *old, size_t old_size, size_t new_size)
{
	char	*new;

	new = malloc(new_size + 1);
	if (!new)
		return (p_syserr(), NULL);
	if (old)
	{
		new = (char *)ft_memcpy(new, old, old_size);
		new[old_size] = '\0';
		free(old);
	}
	return (new);
}
