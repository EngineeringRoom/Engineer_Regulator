#pragma once
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;           // Объявляем класс Таймер
  
  void loopBlynk();                     // Прототип основно функции
  void UpdateScreen(const uint8_t &ID); // Прототип функции обновления экрана Tune-UP
  void UpdateValueDisplay(const int &i);// Прототип функции обновления LabelValue на экране Main Screen
  
  // Объявляем массив Виджетов LED
  WidgetLED ledState[NumberOfRegulators] = {
    (VPIN_Led[0]),
    (VPIN_Led[1]),
    (VPIN_Led[2]),
    (VPIN_Led[3]),
  };

  
// При подключению к серверу отправим данные на сервер
BLYNK_CONNECTED() {
  if(isFestConnection){ // Проверяем первый ли раз подключились к серверу
    isFestConnection=false;
    for (int i=0; i < NumberOfRegulators; i++){
      Blynk.virtualWrite(VPIN_TempSet[i], R[i].getTempSet());
      UpdateValueDisplay(i);
    }
    Blynk.virtualWrite(VPIN_Tune_Menu, RegID+1); // В настройках сразу выбираем 1 регулятор
    UpdateScreen(RegID);
  }
  Serial.print("BLYNK_CONNECTED");
}

///////////////////////////////////////////////
// Раздел с Основным экраном
// Получаем значение в виде Float из приложения и сохраняем уставку
  BLYNK_WRITE(20) {
    R[0].setTempSet(param.asFloat());
  }
  BLYNK_WRITE(21) {R[1].setTempSet(param.asFloat());}
  BLYNK_WRITE(22) {R[2].setTempSet(param.asFloat());}
  BLYNK_WRITE(23) {R[3].setTempSet(param.asFloat());}


///////////////////////////////////////////////
// Раздел с Настройками
  
// Выбираем регулятор для настроек
  BLYNK_WRITE(VPIN_Tune_Menu) {
    RegID = param.asInt()-1;
    UpdateScreen(RegID);
    Serial.print(" RegID");Serial.println(RegID);
  }

// Получаем значение TempSet в виде Float из приложения и сохраняем уставку
  BLYNK_WRITE(VPIN_Tune_TempSet) {
    R[RegID].setTempSet(param.asFloat());
  }

// Получаем значение Hysteresis в виде Float из приложения и сохраняем Дельту
  BLYNK_WRITE(VPIN_Tune_Hysteresis) {
    R[RegID].setDelta(param.asFloat());
  }
  
// Включаем автоматический или ручной режим
  BLYNK_WRITE(VPIN_Tune_ButtonMode) {
    if      (param.asInt()==AUTOMODE)   R[RegID].setAuto();
    else if (param.asInt()==MANUALMODE) R[RegID].setManual();
    FlagUpdateValueDisplay[RegID] = true;
    FlagUpdateProperty[RegID] = true;
  }


// Нажимая на кнопку включить или выключить реле в ручном режиме управляем выходным устройством
// Реагируем на кнопку только если установлен ручной режим
  BLYNK_WRITE(VPIN_Tune_ButtonState) {
    if(R[RegID].getModeState()==MANUALMODE){
      if(param.asInt()) R[RegID].setManualON();
      else              R[RegID].setManualOFF();
    }
  }

// Выбираем тип регулятора(HEATER, COOLER)
// Реагируем на кнопку только если установлен ручной режим
  BLYNK_WRITE(VPIN_Tune_SswitchType) {
    if(R[RegID].getModeState()==MANUALMODE){
      if     (param.asInt()==1)   R[RegID].setType(HEATER);
      else if(param.asInt()==2)   R[RegID].setType(COOLER);
    }
    else {
      uint8_t Type = (uint8_t)R[RegID].getType()+1;
      Blynk.virtualWrite(VPIN_Tune_SswitchType, Type);            // Отправляем Тип регулятора (HEATER, COOLER)
    }
  }

// Выбираем тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
// Реагируем на кнопку только если установлен ручной режим
  BLYNK_WRITE(VPIN_Tune_SswitchRLL) {
    if(R[RegID].getModeState()==MANUALMODE){
      if     (param.asInt()==1)   R[RegID].setRLL(LOW);
      else if(param.asInt()==2)   R[RegID].setRLL(HIGH);
    }
    else {
      uint8_t RLL = (uint8_t)R[RegID].getRLL()+1;
      Blynk.virtualWrite(VPIN_Tune_SswitchRLL, RLL);              // Отправляем тип выходного сигнала Уровень Логики Реле.(HIGH/LOW)
    }
  }

