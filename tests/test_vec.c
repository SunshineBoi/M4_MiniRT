#include "test.h"

static void	test_basic_ops(void)
{
	check_vec("add", vec_add(v(1, 2, 3), v(4, 5, 6)), v(5, 7, 9), TOL_EXACT);
	check_vec("sub", vec_sub(v(4, 5, 6), v(1, 2, 3)), v(3, 3, 3), TOL_EXACT);
	check_vec("scale", vec_scale(v(1, -2, 3), 2), v(2, -4, 6), TOL_EXACT);
	check_vec("scale by 0", vec_scale(v(1, 2, 3), 0), v(0, 0, 0), TOL_EXACT);
	check_double("dot", vec_dot(v(1, 2, 3), v(4, 5, 6)), 32, TOL_EXACT);
	check_double("dot at right angles", vec_dot(v(1, 0, 0), v(0, 1, 0)), 0,
		TOL_EXACT);
}

static void	test_cross(void)
{
	check_vec("cross x, y", vec_cross(v(1, 0, 0), v(0, 1, 0)), v(0, 0, 1),
		TOL_EXACT);
	check_vec("cross y, x flips sign", vec_cross(v(0, 1, 0), v(1, 0, 0)),
		v(0, 0, -1), TOL_EXACT);
	check_vec("cross of parallel is zero", vec_cross(v(0, 2, 0), v(0, 1, 0)),
		v(0, 0, 0), TOL_EXACT);
	check_vec("cross general", vec_cross(v(1, 2, 3), v(4, 5, 6)),
		v(-3, 6, -3), TOL_EXACT);
}

static void	test_length_normalize(void)
{
	t_vec	out;

	check_double("length 3, 4, 0", vec_length(v(3, 4, 0)), 5, TOL_EXACT);
	check_double("length zero", vec_length(v(0, 0, 0)), 0, TOL_EXACT);
	out = v(9, 9, 9);
	check_int("normalize returns 1", vec_normalize(v(3, 4, 0), &out), 1);
	check_vec("normalize value", out, v(0.6, 0.8, 0), TOL_EXACT);
	out = v(9, 9, 9);
	check_int("normalize zero returns 0", vec_normalize(v(0, 0, 0), &out), 0);
	check_vec("normalize zero leaves out", out, v(9, 9, 9), TOL_EXACT);
	check_int("normalize tiny returns 0", vec_normalize(v(1e-10, 0, 0), &out),
		0);
	check_int("normalize NaN returns 0", vec_normalize(v(NAN, 0, 1), &out), 0);
	check_vec("normalize NaN leaves out", out, v(9, 9, 9), TOL_EXACT);
	check_int("normalize in place returns 1", vec_normalize(out, &out), 1);
	check_double("normalize in place has length 1", vec_length(out), 1,
		TOL_EXACT);
}

int	main(void)
{
	test_basic_ops();
	test_cross();
	test_length_normalize();
	return (test_summary("test_vec"));
}
