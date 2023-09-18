#include <Arduino.h>
#include <DHT.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
const char* ssid = "ESP32 WiFI AP";
AsyncWebServer server(80);
unsigned long previousMillis = 0;
const long interval = 5000;
// 溫濕度
#define DHTPIN 27
DHT dht(DHTPIN, DHT11);
float t = 0.0;
float h = 0.0;

// 光敏
// #define sensor 26
// #define internal_led 13
// int value = 0;

// 人體
// int inPin = 13;
// int val;

// 麥克風
// int sound_digital = 13;
// int sound_analog = 26;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .AHT-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP32 DHT Server</h2>
  <p>
    <span class="AHT-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <span class="AHT-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">%</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

String processor(const String& var) {
  Serial.println(var);
  if (var == "TEMPERATURE") {
    return String(t);
  } else if (var == "HUMIDITY") {
    return String(h);
  }
  return String();
}

void setup() {
  Serial.begin(115200);
  // 溫濕度
  dht.begin();

  // 光敏
  // pinMode(sensor, INPUT);
  // pinMode(internal_led, OUTPUT);

  // 人體
  // pinMode(inPin, INPUT);

  // 麥克風
  // pinMode(sound_digital, INPUT);

  t = dht.readTemperature();
  h = dht.readHumidity();
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", String(h).c_str());
  });
  server.begin();
}

void loop() {
  // 溫濕度
  // float temperatureCelsius = dht.readTemperature();
  // float humidity = dht.readHumidity();
  // Serial.print("目前溫度：");
  // Serial.print(temperatureCelsius);
  // Serial.print(" °C\t");
  // Serial.print("目前濕度：");
  // Serial.print(humidity);
  // Serial.println(" %");

  // 光敏
  // value = digitalRead(sensor);
  // if (value == HIGH) {  // in front of it
  //   digitalWrite(internal_led, HIGH);
  //   Serial.println("HIGH");
  // } else {
  //   digitalWrite(internal_led, LOW);
  //   Serial.println("LOW");
  // }

  // 人體
  // val = digitalRead(inPin);
  // if (val == HIGH) {
  //   Serial.println("HIGH");
  // } else {
  //   Serial.println("LOW");
  // }

  // 麥克風
  // int val_digital = digitalRead(sound_digital);
  // int val_analog = analogRead(sound_analog);
  // Serial.print(val_analog);
  // Serial.print("\t");
  // Serial.println(val_digital);
  // if (val_digital == HIGH) {
  //   Serial.println("HIGH");
  // } else {
  //   Serial.println("LOW");
  // }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    float newT = dht.readTemperature();
    t = newT;
    Serial.println(t);
    float newH = dht.readHumidity();
    h = newH;
    Serial.println(h);
  }
}
