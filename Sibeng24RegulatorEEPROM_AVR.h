#ifndef Sibeng24RegulatorEEPROM_AVR_h
#define Sibeng24RegulatorEEPROM_AVR_h

#include <Sibeng24RegulatorEEPROM.h>

////////////////////////////////////////////////
// Работа с Sibeng24RegulatorEEPROM_AVR

class Sibeng24RegulatorEEPROM_AVR : public Sibeng24RegulatorEEPROM {

 
  protected:

    // Сохраняем данные в EEPROM
    void SaveSettigsEEPROM(uint16_t addr, uint8_t &value) override  {
        EEPROM_writeAnything(addr, value);
    }
    // Сохраняем данные в EEPROM
    void SaveSettigsEEPROM(uint16_t addr, float &value) override  {
        EEPROM.put(addr,  value);
    }
    // Сохраняем данные в EEPROM
    void SaveSettigsEEPROM(uint16_t addr, bool &value) override  {
        EEPROM_writeAnything(addr, value);
    }

     // Проверяем что записанно в нулевой ячеке EEPROM
     // Проверяем инициализированна ли память
    bool checkEEPROM() override {
      uint16_t check;
      EEPROM_readAnything(0, check);
      if(EEPROMcheck == check){
        return true;
      }
      else{
        if(count == _id){
          EEPROM_writeAnything(0, EEPROMcheck);
        }
        return false;
       }
    }

public:

    Sibeng24RegulatorEEPROM_AVR() {
      initEEPROM();
    }
protected:    
    // Читаем все настройки из EEPROM
    void ReadAllSettingsEEPROM() {
        EEPROM_readAnything(_a[a_RegulatorType], _RegulatorType);
        EEPROM_readAnything(_a[a_Mode], _Mode);
        EEPROM_readAnything(_a[a_ErrState], _ErrState);
        EEPROM_readAnything(_a[a_RLL], _RLL);
  
        EEPROM.get(_a[a_TempSet],  _TempSet);
        EEPROM.get(_a[a_Delta],   _Delta);
    }
public:
    // Сбросить настройки всех Экземпляров класса после перезагрузки устройства
    void setAllFactorySettings() override {
      EEPROMcheck++;
      EEPROM_writeAnything(0, EEPROMcheck);
    }

}; // Sibeng24RegulatorEEPROM_AVR

#endif Sibeng24RegulatorEEPROM_AVR_h
