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
  if (Serial.available() > 0) {
    char c = Serial.read();
    Serial.println("I received: ");
    Serial.println(c);
    
    if(c == 'r'){
      uint8_t beanBuffer[] = {0};
      Bean.setScratchData(ScratchBank,beanBuffer,1);
      ScratchData ledColor = Bean.readScratchData(1);
      Serial.println(ledColor.data[0]);
      Bean.setLed(50,0,0);
      Bean.sleep(1000);
      Bean.setLed(0,0,0);    
    }
    else if(c == 'g'){
      uint8_t beanBuffer[] = {1};
      Bean.setScratchData(ScratchBank,beanBuffer,1);
      ScratchData ledColor = Bean.readScratchData(1);
      Serial.println(ledColor.data[0]);
      Bean.setLed(0,50,0);
      Bean.sleep(1000);
      Bean.setLed(0,0,0);    
    }  
  }
}
