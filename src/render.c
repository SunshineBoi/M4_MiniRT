#include "mrt.h"

void	render(t_app *app)
{
	int		col;
	int		row;
	t_ray	ray;

	double	dist_to_surface;

	row = 0;
	while (row < WIN_HEIGHT)
	{
		col = 0;
		while (col < WIN_WIDTH)
		{
			ray = set_ray(&app->cam, row, col);
			// test plain rendering:
			// color_pixel(app, row, col, rgb_to_int(
			// 	(ray.dir.x + 1) * 0.5 * 255,
			// 	(ray.dir.y + 1) * 0.5 * 255,
			// 	(ray.dir.z + 1) * 0.5 * 255));

			if (hit_sphere(ray, &app->sp, &dist_to_surface))
				color_pixel(app, row, col, app->sp.color);
			else
				color_pixel(app, row, col, 0);
			col++;
		}
		row++;
	}
}
