#include "rgb_led.h"

static xQueueHandle gpio_evt_queue = NULL;

void gpio_task_example(void* arg);
void IRAM_ATTR gpio_isr_handler(void* arg);
void init_led(void);

void gpio_task_example(void* arg)   //led处理函数，用来打印输出引脚
{
    uint32_t io_num;
    for(;;) 
    {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) 
        {
            printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
        }
    }
}

void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

void init_led() //初始化led程序，需放在main函数里
{
    gpio_config_t io_conf = {};//zero-initialize the config structure.
    io_conf.intr_type = GPIO_INTR_DISABLE; //disable interrupt
    io_conf.mode = GPIO_MODE_OUTPUT;//set as output mode
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;//bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pull_down_en = 0; //disable pull-down mode
    io_conf.pull_up_en = 0;//disable pull-up mode
    gpio_config(&io_conf);//configure GPIO with the given settings
    io_conf.intr_type = GPIO_INTR_POSEDGE;//interrupt of rising edge
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;//bit mask of the pins, use GPIO4/5 here
    io_conf.mode = GPIO_MODE_INPUT;//set as input mode
    io_conf.pull_up_en = 1; //enable pull-up mode

    gpio_config(&io_conf);
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE); //change gpio intrrupt type for one pin

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));//create a queue to handle gpio event from isr
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);//start gpio task

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);//install gpio isr service
    
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);//hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);
    
    gpio_isr_handler_remove(GPIO_INPUT_IO_0);//remove isr handler for gpio number.
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);//hook isr handler for specific gpio pin again
    
    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());
}

// void app_main(void)
// {
//     init_led();
//     int cnt[3];
//     while(1)    //五颜六色的随机出现
//     {
//         printf("cnt: %d,%d,%d\n", cnt[0],cnt[1],cnt[2]);
//         vTaskDelay(100);//vTaskDelay(1);为10ms
//         cnt[0]=rand()%2+0;
//         gpio_set_level(GPIO_OUTPUT_IO_0, cnt[0]);  //设置引脚的电平，cnt取0或1
//         cnt[1]=rand()%2+0;
//         gpio_set_level(GPIO_OUTPUT_IO_1, cnt[1]);
//         cnt[2]=rand()%2+0;
//         gpio_set_level(GPIO_OUTPUT_IO_2, cnt[2]);
//     }
// }
