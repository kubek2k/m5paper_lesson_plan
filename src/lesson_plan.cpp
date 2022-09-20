#include "lesson_plan.h"

#include "Arduino.h"
#include <vector>

#include "config.h"

static std::vector<String> weekDays = {"Pon", "Wt", "Sr", "Czw", "Pt", "Sob", "Nie"};

static std::vector<String> hours = {"08:15", "09:10", "09:55", "10:50", "11:45", "12:50", "13:50", "14:40", "15:20"};

void drawLessonLine(M5EPD_Canvas* canvas, int x, int y, String time, String title) {
  canvas->drawString(time, x, y);
  canvas->drawString(title, x + 130, y);
}

void drawLessonPlan(M5EPD_Canvas* canvas, int x, int y, int weekDayIdx) {
  canvas->setTextSize(4);
  canvas->drawString("Plan lekcji (" + weekDays[weekDayIdx] + ")", x, y);
  canvas->setTextSize(3);
  for (int i = 0; i < lessonPlan[weekDayIdx].size(); i++) {
    drawLessonLine(canvas, x + 5, y + 70 + 40*i, hours[i], lessonPlan[weekDayIdx][i]);
  }
}

