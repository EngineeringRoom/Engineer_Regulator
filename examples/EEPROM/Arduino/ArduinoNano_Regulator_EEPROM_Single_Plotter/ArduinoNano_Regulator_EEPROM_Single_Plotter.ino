/*
      Autor: Alexey Obushenkov (Инжинерка - EngineeringRoom)
      email: sib.eng24@gmail.com
      YouTube: https://www.youtube.com/channel/UCih0Gcl9IEdkR8deDJCiodg
      GitHAB: https://github.com/EngineeringRoom
      Донат можно кинуть тут Мой бусти: https://boosty.to/sibeng24
      Я буду очень вам признателен.
      
    Простой скетч для работы с терморегулятором
    Из коробки идет запись данных в EEPROM
    Тут работаем с одним Регулятором и выводим данные в Плоттер по последовательному соединению.
    Нажмите Ctrl+Shift+L
    
    Во вкладке Help.h находятся все методы класса
    
    Залейте скетч и в первый раз увидете что все настройки регулятора базовые

    Далее раскоментируйте настройки и залейте скетч, потом можно их закоментировать опять
    ведь все настройки в EEPROM и перезалить скетч Что бы не мучить память

    В этом скетче применен фейковый сенсор температуры

    Если регулятору задать обратную логику setRLL(LOW) то состояние выходов ниже
                                              | Включить реле ||Выключить реле |    
    OutState() - это выход логики регулятора  |     HIGH      ||     LOW       |
    OutRelay() - это выход на реле            |     LOW       ||     HIGH      |

    Нажмите Ctrl+Shift+L Плоттер по последовательному соединению
*/

#include "Variables.h"            // Подключаем вкладку с переменными

// Подключаем библиотеку для работы с AVR
#include "Engineer_RegulatorEEPROM_AVR.h"

//Конструктор класса
RegulatorEEPROM_AVR R;    // Создаем регулятор (экземпляр класса)


#include "Timers.h"               // Подключаем вкладку с функцией таймера
#include "Sensor.h"               // Подключаем вкладку с сенсором



void setup() {

  pinMode(PIN_Relay, OUTPUT);     // Объявляем GPIO как выход   

  Serial.begin(9600);             


  // Настроим Регулятор(ы) (Достаточно один раз записать настройки, а после можно закоментировать)
  // Ведь настройки будут храниться в EEPROM
/*
  R.setType(HEATER);    // Выбираем тип регулятора(HEATER, COOLER)
  R.setRLL(LOW);       // Выбираем тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
  R.setErrorState(LOW); // Безопасное состояние логики реле (bool)

  R.setDelta(2.5);      // Устанавливаем Дельта (Гистерезис)
  R.setTempSet(20.0);   // Устанавливаем Уставку температуры
  R.setAuto();          // Выбираем (Автоматический Режим)
*/
 
}

void loop() {
  //Нажмите Ctrl+Shift+L Плоттер по последовательному соединению
  
  ms = millis();  // Записываем в ms количество секунд с начала работы МК

  // Действие происходит по таймеру
  if (Timer(msR, TIMEOUT_R)) {
      float temp = loopSensor();
      
      //В Arduino IDE с версии 1.8.10 добавили возможность подписать графики
      Serial.println("tempSet, tempSensor, outRelay, outState");    
      Serial.print(R.getTempSet());
      Serial.print(",");
      Serial.print(temp);
      Serial.print(",");
      Serial.print(R.OutRelay(temp));
      Serial.print(",");
      Serial.println(R.OutState());
  }
}
