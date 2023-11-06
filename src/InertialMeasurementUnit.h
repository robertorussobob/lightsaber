#ifndef INERTIALMEASUREMENTUNIT_H
#define INERTIALMEASUREMENTUNIT_H


class InertialMeasurementUnit {
};

#ifdef ARDUINO
#include <Adafruit_BNO055.h>

class MicroInertialMeasurementUnit: public InertialMeasurementUnit, public Adafruit_Sensor {
public:
  MicroInertialMeasurementUnit(): InertialMeasurementUnit(), Adafruit_Sensor() {
    bool bnoIsBad = !bno.begin();
    if (bnoIsBad) { Serial.print("No BNO055 detected, check the wiring"); while(1); }
      displaySensorDetails();
      displaySensorStatus();
      bno.setExtCrystalUse(true);
  }
  ~MicroInertialMeasurementUnit() {}
 void enableAutoRange(bool enabled) { iu.enableAutoRange(enabled); };
 bool getEvent(sensors_event_t* sensors_event) { return iu.getEvent(sensors_event); }
 void getSensor(sensor_t* sensor) { iu.getSensor(sensor); }
 void tick() {
      /*
      sensors_event_t event; 
      iu.getEvent(&event);
      Serial.print("X: ");
      Serial.print(event.orientation.x, 4);
      Serial.print("\tY: ");
      Serial.print(event.orientation.y, 4);
      Serial.print("\tZ: ");
      Serial.print(event.orientation.z, 4);
      Serial.print("\taX: ");
      Serial.print(event.acceleration.x, 4);
      Serial.print("\taY: ");
      Serial.print(event.acceleration.y, 4);
      Serial.print("\taZ: ");
      Serial.print(event.acceleration.z, 4);
      Serial.println("");
      displayCalibrationStatus();
      delay(100);
      */
    }
protected:
    void displaySensorDetails() {
        sensor_t sensor;
        bno.getSensor(&sensor);
        Serial.println("------------------------------------");
        Serial.print  ("Sensor:       "); Serial.println(sensor.name);
        Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
        Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
        Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
        Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
        Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
        Serial.println("------------------------------------");
        Serial.println("");
    }
    void displaySensorStatus() {
        uint8_t system_status, self_test_results, system_error;
        system_status = self_test_results = system_error = 0;
        bno.getSystemStatus(&system_status, &self_test_results, &system_error);
        Serial.println("");
        Serial.print("System Status: 0x");
        Serial.println(system_status, HEX);
        Serial.print("Self Test:     0x");
        Serial.println(self_test_results, HEX);
        Serial.print("System Error:  0x");
        Serial.println(system_error, HEX);
        Serial.println("");
    }
    void displayCalibrationStatus() {
        /* Get the four calibration values (0..3) */
        /* Any sensor data reporting 0 should be ignored, */
        /* 3 means 'fully calibrated" */
        uint8_t system, gyro, accel, mag;
        system = gyro = accel = mag = 0;
        bno.getCalibration(&system, &gyro, &accel, &mag);
        /* The data should be ignored until the system calibration is > 0 */
        Serial.print("\t");
        if (!system) Serial.print("! ");
        /* Display the individual values */
        Serial.print("Sys:");
        Serial.print(system, DEC);
        Serial.print(" G:");
        Serial.print(gyro, DEC);
        Serial.print(" A:");
        Serial.print(accel, DEC);
        Serial.print(" M:");
        Serial.print(mag, DEC);
        Serial.println("");
    }
protected:
    Adafruit_BNO055 bno = Adafruit_BNO055(55);
    /* IMU Inertial measurement unit */
    Adafruit_Sensor& iu = bno;
};
#endif



#endif