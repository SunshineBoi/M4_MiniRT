#include "test.h"
#include <dirent.h>
#include <fcntl.h>
#include <string.h>

#define GOOD "scenes/good/"
#define BAD "scenes/bad/"

/*
** Runs parse_scene on path with a fresh scene.
** stderr goes to /dev/null during the call, so that the expected
** error messages do not fill the test output.
*/
static int	load(const char *path, t_scene *scene)
{
	int	saved;
	int	null_fd;
	int	ok;

	*scene = (t_scene){0};
	fflush(stderr);
	saved = dup(2);
	null_fd = open("/dev/null", O_WRONLY);
	dup2(null_fd, 2);
	close(null_fd);
	ok = parse_scene((char *)path, scene);
	dup2(saved, 2);
	close(saved);
	return (ok);
}

/* Returns the number of objects in the list. */
static int	count_objects(t_object *obj)
{
	int	n;

	n = 0;
	while (obj)
	{
		n++;
		obj = obj->next;
	}
	return (n);
}

/* Returns the first object of the type, or NULL. */
static t_object	*find_type(t_object *obj, t_type type)
{
	while (obj && obj->type != type)
		obj = obj->next;
	return (obj);
}

/*
** parse_double, one call per row of the number edge cases.
** 1. Accept: a sign, digits, a dot with digits on both sides. Check the value.
** 2. Reject: a dot at one end, text after the number, no digit,
**    two signs, an exponent, hex, a comma, and 400 digits (inf).
*/
static void	test_parse_double(void)
{
	const char	*bad[] = {".5", "1.", "1.2.3", "12abc", "1e5", "0x10", "1,5",
		"-", "+", "--1", "+-1", "", " 1", "1 ", NULL};
	char		big[402];
	double		d;
	int			i;

	check_int("double 0", parse_double("0", &d) && d == 0, 1);
	check_int("double -50", parse_double("-50", &d) && d == -50, 1);
	check_int("double +1", parse_double("+1", &d) && d == 1, 1);
	check_int("double 007", parse_double("007", &d) && d == 7, 1);
	check_int("double -0", parse_double("-0", &d), 1);
	parse_double("0.2", &d);
	check_double("double 0.2", d, 0.2, TOL_EXACT);
	parse_double("-40.25", &d);
	check_double("double -40.25", d, -40.25, TOL_EXACT);
	i = 0;
	while (bad[i])
	{
		check_int(bad[i], parse_double((char *)bad[i], &d), 0);
		i++;
	}
	memset(big, '9', 400);
	big[400] = '\0';
	check_int("double 400 nines (inf)", parse_double(big, &d), 0);
}

/*
** parse_byte and parse_color.
** 1. parse_byte accepts 0 and 255, and leading zeros (0255 is 255).
** 2. parse_byte rejects 256, a sign, a dot and an empty string.
** 3. parse_color stores each part divided by 255.
** 4. parse_color rejects a bad part, two or four parts, and a comma at an
**    end or next to another comma. On a failure *dest does not change.
*/
static void	test_parse_color(void)
{
	const char	*bad[] = {"256,0,0", "-1,0,0", "1.5,0,0", "+1,0,0",
		"255,255", "255,255,255,0", "255,,255,0", "255,255,255,",
		",255,255,255", "255,,255", "a,0,0", NULL};
	t_color		c;
	int			b;
	int			i;

	check_int("byte 0", parse_byte("0", &b) && b == 0, 1);
	check_int("byte 255", parse_byte("255", &b) && b == 255, 1);
	check_int("byte 0255", parse_byte("0255", &b) && b == 255, 1);
	check_int("byte 256", parse_byte("256", &b), 0);
	check_int("byte -1", parse_byte("-1", &b), 0);
	check_int("byte +1", parse_byte("+1", &b), 0);
	check_int("byte 1.0", parse_byte("1.0", &b), 0);
	check_int("byte empty", parse_byte("", &b), 0);
	check_int("color 255,128,0", parse_color("255,128,0", &c), 1);
	check_vec("color 255,128,0 value", (t_vec){c.r, c.g, c.b},
		v(1, 128 / 255.0, 0), TOL_EXACT);
	i = 0;
	while (bad[i])
	{
		c = (t_color){7, 7, 7};
		check_int(bad[i], parse_color((char *)bad[i], &c), 0);
		check_vec("color unchanged after a failure", (t_vec){c.r, c.g, c.b},
			v(7, 7, 7), TOL_EXACT);
		i++;
	}
}

