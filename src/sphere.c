#include "mrt.h"

t_vec	hit_sphere_normal(t_vec hit, t_sphere *sp)
{
	// hit = O + t*D
	// normal = (hit - Center) / r
	t_vec	normal;

	normal = vec_scale(vec_sub(hit, sp->center), 1 / sp->radius);
	return (normal);
}

int	hit_sphere(t_ray ray, t_sphere *sp, double *t)
{
	t_vec	o_to_c;
	double	b;
	double	c;
	double	disc;

	// substituting 2 eqns and finding the roots
	// ray eqn:        P(t) = O + t*D
	// circle surface: | P - Center | = radius
	o_to_c = vec_sub(ray.src, sp->center);
	b = 2 * vec_dot(o_to_c, ray.dir);
	c = vec_dot(o_to_c, o_to_c) - (sp->radius * sp->radius);
	disc = (b * b) - (4 * c);
	if (disc < 0)
		return (0);
	if ((-b - sqrt(disc)) / 2 > EPSILON)
		*t = (-b - sqrt(disc)) / 2;
	else if ((-b + sqrt(disc)) / 2 > EPSILON)
		*t = (-b + sqrt(disc)) / 2;
	else
		return (0);
	return (1);
}
