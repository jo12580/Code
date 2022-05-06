#include <string.h>
#include <string.h>
#include <stdlib.h>
#include "nvs_flash.h"

#include "tcp_server.h"
#include "rgb_led.h"

void task1()
{
    int cnt[3];
    while(1)    //������ɫ���������
    {
        printf("cnt: %d,%d,%d\n", cnt[0],cnt[1],cnt[2]);
        vTaskDelay(200);//vTaskDelay(1);Ϊ10ms
        cnt[0]=rand()%2+0;
        gpio_set_level(GPIO_OUTPUT_IO_0, cnt[0]);  //�������ŵĵ�ƽ��cntȡ0��1
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
    ����RTOS����ִ�г���
    ����1��task1��������ִ�к���
    ����2��"key"����������������
    ����3��4096�������������ڴ�
    ����4��NULL��������ָ��
    ����5��2�����������ȼ���0��С��
    ����6��NULL���������ش�һ��������Ա㴴����������Ա�����
    */
    xTaskCreate(task1, "key", 4096,NULL, 2, NULL);  
}
