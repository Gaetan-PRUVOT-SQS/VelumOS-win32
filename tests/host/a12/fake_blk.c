#include <string.h>
#include "fake.h"
#include "velum/err.h"

static int	range_ok(t_fakeblk *b, uint64_t lba, uint32_t n)
{
	if (n == 0 || lba >= b->dev.nsectors || n > b->dev.nsectors - lba)
	{
		b->oob++;
		return (0);
	}
	return (1);
}

int	blk_read(t_blkdev *d, uint64_t lba, uint32_t n, void *buf)
{
	t_fakeblk	*b;

	b = d->priv;
	if (!range_ok(b, lba, n) || buf == NULL)
		return (E_IO);
	b->reads++;
	if (b->rfail == 0)
		return (E_IO);
	if (b->rfail > 0)
		b->rfail--;
	memcpy(buf, b->img + lba * d->sector_size, (size_t)n * d->sector_size);
	return (0);
}

int	blk_write(t_blkdev *d, uint64_t lba, uint32_t n, const void *buf)
{
	t_fakeblk	*b;

	b = d->priv;
	if (!range_ok(b, lba, n) || buf == NULL)
		return (E_IO);
	if (d->flags & BLK_RO)
		return (E_PERM);
	b->writes++;
	if (b->wfail == 0)
		return (E_IO);
	if (b->wfail > 0)
		b->wfail--;
	memcpy(b->img + lba * d->sector_size, buf, (size_t)n * d->sector_size);
	return (0);
}

int	blk_flush(t_blkdev *d)
{
	(void)d;
	return (0);
}
