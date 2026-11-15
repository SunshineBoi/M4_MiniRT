#include "mrt.h"

// in 1-byte: r = 0xFF (hex) / 255 (dec)
// int has 4-bytes, doing bitshift to form the full integer.
int	rgb_to_int(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

t_color	norm_rgb(int r, int g, int b)
{
	t_color	color;

	color.r = r / 255.0;
	color.g = g / 255.0;
	color.b = b / 255.0;
	return (color);
}

int	norm_rgb_to_int(t_color c)
{
	int	r;
	int	g;
	int	b;

	// product of color with light results in range of [0.0, 2.0]
	// therefore, need to clip to 1.0
	r = fmin(fmax(c.r, 0), 1) * 255;
	g = fmin(fmax(c.g, 0), 1) * 255;
	b = fmin(fmax(c.b, 0), 1) * 255;
	return (rgb_to_int(r, g, b));
}
