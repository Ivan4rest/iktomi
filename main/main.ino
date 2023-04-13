const int SENSOR_HOLL_PIN = 0;

int hallSignal = 0;

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_HOLL_PIN, INPUT);
}

void loop() {
  hallSignal = analogRead(SENSOR_HOLL_PIN);
  Serial.println(hallSignal);
  hallSignal = abs(map(hallSignal, 0, 1023, -511, 511));
  Serial.println(hallSignal);
}
