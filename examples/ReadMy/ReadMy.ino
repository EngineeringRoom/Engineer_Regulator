#if defined(USE_HELPMY)

      Autor: Alexey Obushenkov (Инжинерка - EngineeringRoom)
      email: sib.eng24@gmail.com
      YouTube: https://www.youtube.com/channel/UCih0Gcl9IEdkR8deDJCiodg
      GitHAB: https://github.com/EngineeringRoom
      Донат можно кинуть тут Мой бусти: https://boosty.to/sibeng24
      Я буду очень вам признателен.

  The temperature controller works with AVR and ESP.
  Advantages
  - Controller type (HEATER, COOLER)
  - Controller mode (Automatic/Manual/Error)(AUTOMODE, MANUALMODE, ERRORMODE)
  - All settings are stored in the EEPROM
  - You can configure the output of the Controller to work with forward and reverse logic RLL (Relay Logic Level)
  - Many examples. There are examples of working with Blynk.
  
  Регулятор температуры работает с AVR и ESP.
  Преимущества
  - Тип регулятора(HEATER, COOLER)
  - Режим регулятора (Автоматический/Ручной/Ошибка)(AUTOMODE, MANUALMODE, ERRORMODE)
  - Все настройки храниться в EEPROM
  - Можно настроит выход Регулятора для работы с прямой и обратной логикой RLL(Relay Logic Level)
  - Много примеров. Есть примеры работы Blynk.
  
  Это библиотека для реализует объект Регулятор температуры. С гистирезисом.
  подключите в скетче библиотеку <Engineer_Regulator.h>
  Объявите класс Regulator
  Работает и с AVR и с ESP
  Это может быть нагреватель или охладитель.
  Метод OutRelay() может работать с прямой и обратной логикой
  задается это методом setRLL()
  Имеет три режима: Автоматический. Ручной(по умолчанию). Ошибка.
  То есть пока не настроите и не переведете в Автоматический режим. Регулятор неработает.

  Может хранить настройки в EEPROM из коробки:
  Для AVR <Engineer_RegulatorEEPROM_AVR.h>
  Объявите класс RegulatorEEPROM_AVR

  Для ESP <Engineer_RegulatorEEPROM_ESP.h>
  Объявите класс RegulatorEEPROM_ESP

  Все сделанно что бы просто было пользоваться этим классом и заморачиваться только с логикой работы своей программы,
  а не с тем как должен работать регулятор.

  Ну и для удобства все методы класса доступные для вас.

#if defined(USE_HELPMY)
// Set "Сетеры"
  R.setAuto();      // Выбираем (Автоматический Режим)
  
  R.setManual();    // Выбираем (Ручной Режим)(Автоматически Состояние на выходи станет Равным Безопасному состояние реле)
  R.setManualON();  // Выбираем (Ручной Режим)(Состояние на выходи станет HIGH)
  R.setManualOFF(); // Выбираем (Ручной Режим)(Cостояние на выходи станет LOW)
  
  R.setError();         // Выбираем (Ручной Ошибка) По датчику
  R.setErrorState(LOW); // Безопасное состояние логики реле (bool)
  
  R.setType(HEATER);    // Выбираем тип регулятора(HEATER, COOLER)
  R.setRLL(HIGH);       // Выбираем тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
  
  R.setDelta(0.5);      // Устанавливаем Дельта (Гистерезис)
  R.setTempSet(20);     // Устанавливаем Уставку температуры
  R.setTempIn(20);      // Устанавливаем Температура от датчика температуры

  R.setFactorySettings();    // Сброс настроек в заводские
  R.setAllFactorySettings(); // (EEPROM) Сбросить настройки всех Экземпляров класса после перезагрузки устройства 

// Get "Гетеры"
  R.Alarm();         // (bool) Возвращает Сигнал Аварии (от датчика)
  R.getModeState();  // (uint8_t) Возвращает режим регулятора состояние (Автоматический/Ручной/Ошибка)(ERRORMODE, MANUALMODE, AUTOMODE)
  
  R.getType();          // (uint8_t) Возвращает тип регулятора(HEATER, COOLER)
  R.getRLL();           // (bool) Возвращает тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)
  R.getErrorState(); // (bool) Возвращает безопасное состояние логики реле 
  
  R.getDelta();    // (float) Возвращает Дельта (Гистерезис)
  R.getTempSet();  // (float) Возвращает Уставку температуры
  R.getTempIn();   // (float) Возвращает Температуру от датчика температуры

  R.getId();     // (uint8_t) ID класса (порядковый номер регулятора)
  R.getCount();  // (uint8_t) Счетчик для ID класса (порядковый номер регулятора)
      
  R.printSet();       // Выводит в Serial все настройки регулятора
  R.printAddr();      // (EEPROM) Выводит таблицу Адресов
  R.getEEPROMaddr();  // (EEPROM)(uint16_t) Возвращает следующий не занятый адрес в EEPROM
  
  R.OutRelay();            // (bool) Производит вычисления и выдает результат на Реле
  R.OutRelay(17.8);        // (bool) Производит вычисления и выдает результат на Реле
  R.OutState();       // (bool) Производит вычисления и выдает результат Логики
  R.OutState(20.8);   // (bool) Производит вычисления и выдает результат Логики

#endif USE_HELPMY

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
