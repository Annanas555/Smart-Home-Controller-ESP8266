#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTPIN D2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "SSID"; // Имя сети
const char *passPhrase = "PASSWORD"; // Пароль
#define HOSTNAME "192.168.211.119"  // Имя сервера
#define TIMEZONE "CET-2CEST,M3.5.0,M10.5.0/3"  // Часовой пояс Москва

ESP8266WebServer server(80);

void handleRoot() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  String page = "<html><head><meta charset=\"UTF-8\"></head><body>";
  page += "<h1>Датчик DHT11</h1>";
  
  if (!isnan(temperature) && !isnan(humidity)) {
    page += "<p>Температура: " + String(temperature) + "°C</p>";
    page += "<p>Влажность: " + String(humidity) + "%</p>";
  } else {
    page += "<p>Ошибка при считывании данных с датчика DHT!</p>";
  }

  page += "<button onclick=\"updateData()\">Обновить</button>";
  page += "<script>function updateData() { location.reload(); }</script>";
  page += "</body></html>";

  server.sendHeader("Content-Length", String(page.length()));
  server.send(200, "text/html; charset=utf-8", page);
}


void setup(void) {
  delay(3000);
  Serial.begin(115200);
  dht.begin();
  Serial.setDebugOutput(false);

  WiFi.mode(WIFI_STA);
  if (strlen(ssid) == 0) {
    WiFi.begin();
  } else {
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

  server.begin();
  Serial.println("Server started.");
}

void loop(void) {
  server.handleClient();

  delay(2000);  // Задержка между измерениями

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
}