#include "lego9771.h"
#include <sys/io.h>
#include <stdio.h>
#include <errno.h>
#define LEGO_9771_IO_PORT 925

int L9771_init(void)
{
	int res = ioperm(LEGO_9771_IO_PORT, 1, 1);
	if (res == 0)
		return 1;
	switch (errno)
	{
		case EINVAL:
			fprintf(stderr, "ioperm(%x, 1, 1): EINVAL\n", LEGO_9771_IO_PORT);
			break;
		case EIO:
			fprintf(stderr, "ioperm(%x, 1, 1): EIO\n", LEGO_9771_IO_PORT);
			break;
		case EPERM:
			fprintf(stderr, "ioperm(%x, 1, 1): EPERM\n", LEGO_9771_IO_PORT);
			break;
		default:
			fprintf(stderr, "ioperm(%x, 1, 1): unknown error %d\n", LEGO_9771_IO_PORT, errno);
			break;
	}
	return 0;
}
void L9771_set(int out_bits)
{
	int in_bits = L9771_get() & LEGO_9771_IN_BIT_MASK;
	out_bits &= LEGO_9771_OUT_BIT_MASK;
	outb_p(out_bits | in_bits, LEGO_9771_IO_PORT);
}
int L9771_get(void)
{
	inb_p(LEGO_9771_IO_PORT);
}
