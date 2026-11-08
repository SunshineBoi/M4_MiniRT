#include "mrt.h"

// in 1-byte: r = 0xFF (hex) / 255 (dec)
// int has 4-bytes, doing bitshift to form the full integer.
int	rgb_to_int(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

int	init_mlx_img(t_mlx *mlx_obj, t_image *img_obj)
{
	img_obj->img_ptr = mlx_new_image(
			mlx_obj->conn_ptr, WIN_WIDTH, WIN_HEIGHT);
	if (!img_obj->img_ptr)
		return (p_err("miniRT: MLX image failure"), -1);
	img_obj->pixel_ptr = mlx_get_data_addr(
			img_obj->img_ptr, &img_obj->bpp, &img_obj->spl, &img_obj->endian);
	if (!img_obj->pixel_ptr)
		return (mlx_destroy_image(mlx_obj->conn_ptr, img_obj->img_ptr),
			p_err("miniRT: MLX image failure"), -1);
	img_obj->is_dirty = 0;
	return (0);
}

t_image	*init_img(t_mlx *mlx_obj)
{
	t_image	*new_img;

	new_img = malloc(sizeof(t_image));
	if (!new_img)
		return (p_syserr(), NULL);
	if (init_mlx_img(mlx_obj, new_img) == -1)
		return (free(new_img), NULL);
	return (new_img);
}

void	color_pixel(t_app *app, int row, int col, int color)
{
	int	offset;
	
	if (col < 0 || col >= WIN_WIDTH || row < 0 || row >= WIN_HEIGHT)
		return ;
	offset = (row * (app->img_obj->spl)) + (col * (app->img_obj->bpp / 8));
	*(unsigned int *)(app->img_obj->pixel_ptr + offset) = color;
}
