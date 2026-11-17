#include "test.h"

/*
** Compares two colours channel by channel.
** The label of each check shows the channel: "label: r", "label: g", "label: b".
*/
static void	check_color(const char *label, t_color got, t_color want,
	double tol)
{
	char	name[80];

	snprintf(name, sizeof(name), "%s: r", label);
	check_double(name, got.r, want.r, tol);
	snprintf(name, sizeof(name), "%s: g", label);
	check_double(name, got.g, want.g, tol);
	snprintf(name, sizeof(name), "%s: b", label);
	check_double(name, got.b, want.b, tol);
}

/*
** The test scene: white ambient 0.2, a white light of brightness 0.6
** at 0,40,0, and a red hit at 0,0,30 with the normal 0,0,-1.
** This is the front of the M5 sphere. Each case changes only what it tests.
*/
static void	test_scene(t_app *app, t_hit *hit)
{
	app->scene.amb.ratio = 0.2;
	app->scene.amb.color = (t_color){1, 1, 1};
	app->scene.light.pos = v(0, 40, 0);
	app->scene.light.ratio = 0.6;
	app->scene.light.color = (t_color){1, 1, 1};
	hit->dist_to_hit = 30;
	hit->hit_vec = v(0, 0, 30);
	hit->normal_at_hit = v(0, 0, -1);
	hit->color = (t_color){1, 0, 0};
}

/*
** norm_rgb divides each channel by 255.
** 142 checks that the division keeps the decimals (no integer division).
** 0 and 255 check both ends of the range.
*/
static void	test_norm_rgb(void)
{
	check_color("norm_rgb 255,0,0", norm_rgb(255, 0, 0),
		(t_color){1, 0, 0}, TOL_EXACT);
	check_color("norm_rgb 142,128,254", norm_rgb(142, 128, 254),
		(t_color){142 / 255.0, 128 / 255.0, 254 / 255.0}, TOL_EXACT);
}

/*
** norm_rgb_to_int clamps each channel to 0..1, multiplies it by 255,
** and drops the decimals.
** 1. One full channel gives 0xFF in its byte only.
** 2. 0.56 gives 142.8, and the cast makes it 142 (0x8E).
** 3. Values above 1 become 0xFF and do not spill into the next channel.
** 4. A value below 0 becomes 0x00. 0.5 gives 127.5, so 127 (0x7F).
*/
static void	test_norm_rgb_to_int(void)
{
	check_int("to_int one full channel",
		norm_rgb_to_int((t_color){1, 0, 0}), 0xFF0000);
	check_int("to_int drops decimals",
		norm_rgb_to_int((t_color){0.56, 0, 0}), 0x8E0000);
	check_int("to_int clamps above 1",
		norm_rgb_to_int((t_color){2, 2, 2}), 0xFFFFFF);
	check_int("to_int clamps below 0",
		norm_rgb_to_int((t_color){-0.5, 0.5, 1}), 0x007FFF);
}

/*
** shade, with the values worked out by hand on the P1M8 page.
** 1. Worked example: dot(N, L) = 0.6, so red = 0.2 + 0.6 * 0.6 = 0.56.
** 2. Facing the light: the light is at the camera, dot = 1, red = 0.8.
** 3. Facing away: the light is behind the sphere, dot = -1. The floor at 0
**    keeps the ambient part, so red = 0.2 and not less.
** 4. Coloured ambient: blue ambient on a red object gives nothing, and the
**    light is behind, so the result is black. This also checks that each
**    channel uses its own ambient value.
** 5. Light at the hit point: vec_normalize fails, so the diffuse part is 0.
** 6. Needs clamping: ambient 1 and light 1 on a white object give 2 per
**    channel. shade does not clamp, norm_rgb_to_int does.
*/
static void	test_shade(void)
{
	t_app	app;
	t_hit	hit;

	// 1
	test_scene(&app, &hit);
	check_color("shade worked example", shade(&app.scene, hit),
		(t_color){0.56, 0, 0}, TOL_EXACT);
	// 2
	app.scene.light.pos = v(0, 0, 0);
	check_color("shade facing the light", shade(&app.scene, hit),
		(t_color){0.8, 0, 0}, TOL_EXACT);
	// 3
	app.scene.light.pos = v(0, 0, 100);
	check_color("shade facing away", shade(&app.scene, hit),
		(t_color){0.2, 0, 0}, TOL_EXACT);
	// 4
	app.scene.amb.ratio = 1;
	app.scene.amb.color = (t_color){0, 0, 1};
	check_color("shade coloured ambient", shade(&app.scene, hit),
		(t_color){0, 0, 0}, TOL_EXACT);
	// 5
	test_scene(&app, &hit);
	app.scene.light.pos = v(0, 0, 30);
	check_color("shade light at the hit point", shade(&app.scene, hit),
		(t_color){0.2, 0, 0}, TOL_EXACT);
	// 6
	test_scene(&app, &hit);
	app.scene.amb.ratio = 1;
	app.scene.light.pos = v(0, 0, 0);
	app.scene.light.ratio = 1;
	hit.color = (t_color){1, 1, 1};
	check_color("shade needs clamping", shade(&app.scene, hit),
		(t_color){2, 2, 2}, TOL_EXACT);
	check_int("shade needs clamping: pixel",
		norm_rgb_to_int(shade(&app.scene, hit)), 0xFFFFFF);
}

int	main(void)
{
	test_norm_rgb();
	test_norm_rgb_to_int();
	test_shade();
	return (test_summary("test_light"));
}
