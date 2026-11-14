#include "mrt.h"

t_app	*init_app(void)
{
	t_app	*new;

	new = malloc(sizeof(t_app));
	if (!new)
		return (p_syserr(), NULL);
	new->img_obj = NULL;
	new->mlx_obj = NULL;
	return (new);
}

t_mlx	*setup_mlx(void)
{
	t_mlx	*mlx_obj;

	mlx_obj = malloc(sizeof(t_mlx));
	if (!mlx_obj)
		return (p_syserr(), NULL);

	mlx_obj->conn_ptr = mlx_init();
	if (!mlx_obj->conn_ptr)
		return (p_err("miniRT: mlx_init failure"), free(mlx_obj), NULL);

	mlx_obj->win_ptr = mlx_new_window(mlx_obj->conn_ptr,
		WIN_WIDTH, WIN_HEIGHT, "MiniRT");
	if (!mlx_obj->win_ptr)
		return (p_err("miniRT: mlx_new_window failure"), freemlx(mlx_obj), NULL);
	mlx_obj->is_clickhold = 0;
	mlx_obj->mouse_x = 0;
	mlx_obj->mouse_y = 0;
	return (mlx_obj);
}

/*
temporary scene
*/
static int	setup_scene(t_app *app)
{
	app->cam.pos = v(0, 0, 0);
	app->cam.fwd = v(0, 0, 1);
	app->cam.fov_deg = 70;
	if (!setup_camera(&app->cam))
		return (p_err("miniRT: bad camera"), 0);

	// setup sphere
	app->sp.center = v(0, 0, 40);
	app->sp.radius = 20 / 2.0;
	app->sp.color = rgb_to_int(255, 0, 0);
	return (1);
}

int	minirt(void)
{
	t_app	*app;

	app = init_app();
	if (!app)
		return (EXIT_FAILURE);
	// todo: parse file
	// ! temporary scene
	if (!setup_scene(app))
		return (free(app), EXIT_FAILURE);

	app->mlx_obj = setup_mlx();
	if (!app->mlx_obj)
		return (freeappexit(app, EXIT_FAILURE));

	app->img_obj = init_img(app->mlx_obj);
	if (!app->img_obj)
		return (freeappexit(app, EXIT_FAILURE));

	render(app);
	mlx_put_image_to_window(app->mlx_obj->conn_ptr, app->mlx_obj->win_ptr, app->img_obj->img_ptr, 0, 0);

	// hook
	setup_events(app);
	// start listening
	mlx_loop(app->mlx_obj->conn_ptr);

	return (EXIT_SUCCESS);
}