/*
** parse_vector and is_unit_vector.
** 1. parse_vector reads three numbers with two commas.
** 2. parse_vector rejects two or four parts, empty parts and a bad number.
** 3. is_unit_vector accepts a length within 0.001 of 1 (0.707,0.707,0).
** 4. is_unit_vector rejects 0,0,0, other lengths, and a part over 1.
*/
static void	test_parse_vector(void)
{
	const char	*bad[] = {"1,2", "1,2,3,4", "1,,2,3", "1,2,3,", ",1,2,3",
		"1,,3", "1,abc,3", "", NULL};
	t_vec		vec;
	int			i;

	check_int("vector -50,0,20", parse_vector("-50,0,20", &vec), 1);
	check_vec("vector -50,0,20 value", vec, v(-50, 0, 20), TOL_EXACT);
	check_int("vector 0.0,0.0,20.6", parse_vector("0.0,0.0,20.6", &vec), 1);
	check_vec("vector 0.0,0.0,20.6 value", vec, v(0, 0, 20.6), TOL_EXACT);
	i = 0;
	while (bad[i])
	{
		check_int(bad[i], parse_vector((char *)bad[i], &vec), 0);
		i++;
	}
	check_int("unit 0,0,1", is_unit_vector(v(0, 0, 1)), 1);
	check_int("unit 0,-1,0", is_unit_vector(v(0, -1, 0)), 1);
	check_int("unit 0.577 x3", is_unit_vector(v(0.577, 0.577, 0.577)), 1);
	check_int("unit 0.707,0.707,0", is_unit_vector(v(0.707, 0.707, 0)), 1);
	check_int("unit 0,0,0", is_unit_vector(v(0, 0, 0)), 0);
	check_int("unit 0,2,0", is_unit_vector(v(0, 2, 0)), 0);
	check_int("unit 0.5,0.5,0", is_unit_vector(v(0.5, 0.5, 0)), 0);
	check_int("unit 0.57 x3", is_unit_vector(v(0.57, 0.57, 0.57)), 0);
	check_int("unit 1.0005,0,0", is_unit_vector(v(1.0005, 0, 0)), 0);
}

/*
** ft_split_set on a scene line with mixed separators.
** 1. Spaces, tabs and '\r' at the start, the end and in runs make no
**    empty field.
** 2. The fields come back in order, and the array ends in NULL.
** 3. An empty string and a string of separators give an empty array.
*/
static void	test_split(void)
{
	char	**f;

	f = ft_split_set("  sp\t0,0,0  \t12 \r", " \t\r");
	check_int("split count", count_lst(f), 3);
	check_int("split field 0", strcmp(f[0], "sp"), 0);
	check_int("split field 1", strcmp(f[1], "0,0,0"), 0);
	check_int("split field 2", strcmp(f[2], "12"), 0);
	free_list(f);
	f = ft_split_set("", " ");
	check_int("split empty string", count_lst(f), 0);
	free_list(f);
	f = ft_split_set(" \t \r", " \t\r");
	check_int("split only separators", count_lst(f), 0);
	free_list(f);
}

/*
** Checks every value of the subject's example scene in *s.
** The light colour is not checked. The mandatory part does not use it.
** The objects can be in any order, so each one is found by its type.
*/
static void	check_subject(const char *name, t_scene *s)
{
	t_object	*o;
	char		label[96];

	snprintf(label, sizeof(label), "%s: 3 objects", name);
	check_int(label, count_objects(s->obj), 3);
	snprintf(label, sizeof(label), "%s: A, C, L", name);
	check_vec(label, v(s->amb.ratio, s->cam.fov_deg, s->light.ratio),
		v(0.2, 70, 0.6), TOL_EXACT);
	check_vec(label, (t_vec){s->amb.color.r, s->amb.color.g,
		s->amb.color.b}, v(1, 1, 1), TOL_EXACT);
	check_vec(label, s->cam.pos, v(-50, 0, 20), TOL_EXACT);
	check_vec(label, s->cam.fwd, v(0, 0, 1), TOL_EXACT);
	check_vec(label, s->light.pos, v(-40, 50, 0), TOL_EXACT);
	o = find_type(s->obj, SPHERE);
	snprintf(label, sizeof(label), "%s: sp", name);
	check_int(label, o != NULL, 1);
	if (o)
	{
		check_vec(label, o->center, v(0, 0, 20.6), TOL_EXACT);
		check_double(label, o->radius, 6.3, TOL_EXACT);
		check_vec(label, (t_vec){o->color.r, o->color.g, o->color.b},
			v(10 / 255.0, 0, 1), TOL_EXACT);
	}
	o = find_type(s->obj, PLANE);
	snprintf(label, sizeof(label), "%s: pl", name);
	check_int(label, o != NULL, 1);
	if (o)
	{
		check_vec(label, o->center, v(0, 0, -10), TOL_EXACT);
		check_vec(label, o->axis, v(0, 1, 0), TOL_EXACT);
		check_vec(label, (t_vec){o->color.r, o->color.g, o->color.b},
			v(0, 0, 225 / 255.0), TOL_EXACT);
	}
	o = find_type(s->obj, CYLINDER);
	snprintf(label, sizeof(label), "%s: cy", name);
	check_int(label, o != NULL, 1);
	if (o)
	{
		check_vec(label, o->center, v(50, 0, 20.6), TOL_EXACT);
		check_vec(label, o->axis, v(0, 0, 1), TOL_EXACT);
		check_double(label, o->radius, 7.1, TOL_EXACT);
		check_double(label, o->height, 21.42, TOL_EXACT);
		check_vec(label, (t_vec){o->color.r, o->color.g, o->color.b},
			v(10 / 255.0, 0, 1), TOL_EXACT);
	}
}

