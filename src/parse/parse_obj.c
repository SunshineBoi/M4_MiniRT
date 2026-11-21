#include "mrt.h"

/*
Parses "pl <x,y,z> <normal> <r,g,b>". The normal must be a unit vector.
Adds the plane to the front of scene->obj.
Returns 0 if the data is bad or malloc fails.
*/
int	parse_plane(char **fields, t_scene *scene)
{
	t_object	pl;
	int			ok;

	pl.type = PLANE;
	pl.height = 0;
	pl.radius = 0;
	pl.next = NULL;
	ok = (count_lst(fields) == 3
			&& parse_vector(fields[0], &pl.center)
			&& parse_vector(fields[1], &pl.axis)
			&& is_unit_vector(pl.axis)
			&& parse_color(fields[2], &pl.color));
	if (!ok)
		return (p_err("miniRT: invalid plane data"), 0);
	vec_normalize(pl.axis, &pl.axis);
	return (add_objects(&scene->obj, pl));
}

/*
Parses "sp <x,y,z> <diameter> <r,g,b>". The diameter must be more than 0.
Adds the sphere to the front of scene->obj.
Returns 0 if the data is bad or malloc fails.
*/
int	parse_sphere(char **fields, t_scene *scene)
{
	t_object	sp;
	int			ok;
	double		diameter;

	sp.type = SPHERE;
	sp.axis = v(0, 0, 0);
	sp.height = 0;
	sp.next = NULL;
	ok = (count_lst(fields) == 3
			&& parse_vector(fields[0], &sp.center)
			&& parse_double(fields[1], &diameter)
			&& diameter > 0
			&& parse_color(fields[2], &sp.color));
	if (!ok)
		return (p_err("miniRT: invalid sphere data"), 0);
	sp.radius = diameter / 2;
	return (add_objects(&scene->obj, sp));
}

/*
Parses "cy <x,y,z> <axis> <diameter> <height> <r,g,b>".
The axis must be a unit vector. The diameter and height must be more than 0.
Adds the cylinder to the front of scene->obj.
Returns 0 if the data is bad or malloc fails.
*/
int	parse_cylinder(char **fields, t_scene *scene)
{
	t_object	cy;
	int			ok;
	double		diameter;

	cy.type = CYLINDER;
	cy.next = NULL;
	ok = (count_lst(fields) == 5
			&& parse_vector(fields[0], &cy.center)
			&& parse_vector(fields[1], &cy.axis)
			&& is_unit_vector(cy.axis)
			&& parse_double(fields[2], &diameter)
			&& diameter > 0
			&& parse_double(fields[3], &cy.height)
			&& cy.height > 0
			&& parse_color(fields[4], &cy.color));
	if (!ok)
		return (p_err("miniRT: invalid cylinder data"), 0);
	cy.radius = diameter / 2;
	vec_normalize(cy.axis, &cy.axis);
	return (add_objects(&scene->obj, cy));
}
