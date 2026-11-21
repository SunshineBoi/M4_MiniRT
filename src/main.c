#include "mrt.h"

int	main(int ac, char **av)
{
	if (ac != 2)
		return (p_err("miniRT: wrong argument count"), EXIT_FAILURE);
	return (minirt(av[1]));
}
