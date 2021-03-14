#include <Arduino.h>  // for type definitions

#ifndef Sibeng24Regulator_h
#define Sibeng24Regulator_h

// Перечесление Типов регулятора
enum enumRegulatorType {
  HEATER,
  COOLER
};

// Перечесление Режимов регулятора
enum enumRegulatorMode {
  AUTOMODE,
  MANUALMODE,
  ERRORMODE
};

class Sibeng24Regulator {

  
  protected:
    uint8_t _id;          // ID класса (порядковый номер регулятора)
    static uint8_t count; // Счетчик для ID класса (порядковый номер регулятора)
    
    uint8_t _RegulatorType; // Тип регулятора (HEATER, COOLER)
    uint8_t _Mode;         // Режим рулятора (Автоматический/Ручной/Ошибка)(AUTOMODE, MANUALMODE, ERRORMODE)По умолчанию Ручной
    float _TempIn;      // Температура от датчика температуры
    float _TempSet;     // Уставка температуры
    float _Delta;       // Дельта (Гистерезис)
    bool _State;        // Состояние выхода (Включен/Выключен)(HIGH/LOW)
    bool _ErrState;     // Состояние выхода (Включен/Выключен)(HIGH/LOW) При ошибке датчика
    bool _RLL;          // Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)

  public:

    // Конструктор класса
    Sibeng24Regulator() {
      _RegulatorType = HEATER;
      _RLL = HIGH;
      _Mode = MANUALMODE;

      _TempIn = 0;
      _TempSet = 0;
      _Delta = 1;
      _State = LOW;
      _ErrState = LOW;

      count++;
      _id = count;
    }

    bool OutRelay();                  // Производит вычисления и выдает результат на Реле
    bool OutRelay(float TempIn);      // Производит вычисления и выдает результат на Реле
    bool OutState();                  // Производит вычисления и выдает результат Логики
    bool OutState(float TempIn);      // Производит вычисления и выдает результат Логики

    void setFactorySettings(); // Сброс настроек в заводские
    void setAuto();     // Выбираем (Автоматический Режим)

    void setManual();   // Выбираем (Ручной Режим)(Автоматически Состояние на выходи станет Равным Безопасному состояние реле )
    void setManualON();   // Выбираем (Ручной Режим)(Состояние на выходи станет HIGH)
    void setManualOFF();   // Выбираем (Ручной Режим)(Автоматически Состояние на выходи станет LOW)

    void setError();    // Выбираем (Ручной Ошибка) По датчику
    void setErrorState(bool ErrState); // Безопасное состояние реле

    void setType(uint8_t Type);  // Выбираем тип регулятора(HEATER, COOLER)
    void setRLL(bool RLL);       // Выбираем тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)

    void setDelta(float Delta);      // Устанавливаем Дельта (Гистерезис)
    void setTempSet(float TempSet);  // Устанавливаем Уставкe температуры
    void setTempIn(float TempIn);    // Устанавливаем Температура от датчика температуры

    bool   Alarm();    // Возвращает Сигнал Аварии (от датчика)
    uint8_t getModeState();  // Возвращает режим регулятора состояние (Автоматический/Ручной/Ошибка)(AUTOMODE, MANUALMODE, ERRORMODE)

    uint8_t getType();  // Возвращает тип регулятора(HEATER, COOLER)
    bool   getRLL();   // Возвращает тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)

    float getDelta();    // Возвращает Дельта (Гистерезис)
    float getTempSet();  // Возвращает Уставкe температуры
    float getTempIn();   // Возвращает Температура от датчика температуры

    uint8_t getId();     // ID класса (порядковый номер регулятора)
    static uint8_t getCount();  // Счетчик для ID класса (порядковый номер регулятора)
    
    void printSet();// Выводит в Serial все настройки регулятора
}; // Sibeng24Regulator

#endif Sibeng24Regulator_h
