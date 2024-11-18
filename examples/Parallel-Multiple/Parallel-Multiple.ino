#include <DS18B20.h>

/* Typical readout of multiple (e.g. over 3) sensors
 * is around N x 0.8 seconds. This is largely spend
 * in waiting for the AD/conversion cycle in the sensor.
 * 
 * It is possible to start the measurement and AD/conversion
 * cycle; but not read it out right away. Then wait for these
 * to complete - and do a fast reading past. The time then
 * becomes 0.7 + N * 0.04 or there about.
 *
 */
#define ONE_WIRE_PIN (5)

DS18B20 ds(ONE_WIRE_PIN);

void setup() {
  Serial.begin(9600);
  Serial.print("\n\n\n");
  Serial.println(__FILE__);
  Serial.println(__DATE__);
  Serial.println(__TIME__);
}

void loop() {
  unsigned long st, dt;

  Serial.print("\n\n\nDevices: ");
  Serial.println(ds.getNumberOfDevices());

  // Read each sensor out one by one; i.e.
  // start the measurement and A/D conversion,
  // wait for it to complete and then read 
  // it out & convert.
  //
  st = millis();
  while (ds.selectNext()) {
    Serial.print("Temperature: ");
    Serial.print(ds.getTempC()); 
    Serial.println("[˚C]");
  };
  dt = millis() - st;
  Serial.print("Time taken for a sequential run: ");
  Serial.print(dt/1000.);
  Serial.println("[sec] (including serial out)");


  // Start the measuring/conversion on all devices; but do
  // not wait to read them out yet. 
  //
  st = millis();
  ds.startConversion(); // starts the conversion on all devices

  // In theory we can now do other stuff fr about 90.8 second

  // And then read them one by one. getTempC() will
  // check, and wait if needed, for the conversion to 
  // complete - so the very first one is likely to be
  // slower than the remaining ones; but the ones 
  // straight after will be near instantaneous.
  //
  while (ds.selectNext()) {
    Serial.print("Temperature: ");
    Serial.print(ds.getTempC(false /* i.e. do not start a conversion and wait */)); 
    Serial.println("[˚C]");
  };
  dt = millis() - st;

  Serial.print("Time taken for a parallel run: ");
  Serial.print(dt/1000.);
  Serial.println("[sec] (including serial out)");
  
  delay(1000);
};
