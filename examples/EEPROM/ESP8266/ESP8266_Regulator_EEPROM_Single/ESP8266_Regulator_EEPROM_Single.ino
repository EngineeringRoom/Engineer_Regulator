/*
      Autor: Alexey Obushenkov (Инжинерка - EngineeringRoom)
      email: sib.eng24@gmail.com
      YouTube: https://www.youtube.com/channel/UCih0Gcl9IEdkR8deDJCiodg
      GitHAB: https://github.com/EngineeringRoom
      Донат можно кинуть тут Мой бусти: https://boosty.to/sibeng24
      Я буду очень вам признателен.
      
    Простой скетч для работы с терморегулятором
    Из коробки идет запись данных в EEPROM
    Тут работаем с одним Регулятором и выводим данные в сериал.

    Во вкладке Help.h находятся все методы класса

    Залейте скетч и в первый раз увидете что все настройки регулятора базовые

    Далее раскоментируйте настройки и залейте скетч, потом можно их закоментировать опять
    ведь все настройки в EEPROM и перезалить скетч Что бы не мучить память

    В этом скетче применен фейковый сенсор температуры

    Если регулятору задать обратную логику setRLL(LOW) то состояние выходов ниже
                                              | Включить реле ||Выключить реле |
    OutState() - это выход логики регулятора  |     HIGH      ||     LOW       |
    OutRelay() - это выход на реле            |     LOW       ||     HIGH      |
*/

#include "Variables.h"            // Подключаем вкладку с переменными

// Подключаем библиотеку для работы с ESP
#include "Engineer_RegulatorEEPROM_ESP.h"

//Конструктор класса
RegulatorEEPROM_ESP R;    // Создаем регуляторов (экземпляр класса)


#include "Timers.h"               // Подключаем вкладку с функцией таймера
#include "Sensor.h"               // Подключаем вкладку с сенсором



void setup() {

    pinMode(PIN_Relay, OUTPUT);     // Объявляем GPIO как выход   


  Serial.begin(9600);             


    R.printSet();  // Выводит в Serial все настройки регулятора
    R.printAddr(); // Выводит таблицу Адресов
  

  // Настроим Регулятор(ы) (Достаточно один раз записать настройки, а после можно закоментировать)
  // Ведь настройки будут храниться в EEPROM
/*
  R.setType(HEATER);    // Выбираем тип регулятора(HEATER, COOLER)
  R.setRLL(HIGH);       // Выбираем тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
  R.setErrorState(LOW); // Безопасное состояние логики реле (bool)

  R.setDelta(2.1);      // Устанавливаем Дельта (Гистерезис)
  R.setTempSet(20.5);   // Устанавливаем Уставку температуры
  R.setAuto();          // Выбираем (Автоматический Режим)
*/
 
  delay(5000); // Задержка просто так :-)
}

void loop() {
  ms = millis();  // Записываем в ms количество секунд с начала работы МК

  // Действие происходит по таймеру
  if (Timer(msR, TIMEOUT_R)) {
          
      Serial.println();
      Serial.print(F("Regulator: "));       Serial.print(R.getId());          // Выводим ID Регулятора
      float temp = loopSensor();                                              // Опрашиваем датчик температуры
      Serial.print(F("  Temp Set: "));      Serial.print(R.getTempSet());     // Выводим Уставку
      Serial.print(F(" / Temp Sensor: "));  Serial.print(temp);               // Выводим Текущую температуру
  
      R.setTempIn(temp);                          // Получаем Температуру от датчика температуры
      
      digitalWrite(PIN_Relay, R.OutRelay());      // Производит вычисления и выдает результат на Реле
                                                  // Выдаем управлящий сигнал на Реле

      Serial.print(F(" ==> OutRelay: "));         // Выводи сотояние выхода регулятора подключенного к Реле
      if(R.OutRelay())    Serial.print("ON");
      else                Serial.print("OFF");
        
      // Выводим состояние Логики регулятора
      // Оно может отличаться от Out() так как Out() зависит от уровня логики (HIGH/LOW)
      // а OutState() выдает именно состояние логики
      Serial.print(F(" / OutState Logic: "));
      if(R.OutState())    Serial.print("ON");
      else                Serial.print("OFF");
      
      Serial.println();
  }
}
