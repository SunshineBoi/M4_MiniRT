#include "mrt.h"

int	handle_close(void *param)
{
	t_app	*app;

	app = (t_app *)param;
	return (freeappexit(app, EXIT_SUCCESS));
}

int	handle_keyboard(int keycode, void *param)
{
	t_app	*app;

	app = (t_app *)param;
	// esc key
	if (keycode == XK_Escape)
		return (freeappexit(app, EXIT_SUCCESS));
	// app->img_obj->is_dirty = 1;
	return (0);
}

int	handle_expose(void *param)
{
	t_app	*app;

	app = (t_app *)param;
	mlx_put_image_to_window(app->mlx_obj->conn_ptr, app->mlx_obj->win_ptr, app->img_obj->img_ptr, 0, 0);
	return (0);
}

void	setup_events(t_app *app)
{
	void	*win_ptr;

	win_ptr = (void *)app->mlx_obj->win_ptr;
	// DestroyNotify - mouse click on window close button
	mlx_hook(win_ptr, DestroyNotify, NoEventMask, handle_close, app);
	// ConfigureNotify - resizing window size
	// mlx_hook(win_ptr, ConfigureNotify, StructureNotifyMask, handle_resize, data);

	// pan + zoom
	// mlx_hook(win_ptr, ButtonPress, ButtonPressMask, handle_mouse_click, data);
	// mlx_hook(win_ptr, ButtonRelease, ButtonReleaseMask, handle_mouse_release, data);
	// mlx_hook(win_ptr, MotionNotify, PointerMotionMask, handle_mouse_move, data);

	// shorthand for event 2 - KeyPress
	mlx_key_hook(win_ptr, handle_keyboard, app);

	// to handle window resize, so it doesnt blacked out the screen.
	mlx_expose_hook(win_ptr, handle_expose, app);

	// detect dirty image and perform rerender
	// mlx_loop_hook(data->mlx_obj->conn_ptr, handle_loop_hook, data);
}
