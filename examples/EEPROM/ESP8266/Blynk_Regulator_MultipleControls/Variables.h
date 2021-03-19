// Количество регуляторов
const uint8_t  NumberOfRegulators = 3;



///////////////////////////////
//      База PIN-ов


const uint8_t PIN_Relay[NumberOfRegulators] = {
  D5,
  D6,
  D7,
};

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
String _ssid     = "Redmi"; // Для хранения SSID
String _password = "7c8561a57ba3"; // Для хранения пароля сети

// Ключ Blynk
String _authB = "dca2a6bd83d3401f80b4e477bcc3a9a4";    // AUTH токен Blynk
String _serverB = "84.22.137.78";                      // Сервер Blynk "blynk-cloud.com"
String _portB = "8080";                                // Порт сервера Blynk "8080"
