#ifndef TEST_H
# define TEST_H

# include <stdio.h>
# include "mrt.h"

# define TOL_EXACT 1e-9
# define TOL_ROUNDED 0.01

static int	g_pass = 0;
static int	g_fail = 0;

static inline void	report(const char *label, int ok)
{
	if (ok)
		g_pass++;
	else
		g_fail++;
	if (!ok)
		printf("  FAIL  %s\n", label);
}

static inline void	check_int(const char *label, int got, int want)
{
	report(label, got == want);
	if (got != want)
		printf("        got %d, want %d\n", got, want);
}

/* Any comparison with NaN is false, so NaN fails these checks. */
static inline void	check_double(const char *label, double got, double want,
	double tol)
{
	int	ok;

	ok = fabs(got - want) <= tol;
	report(label, ok);
	if (!ok)
		printf("        got %.6f, want %.6f\n", got, want);
}

static inline void	check_vec(const char *label, t_vec got, t_vec want,
	double tol)
{
	int	ok;

	ok = fabs(got.x - want.x) <= tol && fabs(got.y - want.y) <= tol
		&& fabs(got.z - want.z) <= tol;
	report(label, ok);
	if (!ok)
		printf("        got (%.6f, %.6f, %.6f), want (%.6f, %.6f, %.6f)\n",
			got.x, got.y, got.z, want.x, want.y, want.z);
}

/* Returns the exit code for main: 0 only if every check passed. */
static inline int	test_summary(const char *name)
{
	printf("%-12s %d passed, %d failed\n", name, g_pass, g_fail);
	return (g_fail != 0);
}

#endif
