/*

typedef struct {
    uint8_t length;
    uint8_t data[20];
} ScratchData;

*/

const uint8_t ScratchBank = 1;
int led_status = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(1, OUTPUT); //for LED test
}
 
// the loop routine runs over and over again forever:
void loop() 
{
  ScratchData scratch1Data = Bean.readScratchData(1);
  Serial.println("scratch 1 data: ");
  Serial.println(scratch1Data.data[0]);
  if(scratch1Data.data[0] == B0){
    led_status = 0;
    digitalWrite(1,LOW); //for LED test
  } else if (scratch1Data.data[0] == B1){
    led_status = 1;
    digitalWrite(1,HIGH); //for LED test
  } else if (scratch1Data.data[0] == B10){
    led_status = 2;
    digitalWrite(1,LOW); //for LED test
  } else if (scratch1Data.data[0] == B11){
    led_status = 3;
    digitalWrite(1,LOW); //for LED test
  }
  if (led_status == 0){
    Bean.setLed(0,0,0);
  } else if (led_status == 1) {
    Bean.setLed(50,0,0);
    Bean.sleep(1000);
    Bean.setLed(0,0,0);
  } else if (led_status == 2) {
    Bean.setLed(0,50,0);
    Bean.sleep(1000);
    Bean.setLed(0,0,0);
  } else if (led_status == 3) {
    Bean.setLed(0,0,50);
    Bean.sleep(1000);
    Bean.setLed(0,0,0);
  }
}
