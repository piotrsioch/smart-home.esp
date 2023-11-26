#include <DHT.h>

#define timeSeconds 2
#define LED_PIN 16
#define MOTION_SENSOR_PIN 17
#define SMOKE_SENSOR_PIN 32
#define REED_SWITCH 22

DHT dht(26, DHT11);

const long INTERUPT_INTERVAL = 2000;

unsigned long now = millis();
unsigned long previousMillis = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;
boolean motion = false;

bool previousState = HIGH;


void IRAM_ATTR detectMovement() {
  digitalWrite(LED_PIN, HIGH);
  startTimer = true;
  lastTrigger = millis();
}

void setup() {
  Serial.begin(115200);
  // DHT SETUP
  dht.begin();
  delay(2000);

  // MOTION SENSOR SETUP
  pinMode(MOTION_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MOTION_SENSOR_PIN), detectMovement, RISING);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // REED SWITCH SETUP
  pinMode(REED_SWITCH, INPUT_PULLUP);
}

void loop() {
  now = millis();

  if((digitalRead(LED_PIN) == HIGH) && (motion == false)) {
    motion = true;
  }
  
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
    digitalWrite(LED_PIN, LOW);
    startTimer = false;
    motion = false;
  }

  if(now - previousMillis >= INTERUPT_INTERVAL) {
    previousMillis = now;

    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    int digitalNumber = analogRead(SMOKE_SENSOR_PIN); //from 0 to 4095

    int actualState = digitalRead(REED_SWITCH); // Odczytaj stan zestyku

    if (actualState != previousState) { // Sprawdź, czy stan się zmienił
      if (actualState == LOW) {
        Serial.println("Zamknięto");
      } else {
        Serial.println("Otwarto");
      }
      previousState = actualState; // Zapisz bieżący stan jako poprzedni
    }

    // Serial.print("Temp: ");
    // Serial.print(temp);
    // Serial.println(" C ");
    // Serial.print("Humidity: ");
    // Serial.print(humidity);
    // Serial.println(" % ");
    
    // Serial.print("From smoke sensor: ");
    // Serial.println(digitalNumber);
  }
}
