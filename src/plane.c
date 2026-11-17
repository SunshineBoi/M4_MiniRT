#include "mrt.h"

int	hit_plane(t_ray ray, t_object *obj, double *t_hit)
{
	double	y;
	double	dy;

	// 1. compute denominator
	// dy is change in y for every unit of ray movement in D direction
	dy = vec_dot(ray.dir, obj->axis);
	if (fabs(dy) < EPSILON)
		return (0);

	// 2. compute numerator
	// y is the shortest distance from source to plane
	y = vec_dot(vec_sub(obj->center, ray.src), obj->axis);

	// 3. compute distance from source to hit point
	// positive means within sight, and vice versa.
	if (y / dy <= EPSILON)
		return (0);
	*t_hit = y / dy;
	return (1);
}
