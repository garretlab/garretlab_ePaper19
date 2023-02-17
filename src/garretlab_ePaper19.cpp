#include <garretlab_ePaper19.h>

// Constructor.
garretlab_ePaper19::garretlab_ePaper19(TwoWire &wire) {
  ist7134 = new garretlab_IST7134(wire);
}

// Initialize system.
void garretlab_ePaper19::begin(byte rstPin, byte busyPin,
                      uint8_t commandAddress, uint8_t dataAddress) {
  ist7134->begin(rstPin, busyPin, commandAddress, dataAddress);
  ist7134->powerOff();
  ist7134->setBoostRatio(0x03);
  ist7134->TSONOn();
  ist7134->frameSet(0x0f);
}

void garretlab_ePaper19::setTemperature(float temperature) {
  this->temperature = temperature;
}

void garretlab_ePaper19::setTemperatureUnit(temperatureUnit_t temperatureUnit) {
  this->temperatureUnit = temperatureUnit;
}

void garretlab_ePaper19::setHumidity(float humidity) {
  this->humidity = humidity;
}

void garretlab_ePaper19::percentOn() {
  this->displayPercent = true;
}

void garretlab_ePaper19::percentOff() {
  this->displayPercent = false;
}

void garretlab_ePaper19::bluetoothOn() {
  this->displayBluetooth = true;
}

void garretlab_ePaper19::bluetoothOff() {
  this->displayBluetooth = false;
}

void garretlab_ePaper19::batteryOn() {
  this->displayBattery = true;
}

void garretlab_ePaper19::batteryOff() {
  this->displayBattery = false;
}

void garretlab_ePaper19::waveFormSetGC() {
  ist7134->waveFormSet(0x20, 0x00, 0xa0, 0x80, 0x40);
}

void garretlab_ePaper19::waveFormSetDUWB() {
  ist7134->waveFormSet(0x80, 0x00, 0xc0, 0x80, 0x80);
}

void garretlab_ePaper19::waveFormSet5S() {
  ist7134->waveFormSet(0x28, 0x20, 0xa8, 0xa0, 0x50);
}

void garretlab_ePaper19::makeData(uint8_t data[]) {
  int number;

  number = abs(temperature * 10);

  for (int i = 0; i < 3; i++) {
    int pos[] = {11, 3, 1};
    uint8_t d = number % 10;

    data[pos[i]] = digits[d][0];
    data[pos[i] + 1] = digits[d][1];
    number /= 10;
  }

  if (temperature >= 100) {
    data[0] = 0x1f;
  } else if ((temperature < 10) && (temperature >= 0)) {
    data[1] = data[2] = 0;
  } else if ((temperature < 0) && (temperature > -10)) {
    data[1] = 0x40;
    data[2] = 0x00;
  } else if (temperature <= -10) {
    data[0] = 0x04;
  }

  number = humidity * 10;
  for (int i = 0; i < 3; i++) {
    int pos[] = {9, 7, 5};
    uint8_t d = number % 10;
    data[pos[i]] = digits[d][0];
    data[pos[i] + 1] = digits[d][1];
    number /= 10;
  }

  if (humidity < 10) {
    data[5] = data[6] = 0;
  }

  data[4] |= 0x20;  // temperature decimal point
  data[8] |= 0x20;  // humidity decimal point

  if (this->displayPercent) {
    data[10] |= 0x20;  // percent sign
  } else {
    data[10] &= 0xdf;
  }

  if (temperatureUnit == celsius) {
    data[13] = 0x05;  // celsius
  } else if (temperatureUnit == fahrenheit) {
    data[13] = 0x06;  // fahrenheit
  } else {
    data[13] = 0;
  }

  if (this->displayBluetooth) {
    data[13] |= 0x08;
  }

  if (this->displayBattery) {
    data[13] |= 0x10;
  }
}

void garretlab_ePaper19::display() {
  uint8_t data[14] = {0};

  makeData(data);
  ist7134->display(0x00, data, sizeof(data));
}

void garretlab_ePaper19::display(float temperature, float humidity, temperatureUnit_t temperatureUnit) {
  setTemperature(temperature);
  setHumidity(humidity);
  setTemperatureUnit(temperatureUnit);
  display();
}

int8_t garretlab_ePaper19::temperatureSensorData() {
  return ist7134->temperatureSensorData();
}

constexpr uint8_t garretlab_ePaper19::digits[10][2];
