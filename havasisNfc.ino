#include <Keyboard.h>
#include <KeyboardLayout.h>
#include <Keyboard_da_DK.h>
#include <Keyboard_de_DE.h>
#include <Keyboard_es_ES.h>
#include <Keyboard_fr_FR.h>
#include <Keyboard_hu_HU.h>
#include <Keyboard_it_IT.h>
#include <Keyboard_pt_PT.h>
#include <Keyboard_sv_SE.h>

#include <Keyboard.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
String tagId = "None";
String extractedData = ""; // Payload'dan alınacak veri için değişken
byte nuidPICC[4];

void setup() {
  Serial.begin(115200);
  Keyboard.begin();
  Serial.println("Sistem başlatıldı");
  nfc.begin();
  Keyboard.begin();
}

void loop() {
  readNFC();
}

void readNFC() {
  if (nfc.tagPresent()) {
    NfcTag tag = nfc.read();
    tag.print();  // Tag'ın bilgilerini yazdır
    
    // Get the UID string
    tagId = tag.getUidString();

    // Payload'ı al
    byte payload[255]; // Payload'ı saklamak için byte dizisi oluşturuluyor
    tag.getNdefMessage().getRecord(0).getPayload(payload); // Payload'ı alıyoruz

    // Payload'ı String'e dönüştür
    String payloadString = "";
    for (int i = 0; i < 255; i++) {
      if (payload[i] == 0) break;  // Null terminator
      payloadString += (char)payload[i]; // byte dizisini string'e dönüştür
    }

    // ".en" den sonrasını al
    int startIdx = payloadString.indexOf(".en") + 4; // ".en" sonrasına bakıyoruz
    if (startIdx != -1) {
      extractedData = payloadString.substring(startIdx); // ".en" sonrasındaki veriyi al
      Serial.println("Extracted Data: " + extractedData);  // Alınan veriyi yazdır
    }
    Keyboard.print(extractedData);

    delay(2500);
  }
}
