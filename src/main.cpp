#include <Arduino.h>
#include <SoftwareSerial.h>
#include "HallSensorMatrix.cpp"

//SoftwareSerial connection(0, 1);

HallSensorMatrix matrix = HallSensorMatrix();

bool isCalibrate8mmValue {false};
bool isCalibrateMinValue {false};
bool isCalibrateMaxValue {false};

String output = "";

void setup() {
//  connection.begin(9600);
  Serial.begin(115200);

  pinMode(0, INPUT);
  pinMode(1, OUTPUT);
  
  for(int i = 9; i <= 53; ++i)
  {
    pinMode(i, OUTPUT);
  }

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  pinMode(A8, INPUT);

  calibrate();
}

void loop() {
  delay(20);
  
  if(isCalibrate8mmValue && isCalibrateMinValue && isCalibrateMaxValue)
  {
    matrix.findWorkingSensors();
    matrix.getVector();

    int x1 = static_cast<int>(matrix.hallSensors[matrix.indexesOfWorkingSensors[0][0]][matrix.indexesOfWorkingSensors[0][1]].getXCoordinate());
    int y1 = static_cast<int>(matrix.hallSensors[matrix.indexesOfWorkingSensors[0][0]][matrix.indexesOfWorkingSensors[0][1]].getYCoordinate());
    int a = static_cast<int>(matrix.hallSensors[matrix.indexesOfWorkingSensors[0][0]][matrix.indexesOfWorkingSensors[0][1]].getValueInMM());
    
    output = "";

    output.concat(static_cast<int>(matrix.x));
    output.concat(";");
    output.concat(static_cast<int>(matrix.y));
    output.concat(";");
    output.concat(static_cast<int>(matrix.z));
  
    Serial.println(output);
  }
}


void calibrate()
{
  isCalibrateMinValue = matrix.calibrateMinValue();
  delay(3000);
  isCalibrateMaxValue = matrix.calibrateMaxValue();
  delay(5000);
  isCalibrate8mmValue = matrix.calibrate8mmValue();
}