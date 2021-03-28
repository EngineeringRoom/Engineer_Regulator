# Engineer Regulator

Autor: Alexey Obushenkov</br>
email: sib.eng24@gmail.com</br>
url: https://www.youtube.com/channel/UCih0Gcl9IEdkR8deDJCiodg</br>

**The temperature controller works with AVR and ESP.
</br> Advantages</br>** 
- Controller type (HEATER, COOLER)</br>
- Controller mode (Automatic/Manual/Error)(AUTOMODE, MANUALMODE, ERRORMODE)</br>
- All settings are stored in the EEPROM</br> 
- You can configure the output of the Controller to work with forward and reverse logic RLL (Relay Logic Level) </br>
- Many examples. There are examples of working with <a href=”http://docs.blynk.cc”>Blynk</a></br>

**Регулятор температуры работает с AVR и ESP. </br>
Преимущества</br>**
- Тип регулятора(HEATER, COOLER) </br>
- Режим регулятора (Автоматический/Ручной/Ошибка)(AUTOMODE, MANUALMODE, ERRORMODE) </br>
- Все настройки храниться в EEPROM</br>
- Можно настроит выход Регулятора для работы с прямой и обратной логикой RLL(Relay Logic Level) </br>
- Много примеров. Есть примеры работы с <a href=”http://docs.blynk.cc”>Blynk</a></br>

Это библиотека для реализует объект Регулятор температуры. С гистирезисом.</br>
подключите в скетче библиотеку <Engineer_Regulator.h></br>
Объявите класс Regulator</br>
Работает и с AVR и с ESP</br>
Это может быть нагреватель или охладитель.</br>
Метод OutRelay() может работать с прямой и обратной логикой</br>
задается это методом setRLL()</br>
Имеет три режима: Автоматический. Ручной(по умолчанию). Ошибка.</br>
То есть пока не настроите и не переведете в Автоматический режим. Регулятор неработает.</br></br>

Может хранить настройки в EEPROM из коробки:</br>
Для AVR <Engineer_RegulatorEEPROM_AVR.h></br>
Объявите класс RegulatorEEPROM_AVR</br></br>

Для ESP <Engineer_RegulatorEEPROM_ESP.h></br>
Объявите класс RegulatorEEPROM_ESP</br>

Все сделанно что бы просто было пользоваться этим классом и заморачиваться только с логикой работы своей программы,
а не с тем как должен работать регулятор.

Ну и для удобства все методы класса доступные для вас.


## // Set "Сетеры"

**R.setAuto();**      // Устанавливаем (Автоматический Режим)</br>

**R.setManual();**    // Устанавливаем (Ручной Режим)(Автоматически Состояние на выходи станет Равным Безопасному состояние реле)</br>
**R.setManualON();**  // Устанавливаем (Ручной Режим)(Состояние на выходи станет HIGH)</br>
**R.setManualOFF();** // Устанавливаем (Ручной Режим)(Cостояние на выходи станет LOW)</br></br>

**R.setError();**         // Устанавливаем (Режим Ошибка) По датчику (в оброботчеке датчика лучши поместить)</br>
**R.setErrorState(LOW);** // (bool) Безопасное состояние Логики реле</br>

**R.setType(HEATER);**    // (uint8_t) Выбираем тип регулятора(HEATER, COOLER)</br>
**R.setRLL(HIGH);**       // (bool) Выбираем тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)</br>

**R.setDelta(0.5);**      // (float) Устанавливаем Дельта (Гистерезис)</br>
**R.setTempSet(20);**     // (float) Устанавливаем Уставку температуры</br>
**R.setTempIn(20);**      // (float) Устанавливаем Температура от датчика температуры</br>

**R.setFactorySettings();**    // Сброс настроек в заводские</br>
**R.setAllFactorySettings();** // Сбросить настройки всех Экземпляров класса после перезагрузки устройства</br></br>

## // Get "Гетеры"
**R.Alarm();**         // (bool) Возвращает Сигнал Аварии (от датчика)</br>
**R.getModeState();**  // (uint8_t) Возвращает режим регулятора состояние (Автоматический/Ручной/Ошибка)(AUTOMODE, MANUALMODE, ERRORMODE)</br></br>

**R.getType();**  // (uint8_t) Возвращает тип регулятора(HEATER, COOLER)</br>
**R.getRLL();**   // (bool) Возвращает тип выходного сигнала Уровень Логики Реле. Каким сигналом управляется (HIGH/LOW)</br>

**R.getDelta();**    // (float) Возвращает Дельта (Гистерезис)</br>
**R.getTempSet();**  // (float) Возвращает Уставку температуры</br>
**R.getTempIn();**   // (float) Возвращает Температуру от датчика температуры</br></br>

**R.getId();**     // (uint8_t) ID класса (порядковый номер регулятора)</br>
**R.getCount();**  // (uint8_t) Счетчик для ID класса (порядковый номер регулятора)</br></br>

**R.printSet();**       // Выводит в Serial все настройки регулятора</br>
**R.printAddr();**      // Выводит таблицу Адресов</br>
**R.getEEPROMaddr();**  // (uint16_t) Возвращает следующий не занятый адрес в EEPROM</br></br>

**R.OutRelay();**       // (bool) Производит вычисления и выдает результат на Реле</br>
**R.OutRelay(17.8);**   // (bool) Производит вычисления и выдает результат на Реле (принимает float)</br>
**R.OutState();**       // (bool) Производит вычисления и выдает результат Логики</br>
**R.OutState(20.8);**   // (bool) Производит вычисления и выдает результат Логики  (принимает float)</br>

