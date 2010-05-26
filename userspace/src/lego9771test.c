#include "lego9771.h"
#include <unistd.h>
#include <stdio.h>

int main()
{
	int sensor_state = -1, new_sensor_state, out_state = 0;
	if (!L9771_init())
		return 1;
	
	while ((1)) {
		new_sensor_state = L9771_get() & LEGO_9771_IN_BIT_MASK;
		if (new_sensor_state != sensor_state)
			printf("Sensors: %d %d\n", (new_sensor_state & 0x40) != 0, (new_sensor_state & 0x40) != 0);
		sensor_state = new_sensor_state;
		L9771_set(out_state);
		++out_state;
		sleep(1);
	}
}