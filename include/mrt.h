#ifndef MRT_H
# define MRT_H

# include <stdlib.h>
# include <math.h>
# include <stdio.h>
# include <unistd.h>
# include <mlx.h>
# include <X11/X.h>
# include <X11/keysym.h>

# define EPSILON 1e-9
# define WIN_WIDTH 1500
# define WIN_HEIGHT 900

typedef struct s_mlx
{
	void	*conn_ptr;
	void	*win_ptr;
	int		is_clickhold;
	int		mouse_x;
	int		mouse_y;
}	t_mlx;

typedef struct s_image
{
	int		is_dirty;
	int		bpp;
	int		spl;
	int		endian;
	void	*img_ptr;
	char	*pixel_ptr;
}	t_image;

typedef struct s_vec
{
	double	x;
	double	y;
	double	z;
}	t_vec;

typedef struct s_ray
{
	t_vec	src;
	t_vec	dir;
}	t_ray;

typedef struct s_sphere
{
	t_vec	center;
	double	radius;
	int		color;
}	t_sphere;

typedef struct s_camera
{
	t_vec	pos;
	t_vec	fwd;	// camera facing direction, z-axis
	t_vec	right;	// camera x-axis
	t_vec	top;	// camera y-axis
	double	fov_deg;
	double	scale;
	double	aspect;
}	t_camera;

typedef struct s_app
{
	t_mlx		*mlx_obj;
	t_image		*img_obj;
	t_camera	cam;
	t_sphere	sp;  // ! temp
}	t_app;

/*
** ---------------------------------------------------------------------
** minirt.c
** ---------------------------------------------------------------------
*/
t_app		*init_app(void);
t_mlx		*setup_mlx(void);
int			minirt(void);

/*
** ---------------------------------------------------------------------
** events.c
** ---------------------------------------------------------------------
*/
int			handle_close(void *param);
int			handle_keyboard(int keycode, void *param);
int			handle_expose(void *param);
void		setup_events(t_app *app);

/*
** ---------------------------------------------------------------------
** image.c
** ---------------------------------------------------------------------
*/
int			rgb_to_int(int r, int g, int b);
int			init_mlx_img(t_mlx *mlx_obj, t_image *img_obj);
t_image		*init_img(t_mlx *mlx_obj);
void		color_pixel(t_app *app, int row, int col, int color);

/*
** ---------------------------------------------------------------------
** camera.c
** ---------------------------------------------------------------------
*/
int			setup_camera(t_camera *cam);
t_ray		set_ray(t_camera *cam, int row, int col);

/*
** ---------------------------------------------------------------------
** render.c
** ---------------------------------------------------------------------
*/
void		render(t_app *app);

/*
** ---------------------------------------------------------------------
** sphere.c
** ---------------------------------------------------------------------
*/
t_vec		hit_sphere_normal(t_vec hit, t_sphere *sp);
int			hit_sphere(t_ray ray, t_sphere *sp, double *t);

/*
** ---------------------------------------------------------------------
** utils/vec_ops1.c
** ---------------------------------------------------------------------
*/
t_vec		vec_add(t_vec a, t_vec b);
t_vec		vec_sub(t_vec a, t_vec b);
t_vec		vec_scale(t_vec v, double f);
double		vec_dot(t_vec a, t_vec b);
t_vec		vec_cross(t_vec a, t_vec b);

/*
** ---------------------------------------------------------------------
** utils/vec_ops2.c
** ---------------------------------------------------------------------
*/
t_vec		v(double x, double y, double z);
double		vec_length(t_vec v);
int			vec_normalize(t_vec v, t_vec *out);

/*
** ---------------------------------------------------------------------
** utils/free.c
** ---------------------------------------------------------------------
*/
void		freemlx(t_mlx *mlx);
int			freeappexit(t_app *app, int code);

/*
** ---------------------------------------------------------------------
** utils/print.c
** ---------------------------------------------------------------------
*/
void		ft_putstr_fd(char *str, int fd);
void		ft_putchar_fd(char ch, int fd);
void		p_err(char *msg);
void		p_syserr(void);

/*
** ---------------------------------------------------------------------
** utils/str.c
** ---------------------------------------------------------------------
*/
size_t		ft_strlen(const char *str);

#endif
