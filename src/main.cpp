#include "masterLib.h"

#define GATEWAY_EMULATE_MODE 1
#define BADGE_EMULATE_MODE 2
#define LOCATION_BEACON_EMULATE_MODE 3
#define BADGE_RECEIVE_MODE 4

int screenModeNum = 0;
int counter = 0;
long lastTime = 0;
int actualSent = 0;
long timeCount = 0;

int startTime;
int endTime;

int packetsSent = 0;
int nextBW = 0;
int delayBetweenPackets = 500;


int switchStuff = 0;



void onReceive(int packetLength);

const char* messages[16] = 
{"01AABBCCDDEEFF00", "03ABBBCCDDEEFF00", "01ACBBCCDDEEFF00","01ACCBCCDDEEFF00",
 "01ACBDCCDDEEFF00","01ACBBCCDDEEEF00","01ACBDDCDDEEFF00","01BCBDDCDDEEFF00",
  "01ACBDDCDDEHFF00","01ECBDDCDDEEFF00","01ECBODCDDEEFF00","01OCBDDCDDEEFF00",
  "01EOBDDCDDEEFF00", "01ECBDDCDDOEFF00", "01PCBDDCDDEEFF00","01ECBDDCPDEEFF00"};


void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.enableCrc();
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(20.8E3);
  
  lastTime = millis();
  screenModeNum = GATEWAY_EMULATE_MODE;

  if(screenModeNum == GATEWAY_EMULATE_MODE){
    delay(5000);
  }
 
  pinMode(38,INPUT);
}


void loop(){
  
  if(digitalRead(38) == 0 && (millis() > (timeCount + 1000))){
    if(screenModeNum == GATEWAY_EMULATE_MODE){
      screenModeNum = BADGE_EMULATE_MODE;
    }else{
      screenModeNum = GATEWAY_EMULATE_MODE;
    }
    timeCount = millis();
  }


  if(screenModeNum == GATEWAY_EMULATE_MODE){
    if((millis() > delayBetweenPackets + lastTime)&& (packetsSent < 50)){
      startTime = millis();
      LoRa.beginPacket();
      LoRa.print(messages[counter]);
      LoRa.endPacket();
      packetsSent++;
      endTime = millis();
      actualSent++;
      //Serial.print("Sent Packets: ");
      //Serial.print(actualSent);
    
      if(counter == 9){
        counter = 0;
      }else{
        counter++;
      }
    
      Serial.println(startTime);
      Serial.println(endTime);
      lastTime = millis();

      
    }

    if(packetsSent == 50){
      switch (switchStuff){
        case 0:
          LoRa.setSignalBandwidth(41.7E3);
          packetsSent = 0;
          switchStuff++;
          delay(45000);
          break;
        case 1:
          LoRa.setSignalBandwidth(125E3);
          packetsSent = 0;
          switchStuff++;
          delay(45000);
          break;
        case 2:
          LoRa.setSignalBandwidth(20.8E3);
          LoRa.setSpreadingFactor(9);
          packetsSent = 0;
          switchStuff++;
          delayBetweenPackets = 1000;
          delay(45000);
          break;
        case 3:
          LoRa.setSignalBandwidth(41.7E3);
          packetsSent = 0;
          switchStuff++;
          delayBetweenPackets = 750;
          delay(45000);
          break;
        case 4:
          LoRa.setSignalBandwidth(125E3);
          packetsSent = 0;
          switchStuff++;
          delayBetweenPackets = 600;
          delay(45000);
          break;
        case 5:
          LoRa.setSignalBandwidth(41.7E3);
          LoRa.setSpreadingFactor(11);
          packetsSent = 0;
          switchStuff++;
          delayBetweenPackets = 1600;
          delay(45000);
          break;
        case 6:
          LoRa.setSignalBandwidth(125E3);
          packetsSent = 0;
          switchStuff++;
          delayBetweenPackets = 1000;
          delay(45000);
          break;
        default:
          packetsSent = 50;
          delay(45000);
        


      }
      
    }




  //onReceive(LoRa.parsePacket());  //Comment this out if you dont want this device to receive and print acknowledgment
  }

  if(screenModeNum == BADGE_EMULATE_MODE){
    //LoRa.receive();
    int packetLength = LoRa.parsePacket();
    if(packetLength != 0){
      //Serial.println("Received");
      LoRa.beginPacket();
      LoRa.print("00:11:22:33:44:55");
      LoRa.endPacket();
      delay(250);
      LoRa.beginPacket();
      LoRa.print("01:11:22:33:44:55");
      LoRa.endPacket();
      delay(250);
      LoRa.beginPacket();
      LoRa.print("02:11:22:33:44:55");
      LoRa.endPacket();
    }
  }


//end loop()

}

void onReceive(int packetLength){
  if(packetLength == 0){
    return;
  }else{
      Serial.print(" Acknowledge: ");
    while(LoRa.available()){
      Serial.print((char)LoRa.read());
    }
  }
  Serial.println("");
}

