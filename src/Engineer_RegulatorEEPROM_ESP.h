#ifndef Engineer_RegulatorEEPROM_ESP_h
#define Engineer_RegulatorEEPROM_ESP_h

#include <Engineer_RegulatorEEPROM.h>

////////////////////////////////////////////////
// Работа с RegulatorEEPROM_ESP

class RegulatorEEPROM_ESP : public RegulatorEEPROM {

 
  protected:

    // Сохраняем данные в EEPROM
    void SaveSettigsEEPROM(uint16_t addr, uint8_t &value) override {
      EEPROM.begin(EEPROMaddr); 
        EEPROM_writeAnything(addr, value);
      EEPROM.end();
    }
    // Сохраняем данные в EEPROM
    void SaveSettigsEEPROM(uint16_t addr, float &value) override  {
      EEPROM.begin(EEPROMaddr);
        EEPROM.put(addr,  value);
      EEPROM.end();
    }
    // Сохраняем данные в EEPROM
    void SaveSettigsEEPROM(uint16_t addr, bool &value) override  {
      EEPROM.begin(EEPROMaddr);
        EEPROM_writeAnything(addr, value);
      EEPROM.end();
    }

     // Проверяем что записанно в нулевой ячеке EEPROM
     // Проверяем инициализированна ли память
    bool checkEEPROM() override {
      uint16_t check;
      EEPROM.begin(sizeof(EEPROMcheck));
      EEPROM_readAnything(0, check);
      EEPROM.end();
      if(EEPROMcheck == check){
        return true;
      }
      else{
        if(count == _id){
          EEPROM.begin(sizeof(EEPROMcheck));
          EEPROM_writeAnything(0, EEPROMcheck);
          EEPROM.end();
        }
        return false;
       }
    }

public:

    RegulatorEEPROM_ESP() {
      initEEPROM();
	  _TempIn = _TempSet;
    }
protected:    
    // Читаем все настройки из EEPROM
    void ReadAllSettingsEEPROM() {
      EEPROM.begin(EEPROMaddr);
        EEPROM_readAnything(_a[a_RegulatorType], _RegulatorType);
        EEPROM_readAnything(_a[a_Mode], _Mode);
        EEPROM_readAnything(_a[a_ErrState], _ErrState);
        EEPROM_readAnything(_a[a_RLL], _RLL);
  
        EEPROM.get(_a[a_TempSet],  _TempSet);
        EEPROM.get(_a[a_Delta],   _Delta);
      EEPROM.end();
    }
public:
    // Сбросить настройки всех Экземпляров класса после перезагрузки устройства
    void setAllFactorySettings()override {
      EEPROMcheck++;
      EEPROM.begin(EEPROMaddr);
      EEPROM_writeAnything(0, EEPROMcheck);
      EEPROM.end();
    }

}; // RegulatorEEPROM_ESP

#endif
