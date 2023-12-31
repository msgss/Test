/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-02     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include "mqttclient.h"
#include "common.h"

#define LED_RUN_PIN  GET_PIN(I, 8)


void Led_run_toggle(void)
{
	static rt_uint8_t sta;
	if(sta)
	{
		sta=0;
		rt_pin_write(LED_RUN_PIN, PIN_HIGH);
	}
	else
	{
		sta=1;
		rt_pin_write(LED_RUN_PIN, PIN_LOW);
	}
}

extern void wlan_autoconnect_init(void);

int main(void)
{
  //  rt_uint32_t count = 1;
    rt_pin_mode(LED0_PIN,    PIN_MODE_OUTPUT);
	  rt_pin_mode(LED_RUN_PIN, PIN_MODE_OUTPUT);

    wlan_autoconnect_init();
    rt_wlan_config_autoreconnect(RT_TRUE);

	  rt_kprintf("soft:01\r\n");
    while(1)
    {
			  Led_run_toggle();
        rt_thread_mdelay(200);
    }
    return RT_EOK;
}

#include "stm32h7xx.h"
static int vtor_config(void)
{
    /* Vector Table Relocation in Internal QSPI_FLASH */
    SCB->VTOR = QSPI_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);





