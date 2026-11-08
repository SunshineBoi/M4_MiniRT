#include "mrt.h"

void	render(t_app *app)
{
	int		col;
	int		row;
	t_ray	ray;

	row = 0;
	while (row < WIN_HEIGHT)
	{
		col = 0;
		while (col < WIN_WIDTH)
		{
			ray = set_ray(&app->cam, row, col);
			color_pixel(app, row, col, rgb_to_int(
				(ray.dir.x + 1) * 0.5 * 255,
				(ray.dir.y + 1) * 0.5 * 255,
				(ray.dir.z + 1) * 0.5 * 255));
			col++;
		}
		row++;
	}
}
