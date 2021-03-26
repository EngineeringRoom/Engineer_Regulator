void WIFIinit() {
  // Попытка подключения к точке доступа
  WiFi.mode(WIFI_STA);
  byte tries = 5; // Количество попыток
  WiFi.begin(_ssid.c_str(), _password.c_str());
  // Делаем проверку подключения до тех пор пока счетчик tries
  // не станет равен нулю или не получим подключение
  while (--tries && WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    // Если не удалось подключиться Скажем об этом
    Serial.println("");
    Serial.println("WiFi not connected");
  }
  else {
    // Иначе удалось подключиться отправляем сообщение
    // о подключении и выводим адрес IP
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void ConnectBlynk(){
  timer.disable(IDt_reconnectBlynk); // Выключаем таймер
  
  //*******************************************************
  // Запускаем WiFi
  if (WiFi.status() != WL_CONNECTED){// Если нет WiFi, то коннектимся
  WIFIinit();    
  }
  /*  Дисконект - для справки 
    WiFi.disconnect(); // отключаемся от сети
    Serial.println("Disconnect WiFi.");
  */
  
  //*******************************************************
  // Запускаем Blynk

  if (WiFi.status() == WL_CONNECTED){// Если нет WiFi, то не коннектимся
    Blynk.config(_authB.c_str(), _serverB.c_str(), _portB.toInt());
    Serial.println(Blynk.connect(5000));     
  }
  

  
  // До бесконечности будем оставаться в цикле while
  // пока не установим связь с сервером
  // while (Blynk.connect() == false) {}
  
  timer.enable(IDt_reconnectBlynk); // Включаем таймер
  timer.restartTimer(IDt_reconnectBlynk); // Перезапускаем таймер
}//ConnectBlynk()

// Реконектимся если обрыв связи
void reconnectBlynk() {
  if (!Blynk.connected()){
    Serial.println("Diconnect...");
    ConnectBlynk();
    if (Blynk.connected()){
        Serial.println("Reconnected");
     } 
    else{
        Blynk.disconnect();
        isFestConnection=true;
        Serial.println("Not reconnected");
      if (WiFi.status() == WL_CONNECTED)  {
          Serial.println("not Server");
      } else {
          Serial.println("not WiFi");           
      }
    }
  }
  
}//reconnectBlynk()
