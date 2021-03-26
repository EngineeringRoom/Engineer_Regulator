/*
	Autor: Alexey Obushenkov
	email: sib.eng24@gmail.com
	url: https://www.youtube.com/channel/UCih0Gcl9IEdkR8deDJCiodg

	Это библиотека для реализует объект Регулятор температуры. С гистирезисом.
	подключите в скетче библиотеку <Regulator.h>
	Объявите класс Regulator
	Работает и с AVR и с ESP
	Это может быть нагреватель или охладитель.
	Метод OutRelay() может работать с прямой и обратной логикой
	задается это методом setRLL()
	Имеет три режима: Автоматический. Ручной(по умолчанию). Ошибка.
	То есть пока не настроите и не переведете в Автоматический режим. Регулятор неработает.

	Может хранить настройки в EEPROM из коробки:
	Для AVR <RegulatorEEPROM_AVR.h>
	Объявите класс RegulatorEEPROM_AVR

	Для ESP <RegulatorEEPROM_ESP.h>
	Объявите класс RegulatorEEPROM_ESP

	Все сделанно что бы просто было пользоваться этим классом и заморачиваться только с логикой работы своей программы,
	а не с тем как должен работать регулятор.

	Ну и для удобства все методы класса доступные для вас.

// Set "Сетеры"
  R.setAuto();      // Устанавливаем (Автоматический Режим)
  
  R.setManual();    // Устанавливаем (Ручной Режим)(Автоматически Состояние на выходи станет Равным Безопасному состояние реле)
  R.setManualON();  // Устанавливаем (Ручной Режим)(Состояние на выходи станет HIGH)
  R.setManualOFF(); // Устанавливаем (Ручной Режим)(Cостояние на выходи станет LOW)
  
  R.setError();         // Устанавливаем (Режим Ошибка) По датчику (в оброботчеке датчика лучши поместить)
  R.setErrorState(LOW); // (bool) Безопасное состояние Логики реле
  
  R.setType(HEATER);    // (uint8_t) Выбираем тип регулятора(HEATER, COOLER)
  R.setRLL(HIGH);       // (bool) Выбираем тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
  
  R.setDelta(0.5);      // (float) Устанавливаем Дельта (Гистерезис)
  R.setTempSet(20);     // (float) Устанавливаем Уставку температуры
  R.setTempIn(20);      // (float) Устанавливаем Температура от датчика температуры

  R.setFactorySettings();    // Сброс настроек в заводские
  R.setAllFactorySettings(); // Сбросить настройки всех Экземпляров класса после перезагрузки устройства

// Get "Гетеры"
  R.Alarm();         // (bool) Возвращает Сигнал Аварии (от датчика)
  R.getModeState();  // (uint8_t) Возвращает режим регулятора состояние (Автоматический/Ручной/Ошибка)(AUTOMODE, MANUALMODE, ERRORMODE)
  
  R.getType();  // (uint8_t) Возвращает тип регулятора(HEATER, COOLER)
  R.getRLL();   // (bool) Возвращает тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
  
  R.getDelta();    // (float) Возвращает Дельта (Гистерезис)
  R.getTempSet();  // (float) Возвращает Уставку температуры
  R.getTempIn();   // (float) Возвращает Температуру от датчика температуры

  R.getId();     // (uint8_t) ID класса (порядковый номер регулятора)
  R.getCount();  // (uint8_t) Счетчик для ID класса (порядковый номер регулятора)
      
  R.printSet();       // Выводит в Serial все настройки регулятора
  R.printAddr();      // Выводит таблицу Адресов
  R.getEEPROMaddr();  // (uint16_t) Возвращает следующий не занятый адрес в EEPROM
  
  R.OutRelay();       // (bool) Производит вычисления и выдает результат на Реле
  R.OutRelay(17.8);   // (bool) Производит вычисления и выдает результат на Реле (принимает float)
  R.OutState();       // (bool) Производит вычисления и выдает результат Логики
  R.OutState(20.8);   // (bool) Производит вычисления и выдает результат Логики  (принимает float)
  
*/
#include <Arduino.h>  // for type definitions

#ifndef Engineer_Regulator_h
#define Engineer_Regulator_h

// Перечесление Типов регулятора
enum enumRegulatorType {
  HEATER,
  COOLER
};

// Перечесление Режимов регулятора
enum enumRegulatorMode {
  ERRORMODE=0,
  MANUALMODE=1,
  AUTOMODE=2
};

class Regulator {

  
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

	bool RLL(const bool &State);	// В зависимости от _RLL Прямая или обратная логика навыходи
  public:

    // Конструктор класса
    Regulator() {
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
	
	bool getErrorState(); // Возвращает Безопасное состояние реле
	
    float getDelta();    // Возвращает Дельта (Гистерезис)
    float getTempSet();  // Возвращает Уставкe температуры
    float getTempIn();   // Возвращает Температура от датчика температуры

    uint8_t getId();     // ID класса (порядковый номер регулятора)
    static uint8_t getCount();  // Счетчик для ID класса (порядковый номер регулятора)
    
    void printSet();// Выводит в Serial все настройки регулятора
}; // Regulator

#endif
