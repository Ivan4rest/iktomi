#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial connection(0, 1);

int hallMatrixWidth = 3;
int hallMatrixLength = 3;

int hallSignals[3][3];

void setup() {
//  connection.begin(9600);
  Serial.begin(9600);
}

void loop() {
  int analogPin = 0;
  for(int i = 0; i < hallMatrixWidth; i++){
    for(int j = 0; j < hallMatrixLength; j++){
      hallSignals[i][j] = transformHallSignal(analogRead(analogPin));
      analogPin++;
    }
  }

//  connection.println(getSendingString());
  Serial.println(getSendingString());
  delay(1000);
}

int transformHallSignal(int hallSignal){
  return abs(map(hallSignal, 0, 1023, -511, 511));
}

String getSendingString(){
  String output;
  for(int i = 0; i < hallMatrixWidth; i++){
    for(int j = 0; j < hallMatrixLength; j++){
      if(i == 0 && j == 0){
        output.concat(hallSignals[i][j]);
      }
      else{
        output.concat(" ");
        output.concat(hallSignals[i][j]);
      }
    }
  }
  output.concat(";");
  return output;
}

void printMatrix(){
  String output;
  output.concat("---------------------\n");
  for(int i = 0; i < hallMatrixWidth; i++){
    for(int j = 0; j < hallMatrixLength; j++){
      if(j == 0){
        output.concat(hallSignals[i][j]);
      }
      else if(j == (hallMatrixLength - 1)){
        output.concat(" ");
        output.concat(hallSignals[i][j]);
        output.concat("\n");
      }
      else{
        output.concat(" ");
        output.concat(hallSignals[i][j]);
      }
    }
  }
  output.concat("---------------------\n");
  Serial.println(output);
  delay(1000);
}