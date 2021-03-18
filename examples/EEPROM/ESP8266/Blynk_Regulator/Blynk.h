/* Закомментируйте это, чтобы отключить BLYNK_LOG и сэкономить место */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

/////////////////////////////////////////
//      База Виртуальных PIN -ов       
const int8_t VPIN_Gauge[NumberOfRegulators] = {
  V0,
  V1,
  V2,
};

const int8_t VPIN_NumericInput[NumberOfRegulators] = {
  V10,
  V11,
  V12,
};

const int8_t VPIN_ButtonMode[NumberOfRegulators] = {
  V20,
  V21,
  V22,
};

const int8_t VPIN_ButtonState[NumberOfRegulators] = {
  V30,
  V31,
  V32,
};

//BLYNK_CONNECTED() { Blynk.syncAll();}

BLYNK_APP_CONNECTED() {
  for (int i=0; i<RegulatorEEPROM_ESP::getCount(); i++){
    Blynk.virtualWrite(VPIN_NumericInput[i], R[i].getTempSet());   // Отправляем данные о Настройки
  }
}

// Получаем значение в виде Float из приложения и сохраняем уставку
BLYNK_WRITE(10) {float f = param.asFloat(); R[0].setTempSet(f);}
BLYNK_WRITE(11) {R[1].setTempSet(param.asFloat());}
BLYNK_WRITE(12) {R[2].setTempSet(param.asFloat());}

BLYNK_WRITE(20) {if(param.asInt()) R[0].setAuto(); else R[0].setManual();}
BLYNK_WRITE(21) {if(param.asInt()) R[1].setAuto(); else R[1].setManual();}
BLYNK_WRITE(22) {if(param.asInt()) R[2].setAuto(); else R[2].setManual();}

BLYNK_WRITE(30) {if(param.asInt()) R[0].setManualON(); else R[0].setManualOFF();}
BLYNK_WRITE(31) {if(param.asInt()) R[1].setManualON(); else R[1].setManualOFF();}
BLYNK_WRITE(32) {if(param.asInt()) R[2].setManualON(); else R[2].setManualOFF();}

void loopBlynk(){
  static int i=0;
  if(i >= RegulatorEEPROM_ESP::getCount()) i=0;  
  Serial.print("i = "); Serial.println(i);
  //for (int i=0; i < RegulatorEEPROM_ESP::getCount(); i++){
//    float Temp;
//    if(i==0) Temp = loopSensorBUG();
//    else     Temp = loopSensor1();
    
      digitalWrite(PIN_Relay[i], R[i].OutRelay(loopSensor1()));            // Принемаем показания от датчика температуры
                                                                  // и тут же вычисляем результат и отпраляем команду на реле

      switch (R[i].getModeState()) {
          case AUTOMODE:
            Blynk.virtualWrite(VPIN_ButtonMode[i], 1);   // Отправляем данные о какой сечас режим (AUTOMODE, MANUALMODE, ERRORMODE)
            Blynk.virtualWrite(VPIN_Gauge[i], R[i].getTempIn());        // Отправляем данные от Датчика
            Blynk.virtualWrite(VPIN_ButtonState[i], R[i].OutState());   // Отправляем данные о состоянии Логики
            //Blynk.virtualWrite(VPIN_NumericInput[i], R[i].getTempSet());   // Отправляем данные о Настройки
            break;
          case MANUALMODE:
            Blynk.virtualWrite(VPIN_ButtonMode[i], 0);   // Отправляем данные о какой сечас режим (AUTOMODE, MANUALMODE, ERRORMODE)
            Blynk.virtualWrite(VPIN_Gauge[i], R[i].getTempIn());        // Отправляем данные от Датчика
            Blynk.virtualWrite(VPIN_ButtonState[i], R[i].OutState());   // Отправляем данные о состоянии Логики
           // Blynk.virtualWrite(VPIN_NumericInput[i], R[i].getTempSet());   // Отправляем данные о Настройки
            break;
          case ERRORMODE:
            String msg="Error";
            Blynk.virtualWrite(VPIN_ButtonMode[i], 0);   // Отправляем данные о какой сечас режим (AUTOMODE, MANUALMODE, ERRORMODE)
            Blynk.virtualWrite(VPIN_Gauge[i], msg);        // Отправляем данные от Датчика
            Blynk.virtualWrite(VPIN_ButtonState[i], R[i].OutState());   // Отправляем данные о состоянии Логики
            //Blynk.virtualWrite(VPIN_NumericInput[i], R[i].getTempSet());  // Отправляем данные о Настройки
            break;            
        }
        
   if(i < RegulatorEEPROM_ESP::getCount()) i++;
  //}//for
}
