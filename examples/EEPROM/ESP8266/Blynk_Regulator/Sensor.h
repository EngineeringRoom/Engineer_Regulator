/*
  Суть примера в том ка воспользоваться методами:
    R.setError(); // Выбираем (Ручной Ошибка) По датчику
    R.Alarm();    // (bool) Возвращает Сигнал Аварии (от датчика)

  При работе с любым датчиком стоит посмотреть что будет происходиться с
  получаемыми от него данными при обрыве связи.
  Будет NAN(не число) или -50, а может фообще при обрыве связи контроллер зависнет
  И это нужно проверить.
  
  Стоит тут же сделать реакцию при выходе за безопасные пределы.
  Например температура котла недолжна привышать 90 Градусов Цельсия
  и эти проверки нужно организовать.

  Стоит попробовать организовать перезапуск датчиков. И если работа датчика
  востоновиться, то вернемся в Автоматический режим.
 */
#define ALLARM_MAX_TEMP 45.0 // Выход за пределы

// Прототипы функций
float loopSensor();         // Основная функция опроса датчики. Тут же проверки и их решения.
float readSensor();         // Непосредственно опрос датчика
void restartSensor();       // Перезапуск сенсора

void skipBug();             // Нужно что бы сбрасывать ошибки
bool ExampleBug_MinTemp();  // Эмуляция ошибки Temp = -50  Такое бывает у DS18B20 (через 30 секунд)
bool ExampleBug_NAN();      // Эмуляция ошибки Temp = NAN  Такое бывает у DHT     (еще через 30 секунд)

float loopSensor(){
  float Temp = readSensor();        // Читаем температуру от датчика

  // Проверяем на ошибки их тут можеть быть много и реакции могут быть разные
  // Но у меня реакция одна и та же
  if      (Temp >= ALLARM_MAX_TEMP){
    // По идеии тут нам стоит предосмотреть действия которые позволят отключить
    // наше оборудование. Если залипло реле или контактор. То отправить сигнал 
    // на независимы расцепитель и обесточить нагреватель
    R.setError();
    restartSensor();  // Тут какой то умный код который перезапустит датчик. Повторная инициализация к пиримеру.
    Serial.print(" НОТ !!! ");
  }
  else if (Temp <= -50){
    R.setError();
    restartSensor();
    Serial.print(" -50 !!! ");
  }
  else if (isnan(Temp)){ // Пришло не число
    R.setError();
    restartSensor();
    Serial.print(" NAN !!! ");
  }
  else if (R.Alarm()){  // Если предидущие условия несработали и есть сигнал Авария значит            
    R.setAuto();        // данные от датчика опять приходят! Значит Возвращаемя в Автоматический режим
  }

  return Temp;
}

//////////////////////////////////////////////////
// !!! ВАЖНО !!!
// По сути весь код ниже нас неволнует так как это эмуляция
// Но для интереса можно посмотреть

// Перезапуск сенсора
void restartSensor(){
  Serial.print(" !!! ERROR -");
}

// Настройки Фейкового датчика температуры 
#define MAX_TEMP 50 // Максимальная температура
#define MIN_TEMP 10 // Минимальная температура

// Фейковая Функция опроса датчика
float readSensor() {
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

  // Ошибки при опросе датчика
  skipBug();
  if(ExampleBug_MinTemp()) return -50;
  if(ExampleBug_NAN()) return NAN;
  
  return Temp;
}

bool Fbug1=false; // Флаг что ошибка включена
bool Fbug2=false; // Флаг что ошибка включена
bool queue=true;  // Очередь ошибок каждые 30 секунд вылазит ошибка


uint32_t msB1=0;
const uint32_t TIMEOUT_B1 = 30000;  

uint32_t msB2=0;
const uint32_t TIMEOUT_B2 = 30000;

uint32_t msBskip=0;              
const uint32_t TIMEOUT_Bskip = 5000;  

// Эмуляция ошибки Temp = -50  Такое бывает у DS18B20
bool ExampleBug_MinTemp(){
  if(Fbug1) return true;
  if (Timer(msB1, TIMEOUT_B1) && queue) {Fbug1=true; msBskip = ms; queue=!queue; msB2=ms; return true;  }
  else  return false;
}


// Эмуляция ошибки Temp = NAN Такое бывает у DHT (пришло не число)
bool ExampleBug_NAN(){
  if(Fbug2) return true;
  if (Timer(msB2, TIMEOUT_B2) && !queue) {Fbug2=true; msBskip = ms; queue=!queue; msB1=ms; return true;  }
  else  return false;
}

void skipBug(){
  if (Timer(msBskip, TIMEOUT_Bskip, false)){
    Fbug1=false;
    Fbug2=false;
  }
}
