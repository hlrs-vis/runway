#include <Arduino.h>
//#include <esp_adc_cal.h>

#include <FastLED.h>

#if 1
# define DBG(...) printf(__VA_ARGS__)
#else
# define DBG(...)
#endif

#define LED 32

static TaskHandle_t main_task_handle;
static TaskHandle_t devsync_task_handle;
static TaskHandle_t ui_task_handle;
static TaskHandle_t blink_task_handle;

SemaphoreHandle_t led_mutex;
#define ACONFIG_STRINGS 8
#define ACONFIG_STRING_LENGTH 100

CRGB test[10];
CRGB acore_framebuf[ACONFIG_STRINGS][ACONFIG_STRING_LENGTH];
CRGB test2[10];

/* turn on / off power supply to leds */
static void led_power(uint8_t on)
{
  /* enable power to leds */
  pinMode(4, OUTPUT);
  if (on)
    digitalWrite(4, 1);
  else
    digitalWrite(4, 0);
}



/* this is called from the arduino core application task once.
 * use it to setup out system (spawning tasks with custom priorities and stack sizes)
 * and then delete ourselves in the arduino loop() function which is called
 * aftrewards
 */
void setup() 
{
  bool cold_start;
  uint32_t err = 0;
  Serial.begin(115200);

  if (esp_reset_reason() != ESP_RST_SW)
  {
    cold_start = true;
    DBG("Cold start\r\n");
  }
  else
  {
    DBG("Warm start\r\n");
    cold_start = false;
  }


  /* configure LED driver */
  FastLED.addLeds<WS2812, 13, RGB>(acore_framebuf[0], ACONFIG_STRING_LENGTH);
  FastLED.addLeds<WS2812, 14, RGB>(acore_framebuf[1], ACONFIG_STRING_LENGTH);
  FastLED.addLeds<WS2812, 27, RGB>(acore_framebuf[2], ACONFIG_STRING_LENGTH);

  FastLED.addLeds<WS2812, 26, RGB>(acore_framebuf[3], ACONFIG_STRING_LENGTH);
  FastLED.addLeds<WS2812, 25, RGB>(acore_framebuf[4], ACONFIG_STRING_LENGTH);
  FastLED.addLeds<WS2812, 33, RGB>(acore_framebuf[5], ACONFIG_STRING_LENGTH);
  
  #define full 128
        fill_solid(acore_framebuf[0], ACONFIG_STRING_LENGTH, CRGB(full,full,full));
        fill_solid(acore_framebuf[1], ACONFIG_STRING_LENGTH, CRGB(full,full,full));
        fill_solid(acore_framebuf[2], ACONFIG_STRING_LENGTH, CRGB(full,full,full));
        fill_solid(acore_framebuf[3], ACONFIG_STRING_LENGTH, CRGB(0,0,full));
        fill_solid(acore_framebuf[4], ACONFIG_STRING_LENGTH, CRGB(0,0,full));
        fill_solid(acore_framebuf[5], ACONFIG_STRING_LENGTH, CRGB(0,0,full));
        for(int l=0;l<14;l++)
        {
          acore_framebuf[1][l]=CRGB(full,0,0);
          acore_framebuf[2][l]=CRGB(full,0,0);
        }
        for(int l=0;l<37;l++)
        {
          acore_framebuf[3][99-l]=CRGB(0,0,0);
        }
  led_power(1);
  delay(100);
  
        FastLED.show();
}

void loop()
{
  for(int i=0;i<100;i++)
  {
      for (int s=0; s<1; s++)
      {
          for(int l=0;l<100;l++)
          {
              if(l<i-20)
               acore_framebuf[s][l]=CRGB(0,0,0);
              else if(l>i)
               acore_framebuf[s][l]=CRGB(0,0,0);
              else
               acore_framebuf[s][l]=CRGB(full,full,full);
          }
      }
      FastLED[0].beginShowLeds();
      FastLED[0].showLedsInternal(128);
      FastLED[0].endShowLeds(acore_framebuf[0]);
      delay(10);
  }
      //for (int s=1; s<ACONFIG_STRINGS; s++)
        fill_solid(acore_framebuf[0], ACONFIG_STRING_LENGTH, CRGB(0,0,0));
      FastLED[0].beginShowLeds();
      FastLED[0].showLedsInternal(128);
      FastLED[0].endShowLeds(acore_framebuf[0]);
  delay(1000);
}



