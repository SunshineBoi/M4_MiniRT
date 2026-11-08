#include "mrt.h"

int main(int ac, char **av)
{
	// char	*file_path;

	if (ac != 2)
	{
		p_err("miniRT: Wrong argument counts");
		exit(EXIT_FAILURE);
	}
	(void)av;
	// file_path = av[1];
	// todo: open and read file
	return (minirt());
}

