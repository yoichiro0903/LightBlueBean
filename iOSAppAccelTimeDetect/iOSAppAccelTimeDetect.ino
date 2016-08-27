/*

typedef struct {
    uint8_t length;
    uint8_t data[20];
} ScratchData;

*/

const uint8_t ScratchBank = 1;
unsigned long top_time = 0;
unsigned long bottom_time = 0;

void setup()
{
  Serial.begin(9600);
}
 
// the loop routine runs over and over again forever:
void loop() 
{
  AccelerationReading accel = Bean.getAcceleration();
  int x = accel.xAxis;
  Serial.print("row x: ");
  Serial.println(accel.xAxis);

  if (x > 0) {
    top_time = millis();
  }
  if (x < 0) {
    bottom_time = millis();
  }
  Serial.print("top_time: ");
  Serial.println(top_time);
  Serial.print("bottom_time: ");
  Serial.println(bottom_time);

  int time_diff = top_time - bottom_time;
  time_diff = abs(time_diff);
  Serial.print("time_diff(abs): ");
  Serial.println(time_diff);
  
  int diff = 512 - x;
  int diff_char = diff /2;
  Serial.print("diff_char: ");
  Serial.println(diff_char);

  ScratchData scratch1Data = Bean.readScratchData(1);
  Serial.print("scratch 1 data: ");
  Serial.println(scratch1Data.data[0]);

  Bean.setScratchNumber(1,diff_char);

  Bean.sleep(1000);
  Serial.println("");

}
