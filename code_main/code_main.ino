
// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <stdint.h>
#include <Fonts/FreeMonoBold9pt7b.h>  //height: 9px
#include <Fonts/Org_01.h>
#include <Fonts/Picopixel.h>
#include <Fonts/Tiny3x3a2pt7b.h>
#include <Fonts/TomThumb.h>
#include <Adafruit_TinyUSB.h>

/*
  int16_t x1, y1;
  uint16_t w, h;
  char tekst[] = "1234drink";
  display.getTextBounds(tekst,100,100,&x1,&y1,&w,&h);

  full refresh diplay
  display.setFullWindow();
*/


// ESP32 CS(SS)=5,SCL(SCK)=18,SDA(MOSI)=23,BUSY=4,RES(RST)=17,DC=16
#define CS_PIN (D1)
#define BUSY_PIN (D4)
#define RES_PIN (D3)
#define DC_PIN (D2)


// 4.2'' EPD Module
GxEPD2_BW<GxEPD2_420_GDEY042T81, GxEPD2_420_GDEY042T81::HEIGHT> display(GxEPD2_420_GDEY042T81(/*CS=5*/ CS_PIN, /*DC=*/DC_PIN, /*RES=*/RES_PIN, /*BUSY=*/BUSY_PIN));  // 400x300, SSD1683

struct MyStruct {
  int startMin;
  int endMin;
  String event_name;
  bool displayed = false;
};
MyStruct events_test[10];
int howManyEvents = 4;
void setup() {
  Serial.begin(115200);
  while (!Serial);
  pinMode(LED_BUILTIN, OUTPUT);
  display.setTextColor(GxEPD_BLACK);
  display.init(115200, true, 50, false);
  display.setRotation(0);
  display.firstPage();
  //helloWorld();
  //helloFullScreenPartialMode();
  events_test[0].startMin = 200;
  events_test[0].event_name = "first one";
  events_test[0].endMin = 260;
  events_test[1].startMin = 360;
  events_test[1].event_name = "hello ehlelo  worl";
  events_test[1].endMin = 500;
  events_test[2].startMin = 300;
  events_test[2].event_name = "sth very funny";
  events_test[2].endMin = 350;
  events_test[2].startMin = 300;
  events_test[3].startMin = 100;
  events_test[3].event_name = "previous one";
  events_test[3].endMin = 180;

  delay(1000);

  drawTopPanel();
  
  
  for(int i = 100; i < 400; i+=3){
    drawEventBox(i, 0, 39, 200, (uint16_t)300 - 38);
    for(int y = 0; y < 4; y++){
      events_test[y].displayed = false;
    }
    delay(200);
  }
  //drawHours();
  //draw15minBlock(50, 100);
  //draw15minBlock(50, 15);
  if (display.epd2.hasFastPartialUpdate) {
    delay(1000);
  }
  display.hibernate();
}

const char HelloWorld[] = "Hello World!";
const char HelloWeACtStudio[] = "WeAct Studio";


void drawEventBox(uint16_t minute, uint8_t x, uint8_t event_box_pos_y, uint16_t width, uint16_t height) {
  display.setPartialWindow(0, event_box_pos_y, width, height);
 
  display.fillRect(0, event_box_pos_y, width, height, GxEPD_WHITE);
  
  //draw hours
  display.setFont(&TomThumb);
  display.setTextSize(2);

  int startMinute = minute - 60;

  width = width - 4;
  for (int i = startMinute; i < startMinute + 262; i++) {
    if (i % 15 == 0) {
      display.drawLine(40, 38 + i - startMinute, 46, 38 + i - startMinute, GxEPD_BLACK);
    }
    if (i % 60 == 0) {
      int actualHour = i / 60;
      display.drawLine(40, 2 + 38 + i - startMinute, 46, 2 +38 + i - startMinute, GxEPD_BLACK);
      display.setCursor(1, i - startMinute + 38 + 12);
      if (actualHour < 10) {
        display.print("0");
        display.print(actualHour);
      } else {
        display.print(actualHour);
      }
      display.print(":00");
    }
    
    for (int y = 0; y < howManyEvents; y++) {
      if (events_test[y].startMin == i && events_test[y].displayed == false) {
        Serial.println("start min");
        Serial.println(i);
        int event_pos_start = 38 + i - startMinute;
        int event_pos_end = 38 + i - startMinute + ( events_test[y].endMin - events_test[y].startMin);
        
        display.drawLine(60, event_pos_start, width, event_pos_start, GxEPD_BLACK);
        display.drawLine(60, event_pos_end, width, event_pos_end, GxEPD_BLACK);
        display.drawLine(60, event_pos_start, 60, event_pos_end, GxEPD_BLACK);
        display.drawLine(width, event_pos_start, width, event_pos_end, GxEPD_BLACK);
        
        display.setCursor(60, event_pos_start + 13);
        display.print(events_test[y].event_name);
        events_test[y].displayed = true;
      }
      if(events_test[y].endMin == i && events_test[y].displayed == false){
        Serial.println("this is the end");
        Serial.println(i);
        int event_pos_start = 38 ;
        int event_pos_end = 38 + i - startMinute;
        /////    38 + 180 - 140 +80
        display.drawLine(60, event_pos_start, width, event_pos_start, GxEPD_BLACK);
        display.drawLine(60, event_pos_end, width, event_pos_end, GxEPD_BLACK);
        display.drawLine(60, event_pos_start, 60, event_pos_end, GxEPD_BLACK);
        display.drawLine(width, event_pos_start, width, event_pos_end, GxEPD_BLACK);

        if((event_pos_end - 38) > 13){
          display.setCursor(60, 38 + 12);
          display.print(events_test[y].event_name);
        }else{
          display.setCursor(60, event_pos_end - 13);
          display.print(events_test[y].event_name);
        }

        events_test[y].displayed = true;
      }

    }
  }

  display.nextPage();
}


