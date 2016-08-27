/*

typedef struct {
    uint8_t length;
    uint8_t data[20];
} ScratchData;

*/

const uint8_t ScratchBank = 1;

void setup() 
{
  Serial.begin(9600);
}
 
// the loop routine runs over and over again forever:
void loop() 
{
  AccelerationReading accel = Bean.getAcceleration();
  char x = abs(accel.xAxis);
  Serial.println(x);

  ScratchData scratch1Data = Bean.readScratchData(1);
  Serial.println("scratch 1 data: ");
  Serial.println(scratch1Data.data[0]);

  Bean.setScratchNumber(1,x);

  Bean.sleep(1000);
}
