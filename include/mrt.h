#ifndef MRT_H
# define MRT_H

# include <stdlib.h>
# include <math.h>

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

typedef struct s_vec
{
	double	x;
	double	y;
	double	z;
} t_vec;

typedef struct s_ray
{
	t_vec	src;
	t_vec	dir;
} t_ray;

typedef struct s_camera
{
	t_vec	pos;
	t_vec	fwd;  // camera facing direction, z-axis
	t_vec	right;  // camera x-axis
	t_vec	top;  // camera y-axis
	double	fov_deg;
	double	scale;
	double	aspect;
} t_camera;

typedef struct s_app
{
	t_mlx		*mlx;
	t_camera	cam;
}	t_app;

/*
** ---------------------------------------------------------------------
** vec_ops1.c
** ---------------------------------------------------------------------
*/
t_vec	vec_add(t_vec a, t_vec b);
t_vec	vec_sub(t_vec a, t_vec b);
t_vec	vec_scale(t_vec v, double f);
double	vec_dot(t_vec a, t_vec b);
t_vec	vec_cross(t_vec a, t_vec b);

/*
** ---------------------------------------------------------------------
** vec_ops2.c
** ---------------------------------------------------------------------
*/
t_vec	v(double x, double y, double z);
double	vec_length(t_vec v);
int		vec_normalize(t_vec v, t_vec *out);

/*
** ---------------------------------------------------------------------
** camera.c
** ---------------------------------------------------------------------
*/
t_camera	make_cam(t_vec fwd, double fov);
int		setup_camera(t_camera *cam);
t_ray	set_ray(t_camera *cam, int row, int col);
void	render(t_camera *cam);


#endif
