#include <string.h>
#include <string.h>
#include <stdlib.h>
#include "nvs_flash.h"

#include "tcp_server.h"
#include "rgb_led.h"

void task1()
{
    int cnt[3];
    while(1)    //五颜六色的随机出现
    {
        printf("cnt: %d,%d,%d\n", cnt[0],cnt[1],cnt[2]);
        vTaskDelay(200);//vTaskDelay(1);为10ms
        cnt[0]=rand()%2+0;
        gpio_set_level(GPIO_OUTPUT_IO_0, cnt[0]);  //设置引脚的电平，cnt取0或1
        cnt[1]=rand()%2+0;
        gpio_set_level(GPIO_OUTPUT_IO_1, cnt[1]);
        cnt[2]=rand()%2+0;
        gpio_set_level(GPIO_OUTPUT_IO_2, cnt[2]);
    }
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    init_led();
    init_tcp();

    /*
    创建RTOS并发执行程序
    参数1：task1————执行函数
    参数2："key"————函数别名
    参数3：4096————所需内存
    参数4：NULL————指针
    参数5：2————优先级（0最小）
    参数6：NULL————回传一个句柄，以便创建的任务可以被关联
    */
    xTaskCreate(task1, "key", 4096,NULL, 2, NULL);  
}
