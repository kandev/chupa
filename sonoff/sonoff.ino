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
#include "web_static.h"
#include <Adafruit_Sensor.h>
ADC_MODE(ADC_VCC);  //read supply voltage by ESP.getVcc()

//Main configuration
const char* _VERSION = "0.183";
const char* _PRODUCT = "Chupa";
String _HOSTNAME = "";
const char* _UPDATE_SERVER = "chupa.kandev.com";
const byte _UPDATE_PORT = 80;
const char* _UPDATE_URL = "/";
const unsigned int _UPDATE_CHECK_INTERVAL = 300; // in seconds
int _DEEPSLEEP_INTERVAL = 900; // in seconds
const unsigned int _WATCHDOG_TIMEOUT = 600; // in seconds
String _MQTT_SERVER;
unsigned int _MQTT_SERVERPORT;
String _MQTT_USERNAME;
String _MQTT_KEY;
String _NTP_SERVER;
const unsigned int _WIFI_TIMEOUT = 300;
//const unsigned int _MQTT_TIMEOUT = 600;
String _TIMEZONE;
String _SMTP_SERVER;
String _SMTP_PORT;
String _SMTP_USER;
String _SMTP_PASS;
String _SMTP_FROM;
String _SMTP_TO;
struct sched {
  byte on_h;
  byte on_m;
  byte off_h;
  byte off_m;
  byte pin;
};
sched schedule[5];

const byte _PIN_RESET = 0;
const byte _PIN_LED = 13;
const byte _PIN1 = 12;
const byte _PIN2 = 14;
const byte _PIN3 = 14;
const byte _PIN4 = 14;

