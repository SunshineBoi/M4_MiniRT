#include "mrt.h"

void	free_objects(t_object *obj)
{
	t_object	*temp;

	while (obj)
	{
		temp = obj->next;
		free(obj);
		obj = temp;
	}
}

int	add_objects(t_object **lst, t_object obj)
{
	t_object	*new;

	new = malloc(sizeof(t_object));
	if (!new)
		return (p_syserr(), 0);

	*new = obj;

	new->next = *lst;
	*lst = new;
	return (1);
}
