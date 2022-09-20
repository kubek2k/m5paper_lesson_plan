#include "Arduino.h"

#include <M5EPD.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <vector>
#include <time.h>

#include "lesson_plan.h"
#include "config.h"

M5EPD_Canvas canvas(&M5.EPD);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

int chooseLessonPlanIdx(int weekdayIdx, rtc_time_t* currentTime) {
  if (currentTime->hour < 9) {
    return weekdayIdx;
  } else {
    if (weekdayIdx < 4) {
      return weekdayIdx + 1;
    } else {
      return 0;
    }
  }
}

bool rtcInitialized() {
  rtc_date_t d;
  M5.RTC.getDate(&d);
  return d.year > 0;
}

time_t getCurrentNTPTime() {
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(1000);
    Serial.print(".");
    canvas.drawString("WIFI not connected yet", 10, 10);
  }
  Serial.println("Getting time from NTP");
  timeClient.begin();
  timeClient.update();
  while (!timeClient.isTimeSet()) {
    Serial.print(".");
    delay(100);
  }
  time_t currentTime = timeClient.getEpochTime();
  timeClient.end();
  WiFi.disconnect();
  return currentTime;
}

void setupTime() {
  Serial.println("Setting up time using NTP");
  time_t currentTime = getCurrentNTPTime();
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
  tzset();
  tm ti;
  localtime_r(&currentTime, &ti);
  rtc_time_t rtcTime = RTC_Time(ti.tm_hour, ti.tm_min, ti.tm_sec);
  M5.RTC.setTime(&rtcTime);

  // Monday is the actual first day of the week
  rtc_date_t rtcDate = RTC_Date((ti.tm_wday + 6) % 7, ti.tm_mon + 1, ti.tm_mday, ti.tm_year + 1900);
  M5.RTC.setDate(&rtcDate);
  Serial.println("Done");
}

void setup() {
  M5.begin(false, false, true, true, false);
  M5.RTC.begin();
  M5.EPD.SetRotation(90);   //Set the rotation of the display.  设置屏幕旋转角度
  M5.EPD.Clear(true);  //Clear the screen.  清屏
  canvas.createCanvas(540, 960);  //Create a canvas.  创建画布
  canvas.setTextSize(3); //Set the text size.  设置文字大小

  if (!rtcInitialized()) {
    setupTime();
  }
}

#define MAX_VOLTAGE 4350

String getRemainingBatteryPercentage() {
  uint32_t currentVoltage = M5.getBatteryVoltage();
  if (currentVoltage > 2200) {
    uint32_t perc = (uint32_t)(((float)currentVoltage - 2200)/(MAX_VOLTAGE - 2200) * 100);
    return String("") + perc + "%";
  } else {
    return "USB-C";
  }
}

void loop() {
  rtc_date_t currentDate;
  M5.RTC.getDate(&currentDate);
  rtc_time_t currentTime;
  M5.RTC.getTime(&currentTime);
  canvas.fillCanvas(0);
  char message[50];
  snprintf(message, 49, "%d/%02d/%02d %02d:%02d:%02d (%s)\n",
                currentDate.year, currentDate.mon, currentDate.day,
                currentTime.hour, currentTime.min, currentTime.sec, getRemainingBatteryPercentage().c_str());
  canvas.drawString(message, 10, 920);
  int lessonPlanWeekDayIdx = chooseLessonPlanIdx(currentDate.week, &currentTime);
  Serial.println("Lesson plan idx " + String(lessonPlanWeekDayIdx) + " " + currentDate.week);
  drawLessonPlan(&canvas, 10, 10, lessonPlanWeekDayIdx);
  canvas.pushCanvas(0,0,UPDATE_MODE_DU4);
  Serial.println("About to sleep");
  delay(500);
  M5.shutdown(60*30);
}