String _SSID;
String _PASS;
String _ADMIN_PASS = "";
bool _CLIENT = false; // are we client or AP?
const char* _CONFIG = "/config.json";
const char* update_path = "/update";
const char* update_username = "chupa";
const char* update_password = "apuhc";
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
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
AsyncMqttClient mqttClient;
unsigned int led_delay = 1;
unsigned long last_wifi_connect_attempt = 0;
String code = "";
String mqtt_status;
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void watchdog() {
  watchdog_counter++;
  if (watchdog_counter >= _WATCHDOG_TIMEOUT) {
    Serial.println("WATCHDOG!");
    ESP.restart();
  }
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

bool loadConfig() {
  _HOSTNAME = String(_PRODUCT) + "-" + getMacAddress().substring(6);
  _HOSTNAME.toLowerCase();
  openFS();
  File configFile = SPIFFS.open(_CONFIG, "r");
  if (!configFile) {
    Serial.println(F("[ERR] No configuration found."));
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println(F("[ERR] configuration file is too large."));
    return false;
  }

  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println(F("[ERR] Can't parse configuration file."));
    return false;
  }
  const char* ssid = json["ssid"];
  const char* pass = json["pass"];
  const char* hostname = json["hostname"];
  const char* mqtt_server = json["mqtt_server"];
  _MQTT_SERVERPORT = json["mqtt_serverport"];
  const char* mqtt_user = json["mqtt_username"];
  const char* mqtt_key = json["mqtt_key"];
  const char* admin_pass = json["admin_password"];
  const char* ntp_server = json["ntp_server"];
  const char* timezone = json["timezone"];
  const char* smtp_server = json["smtp_server"];
  const char* smtp_port = json["smtp_serverport"];
  const char* smtp_user = json["smtp_username"];
  const char* smtp_pass = json["smtp_password"];
  const char* smtp_from = json["smtp_from"];
  const char* smtp_to = json["smtp_to"];
  schedule[0].on_h = json["s1_h_on"];
  schedule[0].on_m = json["s1_m_on"];
  schedule[0].off_h = json["s1_h_off"];
  schedule[0].off_m = json["s1_m_off"];
  schedule[0].pin = json["s1_pin"];
  schedule[1].on_h = json["s2_h_on"];
  schedule[1].on_m = json["s2_m_on"];
  schedule[1].off_h = json["s2_h_off"];
  schedule[1].off_m = json["s2_m_off"];
  schedule[1].pin = json["s2_pin"];
  schedule[2].on_h = json["s3_h_on"];
  schedule[2].on_m = json["s3_m_on"];
  schedule[2].off_h = json["s3_h_off"];
  schedule[2].off_m = json["s3_m_off"];
  schedule[2].pin = json["s3_pin"];
  schedule[3].on_h = json["s4_h_on"];
  schedule[3].on_m = json["s4_m_on"];
  schedule[3].off_h = json["s4_h_off"];
  schedule[3].off_m = json["s4_m_off"];
  schedule[3].pin = json["s4_pin"];
  schedule[4].on_h = json["s5_h_on"];
  schedule[4].on_m = json["s5_m_on"];
  schedule[4].off_h = json["s5_h_off"];
  schedule[4].off_m = json["s5_m_off"];
  schedule[4].pin = json["s5_pin"];

  if (String(hostname) != "")
    _HOSTNAME = hostname;
  _SSID = ssid;
  _PASS = pass;
  _MQTT_SERVER = mqtt_server;
  _MQTT_USERNAME = mqtt_user;
  _MQTT_KEY = mqtt_key;
  _ADMIN_PASS = admin_pass;
  _NTP_SERVER = ntp_server;
  _TIMEZONE = timezone;
  _SMTP_SERVER = smtp_server;
  _SMTP_PORT = smtp_port;
  _SMTP_USER = smtp_user;
  _SMTP_PASS = smtp_pass;
  _SMTP_FROM = smtp_from;
  _SMTP_TO = smtp_to;

  if (_MQTT_SERVER.length() == 0) _MQTT_SERVER = "";
  if (_MQTT_USERNAME.length() == 0) _MQTT_USERNAME = "";
  if (_MQTT_KEY.length() == 0) _MQTT_KEY = "";
  if (_ADMIN_PASS.length() == 0) _ADMIN_PASS = "";
  if (_SSID.length() == 0) _SSID = "";
  if (_PASS.length() == 0) _PASS = "";
  if (_SMTP_SERVER.length() == 0) _SMTP_SERVER = "";
  if (_SMTP_PORT.length() == 0) _SMTP_PORT = "";
  if (_SMTP_USER.length() == 0) _SMTP_USER = "";
  if (_SMTP_PASS.length() == 0) _SMTP_PASS = "";
  if (_SMTP_FROM.length() == 0) _SMTP_FROM = "";
  if (_SMTP_TO.length() == 0) _SMTP_TO = "";
  if (_NTP_SERVER.length() == 0) _NTP_SERVER = "bg.pool.ntp.org";
  if (_TIMEZONE.length() == 0) _TIMEZONE = "2";
  if (_MQTT_SERVERPORT == 0) _MQTT_SERVERPORT = 1883;

  Serial.print(F("SSID: "));
  Serial.println(_SSID);
  closeFS();
  return true;
}

void switchpin(int pin, int state) {
  digitalWrite(pin, state);
}

