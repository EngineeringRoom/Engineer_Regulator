/*
      Autor: Alexey Obushenkov (Инжинерка - EngineeringRoom)
      email: sib.eng24@gmail.com
      YouTube: https://www.youtube.com/channel/UCih0Gcl9IEdkR8deDJCiodg
      GitHAB: https://github.com/EngineeringRoom
      Донат можно кинуть тут Мой бусти: https://boosty.to/sibeng24
      Я буду очень вам признателен.
      
    Простой скетч для работы с терморегуляторами в примере их три. Но можно лего сделать хоть 10
    Из коробки идет запись данных в EEPROM
    Тут работаем с массивом Регуляторов и выводим данные в сериал.
    Количество регуляторов можно задать в Variables.h
    переменная NumberOfRegulators
    
    Во вкладке Help.h находятся все методы класса

    Залейте скетч и в первый раз увидете что все настройки регулятора базовые

    Далее раскоментируйте настройки и залейте скетч, потом можно их закоментировать опять
    ведь все настройки в EEPROM и перезалить скетч Что бы не мучить память.
    Стоит поиграться с настройками и посмотреть как они работают в разных режимах.

    В этом скетче применен фейковый сенсор температуры

    У тетьего регулятора обратная логика setRLL(LOW)
                                              | Включить реле ||Выключить реле |    
    OutState() - это выход логики регулятора  |     HIGH      ||     LOW       |
    OutRelay() - это выход на реле            |     LOW       ||     HIGH      |
*/

#include "Variables.h"            // Подключаем вкладку с переменными

// Подключаем библиотеку для работы с ESP
#include "Engineer_RegulatorEEPROM_ESP.h"

//Конструктор класса
RegulatorEEPROM_ESP R[NumberOfRegulators]; // Создаем массив из регуляторов


#include "Timers.h"               // Подключаем вкладку с функцией таймера
#include "Sensor.h"               // Подключаем вкладку с сенсором



void setup() {
  for (int i=0; i < NumberOfRegulators; i++){
    pinMode(PIN_Relay[i], OUTPUT);     // Объявляем GPIO как выход   
  }

  Serial.begin(9600);             

  for (int i=0; i<RegulatorEEPROM_ESP::getCount(); i++){
    R[i].printSet();  // Выводит в Serial все настройки регулятора
    R[i].printAddr(); // Выводит таблицу Адресов
  }

  // Настроим Регулятор(ы) (Достаточно один раз записать настройки, а после можно закоментировать)
  // Ведь настройки будут храниться в EEPROM
/*
  R[0].setType(HEATER);    // Выбираем тип регулятора(HEATER, COOLER)
  R[0].setRLL(HIGH);       // Выбираем тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
  R[0].setErrorState(LOW); // Безопасное состояние логики реле (bool)

  R[0].setDelta(2.1);      // Устанавливаем Дельта (Гистерезис)
  R[0].setTempSet(20.5);   // Устанавливаем Уставку температуры
  R[0].setAuto();          // Выбираем (Автоматический Режим)

  R[1].setType(HEATER);    // Выбираем тип регулятора(HEATER, COOLER)
  R[1].setRLL(HIGH);       // Выбираем тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
  R[1].setErrorState(LOW); // Безопасное состояние логики реле (bool)

  R[1].setDelta(0.5);      // Устанавливаем Дельта (Гистерезис)
  R[1].setTempSet(25.0);   // Устанавливаем Уставку температуры
  R[1].setAuto();          // Выбираем (Автоматический Режим)

  R[2].setType(HEATER);    // Выбираем тип регулятора(HEATER, COOLER)
  R[2].setRLL(HIGH);        // Выбираем тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
  R[2].setErrorState(LOW); // Безопасное состояние логики реле (bool)

  R[2].setDelta(1.0);      // Устанавливаем Дельта (Гистерезис)
  R[2].setTempSet(23.5);   // Устанавливаем Уставку температуры
  R[2].setAuto();          // Выбираем (Автоматический Режим)  
*/
 
  delay(5000); // Задержка просто так :-)
}

void loop() {
  ms = millis();  // Записываем в ms количество секунд с начала работы МК

  // Действие происходит по таймеру
  if (Timer(msR, TIMEOUT_R)) {
    // Проходимся цыклом for
    // вот это нам возвращает количество регуляторов RegulatorEEPROM_ESP::getCount()
    for (int i=0; i < RegulatorEEPROM_ESP::getCount(); i++){           
      Serial.println();
      Serial.print(F("Regulator: "));       Serial.print(R[i].getId());       // Выводим ID Регулятора
      float temp = loopSensor();                                              // Опрашиваем датчик температуры
      Serial.print(F("  Temp Set: "));      Serial.print(R[i].getTempSet());  // Выводим Уставку
      Serial.print(F(" / Temp Sensor: "));  Serial.print(temp);               // Выводим Текущую температуру
  
      R[i].setTempIn(temp);                     // Получаем Температуру от датчика температуры
      
      digitalWrite(PIN_Relay[i], R[i].OutRelay());   // Производит вычисления и выдает результат на Реле
                                                // Выдаем управлящий сигнал на Реле

      Serial.print(F(" ==> OutRelay: "));   // Выводи сотояние выхода регулятора подключенного к Реле
      if(R[i].OutRelay()) Serial.print("ON");
      else                Serial.print("OFF");
        
      // Выводим состояние Логики регулятора
      // Оно может отличаться от Out() так как Out() зависит от уровня логики (HIGH/LOW)
      // а OutState() выдает именно состояние логики
      Serial.print(F(" / OutState Logic: "));
      if(R[i].OutState()) Serial.print("ON");
      else                Serial.print("OFF");
      
    }
    Serial.println();
  }
}
