#include <DS18B20.h>

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

  st = millis();
  while (ds.selectNext()) {
    Serial.print("Temperature: ");
    Serial.print(ds.getTempC()); 
    Serial.println("[˚C]");
  };
  dt = millis() - st;
  Serial.print("Time taken for a sequential run: ");
  Serial.print(dt/1000.);
  Serial.println("[sec]");


  // Start the measuring/conversion on all devices; but do
  // not wait to readm them out.
  st = millis();
  while (ds.selectNext()) 
    ds.startConversion();

  // And then read them one by one. getTempC() will
  // check, and wait if needed, for the conversion to 
  // complete - so the very first one is likely to be
  // slower than the remaining ones.
  while (ds.selectNext()) {
    Serial.print("Temperature: ");
    Serial.print(ds.getTempC(false /* i.e. do not start a conversion and wait */)); 
    Serial.println("[˚C]");
  };
  dt = millis() - st;

  Serial.print("Time taken for a parallel run: ");
  Serial.print(dt/1000.);
  Serial.println("[sec]");
  
  delay(1000);
};
