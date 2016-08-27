/*
  readScratchData example
 
 Checks scratch bank 1 for changes every second.  The LED will blink green
 if any changes are made to bank 1.
 
 <http://www.punchthrough.com/bean> 
 
 */
 
 
void setup() 
{
  Serial.begin(57600);
}
 
bool compareScratch( ScratchData * scratch1, ScratchData * scratch2 )
{
  bool matched = true;
  
  if ( scratch1->length != scratch2->length )
  {
    matched = false;
  }
  else
  {
    int length = min( scratch1->length, scratch2->length );
    int i = 0;
    
    while ( i < length )
    {
      if ( scratch1->data[i] != scratch2->data[i] )
      {
        matched = false;
        i = length;
      }
      i++;
    }
  }
  
    return matched;
}
 
ScratchData lastScratch;
 
// the loop routine runs over and over again forever:
void loop() 
{
  ScratchData thisScratch = Bean.readScratchData(1);
  if(thisScratch.length > 0){
    Bean.setLed(255,125,0);
    Bean.sleep(1000);
    Bean.setLed(0,0,0);    
  } else {
    Bean.setLed(0,0,255);
    Bean.sleep(1000);
    Bean.setLed(0,0,0);
  }
  
  bool matched = compareScratch( &thisScratch, &lastScratch );
   
  if ( false == matched )
  {
    lastScratch = thisScratch;
    
    Bean.setLed(0,255,0);
    
    Bean.sleep(1000);
    
    Bean.setLed(0,0,0);
  } 
  
  Bean.sleep(1000);
}  
