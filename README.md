# Lesson plan on M5Paper

This is my rather simplistic fridge lesson plan displayer. 
Keep in mind that the code might not be best in class.

## What it does

 * Sets up the RTS according to NTP on init
 * Shows next or current day lesson plan depending of the time
   * Updates screen every hour

## How to use
  * Put config.h file into `include` directory containing:
    * `WIFI_NETWORK` - wifi network name
    * `WIFI_PASSWORD` - wifi password
    * `std::vector<String> lesson_plan[]` - array containing names of the lessons for each day. Example
```
std::vector<String> lessonPlan[] = {
  {"English", "Arts", "Maths"},
  {"English", "Computer Science", "Maths"},
  {"German", "Arts", "Maths"},
  {"French", "Science", "Sports"},
  {"Swimming pool", "Swimming pool", "Maths"}
}; 
```
  * Upload the code running: `pio run -t upload`
