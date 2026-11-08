#include "mrt.h"

void	freemlx(t_mlx *mlx)
{
	if (mlx)
	{
		if (mlx->win_ptr)
		{
			mlx_destroy_window(mlx->conn_ptr, mlx->win_ptr);
		}
		if (mlx->conn_ptr)
		{
			mlx_destroy_display(mlx->conn_ptr);
			free(mlx->conn_ptr);
		}
		free(mlx);
	}
}

int	freeappexit(t_app *app, int code)
{
	if (app->img_obj && app->mlx_obj)
	{
		mlx_destroy_image(app->mlx_obj->conn_ptr, app->img_obj->img_ptr);
		free(app->img_obj);
	}
	if (app->mlx_obj)
		freemlx(app->mlx_obj);
	free(app);
	exit(code);
	return (code);
}
