// Количество регуляторов
const uint8_t  NumberOfRegulators = 4;

/////////////////////////////////////////
//      База Виртуальных PIN -ов       
const int8_t VPIN_Gauge[NumberOfRegulators] = {
  0,
  1,
  2,
  3,
};

const int8_t VPIN_ValueDisplay[NumberOfRegulators] = {
  10,
  11,
  12,
  13,
};

const int8_t VPIN_TempSet[NumberOfRegulators] = {
  20,
  21,
  22,
  23,
};

const int8_t VPIN_Led[NumberOfRegulators] = {
  30,
  31,
  32,
  33,
};

#define VPIN_Tune_Menu              40
#define VPIN_Tune_Gauge             41
#define VPIN_Tune_TempSet           42
#define VPIN_Tune_Hysteresis        43
#define VPIN_Tune_ButtonMode        44
#define VPIN_Tune_ButtonState       45
#define VPIN_Tune_ValueDisplay      46
#define VPIN_Tune_SswitchType       47
#define VPIN_Tune_SswitchRLL        48
#define VPIN_Tune_SswitchSafeState  49

///////////////////////////////
//      База PIN-ов


const uint8_t PIN_Relay[NumberOfRegulators] = {
  D5,
  D6,
  D7,
  D8,
};

// Переменные для таймера на millis();
uint32_t ms;            // Хранит время с начала работы МК


// Первое ли это подключение к Серверу
  bool isFestConnection=true;

// ID для таймеров Blynk
  int IDt_reconnectBlynk; // ID таймера для перезагрузки
  int IDt_loopBlynk;      // ID таймера для jсвязи с Blynk

  // Массив Флагов Обновления настроек
  bool FlagUpdateValueDisplay[NumberOfRegulators]={true, true, true, true};
  bool FlagUpdateProperty[NumberOfRegulators]={true, true, true, true};
  
  String ErrorMessege[NumberOfRegulators];

  uint8_t RegID=0;  // Номер регулятора на который воздействуем  

/////////////////////////////////////////
//   База переменных для подключения   //

// Логин и пароль от WiFi
//String _ssid     = "Redmi"; // Для хранения SSID
//String _password = "7c8561a57ba3"; // Для хранения пароля сети

// Логин и пароль от WiFi
String _ssid     = "dlink11"; // Для хранения SSID
String _password = "2229841212"; // Для хранения пароля сети

// Ключ Blynk
String _authB = "3b23aa467cca4103a047ef0cc386a0d4";    // AUTH токен Blynk
String _serverB = "84.22.137.78";                      // Сервер Blynk "blynk-cloud.com"
String _portB = "8080";                                // Порт сервера Blynk "8080"