// Безопасное состояние логики реле (bool) (HIGH/LOW)
// Реагируем на кнопку только если установлен ручной режим
  BLYNK_WRITE(VPIN_Tune_SswitchSafeState) {
    if(R[RegID].getModeState()==MANUALMODE){
      if     (param.asInt()==1)   R[RegID].setErrorState(LOW);
      else if(param.asInt()==2)   R[RegID].setErrorState(HIGH);
    }
    else {
      uint8_t ErrorState = (uint8_t)R[RegID].getErrorState()+1;
      Blynk.virtualWrite(VPIN_Tune_SswitchSafeState, ErrorState); // Отправляем в Blynk безопасное состояние логики реле если она изменилась
    }
  }

// Обновляем в Blynk Уставку температуры если она изменилась
  void UpdateTempSet(const int &i){
    static float lastTempSet[NumberOfRegulators];       // Массив Последней уставки температуры
    if(lastTempSet[i] != R[i].getTempSet()){
      lastTempSet[i] = R[i].getTempSet();
      Blynk.virtualWrite(VPIN_TempSet[i], R[i].getTempSet());
      if (i == RegID) Blynk.virtualWrite(VPIN_Tune_TempSet, R[i].getTempSet());
    }
  }

// Обновляем в Blynk Гистерезис температуры если она изменилась
  void UpdateHysteresis(const int &i){
    static float lastHysteresis[NumberOfRegulators];    // Массив Последней уставки гистерезиса
    if(lastHysteresis[i] != R[i].getDelta()){
      lastHysteresis[i] = R[i].getDelta();
      Blynk.virtualWrite(VPIN_Tune_Hysteresis, R[i].getDelta());
    }
  }
/*
// Обновляем в Blynk тип регулятора(HEATER, COOLER) если она изменилась
  void UpdateSswitchType(const int &i){
    static uint8_t lastSswitchType[NumberOfRegulators];    // Массив Последней Типа регулятора
    if(lastSswitchType[i] != R[i].getType()){
      lastSswitchType[i] = R[i].getType();
      Blynk.virtualWrite(VPIN_Tune_SswitchType, R[i].getType()+1);
    }
  }

// Обновляем в Blynk тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
  void UpdateSswitchRLL(const int &i){
    static bool lastSswitchRLL[NumberOfRegulators];    // Массив Последней уставки Уровень Логики Реле
    if(lastSswitchRLL[i] != R[i].getRLL()){
      lastSswitchRLL[i] = R[i].getRLL();
      Blynk.virtualWrite(VPIN_Tune_SswitchRLL, (uint8_t)R[i].getRLL()+1);
    }
  }

// Обновляем в Blynk безопасное состояние логики реле если она изменилась
  void UpdateSswitchSafeState(const int &i){
    static bool lastSswitchSafeState[NumberOfRegulators];    // Массив Последней уставки безопасное состояние логики реле
    if(lastSswitchSafeState[i] != R[i].getRLL()){
      lastSswitchSafeState[i] = R[i].getRLL();
      Blynk.virtualWrite(VPIN_Tune_SswitchSafeState, (uint8_t)R[i].getErrorState()+1);
    }
  }
*/ 
  // Обновляем в Blynk настройки регулятора если было изменение
  // Отображаем ошибку
  void UpdateValueDisplay(const int &i){
    if(FlagUpdateValueDisplay[i]){        
      String Messege;
      String Type;
      FlagUpdateValueDisplay[i] = false;
      if      (R[i].getType() == HEATER) Type="Heater ";
      else if (R[i].getType() == COOLER) Type="Cooler ";
      if (R[i].getModeState()==AUTOMODE) {
            Messege = "Auto " + Type;
            Blynk.virtualWrite(VPIN_ValueDisplay[i], Messege);   
      }
      else if (R[i].getModeState()==MANUALMODE) {
            Messege = "Manual " + Type + ErrorMessege[i];
            Blynk.virtualWrite(VPIN_ValueDisplay[i], Messege);
      }      
      else if(R[i].getModeState()==ERRORMODE){
            Messege = "Error " + Type + ErrorMessege[i];  
            Blynk.virtualWrite(VPIN_ValueDisplay[i], Messege);
      }
    }
  }

  // Обновляем в Blynk настройки регулятора если было изменение
  // Отображаем ошибку
  void UpdateProperty(const int &i){
    if(FlagUpdateProperty[i]){        
      String Messege;
      FlagUpdateProperty[i] = false;
      if (R[i].getModeState()==AUTOMODE) {
            Blynk.setProperty(VPIN_Tune_ButtonMode, "onLabel", "Auto");
            Blynk.setProperty(VPIN_Tune_ButtonMode, "offLabel", "Manual");
            Blynk.virtualWrite(VPIN_Tune_ValueDisplay, "Auto Mode");
      }
      else if (R[i].getModeState()==MANUALMODE) {
            Messege = "Auto" + ErrorMessege[i];
            Blynk.setProperty(VPIN_Tune_ButtonMode, "onLabel", Messege);
            Messege = "Manual" + ErrorMessege[i];
            Blynk.setProperty(VPIN_Tune_ButtonMode, "offLabel", Messege);
            Messege = "Manual Mode" + ErrorMessege[i];
            Blynk.virtualWrite(VPIN_Tune_ValueDisplay, Messege);
      }      
      else if(R[i].getModeState()==ERRORMODE){
            Messege = "Error" + ErrorMessege[i];  
            Blynk.setProperty(VPIN_Tune_ButtonMode, "onLabel", Messege);
            Blynk.setProperty(VPIN_Tune_ButtonMode, "offLabel", Messege);
            Messege = "Error" + ErrorMessege[i]; 
            Blynk.virtualWrite(VPIN_Tune_ValueDisplay, Messege);
      }
    }
  }

