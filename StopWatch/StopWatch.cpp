#include "StopWatch.h"

#include "MenuScreen.h"
#include "SF_reg_7px.h"
#include "Shared.h"
#include "codes.h"
#include "sf_reg_27px35pt7b.h"
using namespace Watchy;
RTC_DATA_ATTR bool sw_showing;
RTC_DATA_ATTR int originalMillis;
RTC_DATA_ATTR uint16_t sw_fgColor;  // what is the foreground color. bgColor is
                                    // the background color

RTC_DATA_ATTR int originalSteps;

String displayString = "00:00";
  String cadenceText = "Cadence: N/A";

RTC_DATA_ATTR bool isRunning;
void StopWatch::showMe() {
  Serial.println("in stopwatch");

  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(sw_fgColor);
  int16_t x;
  int16_t y;
  uint16_t width;
  uint16_t height;

  if (isRunning) {
    Serial.println("running...");
    int SecondsPassed = (millis() - originalMillis) / 1000;
    int minutesPassed = SecondsPassed / 60;
    int SecondsPassedMinute = SecondsPassed % 60;
    Serial.println(String(originalMillis) + " //  " + String(millis()) +
                   "  //  " + String(SecondsPassedMinute) + " // " +
                   String(SecondsPassed) + "  //  " + String(minutesPassed));

    displayString = add_zero_before_int(minutesPassed) + ":" +
                    add_zero_before_int(SecondsPassedMinute);

    cadenceText = "Cadence: " +
                  String((sensor.getCounter() - originalSteps) /
                         (float(minutesPassed) + (float(SecondsPassed) / 60)));
  }

  display.setFont(&FontsFree_Net_SFProDisplay_Regular7pt7b);
  display.getTextBounds(cadenceText, 10, 80, &x, &y, &width, &height);
  display.setCursor((200 - width) / 2, 12);
  display.println(cadenceText);

  display.setFont(&sf_reg_27px35pt7b);
  display.getTextBounds("00:00", 10, 80, &x, &y, &width, &height);
  display.setCursor((200 - width) / 2, 125);
  display.println(displayString);
  Watchy::display.display(true);
}

void StopWatch::show() {
  Serial.println("stopwatch");
  sw_showing = true;
  while (sw_showing) {
    sw_fgColor = GxEPD_BLACK;
    display.setFullWindow();
    showMe();
    auto timeout = millis() + 500;
    while (millis() < timeout) {
      Watchy::pollButtonsAndDispatch();
      yield();
    }
  }
}

void StopWatch::up() {
  if (isRunning) {
    isRunning = false;
  } else {
    isRunning = true;
    originalMillis = millis();
    originalSteps = sensor.getCounter();
  }
}

void StopWatch::down() { displayString = "00:00"; }

void StopWatch::back() {
  sw_showing = false;
  MainScreen();
}
String StopWatch::add_zero_before_int(int num) {
  if (num > 9) {
    return (String(num));
  } else {
    return (String("0") + String(num));
  }
};
