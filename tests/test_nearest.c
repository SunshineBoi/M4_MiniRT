#include "test.h"

/* Sphere: centre 0,0,40, radius 10, green. */
static t_object	test_sp(void)
{
	t_object	sp;

	sp.type = SPHERE;
	sp.center = v(0, 0, 40);
	sp.axis = v(0, 0, 0);
	sp.radius = 10;
	sp.height = 0;
	sp.color = (t_color){0, 1, 0};
	sp.next = NULL;
	return (sp);
}

/* Plane through point p with normal n, white. */
static t_object	test_pl(t_vec p, t_vec n)
{
	t_object	pl;

	pl.type = PLANE;
	pl.center = p;
	pl.axis = n;
	pl.radius = 0;
	pl.height = 0;
	pl.color = (t_color){1, 1, 1};
	pl.next = NULL;
	return (pl);
}

static t_ray	test_ray(t_vec src, t_vec dir)
{
	t_ray	ray;

	ray.src = src;
	ray.dir = dir;
	return (ray);
}

/*
** Puts the objects into a new list. add_objects adds to the front, so
** the list order is the opposite of the order in objs.
** If add_objects fails, the test stops.
*/
static void	make_scene(t_scene *scene, t_object *objs, int count)
{
	int	i;

	scene->obj = NULL;
	i = 0;
	while (i < count)
	{
		if (!add_objects(&scene->obj, objs[i]))
		{
			free_objects(scene->obj);
			exit(1);
		}
		i++;
	}
}

static void	check_color(const char *label, t_color got, t_color want)
{
	check_vec(label, (t_vec){got.r, got.g, got.b},
		(t_vec){want.r, want.g, want.b}, TOL_EXACT);
}

/*
** Runs find_nearest_hit and checks every field of the hit.
** want_hit 0: the return value must be 0. The other fields are not read.
*/
static void	check_nearest(const char *label, t_scene *scene, t_ray ray,
	int want_hit, t_hit want)
{
	t_hit	hit;
	char	name[64];

	snprintf(name, sizeof(name), "%s: return value", label);
	check_int(name, find_nearest_hit(ray, scene, &hit), want_hit);
	if (!want_hit)
		return ;
	snprintf(name, sizeof(name), "%s: t", label);
	check_double(name, hit.dist_to_hit, want.dist_to_hit, TOL_EXACT);
	snprintf(name, sizeof(name), "%s: hit point", label);
	check_vec(name, hit.hit_vec, want.hit_vec, TOL_EXACT);
	snprintf(name, sizeof(name), "%s: normal", label);
	check_vec(name, hit.normal_at_hit, want.normal_at_hit, TOL_EXACT);
	snprintf(name, sizeof(name), "%s: colour", label);
	check_color(name, hit.color, want.color);
}

/*
** The test scene is the sphere and the floor (0,-10,0, normal 0,1,0).
** The camera is at the origin.
** 1. Sphere ahead: the ray along +z hits the sphere at t = 30. The floor
**    is parallel to this ray. The normal is 0,0,-1, the colour is green.
** 2. Same, list reversed: the order of the list does not change the
**    winner.
** 3. Floor below: the ray 0,-0.6,0.8 misses the sphere (it passes 24 units 
**    from the centre -- the ray walks 32 units 
**    (hits the floor at t = 16.667, at 0,-10,13.333) and
**    a perpendicular line connects to center of sphere at t = 24
**    The normal is 0,1,0, the colour is white.
** 4. Nothing hit: the ray 0,0.6,0.8 misses the sphere, and the floor is
**    behind it. The return value is 0.
*/
static void	test_sphere_and_floor(void)
{
	t_object	objs[2];
	t_scene		scene;
	t_hit		sp_hit;
	t_hit		fl_hit;

	// expected hit
	sp_hit = (t_hit){30, {0, 0, 30}, {0, 0, -1}, {0, 1, 0}};
	fl_hit = (t_hit){50.0 / 3.0, {0, -10, 40.0 / 3.0}, {0, 1, 0},
	{1, 1, 1}};
	// creating objects
	objs[0] = test_sp();
	objs[1] = test_pl(v(0, -10, 0), v(0, 1, 0));

	make_scene(&scene, objs, 2);
	// 1
	check_nearest("sphere ahead", &scene,
		test_ray(v(0, 0, 0), v(0, 0, 1)), 1, sp_hit);
	// 3
	check_nearest("floor below", &scene,
		test_ray(v(0, 0, 0), v(0, -0.6, 0.8)), 1, fl_hit);
	// 4
	check_nearest("nothing hit", &scene,
		test_ray(v(0, 0, 0), v(0, 0.6, 0.8)), 0, sp_hit);
	free_objects(scene.obj);

	// 2
	objs[0] = test_pl(v(0, -10, 0), v(0, 1, 0));
	objs[1] = test_sp();
	make_scene(&scene, objs, 2);
	check_nearest("sphere ahead, list reversed", &scene,
		test_ray(v(0, 0, 0), v(0, 0, 1)), 1, sp_hit);
	free_objects(scene.obj);
}

