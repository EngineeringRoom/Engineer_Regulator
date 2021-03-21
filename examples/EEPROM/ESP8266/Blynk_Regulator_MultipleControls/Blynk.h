/* Закомментируйте это, чтобы отключить BLYNK_LOG и сэкономить место */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

BLYNK_CONNECTED() {
  if(isFestConnection){
    isFestConnection=false;
    for (int i=0; i<RegulatorEEPROM_ESP::getCount(); i++){
      Blynk.virtualWrite(VPIN_NumericInput[i], R[i].getTempSet());// Отправляем данные о Настройке Уставки температуры
      Blynk.virtualWrite(VPIN_ButtonMode[i], R[i].getModeState());// Отправляем данные о какой сечас режим (ERRORMODE, MANUALMODE, AUTOMODE)
      Blynk.virtualWrite(VPIN_Gauge[i], R[i].getTempIn());        // Отправляем данные от Датчика
      Blynk.virtualWrite(VPIN_ButtonState[i], R[i].OutState());   // Отправляем данные о состоянии Логики
    }
  }
}

BLYNK_APP_CONNECTED() {
  for (int i=0; i<RegulatorEEPROM_ESP::getCount(); i++){
    Blynk.virtualWrite(VPIN_NumericInput[i], R[i].getTempSet());   
  }
}

// Получаем значение в виде Float из приложения и сохраняем уставку
BLYNK_WRITE(10) {R[0].setTempSet(param.asFloat());}
BLYNK_WRITE(11) {R[1].setTempSet(param.asFloat());}
BLYNK_WRITE(12) {R[2].setTempSet(param.asFloat());}

BLYNK_WRITE(20) {if(param.asInt()==AUTOMODE) R[0].setAuto(); else if(param.asInt()==MANUALMODE) R[0].setManual();}
BLYNK_WRITE(21) {if(param.asInt()==AUTOMODE) R[1].setAuto(); else if(param.asInt()==MANUALMODE) R[1].setManual();}
BLYNK_WRITE(22) {if(param.asInt()==AUTOMODE) R[2].setAuto(); else if(param.asInt()==MANUALMODE) R[2].setManual();}

BLYNK_WRITE(30) {if(param.asInt()) R[0].setManualON(); else R[0].setManualOFF(); Blynk.virtualWrite(VPIN_ButtonMode[0], MANUALMODE);}
BLYNK_WRITE(31) {if(param.asInt()) R[1].setManualON(); else R[1].setManualOFF(); Blynk.virtualWrite(VPIN_ButtonMode[1], MANUALMODE);}
BLYNK_WRITE(32) {if(param.asInt()) R[2].setManualON(); else R[2].setManualOFF(); Blynk.virtualWrite(VPIN_ButtonMode[2], MANUALMODE);}


  int i=0;
  float lastTempSet[NumberOfRegulators];

  void UpdateTempSet(){
    if(lastTempSet[i] != R[i].getTempSet()){
      lastTempSet[i] = R[i].getTempSet();
      Blynk.virtualWrite(VPIN_NumericInput[i], R[i].getTempSet());
    }
  }
  
void loopBlynk(){
  //if(i >= RegulatorEEPROM_ESP::getCount()) i=0;  
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
            UpdateTempSet();                                            // Отправляем данные о Уставке 
            break;
          case ERRORMODE:
            String msg="Error";
            Blynk.virtualWrite(VPIN_ButtonMode[i], R[i].getModeState());// Отправляем данные о какой сечас режим (ERRORMODE, MANUALMODE, AUTOMODE)
            Blynk.virtualWrite(VPIN_Gauge[i], msg);                     // Отправляем данные от Датчика
            Blynk.virtualWrite(VPIN_ButtonState[i], R[i].OutState());   // Отправляем данные о состоянии Логики
            UpdateTempSet();                                            // Отправляем данные о Уставке
            break;                         
        }
  }     
  //if(i < RegulatorEEPROM_ESP::getCount()) i++;

}