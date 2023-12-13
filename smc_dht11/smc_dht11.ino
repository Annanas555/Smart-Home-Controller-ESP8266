#include <DHT.h>

#define DHTPIN D2  // Пин подключения датчика DHT11

// Выбор модели датчика DHT (DHT11)
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
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
