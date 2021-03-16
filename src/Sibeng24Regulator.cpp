#include <Arduino.h>


#include "Sibeng24Regulator.h"

// Производит вычисления и выдает результат на Реле
bool Sibeng24Regulator::OutRelay() {
  // Если включен автоматический режим
  if (_Mode == AUTOMODE) {
    if (_RegulatorType == HEATER) {    // Если Тип регулятора Нагреватель
      if (_TempIn < _TempSet - _Delta) {
        _State = HIGH;
      }
      if (_TempIn > _TempSet + _Delta) {
        _State = LOW;   
      }
    }
    else if (_RegulatorType == COOLER) { // Если Тип регулятора Охладитель
      if (_TempIn > _TempSet - _Delta) {
        _State = HIGH;
      }
      if (_TempIn < _TempSet + _Delta) {
        _State = LOW;
      }
    }
	// Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
	if (_RLL) {
      return  _State;
    }
    else     {
      return !_State;
    }
  }
  // Если включен ручной режим
  else if (_Mode == MANUALMODE) {
    // Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
    if (_RLL) {
      return  _State;
    }
    else     {
      return !_State;
    }
  }
  // Если включен режим ошибка
  else  { // if (_Mode == ERRORMODE)
    // Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
    if (_RLL) {
      return  _ErrState;
    }
    else     {
      return !_ErrState;
    }
  }
}

bool Sibeng24Regulator::OutRelay(float TempIn) {
  setTempIn(TempIn);
  return OutRelay();
}

// Производит вычисления и выдает результат Логики
bool Sibeng24Regulator::OutState() {
  OutRelay();
  return _State;
}
bool Sibeng24Regulator::OutState(float TempIn) {
  OutRelay(TempIn);
  return _State;
}




// Сброс настроек в заводские
void Sibeng24Regulator::setFactorySettings() {
  _RegulatorType = HEATER;
  _RLL = HIGH;
  _Mode = MANUALMODE;

  _TempIn = 0;
  _TempSet = 0;
  _Delta = 1;
  _ErrState = LOW;
}


// Выбираем (Автоматический Режим)
void Sibeng24Regulator::setAuto() {
  _Mode = AUTOMODE;
}

// Выбираем (Ручной Режим)(Автоматически Состояние на выходи станет LOW)
void Sibeng24Regulator::setManual() {
  _Mode = MANUALMODE;  _State = _ErrState;
}

// Выбираем (Ручной Режим)(Состояние на выходи станет HIGH)
void Sibeng24Regulator::setManualON() {
  _Mode = MANUALMODE; _State = HIGH;
}
// Выбираем (Ручной Режим)(Автоматически Состояние на выходи станет LOW)
void Sibeng24Regulator::setManualOFF() {
  _Mode = MANUALMODE; _State = LOW;
}

// Выбираем (Ошибка)
void Sibeng24Regulator::setError() {
  _Mode = ERRORMODE;
}

// Безопасное состояние реле
void Sibeng24Regulator::setErrorState(bool ErrState) {
  _ErrState = ErrState;
}


// Выбираем тип регулятора
void Sibeng24Regulator::setType(uint8_t Type) {
  _RegulatorType = Type;
}

// Выбираем тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
void Sibeng24Regulator::setRLL(bool RLL) {
  _RLL = RLL;
}



// Устанавливаем Дельта (Гистерезис)
void Sibeng24Regulator::setDelta(float Delta) {
  _Delta = Delta;
}

// Устанавливаем Уставкe температуры
void Sibeng24Regulator::setTempSet(float TempSet) {
  _TempSet = TempSet;
}

// Устанавливаем Температура от датчика температуры
void Sibeng24Regulator::setTempIn(float TempIn) {
  _TempIn = TempIn;
}



///////////////////////////////////////////////////////////
// GET

// Возвращает Сигнал Аварии (от датчика)
bool   Sibeng24Regulator::Alarm() {
  if (_Mode == ERRORMODE) {
    return true;
  } else {
    return false;
  }
}

// Возвращает режим регулятора состояние (Автоматический/Ручной/Ошибка)(AUTOMODE, MANUALMODE, ERRORMODE)
uint8_t Sibeng24Regulator::getModeState() {
  return _Mode;
}


// Возвращает тип регулятора(HEATER, COOLER)
uint8_t Sibeng24Regulator::getType() {
  return _RegulatorType;
}
// Возвращает тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
bool   Sibeng24Regulator::getRLL() {
  return _RLL;
}


// Возвращает Дельта (Гистерезис)
float Sibeng24Regulator::getDelta() {
  return _Delta;
}

// Возвращает Уставкe температуры
float Sibeng24Regulator::getTempSet() {
  return _TempSet;
}

// Возвращает Температура от датчика температуры
float Sibeng24Regulator::getTempIn() {
  return _TempIn;
}
// ID класса (порядковый номер регулятора)
uint8_t Sibeng24Regulator::getId(){
  return _id;
}

// Счетчик для ID класса (порядковый номер регулятора)
uint8_t Sibeng24Regulator::getCount(){
  return count;  
}

// Выводит в Serial все настройки регулятора
void Sibeng24Regulator::printSet() {
  Serial.println();
  Serial.println();
  Serial.println(F("----------"));
  Serial.print(F("Regulator Settings: "));Serial.println(_id);

  Serial.print(F("Regulator Type: "));
  if (_RegulatorType == HEATER)Serial.println(F("HEATER"));
  else if (_RegulatorType == COOLER)Serial.println(F("COOLER"));

  Serial.print(F("Relay Logic Lavel: "));
  if (_RLL)Serial.println(F("HIGH"));
  else Serial.println(F("LOW"));

  Serial.print(F("Regulator Mode: "));
  if (_Mode == AUTOMODE)Serial.println(F("AUTOMODE"));
  else if (_Mode == MANUALMODE)Serial.println(F("MANUALMODE"));
  else if (_Mode == ERRORMODE)Serial.println(F("ERRORMODE"));

  Serial.print(F("Safe state in case of an error (ErrState): ")); Serial.println(_ErrState);

  Serial.println(F("----------"));
  Serial.print(F("Temperature from the sensor(TempIn): ")); Serial.println(_TempIn, 2);
  Serial.print(F("Temperature setpoint(TempSet): ")); Serial.println(_TempSet, 2);
  Serial.print(F("Hysteresis (Delta): ")); Serial.println(_Delta, 2);

  Serial.println(F("----------"));
  Serial.print(F("Logic output (State): ")); Serial.println(_State);
  Serial.print(F("Output to the relay (actuator): ")); Serial.println(OutRelay());
  Serial.println();
}

  uint8_t Sibeng24Regulator::count=0;        // Счетчик для ID класса (порядковый номер регулятора)