#ifndef Sibeng24RegulatorEEPROM_h
#define Sibeng24RegulatorEEPROM_h

#include <EEPROM.h>
//#include <EEPROMAnything.h> // http://playground.arduino.cc/Code/EEPROMWriteAnything

#include <Sibeng24Regulator.h>

////////////////////////////////////////////////
// Работа с Sibeng24RegulatorEEPROM

class Sibeng24RegulatorEEPROM : public Sibeng24Regulator {

  protected:
  
	 template <class T> int EEPROM_writeAnything(int ee, const T& value)
	{
		const byte* p = (const byte*)(const void*)&value;
		unsigned int i;
		for (i = 0; i < sizeof(value); i++)
		  EEPROM.write(ee++, *p++);
		return i;
	}

	template <class T> int EEPROM_readAnything(int ee, T& value)
	{
		byte* p = (byte*)(void*)&value;
		unsigned int i;
		for (i = 0; i < sizeof(value); i++)
		  *p++ = EEPROM.read(ee++);
		return i;
	} 
  
  
  
    // Статические переменные, одни и те же для всех экземпляров класса
    static uint16_t  EEPROMaddr;        // адрес EEPROM
    static uint16_t  EEPROMcheck;       // служит для проверки записанно у нас что то в EEPROM или нет                              

  public:
    // Возвращает следующий не занятый адрес в EEPROM
    uint16_t getEEPROMaddr(){
      return EEPROMaddr;
    }  
  
  protected:  
     
     uint16_t _a[6];       // Массив адресов EEPROM. Хранятся адреса переменных.

    // Перечесление для удобного доступа к адресу переменной в EEPROM.
    // Сами адреса переменных хранятся в массиве a объявленном выше.
    enum _n_addr {
      a_RegulatorType,
      a_Mode,
      a_TempSet,
      a_Delta,
      a_ErrState,
      a_RLL
    };

    // Сохраняем данные в EEPROM
    virtual void SaveSettigsEEPROM(uint16_t addr, uint8_t &value)=0;
    // Сохраняем данные в EEPROM
    virtual void SaveSettigsEEPROM(uint16_t addr, float &value)=0;
    // Сохраняем данные в EEPROM
    virtual void SaveSettigsEEPROM(uint16_t addr, bool &value)=0;


    void SaveAllSettingsEEPROM() {
        SaveSettigsEEPROM(_a[a_RegulatorType], _RegulatorType);
        SaveSettigsEEPROM(_a[a_Mode], _Mode);
        SaveSettigsEEPROM(_a[a_ErrState], _ErrState);
        SaveSettigsEEPROM(_a[a_RLL], _RLL);
  
        SaveSettigsEEPROM(_a[a_TempSet],  _TempSet);
        SaveSettigsEEPROM(_a[a_Delta],   _Delta);
    }


  public:
    void printAddr() {
      // Выводит таблицу:
      //             / адрес в EEPROM         /       ИМЯ переменной             / Размер занимаемый переменной в байтах /
      Serial.println();
      Serial.print(F("Regulator: "));Serial.print(_id);
      Serial.println(F("  Addr list:"));
      Serial.print(_a[_n_addr::a_RegulatorType]);    Serial.print(" RegulatorType ");  Serial.println(sizeof(_RegulatorType));
      Serial.print(_a[_n_addr::a_Mode]);             Serial.print(" Mode ");           Serial.println(sizeof(_Mode));
      Serial.print(_a[_n_addr::a_ErrState]);         Serial.print(" ErrState ");       Serial.println(sizeof(_ErrState));
      Serial.print(_a[_n_addr::a_RLL]);              Serial.print(" RLL ");            Serial.println(sizeof(_RLL));
      Serial.print(_a[_n_addr::a_TempSet]);          Serial.print(" TempSet ");        Serial.println(sizeof(_TempSet));
      Serial.print(_a[_n_addr::a_Delta]);            Serial.print(" Delta ");          Serial.println(sizeof(_Delta));
      Serial.print(F("Next addr EEPROMRegulator: "));
      Serial.println(_a[_n_addr::a_Delta]+sizeof(_Delta));
      Serial.println();
    }
  protected:
    
