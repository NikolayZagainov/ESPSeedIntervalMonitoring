# Seed monitoring sensor

## Usage

The sensor is used to detect discharged seed time intervals based on lser receiver reading.

## Description

The sensor is impelemented on `ESP WROOM 32` chip. The project is written in `Arduino` framewwork using Bluetooth LE libraries.

The device is recognized by device name:
```kt
#define DEVICE_NAME "SeedMonitoring"
```

Service service is is implemented by
```kt
#define serviceID BLEUUID((uint16_t)0x9951)
```
Service characteristic is is implemented by
```kt
BLEUUID((uint16_t)0x1B01)

seedintervalCharacteristic->addDescriptor(new BLE2902());\
```

## Notes
The actual sensor is implemented using custom PCB board and additional hardare closely described in upcommimg article `Rapid plane laser scanning seed monitoring sensor for IoT applications`

