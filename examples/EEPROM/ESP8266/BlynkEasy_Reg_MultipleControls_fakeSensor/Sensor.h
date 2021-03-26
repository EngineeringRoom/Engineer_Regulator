#pragma once
// В языках программирования Си и C++ #pragma once — нестандартная,
// но широко распространенная препроцессорная директива, разработанная
// для контроля за тем, чтобы конкретный исходный файл при компиляции подключался строго один раз.

/*
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
float ReadSensors(const int &i); // Основная функция опроса датчиков. Тут же проверки и их решения.
float readSensor1();        // Непосредственно опрос датчика (С эмулятором ошибок) 
float readSensor2();        // Непосредственно опрос датчика
float readSensor3();        // Непосредственно опрос датчика

void restartSensor();       // Перезапуск сенсора (по идеии их то же три как и опрос датчиков)

void skipBug();             // Нужно что бы сбрасывать ошибки
bool ExampleBug_MinTemp();  // Эмуляция ошибки Temp = -50  Такое бывает у DS18B20 (через 30 секунд)
bool ExampleBug_NAN();      // Эмуляция ошибки Temp = NAN  Такое бывает у DHT     (еще через 30 секунд)

#include "Blynk.h"          // Подключаем вкладку с функциями Blynk

float ReadSensors(const int &i){
  bool Error=false;
  float Temp;
  // Выбираем какой датчик Читаем.
  switch (i) {
          case 0:
            Temp = readSensor1();        // Читаем температуру от датчика  
            break;
          case 1:
            Temp = readSensor2();        // Читаем температуру от датчика 
            break;
          case 2:
            Temp = readSensor3();        // Читаем температуру от датчика 
            break;
  }

  // Проверяем на ошибки их тут можеть быть много и реакции могут быть разные
  if      (Temp >= ALLARM_MAX_TEMP){
     /*
     По идеии тут нам стоит предосмотреть действия которые позволят отключить
     наше оборудование. Если залипло реле или контактор. То отправить сигнал 
     на независимы расцепитель и обесточить нагреватель
     */
    if(!FlagUpdateProperty[i]){
      FlagUpdateProperty[i] = true;
    }
    Error=true;
    if(R[i].getModeState() != MANUALMODE) R[i].setError();
    ErrorMessege[i] ="| Err: НОТ";
    restartSensor();  // Тут какой то умный код который перезапустит датчик. Повторная инициализация к пиримеру.
  }
  else if (Temp <= -50){
    if(!FlagUpdateProperty[i]){
      FlagUpdateProperty[i] = true;
    }
    Error=true;
    if(R[i].getModeState() != MANUALMODE) R[i].setError();
      ErrorMessege[i] ="| Err: -50";
      restartSensor();
  }
  else if (isnan(Temp)){ // Пришло не число
    if(!FlagUpdateProperty[i]){
      FlagUpdateProperty[i] = true;
    }
    Error=true;
    if(R[i].getModeState() != MANUALMODE) R[i].setError();
      ErrorMessege[i] ="| Err: NAN";
      restartSensor();
  }
  else if (R[i].Alarm()){  // Если предидущие условия несработали и есть сигнал Авария значит            
    R[i].setAuto();        // данные от датчика опять приходят! Значит Возвращаемя в Автоматический режим
    ErrorMessege[i] ="";
    FlagUpdateProperty[i] = true;
  }
  else if (!Error){        // Если предидущие условия несработали и нет сообщения об аварии значит            
                                    // данные от датчика опять приходят! Значит Возвращаемя в Ручной режим режим
    ErrorMessege[i] ="";
    FlagUpdateProperty[i] = true;
  }

  return Temp;
}

//////////////////////////////////////////////////
// !!! ВАЖНО !!!
// По сути весь код ниже нас неволнует так как это эмуляция
// Но для интереса можно посмотреть

// Перезапуск сенсора
void restartSensor(){

}

/////////////////////////////////////////
//      Датчик температуры 1 (с эмуляцией ошибок) 

// Настройки Фейкового датчика температуры 
#define MAX_TEMP1 50 // Максимальная температура
#define MIN_TEMP1 10 // Минимальная температура

// Фейковая Функция опроса датчика
float readSensor1() {
  static float Temp = MIN_TEMP1;
  static bool Direction = true;

  if (Direction)  {
    Temp += (float)random(50, 150) / 100;
    if (Temp > MAX_TEMP1) {
      Direction = false;
    }
  }
  else            {
    Temp -= (float)random(50, 150) / 100;
    if (Temp < MIN_TEMP1) {
      Direction = true;
    }
  }

  // Ошибки при опросе датчика
  skipBug(); // Сброс ошибок
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
const uint32_t TIMEOUT_Bskip = 20000;  

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

/////////////////////////////////////////
//      Датчик температуры 2 

#define MAX_TEMP2 30 // Максимальная температура
#define MIN_TEMP2 10 // Минимальная температура

// Фейковая Функция опроса датчика
float readSensor2() {
  static float Temp = MIN_TEMP2;
  static bool Direction = true;

  if (Direction)  {
    Temp += (float)random(50, 150) / 100;
    if (Temp > MAX_TEMP2) {
      Direction = false;
    }
  }
  else            {
    Temp -= (float)random(50, 150) / 100;
    if (Temp < MIN_TEMP2) {
      Direction = true;
    }
  }
  return Temp;
}

/////////////////////////////////////////
//      Датчик температуры 3 

#define MAX_TEMP3 30 // Максимальная температура
#define MIN_TEMP3 10 // Минимальная температура

// Фейковая Функция опроса датчика
float readSensor3() {
  static float Temp = MIN_TEMP3;
  static bool Direction = true;

  if (Direction)  {
    Temp += (float)random(50, 150) / 100;
    if (Temp > MAX_TEMP3) {
      Direction = false;
    }
  }
  else            {
    Temp -= (float)random(50, 150) / 100;
    if (Temp < MIN_TEMP3) {
      Direction = true;
    }
  }
  return Temp;
}
