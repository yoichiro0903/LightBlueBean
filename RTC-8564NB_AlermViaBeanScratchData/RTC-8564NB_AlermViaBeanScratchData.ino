/*
 参考URL
 http://baticadila.dip.jp/arduino_104.html
 http://www.geocities.jp/zattouka/GarageHouse/micon/Arduino/RTC/RTC.htm
 http://akizukidenshi.com/catalog/g/gI-00233/
*/

// I2C通信を行うためのライブラリ。
#include <Wire.h>

#define I2C_ADDR (0xA2 >> 1)  // 1ビット目未使用のため1ビット右シフトする。

int alerm_min = 16; //0x10 = 10 min、デフォルト値として設定。
int scratch1DataDefaultInt = 0; // ここでScratchDataを取得しようとするとエラーになる。
int scratch1DataLoopInt = 0;    // なので、intをグローバル変数として持っておいて、演算に使用。
            
void setup() {
  Serial.begin(9600);
  Serial.println("RTC test start");

  Wire.begin();  // I2C の初期化。
  delay(1000);   // 発振子の動作待機。

  int default_year = 20; //0x14 変数で指定できるかの実験。

  Wire.beginTransmission(I2C_ADDR);
  Wire.write(0x00); // データを転送する先頭のレジスタ番号を指定。
  Wire.write(0x20); // 00 Control 1　STOP(bit5)-1 をセットし動作を停止させる。
  Wire.write(0x00); // 01 Control 2
  Wire.write(0x00); // 02 Seconds　初期値を転送（秒）0 ～ 59
  Wire.write(0x00); // 03 Minutes　　　 〃 　　（分）0 ～ 59
  Wire.write(0x00); // 04 Hours　　　　 〃 　　（時）0 ～ 23
  Wire.write(0x01); // 05 Days 　　　　 〃 　　（日）1 ～ 31
  Wire.write(0x02); // 06 Weekdays 　　 〃 　　（曜日）0 ～ 6
  Wire.write(0x84); // 07 Months 　　　 〃 　　（月）1 ～ 12  bit7が0だと1900年代。 
  Wire.write(default_year); // 08 Years　　　　 〃 　　（年）0 ～ 99
  Wire.write(0x00); // 09 Minutes Alarm　アラームの初期値を転送（分）0 ～ 59
  Wire.write(0x00); // 0A Hours Alarm　　　　　　 〃 　　　　　（時）0 ～ 23
  Wire.write(0x00); // 0B Days Alarm 　　　　　　 〃 　　　　　（日）1 ～ 31
  Wire.write(0x00); // 0C Weekdays Alarm　　　　　〃 　　　　　（曜日）0 ～ 6
  Wire.write(0x00); // 0D CLKOUT 　　　　タイマー用レジスタ
  Wire.write(0x00); // 0E Timer control　　　　　〃
  Wire.write(0x00); // 0F Timer　　　　　　　　　〃
  Wire.write(0x00); // 00 Control 1　STOP(bit5)-0 をリセットし動作を開始する。
                    //    アドレス 0F の次は先頭アドレスの 00 に戻る。
  Wire.endTransmission();

  // タイマーを停止（初期化）
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(0x0E);
  Wire.write(0x00);
  Wire.endTransmission();

  // アラーム割り込みとタイマ割り込み（Control 2）の設定。
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(0x01);
  Wire.write(0x13); // 02 bit4 TI/TP 動作モードを繰り返しに設定。
                    // よくわからなかったので、bit0,bit1は両方1に。
                    // bit0 TIE   /INT端子へ出力
                    // bit1 AIE   /INT端子へ出力    
  Wire.endTransmission();

  // BeanのScratch1データを取得して確認。
  ScratchData scratch1RowDataDefault = Bean.readScratchData(1);
  byte scratch1DataDefault = scratch1RowDataDefault.data[0];

  // 確認用出力。0のはず。
  Serial.println("scratch1DataDefault:");
  Serial.println(scratch1DataDefault);

  // 確認用出力。0のはず。
  scratch1DataDefaultInt = (int)scratch1DataDefault;
  Serial.println("scratch1DataDefaultInt:");
  Serial.println(scratch1DataDefaultInt);
}

// アラーム設定用関数。ここに無いと読み込み順の問題で実行されなそう。
void setAlerm (int alerm_min){
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(0x09);
  Wire.write(alerm_min); // 09 Minutes Alarm
  Wire.write(0x00); // 0A Hours Alarm　
  Wire.write(0x01); // 0B Days Alarm 　
  Wire.write(0x02); // 0C Weekdays Alarm
  Wire.endTransmission();
  Serial.println("alerm set.");
  // Scratch1をdefaultの０に戻す処理をここにいれて管理しないと、
  // アラームが常に新規設定されるようになってしまう。（現状）

}

void loop() {
  // RTCから情報を取得。
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(0x02);
  Wire.endTransmission();
  delay(1);
  Wire.requestFrom(I2C_ADDR, 7);
  int sec = Wire.read();
  int min = Wire.read();
  int hour = Wire.read();
  int day = Wire.read();
  int week = Wire.read();
  int month = Wire.read();
  int year = Wire.read();

  // 14/4/1(2)0:0:1みたいな感じで出てくる。
  Serial.print(year, HEX);
  Serial.print("/");
  Serial.print(month & 0x1f, HEX);
  Serial.print("/");
  Serial.print(day &0x3f, HEX);
  Serial.print("(");
  Serial.print(week & 0x07, HEX);
  Serial.print(")");
  Serial.print(hour & 0x3f, HEX);
  Serial.print(":");
  Serial.print(min & 0x7f, HEX);
  Serial.print(":");
  Serial.println(sec & 0x7f, HEX);

  delay(1000);

  // BeanのScratch1のデータを取得し、書き込みによって変更されているかどうかを判定。
  // 変更されていたら、それをトリガーにしてアラームを設定する。
  ScratchData scratch1RowDataLoop = Bean.readScratchData(1);
  byte scratch1DataLoop = scratch1RowDataLoop.data[0];
  scratch1DataLoopInt = (int)scratch1DataLoop;

  // 確認用出力。
  Serial.println("scratch1DataLoopInt:");
  Serial.println(scratch1DataLoopInt);

  // これいま、都度走ってるから、一度Alermセットしたら、scratch1のcharacteristicを戻す処理を、
  // setAlermにいれたい。
  if (scratch1DataLoopInt != scratch1DataDefaultInt){
    if (scratch1DataLoop == B10){ // 0x2がデータとして書き込まれたら。
      alerm_min = 2; //0x02 = 2 min
      setAlerm(alerm_min); // アラーム設定を2minに変更。
      // 確認用出力。
      Serial.println("scratch1DataLoop == B1. scratch1DataLoop:");
      Serial.println(scratch1DataLoop);
    } else {
      Serial.println("scratch1DataLoop is unknown but changed. scratch1DataLoop:");
      Serial.println(scratch1DataLoop);      
    }
  } else {
    Serial.println("scratch1DataLoop is 0 (default). scratch1DataLoop:");
    Serial.println(scratch1DataLoop);      
  }
}