    // Функция служит для автоматического заполнения Массива адресов EEPROM.
    void initAddrEEPROM() {
       if(_id == 1){EEPROMaddr += sizeof(EEPROMcheck);}
      _a[a_RegulatorType] = EEPROMaddr;   EEPROMaddr += sizeof(_RegulatorType);
      _a[a_Mode] = EEPROMaddr;            EEPROMaddr += sizeof(_Mode);
      _a[a_ErrState] = EEPROMaddr;        EEPROMaddr += sizeof(_ErrState);
      _a[a_RLL] = EEPROMaddr;             EEPROMaddr += sizeof(_RLL);
      _a[a_TempSet] = EEPROMaddr;         EEPROMaddr += sizeof(_TempSet);
      _a[a_Delta] = EEPROMaddr;           EEPROMaddr += sizeof(_Delta);

    }

    // Читаем все настройки из EEPROM
    virtual void ReadAllSettingsEEPROM()=0;

  public:
    // Сброс настроек в заводские
    void setFactorySettings() {
      _RegulatorType = HEATER;
      _RLL = HIGH;
      _Mode = MANUALMODE;

      _TempIn = 0;
      _TempSet = 0.0;
      _Delta = 1.0;
      _ErrState = LOW;

      SaveAllSettingsEEPROM();
    }

    // Сбросить настройки всех Экземпляров класса после перезагрузки устройства
    virtual void setAllFactorySettings() = 0;
    
  protected:
    // Проверяем что записанно в нулевой ячеке EEPROM
    // Проверяем инициализированна ли память
    virtual bool checkEEPROM()=0;

    // Инициализируем EEPROM
    void initEEPROM(){
      initAddrEEPROM();                        // Заполняем массив адресов
      
      if(checkEEPROM()){                       // Проверяем что записанно в нулевой ячеке EEPROM
        ReadAllSettingsEEPROM();               // Читаем все настройки из EEPROM 
      }
      else{
        setFactorySettings();
      }
    }

  public:
  
    // Выбираем (Автоматический Режим)
    void setAuto() {
      _Mode = AUTOMODE;
      SaveSettigsEEPROM(_a[a_Mode], _Mode);
    }


    // Выбираем (Ручной Режим)(Автоматически Состояние на выходи станет LOW)
    void setManual() {
      _Mode = MANUALMODE;  _State = _ErrState;
      SaveSettigsEEPROM(_a[a_Mode], _Mode);
    }

    // Выбираем (Ручной Режим)(Состояние на выходи станет HIGH)
    void setManualON() {
      _Mode = MANUALMODE; _State = HIGH;
      SaveSettigsEEPROM(_a[a_Mode], _Mode);
    }
    // Выбираем (Ручной Режим)(Автоматически Состояние на выходи станет LOW)
    void setManualOFF() {
      _Mode = MANUALMODE; _State = LOW;
      SaveSettigsEEPROM(_a[a_Mode], _Mode);
    }


    // Выбираем (Ошибка)
    void setError() {
      _Mode = ERRORMODE;
      SaveSettigsEEPROM(_a[a_Mode], _Mode);
    }

    // Безопасное состояние реле
    void setErrorState(bool ErrState) {
      _ErrState = ErrState;
      SaveSettigsEEPROM(_a[a_ErrState], _ErrState);
    }



    // Выбираем тип регулятора
    void setType(uint8_t Type) {
      _RegulatorType = Type;
      SaveSettigsEEPROM(_a[a_RegulatorType], _RegulatorType);
    }

    // Выбираем тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
    void setRLL(bool RLL) {
      _RLL = RLL;
      SaveSettigsEEPROM(_a[a_RLL], _RLL);
    }


    // Устанавливаем Дельта (Гистерезис)
    void setDelta(float Delta) {
      _Delta = Delta;
      SaveSettigsEEPROM(_a[a_Delta], _Delta);
    }

    // Устанавливаем Уставкe температуры
    void setTempSet(float TempSet) {
      _TempSet = TempSet;
      SaveSettigsEEPROM(_a[a_TempSet], _TempSet);
    }


}; // Sibeng24RegulatorEEPROM

    // Статические переменные, одни и те же для всех экземпляров класса
  uint16_t  Sibeng24RegulatorEEPROM::EEPROMaddr=0;          // адрес EEPROM
  uint16_t  Sibeng24RegulatorEEPROM::EEPROMcheck=12456;     // служит для проверки записанно у нас что то в EEPROM или нет  

#endif