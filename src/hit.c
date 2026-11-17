#include "mrt.h"

int	hit_object(t_ray ray, t_object *obj, double *t_hit)
{
	if (obj->type == SPHERE)
		return (hit_sphere(ray, obj, t_hit));
	else if (obj->type == PLANE)
		return (hit_plane(ray, obj, t_hit));
	else if (obj->type == CYLINDER)
		// todo for cylinder
		return (0);
	return (0);
}

t_vec	normal_at_hit_obj(t_object *obj, t_vec hit_vec)
{
	if (obj->type == SPHERE)
		return (hit_sphere_normal(hit_vec, obj));
	else if (obj->type == PLANE)
		return (obj->axis);
	else if (obj->type == CYLINDER)
		// todo for cylinder
		return (v(0, 0, 0));
	return (v(0, 0, 0));
}

int	find_nearest_hit(t_ray ray, t_scene *scene, t_hit *hit)
{
	t_object	*curr_obj;
	t_object	*best;
	double		curr_dist;

	best = NULL;
	curr_obj = scene->obj;
	while (curr_obj)
	{
		if (hit_object(ray, curr_obj, &curr_dist))
		{
			if (best == NULL || curr_dist < hit->dist_to_hit)
			{
				best = curr_obj;
				hit->dist_to_hit = curr_dist;
			}
		}
		curr_obj = curr_obj->next;
	}
	if (!best)
		return (0);
	// hit = O + t*D
	hit->hit_vec = vec_add(ray.src, vec_scale(ray.dir, hit->dist_to_hit));
	hit->normal_at_hit = normal_at_hit_obj(best, hit->hit_vec);
	// this normal vector has to face the camera, it represents the surface we looking
	// when dot product against light source, a negative value means the surface
	// we looking at is not receiving light from light source
	if (vec_dot(ray.dir, hit->normal_at_hit) > 0)
		hit->normal_at_hit = vec_scale(hit->normal_at_hit, -1);  // flipping vector
	hit->color = best->color;
	return (1);
}
