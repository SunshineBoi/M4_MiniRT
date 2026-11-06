#include "test.h"

/* Unset fields are preset to 999 so a silent failure shows up in the output. */
static t_camera	test_cam(t_vec fwd)
{
	t_camera	cam;

	cam.pos = v(-50, 0, 20);
	cam.fwd = fwd;
	cam.right = v(999, 999, 999);
	cam.top = v(999, 999, 999);
	cam.fov_deg = 70;
	cam.scale = 999;
	cam.aspect = 999;
	return (cam);
}

/* Runs setup on one fwd, then compares right and top with worked-out answers. */
static void	check_axes(const char *label, t_vec fwd, t_vec right, t_vec top)
{
	t_camera	cam;
	char		name[64];

	cam = test_cam(fwd);
	snprintf(name, sizeof(name), "%s: setup returns 1", label);
	check_int(name, setup_camera(&cam), 1);
	snprintf(name, sizeof(name), "%s: right", label);
	check_vec(name, cam.right, right, TOL_EXACT);
	snprintf(name, sizeof(name), "%s: top", label);
	check_vec(name, cam.top, top, TOL_EXACT);
}

/*
** Known cameras with answers worked out by hand: level, tilted, the two cases
** that need the fallback, turned, and a fwd that needs normalizing.
*/
static void	test_axes(void)
{
	check_axes("level", v(0, 0, 1), v(-1, 0, 0), v(0, 1, 0));
	check_axes("tilted", v(0, 0.6, 0.8), v(-1, 0, 0), v(0, 0.8, -0.6));
	check_axes("straight up", v(0, 1, 0), v(1, 0, 0), v(0, 0, 1));
	check_axes("straight down", v(0, -1, 0), v(-1, 0, 0), v(0, 0, 1));
	check_axes("facing +x", v(1, 0, 0), v(0, 0, 1), v(0, 1, 0));
	check_axes("non-unit fwd", v(0, 0, 2), v(-1, 0, 0), v(0, 1, 0));
}

/*
** For a slanted camera there's no easy answer to compare with, so test the
** rules instead: all three axes have length 1, and each pair has a dot product
** of 0, meaning right angles.
*/
static void	test_axes_are_a_frame(void)
{
	t_camera	cam;

	cam = test_cam(v(0.577, 0.577, 0.577));
	check_int("slanted setup returns 1", setup_camera(&cam), 1);
	check_double("fwd length 1", vec_length(cam.fwd), 1, TOL_EXACT);
	check_double("right length 1", vec_length(cam.right), 1, TOL_EXACT);
	check_double("top length 1", vec_length(cam.top), 1, TOL_EXACT);
	check_double("right, fwd at right angles", vec_dot(cam.right, cam.fwd), 0,
		TOL_EXACT);
	check_double("top, fwd at right angles", vec_dot(cam.top, cam.fwd), 0,
		TOL_EXACT);
	check_double("right, top at right angles", vec_dot(cam.right, cam.top), 0,
		TOL_EXACT);
}

/*
** scale should be tan(35 degrees), about 0.70. aspect should be a real
** fraction, which also catches integer division (1500 / 900 would give 1).
*/
static void	test_scale_aspect(void)
{
	t_camera	cam;

	cam = test_cam(v(0, 0, 1));
	setup_camera(&cam);
	check_double("scale at FOV 70", cam.scale, 0.70, TOL_ROUNDED);
	check_double("aspect is WIN_WIDTH / WIN_HEIGHT", cam.aspect,
		1500.0 / 900.0, TOL_EXACT);
}

/*
** A zero or NaN fwd has no valid direction. Setup must return 0, and right must
** still hold the 999 marker, proving nothing was written on the way out.
*/
static void	test_bad_input(void)
{
	t_camera	cam;

	cam = test_cam(v(0, 0, 0));
	check_int("zero fwd returns 0", setup_camera(&cam), 0);
	check_vec("zero fwd leaves right unset", cam.right, v(999, 999, 999),
		TOL_EXACT);
	cam = test_cam(v(NAN, 0, 1));
	check_int("NaN fwd returns 0", setup_camera(&cam), 0);
	check_vec("NaN fwd leaves right unset", cam.right, v(999, 999, 999),
		TOL_EXACT);
}

/*
** Full worked examples: the top-left corner and one pixel away from the centre,
** compared with hand-calculated directions. src must be the camera position.
*/
static void	test_single_pixels(void)
{
	t_camera	cam;
	t_ray		ray;

	cam = test_cam(v(0, 0, 1));
	setup_camera(&cam);
	ray = set_ray(&cam, 0, 0);
	check_vec("pixel (0, 0) src", ray.src, v(-50, 0, 20), TOL_EXACT);
	check_vec("pixel (0, 0) dir", ray.dir, v(0.54, 0.33, 0.78), TOL_ROUNDED);
	ray = set_ray(&cam, 225, 1125);
	check_vec("pixel (1125, 225) dir", ray.dir, v(-0.33, 0.19, 0.93),
		TOL_ROUNDED);
}