void get_data() {
  if ((!server.authenticate("admin", _ADMIN_PASS.c_str())) && (_CLIENT))
    server.requestAuthentication();
  if (_NTP_SERVER == "") _NTP_SERVER = "bg.pool.ntp.org";
  if (mqttClient.connected()) {
    mqtt_status="connected";
  } else {
    mqtt_status="disconnected";
  }
  server.send ( 200, F("application/json"), "{ \"hostname\":\"" + String(_HOSTNAME) + "\", \
        \"ssid\":\"" + String(_SSID) + "\", \
        \"ntp_server\":\"" + _NTP_SERVER + "\", \
        \"timezone\":\"" + _TIMEZONE + "\", \
        \"password\":\"" + String(_PASS) + "\", \
        \"mqtt_server\":\"" + _MQTT_SERVER + "\", \
        \"mqtt_serverport\":\"" + _MQTT_SERVERPORT + "\", \
        \"mqtt_username\":\"" + _MQTT_USERNAME + "\", \
        \"mqtt_key\":\"" + _MQTT_KEY + "\", \
        \"smtp_server\":\"" + _SMTP_SERVER + "\", \
        \"smtp_serverport\":\"" + _SMTP_PORT + "\", \
        \"smtp_username\":\"" + _SMTP_USER + "\", \
        \"smtp_password\":\"" + _SMTP_PASS + "\", \
        \"smtp_from\":\"" + _SMTP_FROM + "\", \
        \"smtp_to\":\"" + _SMTP_TO + "\", \
        \"admin_password\":\"" + _ADMIN_PASS + "\", \
        \"version\":\"" + String(_VERSION) + "\", \
        \"rssi\":\"" + String(WiFi.RSSI()) + "\", \
        \"time\":\"" + NTP.getTimeDateString() + "\", \
        \"flash_size\":\"" + ESP.getFlashChipRealSize() / 1024 + "KB\", \
        \"mac\":\"" + getMacAddress() + "\", \
        \"vcc\":\"" + String(float(ESP.getVcc() / 1000.0)) + "\", \
        \"pin1\":\"" + digitalRead(_PIN1) + "\", \
        \"pin2\":\"" + digitalRead(_PIN2) + "\", \
        \"pin3\":\"" + digitalRead(_PIN3) + "\", \
        \"pin4\":\"" + digitalRead(_PIN4) + "\", \
        \"sched1_pin\":\"" + schedule[0].pin + "\", \
        \"sched1_h_on\":\"" + schedule[0].on_h + "\", \
        \"sched1_m_on\":\"" + schedule[0].on_m + "\", \
        \"sched1_h_off\":\"" + schedule[0].off_h + "\", \
        \"sched1_m_off\":\"" + schedule[0].off_m + "\", \
        \"sched2_pin\":\"" + schedule[1].pin + "\", \
        \"sched2_h_on\":\"" + schedule[1].on_h + "\", \
        \"sched2_m_on\":\"" + schedule[1].on_m + "\", \
        \"sched2_h_off\":\"" + schedule[1].off_h + "\", \
        \"sched2_m_off\":\"" + schedule[1].off_m + "\", \
        \"sched3_pin\":\"" + schedule[2].pin + "\", \
        \"sched3_h_on\":\"" + schedule[2].on_h + "\", \
        \"sched3_m_on\":\"" + schedule[2].on_m + "\", \
        \"sched3_h_off\":\"" + schedule[2].off_h + "\", \
        \"sched3_m_off\":\"" + schedule[2].off_m + "\", \
        \"sched4_pin\":\"" + schedule[3].pin + "\", \
        \"sched4_h_on\":\"" + schedule[3].on_h + "\", \
        \"sched4_m_on\":\"" + schedule[3].on_m + "\", \
        \"sched4_h_off\":\"" + schedule[3].off_h + "\", \
        \"sched4_m_off\":\"" + schedule[3].off_m + "\", \
        \"sched5_pin\":\"" + schedule[4].pin + "\", \
        \"sched5_h_on\":\"" + schedule[4].on_h + "\", \
        \"sched5_m_on\":\"" + schedule[4].on_m + "\", \
        \"sched5_h_off\":\"" + schedule[4].off_h + "\", \
        \"sched5_m_off\":\"" + schedule[4].off_m + "\", \
        \"temperature\":\"n/a\", \
        \"humidity\":\"n/a\", \
        \"mqtt_status\":\"" + String(mqtt_status) + "\" \
                }" );
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
  if (_NTP_SERVER == "") _NTP_SERVER = "bg.pool.ntp.org";
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
      } else {
        switchpin(_PIN1, 0);
        mqttClient.publish(String(_HOSTNAME + "/set/pin1").c_str(), 1, true, String(digitalRead(_PIN1)).c_str());
        mqttClient.publish(String(_HOSTNAME + "/status/pin1").c_str(), 1, true, String(digitalRead(_PIN1)).c_str());
      }
    if (server.argName(i) == "pin2")
      if (server.arg(i) == "1") {
        switchpin(_PIN2, 1);
        mqttClient.publish(String(_HOSTNAME + "/set/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
        mqttClient.publish(String(_HOSTNAME + "/status/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
      } else {
        switchpin(_PIN2, 0);
        mqttClient.publish(String(_HOSTNAME + "/set/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
        mqttClient.publish(String(_HOSTNAME + "/status/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
      }
    if (server.argName(i) == "pin3")
      if (server.arg(i) == "1") {
        switchpin(_PIN3, 1);
        mqttClient.publish(String(_HOSTNAME + "/set/pin3").c_str(), 1, true, String(digitalRead(_PIN3)).c_str());
        mqttClient.publish(String(_HOSTNAME + "/status/pin3").c_str(), 1, true, String(digitalRead(_PIN3)).c_str());
      } else {
        switchpin(_PIN3, 0);
        mqttClient.publish(String(_HOSTNAME + "/set/pin3").c_str(), 1, true, String(digitalRead(_PIN3)).c_str());
        mqttClient.publish(String(_HOSTNAME + "/status/pin3").c_str(), 1, true, String(digitalRead(_PIN3)).c_str());
      }
    if (server.argName(i) == "pin4")
      if (server.arg(i) == "1") {
        switchpin(_PIN4, 1);
        mqttClient.publish(String(_HOSTNAME + "/set/pin4").c_str(), 1, true, String(digitalRead(_PIN4)).c_str());
        mqttClient.publish(String(_HOSTNAME + "/status/pin4").c_str(), 1, true, String(digitalRead(_PIN4)).c_str());
      } else {
        switchpin(_PIN4, 0);
        mqttClient.publish(String(_HOSTNAME + "/set/pin4").c_str(), 1, true, String(digitalRead(_PIN4)).c_str());
        mqttClient.publish(String(_HOSTNAME + "/status/pin4").c_str(), 1, true, String(digitalRead(_PIN4)).c_str());
      }
  }
  server.send ( 200, F("application/json"), "{ \
        \"time\":\"" + NTP.getTimeDateString() + "\", \
        \"pin1\":\"" + digitalRead(_PIN1) + "\", \
        \"pin2\":\"" + digitalRead(_PIN2) + "\", \
        \"pin3\":\"" + digitalRead(_PIN3) + "\", \
        \"pin4\":\"" + digitalRead(_PIN4) + "\" \
                }" );
}

void handle_configure() {
  if ((!server.authenticate("admin", _ADMIN_PASS.c_str())) && (_CLIENT))
    server.requestAuthentication();
  bool newssid, newpass;
  for (int i = 0; i < server.args(); i++) {
    if (server.argName(i) == "ssid") {
      (_SSID == server.arg(i)) ? (newssid = false) : (newssid = true);
      _SSID = server.arg(i);
    }
    if (server.argName(i) == "password") {
      (_PASS == server.arg(i)) ? (newpass = false) : (newpass = true);
      _PASS = server.arg(i);
    }
    if (server.argName(i) == "hostname") _HOSTNAME = server.arg(i);
    if (server.argName(i) == "mqtt_server") _MQTT_SERVER = server.arg(i);
    if (server.argName(i) == "mqtt_serverport") _MQTT_SERVERPORT = server.arg(i).toInt();
    if (server.argName(i) == "mqtt_username") _MQTT_USERNAME = server.arg(i);
    if (server.argName(i) == "mqtt_key") _MQTT_KEY = server.arg(i);
    if (server.argName(i) == "admin_password") _ADMIN_PASS = server.arg(i);
    if (server.argName(i) == "timezone") _TIMEZONE = server.arg(i).toInt();
    if (server.argName(i) == "ntp_server") _NTP_SERVER = server.arg(i);
    if (server.argName(i) == "sched1_h_on") schedule[0].on_h = server.arg(i).toInt();
    if (server.argName(i) == "sched1_m_on") schedule[0].on_m = server.arg(i).toInt();
    if (server.argName(i) == "sched1_h_off") schedule[0].off_h = server.arg(i).toInt();
    if (server.argName(i) == "sched1_m_off") schedule[0].off_m = server.arg(i).toInt();
    if (server.argName(i) == "sched1_pin") schedule[0].pin = server.arg(i).toInt();
    if (server.argName(i) == "sched2_h_on") schedule[1].on_h = server.arg(i).toInt();
    if (server.argName(i) == "sched2_m_on") schedule[1].on_m = server.arg(i).toInt();
    if (server.argName(i) == "sched2_h_off") schedule[1].off_h = server.arg(i).toInt();
    if (server.argName(i) == "sched2_m_off") schedule[1].off_m = server.arg(i).toInt();
    if (server.argName(i) == "sched2_pin") schedule[1].pin = server.arg(i).toInt();
    if (server.argName(i) == "sched3_h_on") schedule[2].on_h = server.arg(i).toInt();
    if (server.argName(i) == "sched3_m_on") schedule[2].on_m = server.arg(i).toInt();
    if (server.argName(i) == "sched3_h_off") schedule[2].off_h = server.arg(i).toInt();
    if (server.argName(i) == "sched3_m_off") schedule[2].off_m = server.arg(i).toInt();
    if (server.argName(i) == "sched3_pin") schedule[2].pin = server.arg(i).toInt();
    if (server.argName(i) == "sched4_h_on") schedule[3].on_h = server.arg(i).toInt();
    if (server.argName(i) == "sched4_m_on") schedule[3].on_m = server.arg(i).toInt();
    if (server.argName(i) == "sched4_h_off") schedule[3].off_h = server.arg(i).toInt();
    if (server.argName(i) == "sched4_m_off") schedule[3].off_m = server.arg(i).toInt();
    if (server.argName(i) == "sched4_pin") schedule[3].pin = server.arg(i).toInt();
    if (server.argName(i) == "sched5_h_on") schedule[4].on_h = server.arg(i).toInt();
    if (server.argName(i) == "sched5_m_on") schedule[4].on_m = server.arg(i).toInt();
    if (server.argName(i) == "sched5_h_off") schedule[4].off_h = server.arg(i).toInt();
    if (server.argName(i) == "sched5_m_off") schedule[4].off_m = server.arg(i).toInt();
    if (server.argName(i) == "sched5_pin") schedule[4].pin = server.arg(i).toInt();
    if (server.argName(i) == "smtp_server") _SMTP_SERVER = server.arg(i);
    if (server.argName(i) == "smtp_serverport") _SMTP_PORT = server.arg(i);
    if (server.argName(i) == "smtp_username") _SMTP_USER = server.arg(i);
    if (server.argName(i) == "smtp_password") _SMTP_PASS = server.arg(i);
    if (server.argName(i) == "smtp_from") _SMTP_FROM = server.arg(i);
    if (server.argName(i) == "smtp_to") _SMTP_TO = server.arg(i);
  }
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["ssid"] = _SSID;
  json["pass"] = _PASS;
  json["hostname"] = _HOSTNAME;
  json["mqtt_server"] = _MQTT_SERVER;
  json["mqtt_serverport"] = _MQTT_SERVERPORT;
  json["mqtt_username"] = _MQTT_USERNAME;
  json["mqtt_key"] = _MQTT_KEY;
  json["admin_password"] = _ADMIN_PASS;
  json["ntp_server"] = _NTP_SERVER;
  json["timezone"] = _TIMEZONE;
  json["s1_h_on"] = schedule[0].on_h;
  json["s1_m_on"] = schedule[0].on_m;
  json["s1_h_off"] = schedule[0].off_h;
  json["s1_m_off"] = schedule[0].off_m;
  json["s1_pin"] = schedule[0].pin;
  json["s2_h_on"] = schedule[1].on_h;
  json["s2_m_on"] = schedule[1].on_m;
  json["s2_h_off"] = schedule[1].off_h;
  json["s2_m_off"] = schedule[1].off_m;
  json["s2_pin"] = schedule[1].pin;
  json["s3_h_on"] = schedule[2].on_h;
  json["s3_m_on"] = schedule[2].on_m;
  json["s3_h_off"] = schedule[2].off_h;
  json["s3_m_off"] = schedule[2].off_m;
  json["s3_pin"] = schedule[2].pin;
  json["s4_h_on"] = schedule[3].on_h;
  json["s4_m_on"] = schedule[3].on_m;
  json["s4_h_off"] = schedule[3].off_h;
  json["s4_m_off"] = schedule[3].off_m;
  json["s4_pin"] = schedule[3].pin;
  json["s5_h_on"] = schedule[4].on_h;
  json["s5_m_on"] = schedule[4].on_m;
  json["s5_h_off"] = schedule[4].off_h;
  json["s5_m_off"] = schedule[4].off_m;
  json["s5_pin"] = schedule[4].pin;
  json["smtp_server"] = _SMTP_SERVER;
  json["smtp_port"] = _SMTP_PORT;
  json["smtp_user"] = _SMTP_USER;
  json["smtp_pass"] = _SMTP_PASS;
  json["smtp_from"] = _SMTP_FROM;
  json["smtp_to"] = _SMTP_TO;

  openFS();
  File configFile = SPIFFS.open(_CONFIG, "w");
  int error = 0;
  if (!configFile) {
    Serial.println(F("[ERR] Configuration can't be saved for some reason."));
    error = 1;
  }
  json.printTo(configFile);
  closeFS();
  if (error == 0) {
    server.send(200, F("text/html"), F("<meta http-equiv=\"refresh\" content=\"5; url=/\" />[OK]"));
  } else {
    server.send(200, F("text/plain"), F("[ERR]"));
  }
  if ((newssid) && (newpass)) ESP.restart();
}

void handle_showconfig() {
  if (!server.authenticate("admin", _ADMIN_PASS.c_str()))
    server.requestAuthentication();
  openFS();
  if (SPIFFS.exists(_CONFIG)) {
    File file = SPIFFS.open(_CONFIG, "r");
    size_t sent = server.streamFile(file, F("text/plain"));
    file.close();
  } else {
    server.send(200, F("text/plain"), F("Config file not found."));
  }
  closeFS();
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
  mqttClient.publish(String(_HOSTNAME + "/status/pin3").c_str(), 1, true, String(digitalRead(_PIN3)).c_str());
  mqttClient.publish(String(_HOSTNAME + "/status/pin4").c_str(), 1, true, String(digitalRead(_PIN4)).c_str());
  server.send(200, F("text/plain"), F("Checking for update..."));
  //check if mqtt is disconnected and reconnect
  if ((!mqttClient.connected()) && (_MQTT_SERVER.length() > 0))
    connectToMqtt();
}

void connectToMqtt() {
    IPAddress mqttIP;
    if (WiFi.isConnected()) {
      Serial.println("Connecting to MQTT...");
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
      mqttReconnectTimer.attach(5, connectToMqtt);
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
  }
  if (msg == "/set/pin2") {
    if ((char)payload[0] == '0') switchpin(_PIN2, 0);
    if ((char)payload[0] == '1') switchpin(_PIN2, 1);
    mqttClient.publish(String(_HOSTNAME + "/status/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
  }
  if (msg == "/set/pin3") {
    if ((char)payload[0] == '0') switchpin(_PIN3, 0);
    if ((char)payload[0] == '1') switchpin(_PIN3, 1);
    mqttClient.publish(String(_HOSTNAME + "/status/pin3").c_str(), 1, true, String(digitalRead(_PIN3)).c_str());
  }
  if (msg == "/set/pin4") {
    if ((char)payload[0] == '0') switchpin(_PIN4, 0);
    if ((char)payload[0] == '1') switchpin(_PIN4, 1);
    mqttClient.publish(String(_HOSTNAME + "/status/pin4").c_str(), 1, true, String(digitalRead(_PIN4)).c_str());
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
      digitalWrite(_PIN_LED, LOW);
      delay(20);
      digitalWrite(_PIN_LED, HIGH);
      yield();
      if (digitalRead(_PIN_RESET) == 0) return;
    }
    Serial.println();
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("[OK] Connected.");
      if (_MQTT_SERVER.length() > 0)
        mqttReconnectTimer.attach(5, connectToMqtt);
    } else {
      Serial.println("[ERR] Will try again in a minute.");
      led_delay = 1000;
    }
  }
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  if (_MQTT_SERVER.length() > 0)
    mqttReconnectTimer.attach(5, connectToMqtt);
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.attach(5, wifi_connect);
}

void setup()
{
  //watchdog init
  secondTick.attach(1, watchdog);
  pinMode(_PIN1, OUTPUT); digitalWrite(_PIN1, LOW);
  pinMode(_PIN2, OUTPUT); digitalWrite(_PIN2, LOW);
  pinMode(_PIN3, OUTPUT); digitalWrite(_PIN3, LOW);
  pinMode(_PIN4, OUTPUT); digitalWrite(_PIN4, LOW);
  pinMode(_PIN_LED, OUTPUT); digitalWrite(_PIN_LED, LOW);
  pinMode(_PIN_RESET, INPUT_PULLUP);  //factory reset

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
    if (NTP.begin(String(_NTP_SERVER), _TIMEZONE.toInt(), true))
      Serial.println(F("[OK] NTP..."));
    else
      Serial.println(F("[ERR] NTP not initialized."));
    NTP.setInterval(3600);  //ntp sync once per hour
  }
}


void loop() {
  server.handleClient();

  // FACTORY RESET
  if (digitalRead(_PIN_RESET) == 0) {
    if (reset_hold == 0)
      reset_hold = millis();
    Serial.println(F("!"));
    if (millis() - reset_hold >= 10000) {
      digitalWrite(_PIN_LED, LOW);
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
    }
  } else {
    switch_press = 0;
    switch_press_done = false;
  }

  //handle blinking frequency
  if (millis() - blink_millis >= led_delay) {
    digitalWrite(_PIN_LED, LOW);
    delay(1);
    digitalWrite(_PIN_LED, HIGH);
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

  //scheduler
  for (byte q = 0; q < 5; q++) {
    if (schedule[q].pin != 0) {
      unsigned int on_time = (schedule[q].on_h * 60) + schedule[q].on_m;
      unsigned int off_time = (schedule[q].off_h * 60) + schedule[q].off_m;
      unsigned int time_now = (hour() * 60) + minute();
      if (on_time < off_time) {
        if ((time_now >= on_time) && (time_now < off_time)) {
          if (schedule[q].pin == 1) switchpin(_PIN1, 1);
          if (schedule[q].pin == 2) switchpin(_PIN2, 1);
          if (schedule[q].pin == 3) switchpin(_PIN3, 1);
          if (schedule[q].pin == 4) switchpin(_PIN4, 1);
        } else {
          if (schedule[q].pin == 1) switchpin(_PIN1, 0);
          if (schedule[q].pin == 2) switchpin(_PIN2, 0);
          if (schedule[q].pin == 3) switchpin(_PIN3, 0);
          if (schedule[q].pin == 4) switchpin(_PIN4, 0);
        }
      }
      if (on_time > off_time) {
        if ((time_now >= off_time) && (time_now < on_time)) {
          if (schedule[q].pin == 1) switchpin(_PIN1, 0);
          if (schedule[q].pin == 2) switchpin(_PIN2, 0);
          if (schedule[q].pin == 3) switchpin(_PIN3, 0);
          if (schedule[q].pin == 4) switchpin(_PIN4, 0);
        } else {
          if (schedule[q].pin == 1) switchpin(_PIN1, 1);
          if (schedule[q].pin == 2) switchpin(_PIN2, 1);
          if (schedule[q].pin == 3) switchpin(_PIN3, 1);
          if (schedule[q].pin == 4) switchpin(_PIN4, 1);
        }
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
    }
    if (c == '2') {
      (digitalRead(_PIN2) == 0) ? (switchpin(_PIN2, 1)) : (switchpin(_PIN2, 0));
      mqttClient.publish(String(_HOSTNAME + "/set/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
      mqttClient.publish(String(_HOSTNAME + "/status/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
      Serial.print(F("PIN2 set to "));
      Serial.println(digitalRead(_PIN2));
    }
    if (c == '3') {
      (digitalRead(_PIN3) == 0) ? (switchpin(_PIN3, 1)) : (switchpin(_PIN3, 0));
      mqttClient.publish(String(_HOSTNAME + "/set/pin3").c_str(), 1, true, String(digitalRead(_PIN3)).c_str());
      mqttClient.publish(String(_HOSTNAME + "/status/pin3").c_str(), 1, true, String(digitalRead(_PIN3)).c_str());
      Serial.print(F("PIN3 set to "));
      Serial.println(digitalRead(_PIN3));
    }
    if (c == '4') {
      (digitalRead(_PIN4) == 0) ? (switchpin(_PIN4, 1)) : (switchpin(_PIN4, 0));
      mqttClient.publish(String(_HOSTNAME + "/set/pin4").c_str(), 1, true, String(digitalRead(_PIN4)).c_str());
      mqttClient.publish(String(_HOSTNAME + "/status/pin4").c_str(), 1, true, String(digitalRead(_PIN4)).c_str());
      Serial.print(F("PIN4 set to "));
      Serial.println(digitalRead(_PIN4));
    }
    if (c == '\r') {
      Serial.println();
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
          Serial.println("dBi]");
        }
      }
      if (code == "reset") {
        Serial.println(F("Reset command accepted. Please wait..."));
        digitalWrite(_PIN_LED, LOW);
        delay(1000);
        handle_deleteconfig();
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