/*
** The subject's example, written in five ways, gives the same scene.
** 1. As in the subject.
** 2. The lines in reverse order.
** 3. Extra spaces, tabs and blank lines.
** 4. Windows line endings.
** 5. No newline after the last line.
** The defense copy in ../scenes/subject.rt must load the same way.
*/
static void	test_subject_files(void)
{
	const char	*files[] = {GOOD "subject.rt", GOOD "reverse.rt",
		GOOD "spaces.rt", GOOD "crlf.rt", GOOD "no_eol.rt",
		"../scenes/subject.rt", NULL};
	t_scene		s;
	int			i;

	i = 0;
	while (files[i])
	{
		check_int(files[i], load(files[i], &s), 1);
		check_subject(files[i], &s);
		free_objects(s.obj);
		i++;
	}
}

/*
** Good files at the edges of the rules.
** 1. FOV 0 and FOV 180 are in the subject's range, so they load.
** 2. A, C and L with no object load, with an empty object list.
** 3. 400 spheres load. The file is bigger than BUFFER_SIZE, so read_file
**    grows its buffer.
** 4. Each file in ../scenes/ (the defense scenes) loads.
*/
static void	test_good_edges(void)
{
	const char	*defense[] = {"../scenes/basic.rt", "../scenes/sphere.rt",
		"../scenes/plane.rt", "../scenes/cylinder.rt", NULL};
	t_scene		s;
	int			i;

	check_int("fov 0", load(GOOD "fov0.rt", &s) && s.cam.fov_deg == 0, 1);
	free_objects(s.obj);
	check_int("fov 180", load(GOOD "fov180.rt", &s)
		&& s.cam.fov_deg == 180, 1);
	free_objects(s.obj);
	check_int("no objects", load(GOOD "no_objects.rt", &s), 1);
	check_int("no objects: empty list", s.obj == NULL, 1);
	free_objects(s.obj);
	check_int("400 spheres", load(GOOD "many.rt", &s), 1);
	check_int("400 spheres: count", count_objects(s.obj), 400);
	free_objects(s.obj);
	i = 0;
	while (defense[i])
	{
		check_int(defense[i], load(defense[i], &s), 1);
		free_objects(s.obj);
		i++;
	}
}

/*
** Every file in scenes/bad/ must give 0. The test reads the folder, so a
** new bad file is tested with no change here.
** The objects added before the error are freed here, as freeappexit does
** in miniRT. Memcheck then proves that each failure path frees its own
** memory (lines, fields and the file text).
*/
static void	test_bad_files(void)
{
	DIR				*dir;
	struct dirent	*e;
	t_scene			s;
	char			path[300];
	int				n;

	dir = opendir(BAD);
	check_int("open " BAD, dir != NULL, 1);
	n = 0;
	while (dir && (e = readdir(dir)))
	{
		if (e->d_name[0] == '.')
			continue ;
		snprintf(path, sizeof(path), BAD "%s", e->d_name);
		check_int(path, load(path, &s), 0);
		free_objects(s.obj);
		n++;
	}
	if (dir)
		closedir(dir);
	check_int("bad files found", n >= 40, 1);
}

/*
** File names that validate_file_path or open must reject:
** no ".rt" at the end, ".rt" with no name, the wrong case, and a file
** that does not exist.
*/
static void	test_bad_names(void)
{
	const char	*bad[] = {"", "scene.txt", "scene.rt.txt", ".rt", "scenes/.rt",
		"scene.RT", "rt", "missing.rt", NULL};
	t_scene		s;
	int			i;

	i = 0;
	while (bad[i])
	{
		check_int(bad[i], load(bad[i], &s), 0);
		free_objects(s.obj);
		i++;
	}
}

int	main(void)
{
	test_parse_double();
	test_parse_color();
	test_parse_vector();
	test_split();
	test_subject_files();
	test_good_edges();
	test_bad_files();
	test_bad_names();
	return (test_summary("test_parse"));
}
