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
			// set_ray builds O and D once per pixel from R(t) = O + t*D
			ray = set_ray(&app->scene.cam, row, col);

			if (find_nearest_hit(ray, &app->scene, &hit))
				color_pixel(app, row, col, norm_rgb_to_int(shade(&app->scene, hit)));
			else
				color_pixel(app, row, col, 0);
			col++;
		}
		row++;
	}
}
