#ifndef EPAPER19_CLASS_H
#define EPAPER19_CLASS_H

#include <Arduino.h>
#include <Wire.h>
#include <garretlab_IST7134.h>

class garretlab_ePaper19 {
private:
  garretlab_IST7134 *ist7134;

  float temperature = 0;
  float humidity = 0;

  enum temperatureUnit_t {
    celsius,
    fahrenheit,
    none
  } temperatureUnit = celsius;

  bool displayPercent = true;
  bool displayBluetooth = false;
  bool displayBattery = false;

  static constexpr uint8_t digits[10][2] = {
    {0xbf, 0x1f},  // 0
    {0x00, 0x1f},  // 1
    {0xfd, 0x17},  // 2
    {0xf5, 0x1f},  // 3
    {0x47, 0x1f},  // 4
    {0xf7, 0x1d},  // 5
    {0xff, 0x1d},  // 6
    {0x21, 0x1f},  // 7
    {0xff, 0x1f},  // 8
    {0xf7, 0x1f},  // 9
  };

  void makeData(uint8_t data[]);

public:
  garretlab_ePaper19(TwoWire &wire);

  void begin(byte rstPin = 8, byte busyPin = 7,
             uint8_t commandAddress = 0x3c,
             uint8_t dataAddress = 0x3d);
  void setTemperature(float temperature);
  void setTemperatureUnit(temperatureUnit_t temperatureUnit);
  void setHumidity(float humidity);
  void percentOn();
  void percentOff();
  void bluetoothOn();
  void bluetoothOff();
  void batteryOn();
  void batteryOff();
  void waveFormSetGC();
  void waveFormSetDUWB();
  void waveFormSet5S();
  void display();
  void display(float temperature, float humidity, temperatureUnit_t temperatureUnit = celsius);
  int8_t temperatureSensorData();
};

#endif  // EP19_CLASS_H