
//Candle Light Off

#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const int fanpin = 7;

const float temp_threshold = 35.0;   // temperature at which fan turns on
const float cooloff = 2.0;          // fan turns off once temperature drops this far below threshold

bool fan = false;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(fanpin, OUTPUT);
  digitalWrite(fanpin, LOW);    // fan off at startup
}


void loop() {
  float temperature = dht.readTemperature();  // Celsius

  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT22 sensor.");
    delay(3000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  if (!fan && temperature >= temp_threshold)
  {
    fan = true;
    digitalWrite(fanpin, HIGH);
    Serial.println("Temperature threshold reached. Fan ON (extinguishing candle).");
  } 
  else if (fan && temperature <= (temp_threshold - cooloff)) 
  {
    fanIsOn = false;
    digitalWrite(fanpin, LOW);
    Serial.println("Cooled down. Fan OFF.");
  }

  delay(5000); 
}
