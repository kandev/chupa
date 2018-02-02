#include <Time.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NtpClientLib.h>
#include <TimeLib.h>
#include <ArduinoJson.h>
#include "FS.h"
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266httpUpdate.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include <Adafruit_Sensor.h>
#include <ESP8266HTTPClient.h>
ADC_MODE(ADC_VCC);  //read supply voltage by ESP.getVcc()

//Main configuration
#define _ONBOARD_LED 2 //2 for most nodemcu
#define _PIN_LED 13 //for sonoff it is 13
#define _PIN1 12
#define _PIN2 14
#define _PIN_RESET 0
#define _VERSION "0.190"
#define _PRODUCT "Chupa"
#define _UPDATE_SERVER "chupa.kandev.com"
#define _UPDATE_PORT 80
#define _UPDATE_URL "/"
#define update_path "/update"
#define update_username "chupa"
#define update_password "apuhc"
#define _UPDATE_CHECK_INTERVAL 60 // in seconds
#define _DEEPSLEEP_INTERVAL 900 // in seconds
#define _WATCHDOG_TIMEOUT 600 // in seconds
#define _CONFIG "/config.json"
#define _NTP_SERVER F("bg.pool.ntp.org")
#define _TIMEZONE 2

String _HOSTNAME = "";
String _RFIDS;
struct sched {
  unsigned int time_on = 0;
  unsigned int time_off = 0;
};
sched schedule[2];
String _MQTT_SERVER;
unsigned int _MQTT_SERVERPORT;
String _MQTT_USERNAME;
String _MQTT_KEY;
String _SSID;
String _PASS;
String _ADMIN_PASS = "";
bool _CLIENT = false; // are we client or AP?
IPAddress timeServerIP;
unsigned long reset_hold = 0;
unsigned long switch_press = 0;
bool switch_press_done = false;
unsigned long blink_millis = 0;
unsigned int mqtt_drop_count = 0;
long last_update_check = 10000 - (_UPDATE_CHECK_INTERVAL * 1000);
volatile int watchdog_counter = 0;
unsigned long mqtt_last = 0;
Ticker secondTick;
Ticker mqttReconnectTimer;
Ticker wifiReconnectTimer;
Ticker SiwtchOffDelay;
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
AsyncMqttClient mqttClient;
unsigned int led_delay = 1;
unsigned long last_wifi_connect_attempt = 0;
String code = "";
String mqtt_status;
bool off_countdown = false;
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
#include "web_static.h"


void watchdog() {
  watchdog_counter++;
  if (watchdog_counter >= _WATCHDOG_TIMEOUT) {
    Serial.println(F("WATCHDOG!"));
    ESP.restart();
  }
}

void led_on() {
  digitalWrite(_PIN_LED, LOW);
  digitalWrite(_ONBOARD_LED, LOW);
}

void led_off() {
  digitalWrite(_PIN_LED, HIGH);
  digitalWrite(_ONBOARD_LED, HIGH);
}

String getMacAddress() {
  char mac[12];
  String s = "";
  uint8_t MAC_array[6];
  WiFi.macAddress(MAC_array);
  for (int i = 0; i < sizeof(MAC_array); ++i)
    s += String(MAC_array[i], HEX);
  return s;
}

void send_msg(String s) {
  HTTPClient http;
  String srv = "http://" + String(_UPDATE_SERVER) + "/msg/";
  String payload = "hostname=" + _HOSTNAME + "&mac=" + getMacAddress() + "&msg=" + s;
  http.begin(srv);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int c = http.POST(payload);
  if (c > 0) {
    Serial.print(F("Result code: "));
    Serial.println(c);
  } else {
    Serial.println(F("MSG Sent!"));
  }
}

void AllOff() {
  switchpin(_PIN1, 0);
  switchpin(_PIN2, 0);
  SiwtchOffDelay.detach();
  Serial.println(F("Disconnected for too long - all outputs down!"));
  off_countdown=false;
}

bool openFS() {
  if (!SPIFFS.begin()) {
    Serial.println(F("[ERR] Не може да бъде монтирана файловат система. Ще бъде направен опит за форматиране."));
    if (!SPIFFS.format())
      Serial.println(F("[ERR] Файловата система не може да бъде форматирана."));
    else
      Serial.println(F("[OK] Файловата система е форматирана успешно."));
    return false;
  }
  return true;
}

