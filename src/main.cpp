//BME280
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//screen
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//MQ2
#include <MQ2.h>
// #include <MQ-2-sensor-library-master.h>

//BME280
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

//screen
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16


//MQ2 pin
const int mq2Pin = A0; // Analog pin connected to the MQ-2 sensor
const int alcoholThreshold = 650;  // Alcohol threshold

//buzzer
const int buzzerPin = 28; // Analog pin connected to the buzzer

Adafruit_BME280 bme; // I2C

//unsigned long delayTime;
void setup() {
    Serial.begin(9600);
}
void setup2() {
    pinMode(buzzerPin, OUTPUT); // set pin A1 as output
    Serial.begin(9600);
    while(!Serial);    // time to get serial running
    Serial.println(F("BME280 test"));

    unsigned status;
    
    // default settings
    status = bme.begin(0X76);  

    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        while (1) delay(10);
    }
    
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    delay(1000); // Pause for 2 seconds

    // Clear the buffer
    display.clearDisplay();
    
}
void loop(){
    Serial.print("Hello Margaret");
    delay(1000);
}

void loop2() { 
    float initialHumidity = bme.readHumidity();
    delay(1000); // Wait for a short time to get a baseline humidity reading

    while (true) {
      float currentHumidity = bme.readHumidity();
      float humidityChange = currentHumidity - initialHumidity; // Humidity change
      Serial.print("initial humidity: ");
      Serial.print(initialHumidity);
      Serial.print(", current humidity: ");
      Serial.print(currentHumidity);
      Serial.print(", humidity change: ");
      Serial.println(humidityChange);
      // int alcoholValue = analogRead(mq2Pin);
      // Serial.println("Alcohol Value: ");
      // Serial.println(alcoholValue);

      if (humidityChange > 10.0) {

        //Display humidity change on the OLED screen
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0,0);
        display.print("Humidity Change: ");
        display.println(humidityChange);
        display.display();

        
        // Display alcohol value on Serial Monitor
        int alcoholValue = analogRead(mq2Pin);
        Serial.println("Alcohol Value: ");
        Serial.println(alcoholValue);

        if (alcoholValue > alcoholThreshold) {
          // Detect alcohol vapor
          display.println("");
          display.println("");
          display.println("!!! Alcohol vapor detected!!!");

          // Generate a tone using the buzzer
          tone(buzzerPin, 1000); 
          delay(1000); 
          // Turn off the buzzer
          noTone(buzzerPin);

        } else {
          display.println("");
          display.println("");
          display.println("No alcohol vapor detected");
        }
      } else if (humidityChange < 6.0) {
        // Detect a big decrease in humidity
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0,0);
        display.println("Please aim at the nozzle and blow air");
      } 

      display.display();
      delay(1000); // Wait for a short time before checking again

    }

   
}

