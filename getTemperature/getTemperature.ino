/* 
 This sketch reads the temperature from the Bean's on-board temperature sensor. 
 
 The temperature readings are sent over serial and can be accessed in Arduino's Serial Monitor.
 
 To use the Serial Monitor, set Arduino's serial port to "/tmp/tty.LightBlue-Bean"
 and the Bean as "Virtual Serial" in the OS X Bean Loader.
 
 This example code is in the public domain.
*/

static int8_t temp = 0;

// the setup routine runs once when you press reset:
void setup() 
{
  // initialize serial communication
  Serial.begin(); 
}

void loop()
{ 
 int8_t newTemp = Bean.getTemperature();
 
 if ( newTemp != temp )
 {
   temp = newTemp;
   Serial.print("Temperature is ");
   Serial.print(temp);
   Serial.println(" degrees Celsius");
 }
  
 // Sleep for a second before reading the temperature again  
 Bean.sleep(1000);  
}
