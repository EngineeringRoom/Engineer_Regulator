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
    https://github.com/EngineeringRoom/Engineer_Regulator/tree/main/examples/EEPROM/ESP8266/BlynkFull_Reg_MultipleControls_fakeSensor/img

    В вкладке Variables.h укажите:
    - ваши _ssid и _password от WiFi.
    - _authB Токен Blynk
    - ваш сервер _serverB или оставте родной по умолчанию "blynk-cloud.com"
    - порт _portB стандартный 8080

    В этом скетче 4 регулятора.
    Если на своей плате вы впервые используете мою библиотеку. То все должно заработать из коробки. Но...
    Если у вас ранее было дургое количество регуляторов то вам в void setup()
    нужно раскоментировать строку(68) и установить "зоводские настройки" залить скетч. После опять закоментировать и еще раз залить скетч.
    
    Вкладки Timers.h и Connect.h убраны в папку data и потому тут невидны и немешают.
    
    Во вкладке Help.h находятся все методы класса.
    
    Залейте скетч со своими настройками.

    В этом скетче применен фейковый сенсор температуры код в Stnsor.h

    Если регулятору задать обратную логику setRLL(LOW) то состояние выходов ниже
                                              | Включить реле ||Выключить реле |
    OutState() - это выход логики регулятора  |     HIGH      ||     LOW       |
    OutRelay() - это выход на реле            |     LOW       ||     HIGH      |
*/
  /* Закомментируйте это, чтобы отключить BLYNK_LOG и сэкономить место */


#include "Variables.h"            // Подключаем вкладку с переменными

#include "Engineer_RegulatorEEPROM_ESP.h" // Подключаем библиотеку для работы c ESP c EEPROM
//Конструктор класса
RegulatorEEPROM_ESP R[NumberOfRegulators];    // Создаем регуляторы (экземпляр класса)

#include "data/Timers.h"    // Подключаем вкладку с функцией таймера (Лежит в папке data. Убрал что бы немешало)
#include "Sensor.h"         // Подключаем вкладку с сенсором

#include "Blynk.h"          // Подключаем вкладку с функциями Blynk

#include "data/Connect.h"   // Вкладка с функциями подключения к WiFi и Blynk (Лежит в папке data. Убрал что бы немешало)



void setup() {
  for (int i=0; i < NumberOfRegulators; i++){
    pinMode(PIN_Relay[i], OUTPUT);     // Объявляем GPIO как выход
    //R[i].setFactorySettings();   
  }

  Serial.begin(9600);

// Настроим Регулятор(ы) (Достаточно один раз записать настройки, а после можно закоментировать)
// Ведь настройки будут храниться в EEPROM

  //for (int i=0; i < NumberOfRegulators; i++){R[i].setFactorySettings();}


  for (int i=0; i<RegulatorEEPROM_ESP::getCount(); i++){
    R[i].printSet();  // Выводит в Serial все настройки регулятора
    R[i].printAddr(); // Выводит таблицу Адресов
  }

// Проверяем есть ли связь с сервером каждые 10 секунд
  IDt_reconnectBlynk = timer.setInterval(10*1000, reconnectBlynk);
 
// Вызываем функцию подключения к Blynk
  reconnectBlynk();
  IDt_loopBlynk = timer.setInterval(1000, loopBlynk);


}

void loop() {
  ms = millis();  // Записываем в ms количество секунд с начала работы МК

if (Blynk.connected()){ Blynk.run();} 
  timer.run();
}
