#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include "mqttclient.h"
#include "common.h"

static mqtt_client_t *client = NULL;

#define  DICONNET_TIME_MAX   (60*2)   //

//Ö÷Ìâ
static char tx_topic[20];//????
static char rx_topic[20];//????

//
int UserPocketDataProc(unsigned char* pBuff, unsigned int Len)
{
   //rt_kprintf("\r\n %s",pBuff);
		
	 if(pBuff[0]='o'&&pBuff[1]=='n')
	 {
		  rt_kprintf("open \r\n");
		  Led0_ON();
	 }	
	 if(pBuff[0]='o'&&pBuff[1]=='f'&&pBuff[2]=='f')
	 {
		  rt_kprintf("close \r\n");
		  Led0_OFF();
	 }		 
   return 1;
}

static void mqtt_subscribe_topic_handle(void* client, message_data_t* msg)
{
    rt_kprintf("\r\n topic: %s\n",msg->topic_name);
    UserPocketDataProc((rt_uint8_t*)msg->message->payload,msg->message->payloadlen);
}

static void my_mqtt_thread(void *parameter)
{
    struct gmsg* msg_ptr;
    rt_uint8_t mqtt_times=0;
    rt_uint16_t  times=0;
    rt_uint16_t  diconnet_times=0;
    rt_uint16_t  i=0;

   
	  mqtt_sleep_ms(10000); //must wait to wifi is Ok 
	
    mqtt_log_init();
    client = mqtt_lease();
    mqtt_set_host(client, "bemfa.com");
    mqtt_set_port(client, "9501");
//    mqtt_set_user_name(client, "abuser");
//    mqtt_set_password (client, "abpassword");

    mqtt_set_client_id(client, "f60e98deb0ff4a6596a5c61d81a4ccce");
    mqtt_set_clean_session(client, 1);

    mqtt_set_keep_alive_interval(client, 20);
    mqtt_set_cmd_timeout   (client, 4000);
    mqtt_set_read_buf_size (client, 2048);
    mqtt_set_write_buf_size(client, 2048);
    do
    {
      if(mqtt_connect(client)==RT_NULL)
      {
         rt_kprintf("mqtt_connet ok\r\n");
        
         break;
      }
      mqtt_sleep_ms(5000);  
      rt_kprintf("mqtt_connet time =%d\r\n",++mqtt_times);

      if(mqtt_times>(DICONNET_TIME_MAX/5)) rt_hw_cpu_reset();

    }while(1);
 
    mqtt_subscribe(client,"led002",QOS0,mqtt_subscribe_topic_handle ); //????
    diconnet_times=0;
    while(1)
    {
       mqtt_sleep_ms(5000);  
       if(client->mqtt_client_state!=CLIENT_STATE_CONNECTED)//mqtt tcp???
       {
          if(++diconnet_times>=DICONNET_TIME_MAX)
          {
            diconnet_times=0;
						//rt_kprintf("\r\n rt_hw_cpu_reset");
            //rt_hw_cpu_reset();//????

          }
          rt_kprintf("mqtt diconnet_times=%d\r\n",diconnet_times);
       }
    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
}

//mqtt ??????
static int ka_mqtt(void)
{
    rt_thread_t   tid_mqtt;
    tid_mqtt = rt_thread_create("mqtt_thread", my_mqtt_thread, RT_NULL, 4096, 17, 10);
    if (tid_mqtt == RT_NULL) 
    {
        return -RT_ERROR;
    }
    rt_thread_startup(tid_mqtt);

    return RT_EOK;
}
INIT_APP_EXPORT(ka_mqtt);

