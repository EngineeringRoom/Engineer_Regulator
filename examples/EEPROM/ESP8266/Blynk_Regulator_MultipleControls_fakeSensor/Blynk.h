/* Закомментируйте это, чтобы отключить BLYNK_LOG и сэкономить место */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// При подключению к серверу отправим данные на сервер
BLYNK_CONNECTED() {
  if(isFestConnection){ // Проверяем первый ли раз подключились к серверу
    isFestConnection=false;
    for (int i=0; i<RegulatorEEPROM_ESP::getCount(); i++){
      Blynk.virtualWrite(VPIN_NumericInput[i], R[i].getTempSet());// Отправляем данные о Настройке Уставки температуры
      Blynk.virtualWrite(VPIN_ButtonMode[i], R[i].getModeState());// Отправляем данные о какой сечас режим (ERRORMODE, MANUALMODE, AUTOMODE)
      Blynk.virtualWrite(VPIN_Gauge[i], R[i].getTempIn());        // Отправляем данные от Датчика
      Blynk.virtualWrite(VPIN_ButtonState[i], R[i].OutState());   // Отправляем данные о состоянии Логики
    }
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
  }
  BLYNK_WRITE(21) {if(param.asInt()==AUTOMODE) R[1].setAuto(); else if(param.asInt()==MANUALMODE) R[1].setManual();}
  BLYNK_WRITE(22) {if(param.asInt()==AUTOMODE) R[2].setAuto(); else if(param.asInt()==MANUALMODE) R[2].setManual();}

// Нажимая на кнопку включить или выключить реле в ручном режиме управляем выходным устройством
// Реагируем на кнопку только если установлен ручной режим
  BLYNK_WRITE(30) {
    if(R[0].getModeState()==MANUALMODE){
      if(param.asInt()) R[0].setManualON();
      else              R[0].setManualOFF();
    }
  }
  BLYNK_WRITE(31) {if(R[1].getModeState()==MANUALMODE){if(param.asInt()) R[1].setManualON(); else R[1].setManualOFF(); Blynk.virtualWrite(VPIN_ButtonMode[1], MANUALMODE);}}
  BLYNK_WRITE(32) {if(R[2].getModeState()==MANUALMODE){if(param.asInt()) R[2].setManualON(); else R[2].setManualOFF(); Blynk.virtualWrite(VPIN_ButtonMode[2], MANUALMODE);}}


  //int i=0;
  float lastTempSet[NumberOfRegulators];

  void UpdateTempSet(int &i){
    if(lastTempSet[i] != R[i].getTempSet()){
      lastTempSet[i] = R[i].getTempSet();
      Blynk.virtualWrite(VPIN_NumericInput[i], R[i].getTempSet());
    }
  }

  uint8_t LastMode[NumberOfRegulators]={4,4,4};

  void UpdateProperty(int &i){
    if(LastMode[i] != R[i].getModeState()){
      LastMode[i] = R[i].getModeState();
      if (R[i].getModeState()==MANUALMODE || R[i].getModeState()==AUTOMODE) {
            Blynk.setProperty(VPIN_Gauge[i], "label", "Room");
            //Blynk.setProperty(VPIN_ButtonState[i], "offlabel", "OFF");
      }
      else if(R[i].getModeState()==ERRORMODE){  
            Blynk.setProperty(VPIN_Gauge[i], "label", "ERROR");
            //Blynk.setProperty(VPIN_ButtonState[i], "offlabel", "ERROR");
            //Serial.println(" setProperty ERROR ");
      }
    }
  }
  
void loopBlynk(){ 
  for(int i=0; i < RegulatorEEPROM_ESP::getCount(); i++){
      
    float Temp;
    if(i==0) Temp = loopSensorBUG(); // Принемаем показания от датчика температуры
    else     Temp = loopSensor1();   
    
      digitalWrite(PIN_Relay[i], R[i].OutRelay(Temp));      //вычисляем результат и отпраляем команду на реле
                                                                 

      switch (R[i].getModeState()) {
          case MANUALMODE:
          case AUTOMODE:
            Blynk.virtualWrite(VPIN_ButtonMode[i], R[i].getModeState());// Отправляем данные о какой сечас режим (ERRORMODE, MANUALMODE, AUTOMODE)
            Blynk.virtualWrite(VPIN_Gauge[i], R[i].getTempIn());        // Отправляем данные от Датчика
            Blynk.virtualWrite(VPIN_ButtonState[i], R[i].OutState());   // Отправляем данные о состоянии Логики
            UpdateTempSet(i);                                           // Отправляем данные о Уставке 
            UpdateProperty(i);
            break;
          case ERRORMODE:
            String msg="Error";
            Blynk.virtualWrite(VPIN_ButtonMode[i], R[i].getModeState());// Отправляем данные о какой сечас режим (ERRORMODE, MANUALMODE, AUTOMODE)
            Blynk.virtualWrite(VPIN_Gauge[i], msg);                     // Отправляем данные от Датчика
            Blynk.virtualWrite(VPIN_ButtonState[i], R[i].OutState());   // Отправляем данные о состоянии Логики
            UpdateTempSet(i);                                            // Отправляем данные о Уставке
            UpdateProperty(i);
            break;                         
        }
  }     
}
