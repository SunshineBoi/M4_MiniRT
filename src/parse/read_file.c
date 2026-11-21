#include "mrt.h"

/*
Makes raw->in big enough for extra more bytes.
Starts at BUFFER_SIZE and doubles the size until the bytes fit.
ft_realloc keeps 1 more byte for the final '\0'.
Returns 0 if malloc fails. raw->in then still holds the old text.
*/
static int	grow_raw(t_raw *raw, int extra)
{
	char	*temp;
	int		new_size;

	new_size = raw->in_alloc_size;
	if (new_size == 0)
		new_size = BUFFER_SIZE;
	while (raw->in_size + extra > new_size)
		new_size *= 2;
	if (raw->in && new_size == raw->in_alloc_size)
		return (1);
	temp = ft_realloc(raw->in, raw->in_size, new_size);
	if (!temp)
		return (0);
	raw->in = temp;
	raw->in_alloc_size = new_size;
	return (1);
}

/*
Reads all of fd into raw->in as one string that ends in '\0'.
Returns 0 if read fails, malloc fails or the file is empty.
The caller frees raw->in on every path.
*/
int	read_file(int fd, t_raw *raw)
{
	char	buffer[BUFFER_SIZE];
	ssize_t	bytes_read;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		if (!grow_raw(raw, bytes_read))
			return (0);
		ft_memcpy(raw->in + raw->in_size, buffer, bytes_read);
		raw->in_size += bytes_read;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	if (bytes_read == -1)
		return (p_syserr(), 0);
	if (raw->in_size == 0)
		return (p_err("miniRT: Empty File!"), 0);
	raw->in[raw->in_size] = '\0';
	return (1);
}
