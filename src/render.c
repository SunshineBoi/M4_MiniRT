#include "mrt.h"

void	render(t_app *app)
{
	int		col;
	int		row;
	t_ray	ray;

	t_hit	hit;

	row = 0;
	while (row < WIN_HEIGHT)
	{
		col = 0;
		while (col < WIN_WIDTH)
		{
			// set_ray builds O and D once per pixel
			ray = set_ray(&app->cam, row, col);
			// test plain rendering:
			// color_pixel(app, row, col, rgb_to_int(
			// 	(ray.dir.x + 1) * 0.5 * 255,
			// 	(ray.dir.y + 1) * 0.5 * 255,
			// 	(ray.dir.z + 1) * 0.5 * 255));

			if (hit_sphere(ray, &app->sp, &hit.dist_to_hit))
			{
				// hit = O + t*D
				hit.hit_vec = vec_add(ray.src, vec_scale(ray.dir, hit.dist_to_hit));
				hit.normal_at_hit = hit_sphere_normal(hit.hit_vec, app->sp);
				hit.color = app->sp.color;
				color_pixel(app, row, col, norm_rgb_to_int(shade(app, hit)));
			}
			else
				color_pixel(app, row, col, 0);
			col++;
		}
		row++;
	}
}