/*
** Centre: the middle of an even size falls between pixels, so take the two
** diagonal neighbours beside it. They must mirror each other in x and y. Adding
** them cancels x and y and leaves only z, so a + b points along fwd, and
** normalizing shrinks that length of about 2 down to 1.
** Edges: the dot product of two unit vectors is the cosine of their angle, so
** acos gives the angle. Left to right should be about the 70 degree FOV.
*/
static void	test_centre_and_edges(void)
{
	t_camera	cam;
	t_ray		a;
	t_ray		b;
	t_vec		mid;

	cam = test_cam(v(0, 0, 1));
	setup_camera(&cam);
	a = set_ray(&cam, 449, 749);
	b = set_ray(&cam, 450, 750);
	check_vec("middle pixels sit beside fwd", a.dir, v(0, 0, 1), TOL_ROUNDED);
	check_double("middle pixels mirror in x", a.dir.x, -b.dir.x, TOL_EXACT);
	check_double("middle pixels mirror in y", a.dir.y, -b.dir.y, TOL_EXACT);
	check_int("their average normalizes", vec_normalize(vec_add(a.dir, b.dir),
			&mid), 1);
	check_vec("their average is fwd", mid, v(0, 0, 1), TOL_EXACT);
	a = set_ray(&cam, 450, 0);
	b = set_ray(&cam, 450, 1499);
	check_double("edges are about 70 degrees apart",
		acos(vec_dot(a.dir, b.dir)) * 180 / M_PI, 70, 0.1);
}

/*
** Checks only the sign, to catch an upside-down or mirrored image. Rows count
** downward, so row 0 must look up. Facing +z, right is -x, so the left column
** must look along +x and the right column along -x.
*/
static void	test_image_direction(void)
{
	t_camera	cam;

	cam = test_cam(v(0, 0, 1));
	setup_camera(&cam);
	check_int("top row looks up", set_ray(&cam, 0, 750).dir.y > 0, 1);
	check_int("bottom row looks down", set_ray(&cam, 899, 750).dir.y < 0, 1);
	check_int("left column looks along +x, as right is -x",
		set_ray(&cam, 450, 0).dir.x > 0, 1);
	check_int("right column looks along -x",
		set_ray(&cam, 450, 1499).dir.x < 0, 1);
}

/*
** Position and direction are separate jobs. Moving the camera must change only
** src, with no new setup. Turning it changes fwd, so setup runs again and the
** same pixel must get a new dir.
*/
static void	test_moved_or_turned(void)
{
	t_camera	cam;
	t_ray		before;
	t_ray		after;

	cam = test_cam(v(0, 0, 1));
	setup_camera(&cam);
	before = set_ray(&cam, 225, 1125);
	cam.pos = v(0, 0, 0);
	after = set_ray(&cam, 225, 1125);
	check_vec("moving keeps dir", after.dir, before.dir, TOL_EXACT);
	check_vec("moving changes src", after.src, v(0, 0, 0), TOL_EXACT);
	cam = test_cam(v(1, 0, 0));
	setup_camera(&cam);
	after = set_ray(&cam, 225, 1125);
	check_vec("turning to +x", after.dir, v(0.93, 0.19, 0.33), TOL_ROUNDED);
}

/* Counts bad pixels, so one bug gives one failure instead of a million. */
static void	check_every_pixel(const char *label, t_camera *cam)
{
	int		col;
	int		row;
	int		bad;
	t_ray	ray;

	bad = 0;
	col = 0;
	while (col < WIN_WIDTH)
	{
		row = 0;
		while (row < WIN_HEIGHT)
		{
			ray = set_ray(cam, row, col);
			if (!(fabs(vec_length(ray.dir) - 1) < TOL_EXACT))
				bad++;
			row++;
		}
		col++;
	}
	check_int(label, bad, 0);
}

/*
** Every ray must have length 1, or t stops being a real distance. Runs the full
** image on the hard cameras: slanted, straight up, and the FOV limits 0 and 180.
*/
static void	test_every_pixel(void)
{
	t_camera	cam;

	cam = test_cam(v(0.577, 0.577, 0.577));
	setup_camera(&cam);
	check_every_pixel("all pixels unit length, slanted", &cam);
	cam = test_cam(v(0, 1, 0));
	setup_camera(&cam);
	check_every_pixel("all pixels unit length, straight up", &cam);
	cam = test_cam(v(0, 0, 1));
	cam.fov_deg = 0;
	setup_camera(&cam);
	check_every_pixel("all pixels unit length, FOV 0", &cam);
	cam = test_cam(v(0, 0, 1));
	cam.fov_deg = 180;
	setup_camera(&cam);
	check_every_pixel("all pixels unit length, FOV 180", &cam);
}

int	main(void)
{
	test_axes();
	test_axes_are_a_frame();
	test_scale_aspect();
	test_bad_input();
	test_single_pixels();
	test_centre_and_edges();
	test_image_direction();
	test_moved_or_turned();
	test_every_pixel();
	return (test_summary("test_camera"));
}
