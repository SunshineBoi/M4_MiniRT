#include "mrt.h"

t_camera	make_cam(t_vec fwd, double fov)
{
	t_camera	cam;

	cam.pos = v(-50, 0, 20);
	cam.fwd = fwd;
	cam.right = v(999, 999, 999);
	cam.top = v(999, 999, 999);
	cam.fov_deg = fov;
	cam.scale = 999;
	cam.aspect = 999;
	return (cam);
}

static int	set_cam_right(t_camera *cam)
{
	t_vec	top_glob;

	top_glob = v(0, 1, 0);
	if (vec_normalize(vec_cross(cam->fwd, top_glob), &cam->right))
		return (1);
	// same vector direction
	top_glob.y = 0;
	top_glob.z = 1;
	return (vec_normalize(vec_cross(cam->fwd, top_glob), &cam->right));
}

static void	set_scale(t_camera *cam)
{
	double	fov_deg;
	double	fov_rad;

	fov_deg = (cam->fov_deg / 2);
	fov_rad = fov_deg * M_PI / 180;
	cam->scale = tan(fov_rad);
}

int	setup_camera(t_camera *cam)
{
	if (!vec_normalize(cam->fwd, &cam->fwd))
		return (0);
	if (set_cam_right(cam) == 0)  // step 1
		return (0);
	cam->top = vec_cross(cam->right, cam->fwd);  // step 2: set_cam_top (y-axis)
	set_scale(cam);  // step 3
	cam->aspect = (double)WIN_WIDTH / WIN_HEIGHT;  // step 4
	return (1);
}

t_ray	set_ray(t_camera *cam, int row, int col)
{
	double	x_screen;
	double	y_screen;
	t_vec	offset;
	t_ray	ray;

	x_screen = (2 * ((col + 0.5) / WIN_WIDTH)) - 1;  // inner bracket is x_ndc
	y_screen = 1 - (2 * ((row + 0.5) / WIN_HEIGHT));  // inner bracket is y_ndc
	x_screen *= cam->scale;
	y_screen *= cam->scale / cam->aspect;
	// offset formula = (x_screen * cam_right) + (y_screen * cam_up) + cam_fwd
	offset = vec_add(
				vec_add(
					vec_scale(cam->right, x_screen),
					vec_scale(cam->top, y_screen)), cam->fwd);

	ray.src = cam->pos;
	vec_normalize(offset, &ray.dir);
	return (ray);
}

void	render(t_camera *cam)
{
	int	col;
	int	row;

	row = 0;
	while (row < WIN_HEIGHT)
	{
		col = 0;
		while (col < WIN_WIDTH)
		{
			set_ray(cam, row, col);
			col++;
		}
		row++;
	}
}

