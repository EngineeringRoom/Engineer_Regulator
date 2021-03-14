// Функция таймера на millis()
// Взозвращает true или false
// &msF вот занчек & перед переменной означает, что все действия внутри функции будут записанны в переменную которую мы сюда подставим
// это называется ссылка

bool Timer(uint32_t &msF, uint32_t timeOut, bool refresh_ms = true) {
  if ((int32_t)(ms - msF) >= timeOut) {
    if (refresh_ms) {
      msF = ms; //Если refresh_ms=true (значение по умолчанию), то обновим время в переменной &msF
    }
    return true;
  }
  else {
    return false;
  }
}
