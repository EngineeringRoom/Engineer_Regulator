

///////////////////////////////
// База PIN-ов

const uint8_t PIN_Relay = 13;

// Переменные для таймера на millis();
uint32_t ms;            // Хранит время с начала работы МК

// Хранят время с последнего события
uint32_t msR;           // Служит для опроса датчика температуры с заданным интевалом
uint32_t msM;           // Служит для эмуляции нажатия кнопок Включить или выключить в ручном режиме

const uint32_t TIMEOUT_R = 500;  // Частота опроса датчика температуры (ставим const если не собираемся менять это значение в скетче)
const uint32_t TIMEOUT_M = 10000;  // Частота нажатия кнопок (ставим const если не собираемся менять это значение в скетче)
