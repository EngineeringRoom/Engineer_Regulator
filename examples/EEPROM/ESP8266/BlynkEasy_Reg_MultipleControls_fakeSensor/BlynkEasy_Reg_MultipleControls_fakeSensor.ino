/*
      Autor: Alexey Obushenkov (Инжинерка - EngineeringRoom)
      email: sib.eng24@gmail.com
      YouTube: https://www.youtube.com/channel/UCih0Gcl9IEdkR8deDJCiodg
      GitHAB: https://github.com/EngineeringRoom
      Донат можно кинуть тут Мой бусти: https://boosty.to/sibeng24
      Я буду очень вам признателен.

    Пример работы с Blynk.
    Тут простой интерйфес QR-code (для повтора проекта в Blynk) и скришот интерфейса
    в папке img в самом примере. Или тут на GitHAB

    В вкладке Variables.h укажите:
    - ваши _ssid и _password от WiFi.
    - _authB Токен Blynk
    - ваш сервер _serverB или оставте родной по умолчанию "blynk-cloud.com"
    - порт _portB стандартный 8080
    
    Вкладки Timers.h и Connect.h убраны в папку data и потому тут невидны и немешают.
    
    Во вкладке Help.h находятся все методы класса. Все методы которые
    касаются EEPROM просто непозволят скомпилировать скетч. Они помечены ==> (EEPROM).

    Залейте скетч со своими настройками.

    В этом скетче применен фейковый сенсор температуры код в Stnsor.h

    Если регулятору задать обратную логику setRLL(LOW) то состояние выходов ниже
                                              | Включить реле ||Выключить реле |
    OutState() - это выход логики регулятора  |     HIGH      ||     LOW       |
    OutRelay() - это выход на реле            |     LOW       ||     HIGH      |
*/


#include "Variables.h"            // Подключаем вкладку с переменными

#include "Engineer_RegulatorEEPROM_ESP.h" // Подключаем библиотеку для работы c ESP c EEPROM
//Конструктор класса
RegulatorEEPROM_ESP R[NumberOfRegulators];    // Создаем регуляторы (экземпляр класса)

#include "data/Timers.h"    // Подключаем вкладку с функцией таймера (Лежит в папке data. Убрал что бы немешало)
#include "Sensor.h"         // Подключаем вкладку с сенсором

#include "Blynk.h"          // Подключаем вкладку с функциями Blynk
BlynkTimer timer;           // Объявляем класс Таймер
#include "data/Connect.h"   // Вкладка с функциями подключения к WiFi и Blynk (Лежит в папке data. Убрал что бы немешало)



void setup() {
  for (int i=0; i < NumberOfRegulators; i++){
    pinMode(PIN_Relay[i], OUTPUT);     // Объявляем GPIO как выход   
  }

  Serial.begin(9600);

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

  for (int i=0; i<RegulatorEEPROM_ESP::getCount(); i++){
    R[i].printSet();  // Выводит в Serial все настройки регулятора
    R[i].printAddr(); // Выводит таблицу Адресов
  }

// Проверяем есть ли связь с сервером каждые 10 секунд
  IDt_reconnectBlynk = timer.setInterval(10*1000, reconnectBlynk);
 
// Вызываем функцию подключения к Blynk
  reconnectBlynk();
  timer.setInterval(1000L, loopBlynk);
}

void loop() {
  ms = millis();  // Записываем в ms количество секунд с начала работы МК

if (Blynk.connected()){ Blynk.run();} 
  timer.run();
}
