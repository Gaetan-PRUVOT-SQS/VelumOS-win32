#include "luna_int.h"

static int32_t	corner_in(int32_t r8, int32_t x, int32_t y)
{
	int32_t	dx;
	int32_t	dy;

	dx = r8 - x;
	dy = r8 - y;
	return (dx * dx + dy * dy <= r8 * r8);
}

int32_t	lp_corner_cov(int32_t r, int32_t ix, int32_t iy)
{
	int32_t	n;
	int32_t	sx;
	int32_t	sy;

	n = 0;
	sy = 0;
	while (sy < 4)
	{
		sx = 0;
		while (sx < 4)
		{
			n += corner_in(r * 8, ix * 8 + sx * 2 + 1, iy * 8 + sy * 2 + 1);
			sx++;
		}
		sy++;
	}
	return (n);
}
