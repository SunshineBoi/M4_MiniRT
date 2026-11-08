#include "mrt.h"

void	ft_putstr_fd(char *str, int fd)
{
	if (!str)
		return ;
	write(fd, str, ft_strlen(str));
}

void	ft_putchar_fd(char ch, int fd)
{
	write(fd, &ch, 1);
}

void	p_err(char *msg)
{
	ft_putstr_fd("Error\n", 2);
	if (msg)
	{
		ft_putstr_fd(msg, 2);
		ft_putchar_fd('\n', 2);
	}
}

void	p_syserr(void)
{
	ft_putstr_fd("Error\n", 2);
	perror("miniRT");
}
