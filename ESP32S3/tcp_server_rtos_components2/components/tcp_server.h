#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "tcp_server.h"

void init_tcp(void);    //tcp初始化函数，需放在main()函数里
void do_retransmit(const int sock);   //信息接收函数
void tcp_server_task(void *pvParameters);

extern int len;
extern char rx_buffer[128];    //接收数据的存储数组

#define PORT                        CONFIG_EXAMPLE_PORT
#define KEEPALIVE_IDLE              CONFIG_EXAMPLE_KEEPALIVE_IDLE
#define KEEPALIVE_INTERVAL          CONFIG_EXAMPLE_KEEPALIVE_INTERVAL
#define KEEPALIVE_COUNT             CONFIG_EXAMPLE_KEEPALIVE_COUNT

#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#endif 
