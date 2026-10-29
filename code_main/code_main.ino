/*
  int16_t x1, y1;
  uint16_t w, h;
  char tekst[] = "1234drink";
  display.getTextBounds(tekst,100,100,&x1,&y1,&w,&h);

  full refresh diplay
  display.setFullWindow();
*/
#include <vector>
#include <GxEPD2_BW.h>
#include <stdint.h>
#include <Fonts/FreeMonoBold9pt7b.h>  //height: 9px
#include <Fonts/Org_01.h>
#include <Fonts/Picopixel.h>
#include <Fonts/Tiny3x3a2pt7b.h>
#include <Fonts/TomThumb.h>
#include <memory>
//#include <Adafruit_TinyUSB.h>

#define CS_PIN (D1)
#define BUSY_PIN (D4)
#define RES_PIN (D3)
#define DC_PIN (D2)

// 4.2'' EPD Module
GxEPD2_BW<GxEPD2_420_GDEY042T81, GxEPD2_420_GDEY042T81::HEIGHT> display(GxEPD2_420_GDEY042T81(/*CS=*/CS_PIN, /*DC=*/DC_PIN, /*RES=*/RES_PIN, /*BUSY=*/BUSY_PIN));  // 400x300, SSD1683



#define MAX_SINGLE_EVENTS 16
#define MAX_ALLDAY_EVENTS 4

struct CalendarSingleEvent {
  int start_min;
  int end_min;
  String event_name;
  bool is_displayed = false;
};

struct CalendarAllDay {
  int days_left;
  String allday_name;
};

std::vector<CalendarSingleEvent> v_single_events;
std::vector<CalendarAllDay> v_allday_events;


