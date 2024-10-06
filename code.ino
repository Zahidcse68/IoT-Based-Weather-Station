#include <Wire.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>  // Include library for real-time clock (RTC)

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT Sensor settings
#define DHTPIN 4 // Pin where the DHT sensor is connected
#define DHTTYPE DHT22 // DHT11 or DHT22
DHT dht(DHTPIN, DHTTYPE);

// RTC settings
RTC_DS3231 rtc; // Use your specific RTC module

void setup() {
    Serial.begin(115200);
    dht.begin();
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    if (!rtc.begin()) {
        display.println("RTC Error");
        display.display();
        while (1); // Loop forever
    }

    // Show initialization message
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Smart Weather");
    display.setTextSize(1);
    display.setCursor(0, 30);
    display.println("Initialized");
    display.display();
    delay(3000); // Show for 3 seconds
}

void drawDottedLine(int y) {
    for (int x = 0; x < SCREEN_WIDTH; x += 2) {
        display.drawPixel(x, y, SSD1306_WHITE);
    }
}

void drawWeatherAnimation() {
    // Simple animated cloud
    for (int i = 0; i < 5; i++) {
        display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK); // Clear the display
        display.setCursor(30 + i, 10); // Move cloud position
        display.print("TECH ZAID");
        display.display(); // Refresh display
        delay(100);
    }
}

void loop() {
    // Read humidity and temperature
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Check if readings are valid
    if (isnan(humidity) || isnan(temperature)) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Failed to read");
        display.display();
        return;
    }

    // Get current time
    DateTime now = rtc.now();

    // Clear display for temperature
    display.clearDisplay();

    // Display Temperature with animation
    drawWeatherAnimation(); // Show animated weather
    display.setTextSize(2);
    display.setCursor(0, 30);
    display.print("Temp: ");
    display.print(temperature);
    display.print(" C");
    drawDottedLine(55); // Draw dotted line below temperature
    display.display(); // Refresh display
    delay(2000); // Show temperature for 2 seconds

    // Clear display for humidity
    display.clearDisplay();

    // Display Humidity with animation
    drawWeatherAnimation(); // Show animated weather
    display.setTextSize(2);
    display.setCursor(0, 30);
    display.print("Humidity: ");
    display.print(humidity);
    display.print(" %");
    drawDottedLine(55); // Draw dotted line below humidity
    display.display(); // Refresh display
    delay(2000); // Show humidity for 2 seconds

    // Clear display for time
    display.clearDisplay();

    // Display time
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("🕒 Time:");
    display.setTextSize(2);
    display.setCursor(0, 15);
    display.print(now.hour());
    display.print(":");
    if (now.minute() < 10) {
        display.print("0");
    }
    display.print(now.minute());
    display.print(":");
    if (now.second() < 10) {
        display.print("0");
    }
    display.print(now.second());
    display.display(); // Refresh display
    delay(2000); // Show time for 2 seconds
}
