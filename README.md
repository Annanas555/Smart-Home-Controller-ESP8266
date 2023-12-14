# Smart Home Controller

## Описание проекта

Проект "Smart Home Controller" представляет собой веб-сервер на базе ESP8266, который предоставляет информацию о температуре и влажности с датчика DHT11, управляет реле, а также выводит текущее время и дату.

## Подключение

- **DHT11:** Подключен к пину D2.
- **Реле:** Подключено к пину D3.

## Настройки Wi-Fi

```cpp
const char *ssid = "SSID";            // Имя сети
const char *passPhrase = "PASSWORD";   // Пароль
```

## Библиотеки

- [ESP8266WebServer](https://github.com/esp8266/ESP8266WebServer)
- [DHT](https://github.com/adafruit/DHT-sensor-library)
- [NTPClient](https://github.com/arduino-libraries/NTPClient)
- [WiFiUdp](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/src)
- [TimeLib](https://github.com/PaulStoffregen/Time)

## Установка

1. Клонируйте репозиторий.
2. Откройте Arduino IDE.
3. Установите необходимые библиотеки с помощью Менеджера библиотек (`Скетч` > `Подключить библиотеку` > `Управление библиотеками`).
4. Настройте переменные `ssid` и `passPhrase` с вашими учетными данными WiFi, при необходимости измените часовой пояс.
5. Загрузите код на вашу плату ESP8266.

## Использование

1. Подключите ваш ESP8266 к нужным устройствам (датчик DHT11, реле).
2. Включите ESP8266.
3. Получите доступ к веб-интерфейсу, перейдя по адресу `http://<ESP8266_IP>` в веб-браузере.
4. Просматривайте и управляйте температурой, влажностью, статусом реле, датой и временем.

### Просмотр страницы на ПК

![image](https://github.com/Annanas555/Smart-Home-Controller-ESP8266/assets/128131401/66644a8c-889b-4c35-ba52-25241c324c8d)

### Просмотр страницы на мобильном устройстве

![image](https://github.com/Annanas555/Smart-Home-Controller-ESP8266/assets/128131401/43b71762-04db-4ae9-80a7-96a59f2db587)


Не стесняйтесь настраивать и расширять этот проект в соответствии с вашими конкретными потребностями для умного дома!