void update_events() {
  // Create a vector called cars that will store strings

  CalendarSingleEvent ev_1 = { 720, 780, "event first one event" };
  CalendarSingleEvent ev_2 = { 795, 825, "event 2" };
  CalendarSingleEvent ev_3 = { 825, 945, "event three" };
  CalendarSingleEvent ev_4 = { 975, 990, "event fououor" };
  v_single_events.push_back(ev_1);
  v_single_events.push_back(ev_2);
  v_single_events.push_back(ev_3);
  v_single_events.push_back(ev_4);

  int index = 0;
  for (auto i : v_single_events) {
    Serial.print("event name num ");
    Serial.print(index);
    Serial.println(i.event_name);
    index++;
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  pinMode(LED_BUILTIN, OUTPUT);
  display.setTextColor(GxEPD_BLACK);
  display.init(115200, true, 50, false);
  display.setRotation(0);
  display.firstPage();
  delay(1000);

  drawTopPanel();
  update_events();
  
  for (int i = 700; i < 1000; i += 3) {
    drawEventBox(i);
    for (auto i : v_single_events) {
      i.is_displayed = false;
      
    }
    delay(1000);
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

#define BOX_EVENT_X_POS 0
#define BOX_EVENT_Y_POS 39
#define BOX_EVENT_WIDTH 200
#define BOX_EVENT_HEIGHT (300 - 38)


void drawEventBox(uint16_t minute) {
  display.setPartialWindow(BOX_EVENT_X_POS, BOX_EVENT_Y_POS, BOX_EVENT_WIDTH, BOX_EVENT_HEIGHT);

  display.fillRect(BOX_EVENT_X_POS, BOX_EVENT_Y_POS, BOX_EVENT_WIDTH, BOX_EVENT_HEIGHT, GxEPD_WHITE);
  display.drawLine(BOX_EVENT_X_POS, BOX_EVENT_Y_POS + 60, BOX_EVENT_X_POS+BOX_EVENT_WIDTH, BOX_EVENT_Y_POS + 60, GxEPD_BLACK);
  display.drawLine(BOX_EVENT_X_POS, BOX_EVENT_Y_POS + 60, BOX_EVENT_X_POS+BOX_EVENT_WIDTH, BOX_EVENT_Y_POS + 60, GxEPD_BLACK);

  //draw hours
  display.setFont(&TomThumb);
  display.setTextSize(2);

  int start_minute = minute - 60;

  for (int min = start_minute; min < start_minute + 262; min++) {
    int minuteDifference = min - start_minute;
    // draw 15min line
    if (min % 15 == 0) {
      display.drawLine(BOX_EVENT_X_POS+40, BOX_EVENT_Y_POS + minuteDifference, BOX_EVENT_X_POS+45, BOX_EVENT_Y_POS + minuteDifference, GxEPD_BLACK);
    }
    
    // Draw hour line and hour itself
    if (min % 60 == 0) {  
      int actualHour = min / 60;
      display.drawLine(BOX_EVENT_X_POS, BOX_EVENT_Y_POS + minuteDifference, BOX_EVENT_X_POS+45, BOX_EVENT_Y_POS + minuteDifference, GxEPD_BLACK);

      display.setCursor(BOX_EVENT_X_POS + 1, BOX_EVENT_Y_POS + minuteDifference + 12);
      if (actualHour < 10) {
        display.print("0");
        display.print(actualHour);
      } else {
        display.print(actualHour);
      }
      display.print(":00");
    }
    for (auto i : v_single_events) {
      if(i.start_min == min && i.is_displayed == false){
        int event_y_start = BOX_EVENT_Y_POS + minuteDifference;
        int event_y_end = event_y_start + (i.end_min - i.start_min);
        int event_x_start = BOX_EVENT_X_POS + 50;
        int event_x_end = BOX_EVENT_X_POS + BOX_EVENT_WIDTH-1;
        //Draw a box
        display.drawLine(event_x_start, event_y_start,event_x_start,event_y_end,GxEPD_BLACK);
        display.drawLine(event_x_start,event_y_end, event_x_end, event_y_end, GxEPD_BLACK);
        display.drawLine(event_x_end, event_y_end, event_x_end, event_y_start, GxEPD_BLACK);
        display.drawLine(event_x_end, event_y_start, event_x_start, event_y_start, GxEPD_BLACK);
        i.is_displayed = true;
      }
      
    }
    /*
    for (int y = 0; y < 10; y++) {
      if (single_events[y].start_min == i && single_events[y].is_displayed == false) {
        //Serial.println("start min");
        //Serial.println(i);
        int event_pos_start = 38 + i - start_minute;
        int event_pos_end = 38 + i - start_minute + (single_events[y].end_min - single_events[y].start_min);

        display.drawLine(60, event_pos_start, width, event_pos_start, GxEPD_BLACK);
        display.drawLine(60, event_pos_end, width, event_pos_end, GxEPD_BLACK);
        display.drawLine(60, event_pos_start, 60, event_pos_end, GxEPD_BLACK);
        display.drawLine(width, event_pos_start, width, event_pos_end, GxEPD_BLACK);

        display.setCursor(60, event_pos_start + 13);
        display.print(single_events[y].event_name);
        single_events[y].is_displayed = true;
      }
      if (single_events[y].end_min == i && single_events[y].is_displayed == false) {
        //Serial.println("this is the end");
        //Serial.println(i);
        int event_pos_start = 38;
        int event_pos_end = 38 + i - start_minute;
        /////    38 + 180 - 140 +80
        display.drawLine(60, event_pos_start, width, event_pos_start, GxEPD_BLACK);
        display.drawLine(60, event_pos_end, width, event_pos_end, GxEPD_BLACK);
        display.drawLine(60, event_pos_start, 60, event_pos_end, GxEPD_BLACK);
        display.drawLine(width, event_pos_start, width, event_pos_end, GxEPD_BLACK);

        if ((event_pos_end - 38) > 13) {
          display.setCursor(60, 38 + 12);
          display.print(single_events[y].event_name);
        } else {
          display.setCursor(60, event_pos_end - 13);
          display.print(single_events[y].event_name);
        }

        single_events[y].is_displayed = true;
      }
    }*/
  }

  display.nextPage();
}


#define BOX_PANEL_X_POS 0
#define BOX_PANEL_Y_POS 0
#define BOX_PANEL_WIDTH 400
#define BOX_PANEL_HEIGHT 38

void drawTopPanel() {
  display.setPartialWindow(BOX_PANEL_X_POS, BOX_PANEL_Y_POS, BOX_PANEL_WIDTH, BOX_PANEL_HEIGHT);
  display.fillRect(BOX_PANEL_X_POS, BOX_PANEL_Y_POS, BOX_PANEL_WIDTH, BOX_PANEL_HEIGHT, GxEPD_WHITE);
  
  display.drawLine(BOX_PANEL_X_POS, BOX_PANEL_HEIGHT + BOX_PANEL_Y_POS, BOX_PANEL_X_POS + BOX_PANEL_WIDTH , BOX_PANEL_HEIGHT + BOX_PANEL_Y_POS, GxEPD_BLACK);
  display.drawLine(BOX_PANEL_X_POS, BOX_PANEL_HEIGHT + BOX_PANEL_Y_POS +1, BOX_PANEL_X_POS + BOX_PANEL_WIDTH, BOX_PANEL_HEIGHT + BOX_PANEL_Y_POS +1, GxEPD_BLACK);

  uint16_t title_width;
  char text[] = "Thursday, Oct 29";
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextSize(2);
  display.getTextBounds(text, 0, 100, NULL, NULL, &title_width, NULL);

  display.setCursor(((BOX_PANEL_X_POS +  BOX_PANEL_WIDTH) /2)- (title_width / 2), BOX_PANEL_Y_POS + BOX_PANEL_HEIGHT);

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