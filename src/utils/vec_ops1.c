#include "mrt.h"

t_vec	vec_add(t_vec a, t_vec b)
{
	t_vec	new;

	new.x = a.x + b.x;
	new.y = a.y + b.y;
	new.z = a.z + b.z;
	return (new);
}

t_vec	vec_sub(t_vec a, t_vec b)
{
	t_vec	new;

	new.x = a.x - b.x;
	new.y = a.y - b.y;
	new.z = a.z - b.z;
	return (new);
}

t_vec	vec_scale(t_vec v, double f)
{
	t_vec	new;

	new.x = v.x * f;
	new.y = v.y * f;
	new.z = v.z * f;
	return (new);
}

double	vec_dot(t_vec a, t_vec b)
{
	double	ret;

	ret = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	return (ret);
}

t_vec	vec_cross(t_vec a, t_vec b)
{
	t_vec	new;

	new.x = (a.y * b.z) - (a.z * b.y);
	new.y = (a.z * b.x) - (a.x * b.z);
	new.z = (a.x * b.y) - (a.y * b.x);
	return (new);
}