// Прототип функции обновления экрана Tune-UP
void UpdateScreen(const uint8_t &ID){
   Blynk.virtualWrite(VPIN_Tune_Gauge, R[ID].getTempIn());        // Отправляем данные от Датчика
   
   Blynk.virtualWrite(VPIN_Tune_TempSet, R[ID].getTempSet());     // Отправляем данные о Уставке
   Blynk.virtualWrite(VPIN_Tune_Hysteresis, R[ID].getDelta());    // Отправляем данные о Гистерезисе
   
   uint8_t Type = (uint8_t)R[RegID].getType()+1;
   Blynk.virtualWrite(VPIN_Tune_SswitchType, Type);            // Отправляем Тип регулятора (HEATER, COOLER)
   
   uint8_t RLL = (uint8_t)R[RegID].getRLL()+1;
   Blynk.virtualWrite(VPIN_Tune_SswitchRLL, RLL);              // Отправляем тип выходного сигнала Уровень Логики Реле.(HIGH/LOW)
   
   uint8_t ErrorState = (uint8_t)R[RegID].getErrorState()+1;
   Blynk.virtualWrite(VPIN_Tune_SswitchSafeState, ErrorState); // Отправляем в Blynk безопасное состояние логики реле если она изменилась
}
  
void loopBlynk(){
  timer.disable(IDt_loopBlynk); 
  for(int i=0; i < RegulatorEEPROM_ESP::getCount(); i++){
    digitalWrite(PIN_Relay[i], R[i].OutRelay(ReadSensors(i)));  // Считываем температуру от датчика, вычесляем результат и управляем реле
// В основном экране
    Blynk.virtualWrite(VPIN_Gauge[i], R[i].getTempIn());        // Отправляем данные от Датчика
    UpdateValueDisplay(i);                                      // Отправляем информационное сообщение на ValueDisplay
    UpdateTempSet(i);                                           // Отправляем данные о Уставке 
    if(R[i].OutState())ledState[i].on(); else ledState[i].off();// Отправляем данные о состоянии выхода
  }// for
  
// В настройках регулятора
    Blynk.virtualWrite(VPIN_Tune_Gauge, R[RegID].getTempIn());        // Отправляем данные от Датчика
    Blynk.virtualWrite(VPIN_Tune_ButtonState, R[RegID].OutState());   // Отправляем данные о состоянии Логики
        
    switch (R[RegID].getModeState()) {
      case MANUALMODE:
      case AUTOMODE:
        Blynk.virtualWrite(VPIN_Tune_ButtonMode, R[RegID].getModeState());  
        UpdateProperty(RegID);                                        // Отправляем данные о какой сечас режим (ERRORMODE, MANUALMODE, AUTOMODE)
        break;
      case ERRORMODE:
        Blynk.virtualWrite(VPIN_Tune_ButtonMode, AUTOMODE);   // Отправляем данные какой сечас режим и AUTOMODE не просто так и это не ошибка
                                                              // что бы перейти по нажатию кнопки в MANUALMODE
        UpdateProperty(RegID);
        break;                     
    }       
  timer.enable(IDt_loopBlynk); // Включаем таймер
  timer.restartTimer(IDt_loopBlynk); // Перезапускаем таймер
}