void drawHours() {

  display.setPartialWindow(0, 0, display.width(), display.height());
  display.setCursor(200, 150);
}
void drawTopPanel() {


  display.setTextColor(GxEPD_BLACK);
  display.setRotation(0);
  display.setPartialWindow(0, 0, display.width(), display.height());
  int line_position_y = 37;
  display.drawLine(0, line_position_y, 400, line_position_y, GxEPD_BLACK);
  display.drawLine(0, line_position_y + 1, 400, line_position_y + 1, GxEPD_BLACK);

  uint16_t title_width;
  char text[] = "Monday, Jun 16";
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextSize(2);
  display.getTextBounds(text, 0, 50, NULL, NULL, &title_width, NULL);

  display.setCursor(200 - (title_width / 2), 25);

  display.print(text);
  display.nextPage();
}


void helloFullScreenPartialMode() {
  //Serial.println("helloFullScreenPartialMode");
  const char fullscreen[] = "full screen update";
  const char fpm[] = "fast partial mode";
  const char spm[] = "slow partial mode";
  const char npm[] = "no partial mode";
  display.setPartialWindow(0, 0, display.width(), display.height());
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  if (display.epd2.WIDTH < 104) display.setFont(0);
  if ((display.epd2.panel == GxEPD2::GDEY029F51H) || (display.epd2.panel == GxEPD2::GDEM0154F51H))
    display.setTextColor(GxEPD_YELLOW);
  else
    display.setTextColor(GxEPD_BLACK);
  const char* updatemode;
  if (display.epd2.hasFastPartialUpdate) {
    updatemode = fpm;
  } else if (display.epd2.hasPartialUpdate) {
    updatemode = spm;
  } else {
    updatemode = npm;
  }
  // do this outside of the loop
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  // center update text
  display.getTextBounds(fullscreen, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t utx = ((display.width() - tbw) / 2) - tbx;
  uint16_t uty = ((display.height() / 4) - tbh / 2) - tby;
  // center update mode
  display.getTextBounds(updatemode, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t umx = ((display.width() - tbw) / 2) - tbx;
  uint16_t umy = ((display.height() * 3 / 4) - tbh / 2) - tby;
  // center HelloWorld
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t hwx = ((display.width() - tbw) / 2) - tbx;
  uint16_t hwy = ((display.height() - tbh) / 2) - tby;
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(hwx, hwy);
    display.print(HelloWorld);
    display.setCursor(utx, uty);
    display.print(fullscreen);
    display.setCursor(umx, umy);
    display.print(updatemode);
  } while (display.nextPage());
  //Serial.println("helloFullScreenPartialMode done");
}

void showPartialUpdate() {
  // some useful background
  //helloWorld();
  // use asymmetric values for test
  uint16_t box_x = 10;
  uint16_t box_y = 15;
  uint16_t box_w = 70;
  uint16_t box_h = 20;
  uint16_t cursor_y = box_y + box_h - 6;
  if (display.epd2.WIDTH < 104) cursor_y = box_y + 6;
  float value = 13.95;
  uint16_t incr = display.epd2.hasFastPartialUpdate ? 1 : 3;
  display.setFont(&FreeMonoBold9pt7b);
  if (display.epd2.WIDTH < 104) display.setFont(0);
  display.setTextColor(GxEPD_BLACK);
  // show where the update box is
  for (uint16_t r = 0; r < 4; r++) {
    display.setRotation(r);
    display.setPartialWindow(box_x, box_y, box_w, box_h);
    display.firstPage();
    do {
      display.fillRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
      //display.fillScreen(GxEPD_BLACK);
    } while (display.nextPage());
    delay(2000);
    display.firstPage();
    do {
      display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    } while (display.nextPage());
    delay(1000);
  }
  //return;
  // show updates in the update box
  /*
  for (uint16_t r = 0; r < 4; r++)
  {
    display.setRotation(r);
    display.setPartialWindow(box_x, box_y, box_w, box_h);
    for (uint16_t i = 1; i <= 10; i += incr)
    {
      display.firstPage();
      do
      {
        display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
        display.setCursor(box_x, cursor_y);
        display.print(value * i, 2);
      }
      while (display.nextPage());
      delay(500);
    }
    delay(1000);
    display.firstPage();
    do
    {
      display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    }
    while (display.nextPage());
    delay(1000);
  }*/
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);
}