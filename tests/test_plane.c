#include "test.h"

/* Makes a plane through point p with normal n. The colour is not used. */
static t_object	test_pl(t_vec p, t_vec n)
{
	t_object	pl;

	pl.type = PLANE;
	pl.center = p;
	pl.axis = n;
	pl.radius = 0;
	pl.height = 0;
	pl.color = (t_color){0, 0, 0};
	pl.next = NULL;
	return (pl);
}

/*
** Runs hit_plane on one ray. t is preset to 999.
** On a hit, the test compares t with the worked-out distance.
** On a miss, t must stay at 999, because hit_plane must not write it.
*/
static void	check_hit(const char *label, t_ray ray, t_object pl, int want_hit,
	double want_t)
{
	double	t;
	char	name[64];

	t = 999;
	snprintf(name, sizeof(name), "%s: return value", label);
	check_int(name, hit_plane(ray, &pl, &t), want_hit);
	snprintf(name, sizeof(name), "%s: t", label);
	if (want_hit)
		check_double(name, t, want_t, TOL_EXACT);
	else
		check_double(name, t, 999, TOL_EXACT);
}

/*
** Six rays, one for each path through hit_plane. The floor is the plane
** through 0,-10,0. The camera is at the origin, except in case 5.
** 1. In front: the ray goes down, t = -10 / -0.6 = 16.667.
** 2. Normal flipped: the same ray, normal 0,-1,0. The numerator and the
**    denominator both change sign, so t does not change.
** 3. Parallel: the denominator is 0, so it is a miss.
** 4. Behind: the ray goes up, t = -16.667 is negative, so it is a miss.
** 5. Starts on the plane: the numerator is 0, so t = 0. t is not more
**    than EPSILON, so it is a miss. Shadow rays in M9 need this.
** 6. Wall straight on: a plane at z = 100 with normal 0,0,-1, t = 100.
*/
static void	test_hit_plane(void)
{
	t_object	floor;
	t_ray		ray;

	// 1
	floor = test_pl(v(0, -10, 0), v(0, 1, 0));
	ray.src = v(0, 0, 0);
	ray.dir = v(0, -0.6, 0.8);
	check_hit("in front", ray, floor, 1, 50.0 / 3.0);	
	// 2
	check_hit("normal flipped", ray,
		test_pl(v(0, -10, 0), v(0, -1, 0)), 1, 50.0 / 3.0);	
	// 3
	ray.dir = v(0, 0, 1);
	check_hit("parallel", ray, floor, 0, 0);
	// 4
	ray.dir = v(0, 0.6, 0.8);
	check_hit("behind", ray, floor, 0, 0);
	// 5
	ray.src = v(0, -10, 0);
	ray.dir = v(0, -0.6, 0.8);
	check_hit("starts on the plane", ray, floor, 0, 0);
	// 6
	ray.src = v(0, 0, 0);
	ray.dir = v(0, 0, 1);
	check_hit("wall straight on", ray,
		test_pl(v(0, 0, 100), v(0, 0, -1)), 1, 100);
}

int	main(void)
{
	test_hit_plane();
	return (test_summary("test_plane"));
}
