/*
      Autor: Alexey Obushenkov (Инжинерка - EngineeringRoom)
      email: sib.eng24@gmail.com
      YouTube: https://www.youtube.com/channel/UCih0Gcl9IEdkR8deDJCiodg
      GitHAB: https://github.com/EngineeringRoom
      Донат можно кинуть тут Мой бусти: https://boosty.to/sibeng24
      Я буду очень вам признателен.
      
    Простой скетч для работы с терморегулятором в Ручном режиме.
    // в setup Настраиваем Автоматический режим.
    // Через 10 секунд будет эмулированно нажатие кномпки ручного режима.
    // И регулятор перейдет в ручной режим
    // А 10 секунд посмотрим как работает регулятор
        
    Работа с EEPROM в других примерах. Это для тех кто хочет сам организовать
    хранение в EEPROM настроек.
    Скетч работает с любой архитектурой. 
    В примере используются Nano. Захотите другой контроллер измените пины реле
    во вкладке Variables.h
    
    Тут работаем с одним Регулятором и выводим данные в сериал.

    Во вкладке Help.h находятся все методы класса. Все методы которые
    касаются EEPROM просто непозволят скомпилировать скетч. Они помечены ==> (EEPROM).

    Залейте скетч со своими настройками.

    В этом скетче применен фейковый сенсор температуры

    Если регулятору задать обратную логику setRLL(LOW) то состояние выходов ниже
                                              | Включить реле ||Выключить реле |
    OutState() - это выход логики регулятора  |     HIGH      ||     LOW       |
    OutRelay() - это выход на реле            |     LOW       ||     HIGH      |
*/

#include "Variables.h"            // Подключаем вкладку с переменными

// Подключаем библиотеку для работы с AVR
#include "RegulatorEEPROM_AVR.h"

//Конструктор класса
Regulator R;    // Создаем регуляторов (экземпляр класса)


#include "Timers.h"               // Подключаем вкладку с функцией таймера
#include "Sensor.h"               // Подключаем вкладку с сенсором



void setup() {
  pinMode(PIN_Relay, OUTPUT);     // Объявляем GPIO как выход

  Serial.begin(9600);

  // Настроим Регулятор(ы) 

    R.setType(HEATER);    // Выбираем тип регулятора(HEATER, COOLER). По умолчанию HEATER
    R.setRLL(HIGH);       // Выбираем тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW). По умолчанию HIGH
    R.setErrorState(LOW); // Безопасное состояние логики реле (bool). По умолчанию LOW

    R.setDelta(2.1);      // Устанавливаем Дельта (Гистерезис). По умолчанию 0.0
    R.setTempSet(20.5);   // Устанавливаем Уставку температуры. По умолчанию 0.0

    // Через 10 секунд будет эмулированно нажатие кномпки ручного режима.
    // И регулятор перейдет в ручной режим
    // А 10 секунд посмотрим как работает регулятор
    R.setAuto();          // Выбираем (Автоматический Режим).
    //R.setManual();      // Выбираем (Ручной Режим). По умолчанию MANUALMODE. То есть можно и ненастраивать
    // Причем если выбрать другой режим, то когда сработает метод setManualON() или setManualOFF()
    // режим все равно перейдет в ручной

  R.printSet();  // Выводит в Serial все настройки регулятора

  delay(5000); // Задержка просто так :-)
}

void loop() {
  ms = millis();  // Записываем в ms количество секунд с начала работы МК

  // Действие происходит по таймеру
  if (Timer(msR, TIMEOUT_R)) {
    Serial.println();
    Serial.print(F("Regulator: "));       Serial.print(R.getId());          // Выводим ID Регулятора


    // Вы увидете что в ручном режиме температура от датчика ни как не влияет.
    // На выходной результат
    float temp = loopSensor();                                              // Опрашиваем датчик температуры
    Serial.print(F("  Temp Set: "));      Serial.print(R.getTempSet());     // Выводим Уставку
    Serial.print(F(" / Temp Sensor: "));  Serial.print(temp);               // Выводим Текущую температуру
    R.setTempIn(temp);                          // Получаем Температуру от датчика температуры


    Serial.print(F(" Regulator Mode: "));    // Выводим текущий режим работы
    if     (R.getModeState() == AUTOMODE)      Serial.print("AUTOMODE");
    else if(R.getModeState() == MANUALMODE)    Serial.print("MANUALMODE");
    else if(R.getModeState() == ERRORMODE)     Serial.print("ERRORMODE");

    digitalWrite(PIN_Relay, R.OutRelay());      // Производит вычисления и выдает результат на Реле
    // Выдаем управлящий сигнал на Реле

    Serial.print(F(" ==> OutRelay: "));         // Выводи сотояние выхода регулятора подключенного к Реле
    if (R.OutRelay())    Serial.print("ON");
    else                Serial.print("OFF");

    // Выводим состояние Логики регулятора
    // Оно может отличаться от Out() так как Out() зависит от уровня логики (HIGH/LOW)
    // а OutState() выдает именно состояние логики
    Serial.print(F(" / OutState Logic: "));
    if (R.OutState())    Serial.print("ON");
    else                Serial.print("OFF");

    Serial.println();
    
  }

// Эмулируем нажатие на кнопку включить или выключить реле каждые 10 секунд
  if (Timer(msM, TIMEOUT_M)) {
    if(R.OutState()) R.setManualOFF();
    else             R.setManualON();
  }
}