/*
** Checks that the normal is flipped toward the ray.
** 1. Floor normal flipped: the floor alone, with normal 0,-1,0. The ray
**    goes down, so the normal must come back as 0,1,0.
** 2. Camera inside the sphere: the sphere alone, camera at the centre.
**    The ray along +z hits the inner wall at t = 10. The outward normal
**    is 0,0,1, so it must come back as 0,0,-1.
*/
static void	test_flip(void)
{
	t_object	obj;
	t_scene		scene;

	obj = test_pl(v(0, -10, 0), v(0, -1, 0));
	make_scene(&scene, &obj, 1);
	check_nearest("floor normal flipped", &scene,
		test_ray(v(0, 0, 0), v(0, -0.6, 0.8)), 1,
		(t_hit){50.0 / 3.0, {0, -10, 40.0 / 3.0}, {0, 1, 0}, {1, 1, 1}});
	free_objects(scene.obj);

	obj = test_sp();
	make_scene(&scene, &obj, 1);
	check_nearest("camera inside the sphere", &scene,
		test_ray(v(0, 0, 40), v(0, 0, 1)), 1,
		(t_hit){10, {0, 0, 50}, {0, 0, -1}, {0, 1, 0}});
	free_objects(scene.obj);
}

/*
** 1. Sphere in front of a wall (0,0,100, normal 0,0,-1), in both list
**    orders: the ray along +z hits both. The sphere is nearer, so it
**    wins with t = 30 both times.
** 2. Empty list: the return value is 0. The loop does not run, and best
**    stays NULL.
*/
static void	test_order_and_empty(void)
{
	t_object	objs[2];
	t_scene		scene;
	t_hit		sp_hit;

	// 1
	sp_hit = (t_hit){30, {0, 0, 30}, {0, 0, -1}, {0, 1, 0}};
	objs[0] = test_pl(v(0, 0, 100), v(0, 0, -1));
	objs[1] = test_sp();
	make_scene(&scene, objs, 2);
	check_nearest("sphere before wall", &scene,
		test_ray(v(0, 0, 0), v(0, 0, 1)), 1, sp_hit);
	free_objects(scene.obj);

	objs[0] = test_sp();
	objs[1] = test_pl(v(0, 0, 100), v(0, 0, -1));
	make_scene(&scene, objs, 2);
	check_nearest("sphere before wall, reversed", &scene,
		test_ray(v(0, 0, 0), v(0, 0, 1)), 1, sp_hit);
	free_objects(scene.obj);

	// 2
	scene.obj = NULL;
	check_nearest("empty list", &scene,
		test_ray(v(0, 0, 0), v(0, 0, 1)), 0, sp_hit);
}

int	main(void)
{
	test_sphere_and_floor();
	test_flip();
	test_order_and_empty();
	return (test_summary("test_nearest"));
}
