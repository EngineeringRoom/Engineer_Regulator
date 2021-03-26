#pragma once
// В языках программирования Си и C++ #pragma once — нестандартная,
// но широко распространенная препроцессорная директива, разработанная
// для контроля за тем, чтобы конкретный исходный файл при компиляции подключался строго один раз.

#include "Sensor.h"               // Подключаем вкладку с сенсором
/* Закомментируйте это, чтобы отключить BLYNK_LOG и сэкономить место */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

  // Массив Последней уставки температуры
  float lastTempSet[NumberOfRegulators];
  
  // Массив Флагов Обновления настроек
  bool FlagUpdateProperty[NumberOfRegulators]={true, true, true};
  String ErrorMessege[NumberOfRegulators];

  void loopBlynk();

// При подключению к серверу отправим данные на сервер
BLYNK_CONNECTED() {
  if(isFestConnection){ // Проверяем первый ли раз подключились к серверу
    isFestConnection=false;
    loopBlynk();
  }
}

// Получаем значение в виде Float из приложения и сохраняем уставку
  BLYNK_WRITE(10) {
    R[0].setTempSet(param.asFloat());
  }
  BLYNK_WRITE(11) {R[1].setTempSet(param.asFloat());}
  BLYNK_WRITE(12) {R[2].setTempSet(param.asFloat());}

// Включаем автоматический или ручной режим
  BLYNK_WRITE(20) {
    if      (param.asInt()==AUTOMODE)   R[0].setAuto();
    else if (param.asInt()==MANUALMODE) R[0].setManual();
    FlagUpdateProperty[0] = true;
  }
  BLYNK_WRITE(21) {if(param.asInt()==AUTOMODE) R[1].setAuto(); else if(param.asInt()==MANUALMODE) R[1].setManual(); FlagUpdateProperty[1] = true;}
  BLYNK_WRITE(22) {if(param.asInt()==AUTOMODE) R[2].setAuto(); else if(param.asInt()==MANUALMODE) R[2].setManual(); FlagUpdateProperty[2] = true;}

// Нажимая на кнопку включить или выключить реле в ручном режиме управляем выходным устройством
// Реагируем на кнопку только если установлен ручной режим
  BLYNK_WRITE(30) {
    if(R[0].getModeState()==MANUALMODE){
      if(param.asInt()) R[0].setManualON();
      else              R[0].setManualOFF();
    }
  }
  BLYNK_WRITE(31) {if(R[1].getModeState()==MANUALMODE){if(param.asInt()) R[1].setManualON(); else R[1].setManualOFF();}}
  BLYNK_WRITE(32) {if(R[2].getModeState()==MANUALMODE){if(param.asInt()) R[2].setManualON(); else R[2].setManualOFF();}}




// Обновляем в Blynk Уставку температуры если она изменилась
  void UpdateTempSet(const int &i){
    if(lastTempSet[i] != R[i].getTempSet()){
      lastTempSet[i] = R[i].getTempSet();
      Blynk.virtualWrite(VPIN_NumericInput[i], R[i].getTempSet());
    }
  }


  
  // Обновляем в Blynk настройки регулятора если было изменение
  // Отображаем ошибку
  void UpdateProperty(const int &i){
    if(FlagUpdateProperty[i]){        
      String Messege;
      FlagUpdateProperty[i] = false;
      if (R[i].getModeState()==AUTOMODE) {
            Blynk.setProperty(VPIN_ButtonMode[i], "onLabel", "Auto");
            Blynk.setProperty(VPIN_ButtonMode[i], "offLabel", "Manual");
      }
      else if (R[i].getModeState()==MANUALMODE) {
            Messege = "Auto" + ErrorMessege[i];
            Blynk.setProperty(VPIN_ButtonMode[i], "onLabel", Messege);
            Messege = "Manual" + ErrorMessege[i];
            Blynk.setProperty(VPIN_ButtonMode[i], "offLabel", Messege);
      }      
      else if(R[i].getModeState()==ERRORMODE){
            Messege = "Error" + ErrorMessege[i];  
            Blynk.setProperty(VPIN_ButtonMode[i], "onLabel", Messege);
            Blynk.setProperty(VPIN_ButtonMode[i], "offLabel", Messege);
      }
    }
  }
  
void loopBlynk(){ 
  for(int i=0; i < RegulatorEEPROM_ESP::getCount(); i++){
      
      digitalWrite(PIN_Relay[i], R[i].OutRelay(ReadSensors(i)));      // Считываем температуру от датчика и
                                                                      // вычисляем результат и отпраляем команду на реле

      switch (R[i].getModeState()) {
          case MANUALMODE:
          case AUTOMODE:
            Blynk.virtualWrite(VPIN_ButtonMode[i], R[i].getModeState());// Отправляем данные о какой сечас режим (ERRORMODE, MANUALMODE, AUTOMODE)
            UpdateProperty(i);
            Blynk.virtualWrite(VPIN_Gauge[i], R[i].getTempIn());        // Отправляем данные от Датчика
            Blynk.virtualWrite(VPIN_ButtonState[i], R[i].OutState());   // Отправляем данные о состоянии Логики
            UpdateTempSet(i);                                           // Отправляем данные о Уставке 
            break;
          case ERRORMODE:
            Blynk.virtualWrite(VPIN_ButtonMode[i], AUTOMODE);            // Отправляем данные о какой сечас режим (ERRORMODE, MANUALMODE, AUTOMODE)
            UpdateProperty(i);
            Blynk.virtualWrite(VPIN_Gauge[i],  R[i].getTempIn());        // Отправляем данные от Датчика
            Blynk.virtualWrite(VPIN_ButtonState[i], R[i].OutState());    // Отправляем данные о состоянии Логики
            UpdateTempSet(i);                                            // Отправляем данные о Уставке
            break;                         
        }
  }     
}
