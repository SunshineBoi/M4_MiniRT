#include "mrt.h"

/*
Returns 1 if file_path ends in ".rt" and has a name before the dot.
Prints an error and returns 0 for NULL, a short path, or "dir/.rt".
*/
static int	validate_file_path(char *file_path)
{
	int	i;

	if (!file_path)
		return (p_err("miniRT: invalid file"), 0);
	i = ft_strlen(file_path);
	if (i < 4)
		return (p_err("miniRT: invalid file"), 0);
	i -= 4;
	if (file_path[i] == '/'
		|| file_path[i + 1] != '.'
		|| file_path[i + 2] != 'r'
		|| file_path[i + 3] != 't')
		return (p_err("miniRT: invalid file"), 0);
	return (1);
}

static t_raw	init_raw(void)
{
	t_raw	raw;

	raw.in = NULL;
	raw.in_size = 0;
	raw.in_alloc_size = 0;
	return (raw);
}

/*
Splits one line into fields and sends them to the parser of its identifier.
The element parser gets the fields after the identifier.
Returns 1 for a blank line. Returns 0 for an unknown identifier or bad data.
*/
int	parse_fields(char *raw_line, t_scene *scene)
{
	char	**field_lst;
	int		ok;

	field_lst = ft_split_set(raw_line, " \t\r");
	if (!field_lst)
		return (0);
	if (!field_lst[0])
		return (free_list(field_lst), 1);
	ok = 0;
	if (ft_strcmp(field_lst[0], "A") == 0)
		ok = parse_ambient(field_lst + 1, scene);
	else if (ft_strcmp(field_lst[0], "C") == 0)
		ok = parse_camera(field_lst + 1, scene);
	else if (ft_strcmp(field_lst[0], "L") == 0)
		ok = parse_light(field_lst + 1, scene);
	else if (ft_strcmp(field_lst[0], "pl") == 0)
		ok = parse_plane(field_lst + 1, scene);
	else if (ft_strcmp(field_lst[0], "sp") == 0)
		ok = parse_sphere(field_lst + 1, scene);
	else if (ft_strcmp(field_lst[0], "cy") == 0)
		ok = parse_cylinder(field_lst + 1, scene);
	else
		p_err("miniRT: unknown identifier");
	free_list(field_lst);
	return (ok);
}

/*
Splits the file text into lines and parses each line.
Stops at the first bad line.
Returns 0 if A, C or L is missing after the last line.
*/
int	parse_objects(char *raw_in, t_scene *scene)
{
	char	**obj_lst;
	int		i;
	int		ok;

	obj_lst = ft_split_set(raw_in, "\n");
	if (!obj_lst)
		return (0);
	ok = 1;
	i = 0;
	while (obj_lst[i])
	{
		ok = parse_fields(obj_lst[i], scene);
		if (!ok)
			break ;
		i++;
	}
	free_list(obj_lst);
	if (ok && (!scene->a_flag || !scene->c_flag || !scene->l_flag))
		return (p_err("miniRT: A, C and L are required"), 0);
	return (ok);
}

/*
Reads the .rt file at file_path into scene.
Returns 0 on any error. The objects added before the error stay in
scene->obj, and freeappexit frees them.
*/
int	parse_scene(char *file_path, t_scene *scene)
{
	t_raw	raw;
	int		fd;
	int		ok;

	if (!validate_file_path(file_path))
		return (0);
	fd = open(file_path, O_RDONLY);
	if (fd < 0)
		return (p_syserr(), 0);
	raw = init_raw();
	ok = read_file(fd, &raw);
	close(fd);
	if (!ok)
		return (free(raw.in), 0);
	ok = parse_objects(raw.in, scene);
	free(raw.in);
	return (ok);
}
