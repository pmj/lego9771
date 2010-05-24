#ifndef LEGO9771_H
#define LEGO9771_H

#define LEGO_9771_IN_BIT_MASK 0xC0
#define LEGO_9771_OUT_BIT_MASK 0x3F

int L9771_init(void);
void L9771_set(int out_bits);
int L9771_get(void);

#endif
