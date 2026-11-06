#include "mrt.h"

t_vec	v(double x, double y, double z)
{
	t_vec	new;

	new.x = x;
	new.y = y;
	new.z = z;
	return (new);
}

double	vec_length(t_vec v)
{
	double	res;

	res = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
	return (sqrt(res));
}

int	vec_normalize(t_vec v, t_vec *out)
{
	double	l;
	
	l = vec_length(v);
	if (!(l > EPSILON))  // to handle NaN value where it gives false no matter the comparison
		return (0);
	out->x = v.x / l;
	out->y = v.y / l;
	out->z = v.z / l;
	return (1);
}
