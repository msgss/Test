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

#define LED_PIN GET_PIN(I, 8)

extern void wlan_autoconnect_init(void);
static int mqtt_connet_init(void);

int main(void)
{
    rt_uint32_t count = 1;
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    /* init Wi-Fi auto connect feature */
    wlan_autoconnect_init();
    /* enable auto reconnect on WLAN device */
    rt_wlan_config_autoreconnect(RT_TRUE);

    //等待网络连接成功后，建立mqtt.
    rt_thread_mdelay(10000);
    mqtt_connet_init();
    rt_uint16_t tick_cnt=0; 
    rt_uint16_t tick_time=0; 
    rt_uint16_t test=0;
    
    rt_kprintf("test02\r\n");
    while(count++)
    {
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_LOW);
        if(++tick_cnt>=3)
        {
            tick_cnt=0;
            rt_kprintf("thread count: %d\n", tick_time++);
        }
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



static void sub_topic_handle1(void* client, message_data_t* msg)
{
    (void) client;
    KAWAII_MQTT_LOG_I("-----------------------------------------------------------------------------------");
    KAWAII_MQTT_LOG_I("%s:%d %s()...\ntopic: %s\nmessage:%s", __FILE__, __LINE__, __FUNCTION__, msg->topic_name, (char*)msg->message->payload);
    KAWAII_MQTT_LOG_I("-----------------------------------------------------------------------------------");
}

static int mqtt_publish_handle1(mqtt_client_t *client)
{
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));

    msg.qos = QOS0;
    msg.payload = (void *)"this is a kawaii mqtt test ...";

    return mqtt_publish(client, "rtt-topic", &msg);
}

static int mqtt_connet_init(void)
{
    mqtt_client_t *client = NULL;
    
    rt_thread_delay(6000);
    
    mqtt_log_init();

    client = mqtt_lease();

    mqtt_set_host(client, "bemfa.com");
    mqtt_set_port(client, "9501");
  //  mqtt_set_user_name(client, "rt-thread");
  //  mqtt_set_password(client, "rt-thread");
    mqtt_set_client_id(client, "5a1a8bfdcbcbe54bbf7fe4aa3a5af2a0");
    mqtt_set_clean_session(client, 1);

    mqtt_connect(client);
    
    mqtt_subscribe(client, "leda006", QOS0, sub_topic_handle1);
    
    while (1) {
        mqtt_publish_handle1(client);
                               
        mqtt_sleep_ms(4 * 1000);
    }
}
