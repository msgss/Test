#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "common.h"


void Led0_ON(void)
{
	rt_pin_write(LED0_PIN, PIN_HIGH);
}
void Led0_OFF(void)
{
	rt_pin_write(LED0_PIN, PIN_LOW);
}

void Led0_toggle(void)
{
	static rt_uint8_t sta;
	if(sta)
	{
		sta=0;
		rt_pin_write(LED0_PIN, PIN_HIGH);
	}
	else
	{
		sta=1;
		rt_pin_write(LED0_PIN, PIN_LOW);
	}
}
