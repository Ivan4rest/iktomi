int hallMatrixWidth = 3;
int hallMatrixLength = 3;

int hallSignals[3][3];

void setup() {
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
  printMatrix();
}

int transformHallSignal(int hallSignal){
  return abs(map(hallSignal, 0, 1023, -511, 511));
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
