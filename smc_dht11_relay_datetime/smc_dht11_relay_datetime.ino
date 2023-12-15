#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

#define DHTPIN D2  // Пин, к которому подключен dht
#define DHTTYPE DHT11

#define RELAY_PIN D3  // Пин, к которому подключено реле

DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "SSID";              // Имя сети
const char *passPhrase = "PASSWORD";   // Пароль

#define HOSTNAME "192.168.211.119"     // Имя сервера
#define TIMEZONE "CET-2CEST,M3.5.0,M10.5.0/3"  // Часовой пояс Москва

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 10800); // Смещение для московского времени

ESP8266WebServer server(80);

bool relayState = false;

void handleRoot() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  String page = "<html><head><meta charset=\"UTF-8\"><style>";
  page += "body { background-color: white; color: #1E88E5; font-family: 'Roboto', sans-serif; text-align: center; margin-top: 5%; }";
  page += "h1 { color: #1E88E5; font-size: 2.5em; }";
  page += "p { color: #1E88E5; font-size: 2em; }";
  page += "div#anekdot-container { color: #1E88E5; font-size: 2em; margin: 10px 0; }";
  page += "button { font-family: 'Roboto', sans-serif; font-size: 2em; padding: 15px 30px; margin-top: 20px; background-color: #1E88E5; color: white; }";
  page += "</style></head><body>";
  page += "<div style=\"width: 70%; margin: 0 auto;\">";  // Обертка для центрирования
  
  // Добавление вывода текущей даты и времени
  timeClient.update();

  String formattedTime = timeClient.getFormattedTime();
  
  int currentYear = year(timeClient.getEpochTime());
  int currentMonth = month(timeClient.getEpochTime());
  int currentDay = day(timeClient.getEpochTime());
  int currentWeekday = weekday(timeClient.getEpochTime());

  // Определение названия дня недели
  String weekdays[] = {"Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};
  String currentWeekdayName = weekdays[currentWeekday-1];

  page += "<h1>Текущее время</h1>";
  page += "<p>" + formattedTime + "</p>";
  
  page += "<h1>Текущий день недели</h1>";
  page += "<p>" + currentWeekdayName + "</p>";

  page += "<h1>Текущая дата</h1>";
  page += "<p>" + String(currentDay) + "." + String(currentMonth) + "." + String(currentYear) + "</p>";

  // DHT11
  page += "<h1>Датчик DHT11</h1>";

  if (!isnan(temperature) && !isnan(humidity)) {
    page += "<p>Температура: " + String(temperature) + "°C</p>";
    page += "<p>Влажность: " + String(humidity) + "%</p>";
  }
  else {
    page += "<p>Ошибка при считывании данных с датчика DHT!</p>";
  }
  page += "<button onclick=\"updateData()\">Обновить</button>";

  // Реле
  page += "<h1>Реле</h1>";
  page += "<p>Статус: " + String(relayState ? "Выключено" : "Включено") + "</p>";
  page += "<button onclick=\"toggleRelay(); updateData();\">" + String(relayState ? "On" : "Off") + "</button>";

  // Добавление вывода случайного анекдота
  page += "<h1>Случайный анекдот</h1>";
  page += "<div id='anekdot-container'>";
  page += "<script src=\"https://www.anekdot.ru/rss/randomu.html\" charset=\"utf-8\"></script>";
  page += "</div>";

  page += "</div>";  // Закрытие обертки
  
  // Скрипты
  page += "<script>function toggleRelay() { fetch('/toggleRelay').then(response => response.text()).then(data => { document.querySelector('p').innerText = 'Статус: ' + data; document.querySelector('button').innerText = data === 'On' ? 'Off' : 'On'; }); }</script>";
  page += "<script>function updateData() { location.reload(); }</script>";
  page += "</body></html>";

  server.sendHeader("Content-Length", String(page.length()));
  server.send(200, "text/html; charset=utf-8", page);
}

void handleToggleRelay() {
  relayState = !relayState;
  digitalWrite(RELAY_PIN, relayState ? HIGH : LOW);
  server.send(200, "text/plain", relayState ? "Off" : "On");
}

void setup(void) {
  delay(3000);
  Serial.begin(115200);
  dht.begin();
  Serial.setDebugOutput(false);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  WiFi.mode(WIFI_STA);
  if (strlen(ssid) == 0) {
    WiFi.begin();
  }
  else {
    WiFi.begin(ssid, passPhrase);
  }

  WiFi.setHostname(HOSTNAME);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected.");

  configTime(TIMEZONE, "pool.ntp.org");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/toggleRelay", HTTP_GET, handleToggleRelay);

  server.begin();
  Serial.println("Server started.");
}

void loop(void) {
  server.handleClient();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Ошибка при считывании данных с датчика DHT!");
    return;
  }

  Serial.print("Влажность: ");
  Serial.print(humidity);
  Serial.print("%\tТемпература: ");
  Serial.print(temperature);
  Serial.println("°C");

  delay(2000);  // Интервал между измерениями и обновлением данных
}