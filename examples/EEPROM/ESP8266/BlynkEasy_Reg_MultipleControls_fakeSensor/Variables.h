// Количество регуляторов
const uint8_t  NumberOfRegulators = 3;

/////////////////////////////////////////
//      База Виртуальных PIN -ов       
const int8_t VPIN_Gauge[NumberOfRegulators] = {
  0,
  1,
  2,
};

const int8_t VPIN_NumericInput[NumberOfRegulators] = {
  10,
  11,
  12,
};

const int8_t VPIN_ButtonMode[NumberOfRegulators] = {
  20,
  21,
  22,
};

const int8_t VPIN_ButtonState[NumberOfRegulators] = {
  30,
  31,
  32,
};


///////////////////////////////
//      База PIN-ов


const uint8_t PIN_Relay[NumberOfRegulators] = {
  D5,
  D6,
  D7,
};

// Переменные для таймера на millis();
uint32_t ms;            // Хранит время с начала работы МК


// Первое ли это подключение к Серверу
  bool isFestConnection=true;

// ID для таймеров Blynk
  int IDt_reconnectBlynk; // ID таймера для перезагрузки

/////////////////////////////////////////
//   База переменных для подключения   //

// Логин и пароль от WiFi
String _ssid     = "YourNetworkName"; // Для хранения SSID
String _password = "YourPassword"; // Для хранения пароля сети

// Ключ Blynk
String _authB = "11112222333344445555666677778888";    // AUTH токен Blynk
String _serverB = "blynk-cloud.com";                   // Сервер Blynk "blynk-cloud.com"
String _portB = "8080";                                // Порт сервера Blynk "8080"
