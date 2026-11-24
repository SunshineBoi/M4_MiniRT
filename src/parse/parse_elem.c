#include "mrt.h"

/*
Parses "A <ratio> <r,g,b>". The ratio must be in 0 to 1.
Returns 0 if A is already set or the data is bad.
*/
int	parse_ambient(char **fields, t_scene *scene)
{
	int	ok;

	if (scene->a_flag)
		return (p_err("miniRT: ambient defined twice"), 0);
	scene->a_flag = 1;
	ok = (count_lst(fields) == 2
			&& parse_double(fields[0], &scene->amb.ratio)
			&& is_in_range(scene->amb.ratio, 0, 1)
			&& parse_color(fields[1], &scene->amb.color));
	if (!ok)
		p_err("miniRT: invalid ambient data");
	return (ok);
}

/*
Parses "C <x,y,z> <direction> <fov>".
The direction must be a unit vector. The FOV must be in 0 to 180.
Returns 0 if C is already set or the data is bad.
*/
int	parse_camera(char **fields, t_scene *scene)
{
	int	ok;

	if (scene->c_flag)
		return (p_err("miniRT: camera defined twice"), 0);
	scene->c_flag = 1;
	ok = (count_lst(fields) == 3
			&& parse_vector(fields[0], &scene->cam.pos)
			&& parse_vector(fields[1], &scene->cam.fwd)
			&& is_unit_vector(scene->cam.fwd)
			&& parse_double(fields[2], &scene->cam.fov_deg)
			&& is_in_range(scene->cam.fov_deg, 0, 180));
	if (!ok)
		p_err("miniRT: invalid camera data");
	return (ok);
}

/*
Parses "L <x,y,z> <ratio> <r,g,b>". The ratio must be in 0 to 1.
Returns 0 if L is already set or the data is bad.
*/
int	parse_light(char **fields, t_scene *scene)
{
	int	ok;

	if (scene->l_flag)
		return (p_err("miniRT: light defined twice"), 0);
	scene->l_flag = 1;
	ok = (count_lst(fields) == 3
			&& parse_vector(fields[0], &scene->light.pos)
			&& parse_double(fields[1], &scene->light.ratio)
			&& is_in_range(scene->light.ratio, 0, 1)
			&& parse_color(fields[2], &scene->light.color));
	if (!ok)
		p_err("miniRT: invalid light data");
	return (ok);
}
