/////////////////////////////////////////
//      База Виртуальных PIN -ов       

///////////////////////////////
//      База PIN-ов

const uint8_t PIN_Relay = D5;

// Переменные для таймера на millis();
uint32_t ms;            // Хранит время с начала работы МК

// Хранят время с последнего события
uint32_t msR;           // Служит для опроса датчика температуры с заданным интевалом

const uint32_t TIMEOUT_R = 1000;  // Частота опроса датчика температуры (ставим const если не собираемся менять это значение в скетче)


// Первое ли это подключение к Серверу
  bool isFestConnection=true;

// ID для таймеров Blynk
  int IDt_reconnectBlynk; // ID таймера для перезагрузки

/////////////////////////////////////////
//   База переменных для подключения   //

// Логин и пароль от WiFi
String _ssid     = "MTS_sib.eng24"; // Для хранения SSID
String _password = "2404498A"; // Для хранения пароля сети

// Ключ Blynk
String _authB = "1bceb51ed96648b199df791079c176c6";    // AUTH токен Blynk
String _serverB = "blynk-cloud.com";                   // Сервер Blynk "blynk-cloud.com"
String _portB = "8080";                                // Порт сервера Blynk "8080"
