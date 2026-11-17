#include "mrt.h"

t_app	*init_app(void)
{
	t_app	*new;

	new = malloc(sizeof(t_app));
	if (!new)
		return (p_syserr(), NULL);
	new->img_obj = NULL;
	new->mlx_obj = NULL;
	new->scene.obj = NULL;
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
	app->scene.cam.pos = v(0, 0, 0);
	app->scene.cam.fwd = v(0, 0, 1);
	app->scene.cam.fov_deg = 70;
	if (!setup_camera(&app->scene.cam))
		return (p_err("miniRT: bad camera"), 0);

	// temp: setup amb and light
	app->scene.amb.ratio = 0.2;
	app->scene.amb.color = norm_rgb(255, 255, 255);
	app->scene.light.pos = v(0, 40, 0);
	app->scene.light.ratio = 0.6;
	app->scene.light.color = norm_rgb(255, 255, 255);

	// setup object
	t_object	sp;
	sp.type = SPHERE;
	sp.center = v(0, 0, 40);
	sp.axis = v(0, 0, 0);
	sp.radius = 20 / 2.0;
	sp.height = 0;
	sp.color = norm_rgb(0, 255, 0);
	sp.next = NULL;
	if (!add_objects(&app->scene.obj, sp))
		return (0);

	// floor: 10 below the camera, normal up, white
	t_object	pl;
	pl.type = PLANE;
	pl.center = v(0, -5, 0);
	pl.axis = v(0, 1, 0); // v(0.707, 0.707, 0)
	pl.radius = 0;
	pl.height = 0;
	pl.color = norm_rgb(255, 255, 255);
	pl.next = NULL;
	if (!add_objects(&app->scene.obj, pl))
		return (0);
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
		return (freeappexit(app, EXIT_FAILURE));

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
