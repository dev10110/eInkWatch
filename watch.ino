// "watch.ino" by Devansh Agrawal
// Adapted from "GxEPD2_HelloWorld.ino by Jean-Marc Zingg"

// Uses the library GxEPD2: https://github.com/ZinggJM/GxEPD2
// which can be installed directly from inside the arduino package manager

/******* TIME *****/
#include <TimeLib.h>

time_t RTCTime;

// use internal RTC of the teensy
time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

/******* DISPLAY *****/
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h> // not sure why this is needed
#include <Fonts/FreeSans18pt7b.h>
#include "GxEPD2_display_selection_new_style.h"

// See other font choices here: https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts



GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS*/ 10, /*DC*/ 14, /*RST*/ 15, /*BUSY*/ 16));


/******* SETUP *****/
void setup()
{
  Serial.begin(115200);
  Serial.println("\n" __FILE__ " " __DATE__ " " __TIME__);
  setSyncProvider(getTeensy3Time);
  
  display.init();
  displaySetup();
  displayTime(false);
  // display.hibernate();
}

void displaySetup(){
    display.setRotation(1);
  display.setFont(&FreeSans18pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
}


/******* LOOP *****/
void loop() {
  displayTime(true);
  delay(20*1000); // update the watch every 20 seconds
};






/******* HELPERS *****/

// grab the current time, and display it
void displayTime(bool mode){
  int h = hour();
  int m = minute();
  int s = second();

  int dd = day();
  int mm = month();
//  int yy = year();
  int ww = weekday();

  char text[40];

  sprintf(text, " %d:%.2d\n %d %s\n %s", h, m, dd, month2month(mm), day2day(ww)); 
  // sprintf(text, " %d:%.2d:%.2d\n %d %s\n %s", h, m, s, dd, month2month(mm), day2day(ww));
  displayText(text, mode);
}


// display any arbitrary text string
// if mode = false, will update the entire screen
// if mode = true, will perform partial update, only updating the part of the screen that has changed
void displayText(char* data, bool mode){

  int16_t tbx, tby; 
  uint16_t tbw, tbh;
  display.getTextBounds(data, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t x = 0;
  uint16_t y = ((display.height() - tbh) / 2) - tby;

    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(data);
    display.display(mode);

}


char* day2day(int d){
  switch(d){
    case 1: return "Sunday";
    case 2: return "Monday";
    case 3: return "Tuesday";
    case 4: return "Wednesday";
    case 5: return "Thursday";
    case 6: return "Friday";
    case 7: return "Saturday";
    default: return "";
  }
}

char* month2month(int m){
  switch(m){
    case 1: return "Jan";
    case 2: return "Feb";
    case 3: return "Mar";
    case 4: return "Apr";
    case 5: return "May";
    case 6: return "Jun";
    case 7: return "Jul";
    case 8: return "Aug";
    case 9: return "Sep";
    case 10: return "Oct";
    case 11: return "Nov";
    case 12: return "Dec";
    default: return "";
  }
}

