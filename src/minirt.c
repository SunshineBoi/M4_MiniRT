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
	new->scene.a_flag = 0;
	new->scene.c_flag = 0;
	new->scene.l_flag = 0;
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

int	minirt(char *file_path)
{
	t_app	*app;

	app = init_app();
	if (!app)
		return (EXIT_FAILURE);
	if (!parse_scene(file_path, &app->scene))
		return (freeappexit(app, EXIT_FAILURE));
	if (!setup_camera(&app->scene.cam))
		return (p_err("miniRT: bad camera"), freeappexit(app, EXIT_FAILURE));
	app->mlx_obj = setup_mlx();
	if (!app->mlx_obj)
		return (freeappexit(app, EXIT_FAILURE));
	app->img_obj = init_img(app->mlx_obj);
	if (!app->img_obj)
		return (freeappexit(app, EXIT_FAILURE));
	render(app);
	mlx_put_image_to_window(app->mlx_obj->conn_ptr, app->mlx_obj->win_ptr,
		app->img_obj->img_ptr, 0, 0);
	setup_events(app);
	mlx_loop(app->mlx_obj->conn_ptr);
	return (EXIT_SUCCESS);
}
