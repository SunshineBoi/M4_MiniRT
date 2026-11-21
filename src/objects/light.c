#include "mrt.h"

t_color shade(t_scene *scene, t_hit hit)
{
	t_vec	light_vec;
	double	diffuse;
	t_color amb;
	t_color light;
	t_color	res;

	diffuse = 0.0;
	// 1. light vector from hit point to light source
	if (vec_normalize(vec_sub(scene->light.pos, hit.hit_vec), &light_vec))
		// how much hit point facing light source
		// diffuse can be negative, i.e. facing away from light source
		// floor at 0 so it doesnt affect ambient light.
		diffuse = fmax(0.0, vec_dot(light_vec, hit.normal_at_hit));
	// 2. ambient contribution
	amb.r = scene->amb.ratio * scene->amb.color.r;
	amb.g = scene->amb.ratio * scene->amb.color.g;
	amb.b = scene->amb.ratio * scene->amb.color.b;
	// 3. light source contribution
	// ! light color is unused for mandatory, keep first.
	light.r = scene->light.ratio * scene->light.color.r * diffuse;
	light.g = scene->light.ratio * scene->light.color.g * diffuse;
	light.b = scene->light.ratio * scene->light.color.b * diffuse;
	// 4. total light * surface color
	res.r = (amb.r + light.r) * hit.color.r;
	res.g = (amb.g + light.g) * hit.color.g;
	res.b = (amb.b + light.b) * hit.color.b;
	return (res);
}
