/**************************************
  デバイスはM5Stack Basic
  フット・スイッチに接続し、プルアップ
  カウンタは －9999 ～ +9999まで表示可
***************************************/
#include <M5Stack.h>

#define FS1 21        //フットスイッチ1
#define FS2 22        //フットスイッチ2

int32_t delayTime = 80; //チャタリング防止のための時間
int32_t fsCount = 0; //カウンタ数

bool fs1Cnt = false; //連続で読み取った回数
bool fs2Cnt = false;

volatile bool lcdUpdate = false; //LCD画面の更新フラグ、カウントアップ表示
/*********************************************
              セットアップ関数
*********************************************/
void setup() {
  M5.begin();
  M5.Power.begin();

  M5.Speaker.begin();       // 呼ぶとノイズ(ポップ音)が出る
  M5.Speaker.setVolume(1);  // 0は無音、1が最小、8が初期値(結構大きい)


  //LCD表示設定
  //  M5.Lcd.setRotation(1);     //画面の向きを指定(0～3)
  //  M5.Lcd.setTextFont(2);     //フォントを指定します。（1～8)1:Adafruit 8ピクセルASCIIフォント、2:16ピクセルASCIIフォント、4:26ピクセルASCIIフォント
  M5.Lcd.setTextColor(WHITE); //文字色設定(color,backgroundcolor)引数2番目省略時は(WHITE, BLACK, RED, GREEN, BLUE, YELLOW...)

  pinMode(FS1, GPIO_PULLUP_ONLY);
  pinMode(FS2, GPIO_PULLUP_ONLY);
  
  lcdUpdate = true;
}

void loop() {
  M5.update();

  if ( M5.BtnA.wasPressed()) //ボタンAを押したとき
  {
    esp_restart(); //リセット
  }

  //フットスイッチ1でカウンタ加算
  if (digitalRead(FS1) == HIGH) //プルアップのためスイッチがONのときの処理
  {
    fs1Cnt = true;
    Serial.println(F("FS1test1"));
  }
  else
  {
    if (fs1Cnt == true) //スイッチを押して放したときにカウントアップ
    {
      fsCount ++; //カウンタを1つ増やす
      Serial.println(F("FS1"));
      fs1Cnt = false;
      lcdUpdate = true;
    }
    else
    {
      Serial.println(F("FS1test1"));
      fs1Cnt = false;
    }
  }


  //フットスイッチ２でカウンタ減算
  
  if (digitalRead(FS2) == LOW )
  {
    fs2Cnt = true;
  }
  else
  {
    if (fs2Cnt == true)
    {
      fsCount --; //カウンタを1つ減らす
      Serial.println(("FS2"));
      fs2Cnt = false;
      lcdUpdate = true;
    }
    else
    {
      fs2Cnt = false;
    }
  }

  //画面の更新
  if (lcdUpdate == true)
  {
    Serial.println(fsCount);

    //LCD表示
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextFont(4);      //フォントを指定します。（1～8)1:Adafruit 8ピクセルASCIIフォント、2:16ピクセルASCIIフォント、4:26ピクセルASCIIフォント
    M5.Lcd.setTextSize(2);      //文字の大きさを設定（1～7）
    M5.Lcd.println("FS Counter");
    if (fsCount > -1000 || fsCount < 1000) //1000以下のカウントは画面に収まる程度に文字を大きくする。
    {
      M5.Lcd.setTextSize(6);
    }
    else
    {
      M5.Lcd.setTextSize(4);
    }
    M5.Lcd.println(fsCount);

    //ビープ音を鳴らす
    if ( fs1Cnt == false)
    {
      M5.Speaker.tone(659, 200);
      delay(100);               // 100ms鳴らす(beep()のデフォルト)
      M5.Speaker.mute();        //　ビープ停止
    }
    else if ( fs2Cnt == false)
    {
      M5.Speaker.tone(440, 100); // ビープ開始
      delay(50);                 // 100ms鳴らす(beep()のデフォルト)
      M5.Speaker.mute();         //　ビープ停止
      delay(50);                 // 100ms鳴らす(beep()のデフォルト)
      M5.Speaker.tone(440, 100); // ビープ開始
      delay(50);                 // 100ms鳴らす(beep()のデフォルト)
      M5.Speaker.mute();         //　ビープ停止
    }

    lcdUpdate = false;
  }

  delay(delayTime);
}
