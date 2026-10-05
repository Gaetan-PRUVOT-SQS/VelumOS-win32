#include "luna_int.h"

uint32_t	lp_hash(uint32_t x, uint32_t y, uint32_t seed)
{
	uint32_t	h;

	h = x * 374761393u + y * 668265263u + seed * 2246822519u;
	h = (h ^ (h >> 13)) * 1274126177u;
	return (h ^ (h >> 16));
}

static int32_t	smooth(int32_t f)
{
	return ((f * f * (768 - 2 * f)) >> 16);
}

int32_t	lp_noise1(int32_t x256, uint32_t seed)
{
	uint32_t	cell;
	int32_t		f;
	int32_t		a;
	int32_t		b;

	cell = (uint32_t)x256 >> 8;
	f = smooth(x256 & 255);
	a = (int32_t)(lp_hash(cell, 0, seed) & 255);
	b = (int32_t)(lp_hash(cell + 1, 0, seed) & 255);
	return (a + (b - a) * f / 256);
}

int32_t	lp_noise2(int32_t x256, int32_t y256, uint32_t seed)
{
	uint32_t	cx;
	uint32_t	cy;
	int32_t		fx;
	int32_t		top;
	int32_t		bot;

	cx = (uint32_t)x256 >> 8;
	cy = (uint32_t)y256 >> 8;
	fx = smooth(x256 & 255);
	top = (int32_t)(lp_hash(cx, cy, seed) & 255);
	top += ((int32_t)(lp_hash(cx + 1, cy, seed) & 255) - top) * fx / 256;
	bot = (int32_t)(lp_hash(cx, cy + 1, seed) & 255);
	bot += ((int32_t)(lp_hash(cx + 1, cy + 1, seed) & 255) - bot) * fx / 256;
	return (top + (bot - top) * smooth(y256 & 255) / 256);
}

int32_t	lp_fbm(int32_t x256, int32_t y256, uint32_t seed)
{
	int32_t	n;

	n = lp_noise2(x256, y256, seed) * 4;
	n += lp_noise2(x256 * 2, y256 * 2, seed + 1) * 3;
	n += lp_noise2(x256 * 4, y256 * 4, seed + 2) * 2;
	return (n / 9);
}
