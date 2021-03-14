
#define MAX_TEMP 30 // Максимальная температура
#define MIN_TEMP 10 // Минимальная температура

// Фейковая Функция опроса датчика
float loopSensor() {
  static float Temp = MIN_TEMP;
  static bool Direction = true;

  if (Direction)  {
    Temp += (float)random(50, 150) / 100;
    if (Temp > MAX_TEMP) {
      Direction = false;
    }
  }
  else            {
    Temp -= (float)random(50, 150) / 100;
    if (Temp < MIN_TEMP) {
      Direction = true;
    }
  }
  return Temp;
}
