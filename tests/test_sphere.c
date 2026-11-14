#include "test.h"

/* All tests use one sphere: centre 0,0,40, radius 10. */
static t_sphere	test_sp(void)
{
	t_sphere	sp;

	sp.center = v(0, 0, 40);
	sp.radius = 10;
	sp.color = 0;
	return (sp);
}

/*
** Runs hit_sphere on one ray that points along +z. t is preset to 999.
** On a hit, the test compares t with the worked-out distance.
** On a miss, t must stay at 999, because hit_sphere must not write it.
*/
static void	check_hit(const char *label, t_vec src, int want_hit, double want_t)
{
	t_sphere	sp;
	t_ray		ray;
	double		t;
	char		name[64];

	sp = test_sp();
	ray.src = src;
	ray.dir = v(0, 0, 1);
	t = 999;
	snprintf(name, sizeof(name), "%s: return value", label);
	check_int(name, hit_sphere(ray, &sp, &t), want_hit);
	snprintf(name, sizeof(name), "%s: t", label);
	if (want_hit)
		check_double(name, t, want_t, TOL_EXACT);
	else
		check_double(name, t, 999, TOL_EXACT);
}

/*
** Five rays, one for each path through hit_sphere:
** 1. Straight on: the smaller answer is in front of the camera (t = 30).
** 2. From the centre: the smaller answer is behind the camera, so the
**    larger answer is used (t = 10).
** 3. Sphere behind: both answers are behind the camera, so it is a miss.
** 4. Touching the edge: the discriminant is exactly 0, one hit (t = 40).
** 5. Clean miss: the discriminant is less than 0.
*/
static void	test_hit_sphere(void)
{
	check_hit("straight on", v(0, 0, 0), 1, 30);
	check_hit("from the centre", v(0, 0, 40), 1, 10);
	check_hit("sphere behind", v(0, 0, 100), 0, 0);
	check_hit("touching the edge", v(10, 0, 0), 1, 40);
	check_hit("clean miss", v(20, 0, 0), 0, 0);
}

/*
** Normals at three points on the surface: front, top and side.
** Each normal must point from the centre through the hit point.
** Each normal must have a length of 1, because the lighting in M8 needs
** a unit vector.
*/
static void	test_normal(void)
{
	t_sphere	sp;
	t_vec		hit;

	sp = test_sp();
	hit = v(0, 0, 30);
	check_vec("normal front", hit_sphere_normal(hit, &sp), v(0, 0, -1),
		TOL_EXACT);
	hit = v(0, 10, 40);
	check_vec("normal top", hit_sphere_normal(hit, &sp), v(0, 1, 0),
		TOL_EXACT);
	hit = v(10, 0, 40);
	check_vec("normal side", hit_sphere_normal(hit, &sp), v(1, 0, 0),
		TOL_EXACT);
	check_double("normal side has length 1",
		vec_length(hit_sphere_normal(hit, &sp)), 1, TOL_EXACT);
}

int	main(void)
{
	test_hit_sphere();
	test_normal();
	return (test_summary("test_sphere"));
}
