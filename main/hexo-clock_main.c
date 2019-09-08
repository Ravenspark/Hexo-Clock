
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "led_strip.h"

//TODO: clean up and check what's really needed for ESP32_led_strip library...


#define LED_STRIP_LENGTH CONFIG_LED_STRIP_NUM_PIXELS

static struct led_color_t led_strip_buf_1[LED_STRIP_LENGTH];
static struct led_color_t led_strip_buf_2[LED_STRIP_LENGTH];

int number[][5][3] = {  {{1,1,0}, // 0
                        {1,0,1},
                        {0,0,0},
                        {1,0,1},
                        {1,1,0}},

                        {{0,1,0}, // 1
                        {0,1,0},
                        {0,1,0},
                        {0,1,0},
                        {0,1,0}},

                        {{1,1,0}, // 2
                        {1,0,1},
                        {0,1,0},
                        {0,1,0},
                        {1,1,1}},

                        {{1,1,0}, // 3
                        {1,0,1},
                        {0,1,0},
                        {1,0,1},
                        {1,1,0}},
                        
                        {{1,0,0}, // 4
                        {1,0,1},
                        {1,1,0},
                        {0,0,1},
                        {0,1,0}},
                        
                        {{1,1,0}, // 5
                        {1,0,0},
                        {1,1,0},
                        {0,0,1},
                        {1,1,0}},
                        
                        {{0,1,0}, // 6
                        {0,1,0},
                        {1,1,0},
                        {1,0,1},
                        {1,1,0}},

                        {{1,1,0}, // 7
                        {0,0,1},
                        {0,1,0},
                        {0,1,0},
                        {1,0,0}},
                        
                        {{1,1,0}, // 8
                        {1,0,1},
                        {1,1,0},
                        {1,0,1},
                        {1,1,0}},
                        
                        {{1,1,0}, // 9
                        {1,0,1},
                        {1,1,0},
                        {0,1,0},
                        {1,0,0}}

};

typedef struct color_t {uint8_t r; uint8_t g; uint8_t b} color_t;

int lineLen[] = {3,4,5,4,5,4,3}; //{17,18,19,20,19,18,17};
int lineOff[] = {1,4,8,13,17,22,26}; //{0,17,35,54,74,93,111};
int lineInd[] = {1,0,0,0,0,0,1}; //{1,1,0,0,0,1,1};
void setPixel(int x, int y, struct led_strip_t* p_led_strip, color_t color)
{
    int pid=0;
    //xmax = 20, y =7
    //if(x>=20 || y>=7) return;
    if(x>=5 || y>=7) return;

    if(x>=lineInd[y] && x<lineInd[y]+lineLen[y])
    {
        if(y%2==0)
            pid = lineOff[y]+x-lineInd[y];
        else
            pid = lineOff[y]+lineLen[y]-x+lineInd[y]-1;
    }
    else 
    {
        return;
    }

    led_strip_set_pixel_rgb(p_led_strip,pid,color.r,color.g,color.b);
}

void printNumber(int x, int y, int num, struct led_strip_t* p_led_strip, color_t color)
{
    num = num<=9? num:9;
    for(int x_=0; x_<3; x_++)
    {
        for(int y_=0; y_<5; y_++)
        {
            if(number[num][y_][x_])
            {
              setPixel(x+x_, y+y_, p_led_strip,color);
            }
        }
    }

}

void app_main()
{
    color_t colors[] = {{250,250,0},{0,250,250},{250,0,250},{250,250,250}};

    struct led_strip_t led_strip = {
      .rgb_led_type = NEO_RGB,         	// LED type (see led_strip.h" )
      .rmt_channel = CONFIG_RMT_CHANNEL,// RMT channel, 0 - 7
      .gpio = CONFIG_LED_STRIP_GPIO_PIN,// LED's GPIO pin
      .led_strip_buf_1 = led_strip_buf_1,
      .led_strip_buf_2 = led_strip_buf_2,
      .led_strip_length = LED_STRIP_LENGTH // number of pixels(LEDs) in strand
  };
  led_strip.access_semaphore = xSemaphoreCreateBinary();

  bool led_init_ok = led_strip_init(&led_strip);
  assert(led_init_ok);

  while (true) {
      
      for(int n=0; n<10; n++)
      { 
        for(int i=0; i<LED_STRIP_LENGTH; i++) led_strip_set_pixel_rgb(&led_strip,i,0,0,0);

          printNumber(1,1,n,&led_strip,colors[n%3]);
          led_strip_show(&led_strip); 
          delay(1000);
      }
  }
   
}