void closeFS() {
  SPIFFS.end();
}

void switchpin(int pin, int state) {
  digitalWrite(pin, state);
}


void scan_data() {
  if ((!server.authenticate("admin", _ADMIN_PASS.c_str())) && (_CLIENT))
    server.requestAuthentication();
  int w = WiFi.scanNetworks();
  String wifis = "{";
  byte i;
  for (i = 0; i < w; i++) {
    wifis += "\"" + WiFi.SSID(i) + "\":\"" + WiFi.RSSI(i) + "\"";
    if (i < w - 1) wifis += ", ";
  }
  wifis += "}";
  server.send ( 200, F("application/json"), "{ \"ssid\":\"" + String(_SSID) + "\", \
    \"wifis\":" + wifis + " }");
}

void html_root() {
  if ((!server.authenticate("admin", _ADMIN_PASS.c_str())) && (_CLIENT))
    server.requestAuthentication();
  server.send(200, F("text/html"), PAGE_root);
}

void html_favicon() {
  server.send(200, F("image/svg+xml"), PAGE_favicon);
}

void html_gpio() {
  if ((!server.authenticate("admin", _ADMIN_PASS.c_str())) && (_CLIENT))
    server.requestAuthentication();
  for (int i = 0; i < server.args(); i++) {
    if (server.argName(i) == "pin1")
      if (server.arg(i) == "1") {
        switchpin(_PIN1, 1);
        mqttClient.publish(String(_HOSTNAME + "/set/pin1").c_str(), 1, true, String(digitalRead(_PIN1)).c_str());
        mqttClient.publish(String(_HOSTNAME + "/status/pin1").c_str(), 1, true, String(digitalRead(_PIN1)).c_str());
        send_msg(String("Web, PIN1=" + String(digitalRead(_PIN1))));
      } else {
        switchpin(_PIN1, 0);
        mqttClient.publish(String(_HOSTNAME + "/set/pin1").c_str(), 1, true, String(digitalRead(_PIN1)).c_str());
        mqttClient.publish(String(_HOSTNAME + "/status/pin1").c_str(), 1, true, String(digitalRead(_PIN1)).c_str());
        send_msg(String("Web, PIN1=" + String(digitalRead(_PIN1))));
      }
    if (server.argName(i) == "pin2")
      if (server.arg(i) == "1") {
        switchpin(_PIN2, 1);
        mqttClient.publish(String(_HOSTNAME + "/set/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
        mqttClient.publish(String(_HOSTNAME + "/status/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
        send_msg(String("Web, PIN2=" + String(digitalRead(_PIN2))));
      } else {
        switchpin(_PIN2, 0);
        mqttClient.publish(String(_HOSTNAME + "/set/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
        mqttClient.publish(String(_HOSTNAME + "/status/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
        send_msg(String("Web, PIN2=" + String(digitalRead(_PIN2))));
      }
  }
  server.send ( 200, F("application/json"), "{ \
        \"time\":\"" + NTP.getTimeDateString() + "\", \
        \"pin1\":\"" + digitalRead(_PIN1) + "\", \
        \"pin2\":\"" + digitalRead(_PIN2) + "\" \
                }" );
}

void handle_deleteconfig() {
  if (!server.authenticate("admin", _ADMIN_PASS.c_str()))
    server.requestAuthentication();
  if (!SPIFFS.format())
    Serial.println(F("[ERR] Unable to format flash."));
  else
    Serial.println(F("[OK] Flash formatted."));
  Serial.println(F("Restarting..."));
  ESP.restart();
}

void handle_reboot() {
  if (!server.authenticate("admin", _ADMIN_PASS.c_str()))
    server.requestAuthentication();
  Serial.println(F("Restarting..."));
  server.send(200, F("text/html"), F("<meta http-equiv=\"refresh\" content=\"5; url=/\" />[OK] Restarting..."));
  ESP.restart();
}

void checkforupdate() {
  float vcc = ESP.getVcc() / 1000.0; // supply voltage
  Serial.println(F("Checking for update..."));
  auto ret = ESPhttpUpdate.update(_UPDATE_SERVER, _UPDATE_PORT, _UPDATE_URL, _VERSION);
  mqttClient.publish(String(_HOSTNAME + "/status/online").c_str(), 1, true, "1");
  mqttClient.publish(String(_HOSTNAME + "/status/rssi").c_str(), 1, true, String(WiFi.RSSI()).c_str());
  mqttClient.publish(String(_HOSTNAME + "/status/vcc").c_str(), 1, true, String(vcc).c_str());
  mqttClient.publish(String(_HOSTNAME + "/status/ip").c_str(), 1, true, WiFi.localIP().toString().c_str());
  mqttClient.publish(String(_HOSTNAME + "/status/uptime").c_str(), 1, true, String(millis() / 1000).c_str());
  mqttClient.publish(String(_HOSTNAME + "/status/version").c_str(), 1, true, _VERSION);
  mqttClient.publish(String(_HOSTNAME + "/status/mac").c_str(), 1, true, getMacAddress().c_str());
  mqttClient.publish(String(_HOSTNAME + "/status/time").c_str(), 1, true, NTP.getTimeDateString().c_str());
  mqttClient.publish(String(_HOSTNAME + "/status/pin1").c_str(), 1, true, String(digitalRead(_PIN1)).c_str());
  mqttClient.publish(String(_HOSTNAME + "/status/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
  server.send(200, F("text/plain"), F("Checking for update..."));
  //check if mqtt is disconnected and reconnect
  if ((!mqttClient.connected()) && (_MQTT_SERVER.length() > 0))
    connectToMqtt();
}

void connectToMqtt() {
  IPAddress mqttIP;
  if (WiFi.isConnected()) {
    Serial.println(F("Connecting to MQTT..."));
    WiFi.hostByName(_MQTT_SERVER.c_str(), mqttIP);
    Serial.println(String("Connecting to MQTT broker " + _MQTT_SERVER + ", resolved to " + mqttIP.toString()));
    mqttClient.setServer(mqttIP, _MQTT_SERVERPORT);
    mqttReconnectTimer.detach();
    mqttClient.connect();
  }
}
void onMqttConnect(bool sessionPresent) {
  String subs = _HOSTNAME + "/set/#";
  mqttClient.subscribe(subs.c_str(), 2);
  Serial.print(String("Subscribing to: [" + subs + "]... "));
}
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  mqttReconnectTimer.attach(30, connectToMqtt);
}
void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println(F("[OK] MQTT subscription done!"));
}
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  String msg = String(topic);
  msg = msg.substring(msg.indexOf("/set/"));
  Serial.println(String("Received: " + msg + "=" + payload));
  if (msg == "/set/pin1") {
    if ((char)payload[0] == '0') switchpin(_PIN1, 0);
    if ((char)payload[0] == '1') switchpin(_PIN1, 1);
    mqttClient.publish(String(_HOSTNAME + "/status/pin1").c_str(), 1, true, String(digitalRead(_PIN1)).c_str());
    send_msg(String("MQTT request, PIN1=" + String(digitalRead(_PIN1))));
  }
  if (msg == "/set/pin2") {
    if ((char)payload[0] == '0') switchpin(_PIN2, 0);
    if ((char)payload[0] == '1') switchpin(_PIN2, 1);
    mqttClient.publish(String(_HOSTNAME + "/status/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
    send_msg(String("MQTT request, PIN2=" + String(digitalRead(_PIN2))));
  }
}

void wifi_connect() {
  if (_SSID == "") _SSID = _HOSTNAME;
  WiFi.hostname(_HOSTNAME);
  if (!_CLIENT) {
    wifiReconnectTimer.detach();
    led_delay = 1000;  //if in mode AP blink faster
    Serial.printf("SSID: %s\r\n", _SSID.c_str());
    if (_PASS != "") Serial.printf("Password: %s\r\n", _PASS.c_str());
    WiFi.mode(WIFI_AP);
    //    WiFi.setOutputPower(1);
    WiFi.softAP(_SSID.c_str(), _PASS.c_str(), random(1, 13));
    //WiFi.softAPConfig(_IP, _GATE, _MASK);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print(F("To configure the device, please connect to the wifi network and open http://"));
    Serial.println(myIP.toString().c_str());
  } else {
    wifiReconnectTimer.detach();
    led_delay = 5000;  //if client - blink once every 5 seconds
    last_wifi_connect_attempt = millis();
    Serial.print(F("Connecting to "));
    Serial.println(_SSID.c_str());
    WiFi.mode(WIFI_STA);
    WiFi.begin(_SSID.c_str(), _PASS.c_str());
    while ((WiFi.status() != WL_CONNECTED) && ((millis() - last_wifi_connect_attempt) < 20000)) {
      delay(200);
      Serial.print(".");
      led_on();
      delay(20);
      led_off();
      yield();
      if (digitalRead(_PIN_RESET) == 0) return;
    }
    Serial.println();
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println(F("[OK] Connected."));
      if (_MQTT_SERVER.length() > 0)
        mqttReconnectTimer.attach(5, connectToMqtt);
    } else {
      Serial.println(F("[ERR] Will try again in a minute."));
      led_delay = 1000;
    }
  }
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println(F("Connected to Wi-Fi."));
  if (_MQTT_SERVER.length() > 0)
    mqttReconnectTimer.attach(5, connectToMqtt);
  SiwtchOffDelay.detach();
  off_countdown=false;
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println(F("Disconnected from Wi-Fi"));
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.attach(10, wifi_connect);
  if (!off_countdown) {
    SiwtchOffDelay.attach(60, AllOff);
    off_countdown=true;
  }
}

void setup()
{
  //watchdog init
  secondTick.attach(1, watchdog);
  pinMode(_PIN1, OUTPUT); digitalWrite(_PIN1, LOW);
  pinMode(_PIN2, OUTPUT); digitalWrite(_PIN2, LOW);
  pinMode(_PIN_LED, OUTPUT);
  pinMode(_PIN_RESET, INPUT_PULLUP);  //factory reset
  pinMode(_ONBOARD_LED, OUTPUT); //onboard led
  led_off();

  Serial.begin(115200); Serial.println();
  Serial.print(_PRODUCT);
  Serial.print(F(", "));
  Serial.println(_VERSION);
  _CLIENT = loadConfig();
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  wifi_connect();
  server.on("/configure", handle_configure);
  server.on("/config.json", handle_showconfig);
  server.on("/check", checkforupdate);
  server.on("/data", get_data);
  server.on("/scan", scan_data);
  server.on("/gpio", html_gpio);
  server.on("/reboot", handle_reboot);
  server.on("/", html_root);
  server.on("/favicon.svg", html_favicon);
  server.begin();
  //mqtt
  if (_CLIENT) {
    httpUpdater.setup(&server, update_path, update_username, update_password);
    Serial.print(F("Име: "));
    Serial.println(_HOSTNAME);
    Serial.print(F("Получен ИП адрес: "));
    Serial.println(WiFi.localIP().toString().c_str());
    IPAddress mqttIP;
    WiFi.hostByName(_MQTT_SERVER.c_str(), mqttIP);
    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onSubscribe(onMqttSubscribe);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.setServer(mqttIP, _MQTT_SERVERPORT);
    mqttClient.setKeepAlive(5).setCleanSession(false).setWill(String(_HOSTNAME + "/status/online").c_str(), 2, true, "0").setCredentials(_MQTT_USERNAME.c_str(), _MQTT_KEY.c_str()).setClientId(_HOSTNAME.c_str());
    Serial.print(F("Connecting to MQTT broker "));
    Serial.print(_MQTT_SERVER);
    Serial.print(F(", resolved to "));
    Serial.println(mqttIP.toString());
    if (NTP.begin(String(_NTP_SERVER), _TIMEZONE, true))
      Serial.println(F("[OK] NTP..."));
    else
      Serial.println(F("[ERR] NTP not initialized."));
    NTP.setInterval(3600);  //ntp sync once per hour
  }
  Serial.println("RFIDs:");
}

void loop() {
  server.handleClient();

  // FACTORY RESET
  if (digitalRead(_PIN_RESET) == 0) {
    if (reset_hold == 0)
      reset_hold = millis();
    Serial.println(F("!"));
    if (millis() - reset_hold >= 10000) {
      led_on();
      delay(1000);
      handle_deleteconfig();
    }
  } else {
    reset_hold = 0;
  }

  // Switch PIN1 when reset pin is pressed
  if (digitalRead(_PIN_RESET) == 0) {
    if (switch_press == 0)
      switch_press = millis();
    if ((millis() - switch_press >= 50) and (!switch_press_done)) {
      (digitalRead(_PIN1) == 0) ? (switchpin(_PIN1, 1)) : (switchpin(_PIN1, 0));
      mqttClient.publish(String(_HOSTNAME + "/set/pin1").c_str(), 1, true, String(digitalRead(_PIN1)).c_str());
      mqttClient.publish(String(_HOSTNAME + "/status/pin1").c_str(), 1, true, String(digitalRead(_PIN1)).c_str());
      switch_press_done = true;
      send_msg(String("Button press, PIN1=" + String(digitalRead(_PIN1))));
    }
  } else {
    switch_press = 0;
    switch_press_done = false;
  }

  //handle blinking frequency
  if (millis() - blink_millis >= led_delay) {
    led_on();
    delay(2);
    led_off();
    if (digitalRead(_PIN1) == 1) {
      delay(100);
      led_on();
      delay(2);
      led_off();
    }
    blink_millis = millis();
  }

  if (_CLIENT) {
    //handle disconnect event
    if ((WiFi.status() == WL_DISCONNECTED) && (millis() - last_wifi_connect_attempt > 90000)) {
      Serial.println(F("[ERR] Reconnecting."));
      WiFi.disconnect();
      mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
      wifiReconnectTimer.attach(5, wifi_connect);
    }

    //update check
    if ((millis() - last_update_check) / 1000 >= _UPDATE_CHECK_INTERVAL) {
      last_update_check = millis();
      if (WiFi.status() == WL_CONNECTED) {
        checkforupdate();
      }
    }
  }

  if (Serial.available()) {
    char c = Serial.read();
    Serial.print(c);
    if (c == '1') {
      (digitalRead(_PIN1) == 0) ? (switchpin(_PIN1, 1)) : (switchpin(_PIN1, 0));
      mqttClient.publish(String(_HOSTNAME + "/set/pin1").c_str(), 1, true, String(digitalRead(_PIN1)).c_str());
      mqttClient.publish(String(_HOSTNAME + "/status/pin1").c_str(), 1, true, String(digitalRead(_PIN1)).c_str());
      Serial.print(F("PIN1 set to "));
      Serial.println(digitalRead(_PIN1));
      send_msg(String("Serial, PIN1=" + String(digitalRead(_PIN1))));
    }
    if (c == '2') {
      (digitalRead(_PIN2) == 0) ? (switchpin(_PIN2, 1)) : (switchpin(_PIN2, 0));
      mqttClient.publish(String(_HOSTNAME + "/set/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
      mqttClient.publish(String(_HOSTNAME + "/status/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
      Serial.print(F("PIN2 set to "));
      Serial.println(digitalRead(_PIN2));
      send_msg(String("Serial, PIN2=" + String(digitalRead(_PIN2))));
    }
    if (c == '\r') {
      Serial.println();
      if (code == "?") {
        Serial.println(F("Chupa shell help."));
        Serial.println(F("w\t- Wifi scan"));
        Serial.println(F("msg\t- Send test email message"));
        Serial.println(F("reset\t- Factory reset"));
      }
      if (code == "w") {
        Serial.println(F("Scanning for wifi networks..."));
        int w = WiFi.scanNetworks();
        if (w == 0)
          Serial.println(F("no wifi networks found"));
        byte i;
        for (i = 0; i < w; i++) {
          Serial.print(WiFi.SSID(i));
          Serial.print(F("\t\t ["));
          Serial.print(WiFi.RSSI(i));
          Serial.print("dBi, ch:");
          Serial.print(WiFi.channel(i));
          Serial.print(", ");
          Serial.print(WiFi.BSSIDstr(i));
          Serial.println("]");
        }
      }
      if (code == "reset") {
        Serial.println(F("Reset command accepted. Please wait..."));
        led_on();
        delay(1000);
        handle_deleteconfig();
      }
      if (code == "msg") {
        Serial.println(F("Sending message..."));
        send_msg("hello world!");
      }
      Serial.print(_HOSTNAME);
      Serial.print(">");
      code = "";
    } else
      code += c;
  }
  watchdog_counter = 0;
  yield();
}
