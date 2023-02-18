#include <Arduino_HTS221.h>
#include <garretlab_ePaper19.h>

#undef DEBUG
garretlab_ePaper19 epaper(Wire);

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
#else
  delay(1000);
#endif
  if (!HTS.begin()) {
    while (1)
      ;
  }

  epaper.begin();
}

void loop() {
  float temperature = HTS.readTemperature();
  float humidity = HTS.readHumidity();

#ifdef DEBUG
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
#endif

  epaper.setTemperature(temperature);
  epaper.setHumidity(humidity);
  epaper.waveFormSetGC();
  epaper.display();
  delay(30 * 1000);
}
