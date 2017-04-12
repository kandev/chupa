#include <NtpClientLib.h>
#include <TimeLib.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "FS.h"
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266httpUpdate.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
ADC_MODE(ADC_VCC);  //read supply voltage by ESP.getVcc()

//Main configuration
const char* _VERSION = "0.13";
const char* _PRODUCT = "Chupa";
String _HOSTNAME = "";
const char* _UPDATE_SERVER = "chupa.kandev.com";
const byte _UPDATE_PORT = 80;
const char* _UPDATE_URL = "/";
const unsigned int _UPDATE_CHECK_INTERVAL = 300; // in seconds
int _DEEPSLEEP_INTERVAL = 900; // in seconds
const unsigned int _WATCHDOG_TIMEOUT = 600; // in seconds
String _MQTT_SERVER;
String _MQTT_SERVERPORT;
String _MQTT_USERNAME;
String _MQTT_KEY;
String _NTP_SERVER;
const unsigned int _WIFI_TIMEOUT = 300;
const unsigned int _MQTT_TIMEOUT = 600;
String _TIMEZONE;

const byte _PIN_RESET = 0;
const byte _PIN_LED = 2;
const byte _PIN1 = 4;
const byte _PIN2 = 5;
const byte _PIN3 = 12;
const byte _PIN4 = 13;

String _SSID;
String _PASS;
String _ADMIN_PASS = "";
bool _CLIENT = false; // dali e wifi client ili ap
const byte _CHANNEL = random(1,13);
const char* _CONFIG = "/config.json";
const char* update_path = "/update";
const char* update_username = "chupa";
const char* update_password = "apuhc";
IPAddress _IP(192, 168, 55, 1);
IPAddress _MASK(255, 255, 255, 0);
IPAddress _GATE(0, 0, 0, 0);
IPAddress timeServerIP;
unsigned long reset_hold = 0;
unsigned long switch_press = 0;
bool switch_press_done=false;
unsigned long blink_millis = 0;
unsigned int mqtt_drop_count = 0;
long last_update_check = 10000 - (_UPDATE_CHECK_INTERVAL * 1000);
volatile int watchdog_counter = 0;
unsigned long mqtt_lastReconnectAttempt = 0;
unsigned long wifi_lastReconnectAttempt = 0;
unsigned long mqtt_last=0;
Ticker secondTick;
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
AsyncMqttClient mqttClient;
DNSServer dnsServer;
unsigned int led_delay = 1;

const char PAGE_favicon[] PROGMEM = R"=====(
<svg version="1.0" id="Layer_1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px"
   viewBox="0 0 150 150" enable-background="new 0 0 150 150" xml:space="preserve">
<g>
  <path fill="#000000" d="M74.8,48.3c-8.6,6.2-17.5,12-26.1,18.2l-0.2,0C42.1,71,21.3,85.5,8,94.6l7.5,1.2c18.8-13,44.8-31,56.6-39.3
    c1.2-0.7,2.7-2.5,4.1-1c11.5,8,23.1,16,34.7,24c7.6,5.2,15.2,10.5,22.8,15.7l7.1-1.1l-33.6-23.7C96.3,63.2,85.7,55.6,74.8,48.3z"/>
  <path fill="#000000" d="M40.3,64.2c11.5-8,23.1-16.1,34.7-24c11.8,8.2,23.7,16.4,35.5,24.6l16.9-29.4l-39.3,12L73,9.3L57.9,47.5
    l-39.3-12l18,31.3c1.5-1,2.7-1.8,3.5-2.3L40.3,64.2z"/>
  <path fill="#000000" d="M75,62.7c-10.6,7.1-32.1,22.2-49.9,34.6l20.8,3.2l-3,41L73,113.5l30.2,27.9l-3-41l23.9-3.6
    c-6-4.2-12-8.4-18-12.5C95.8,77,85.3,70,75,62.7z M73.4,109.6c-4.1,0.1-8.1,0.1-12.2,0.1c0-4.1,0-8.1,0.1-12.2c4,0,8.1,0,12.1,0
    C73.4,101.6,73.4,105.6,73.4,109.6z M61.2,94.4c0-4,0-8,0-12.1c4.1,0,8.1,0,12.2,0c0,4,0,8,0,12.1C69.3,94.4,65.3,94.4,61.2,94.4z
     M88.7,82.3c0,4,0,8.1,0,12.1c-4,0-8.1,0-12.1,0c0-4.1,0-8.1,0-12.1C80.6,82.3,84.7,82.3,88.7,82.3z M76.6,109.7
    c0-4,0-8.1,0.1-12.1c4,0,8,0,12.1,0c0,4,0,8,0.1,12.1C84.7,109.8,80.6,109.8,76.6,109.7z"/>
</g>
</svg>
)=====";

const char PAGE_root[] PROGMEM = R"=====(
  <!DOCTYPE HTML><html>
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="/favicon.svg" type="image/svg+xml">
    <title></title>
<style>
body {
  background-color: #000;
  color: #ccc;
  font-family: Arial;
  font-size: 14px;
  padding: 0px;
  margin: 0px;
}

h3 {
  background-color: #444444;
  color: #ffa600;
  margin: 0px;
  padding: 8px;
} 
input[type=text] {
  background-color: #000;
  border: 1px solid #ffa600;
  border-radius: 4px;
  color: #fff;
  margin:2px;
}
input[type=submit] {
  background-color: #ffa600;
  border: 1px solid #000;
  border-radius: 4px;
  padding: 5px;
  padding-left: 20px;
  padding-right: 20px;
  font-weight: bold;
}
label {
  font-size: 0.8em;
}
fieldset{
  border: solid 1px #00c8ff;  
  float:left;
  font-size: 0.9em;
  margin:2px;
}
fieldset legend {
  color: #fff;
}
.cc {
  color: #ffa600;
  font-size: 0.75em;
  text-decoration: none;
}
.cc:hover {
  color: #ffa600;
  text-decoration: underline;
}
.data {
  color: #ffd500;
  font-weight: bold;
}
</style>

  </head><body>
  <h3>Конфигурационни параметри</h3>
  <div style="margin:10px; width: *; overflow:hidden;"><form action="/configure" method="post">
  <fieldset>
  <legend>Основни</legend>
    <label for="hostname">Уникално име:</label><br>
    <input type="text" name="hostname" id="hostname" value=""><br>
    <label for="dmin_password">Админ парола:</label><br>
    <input type="text" name="admin_password" id="admin_password" value=""><br>
    <label for="ntp_server">NTP сървър:</label><br>
    <input type="text" name="ntp_server" id="ntp_server" value=""><br>
    <label for="timezone">Часова зона [UTC=0]:</label><br>
    <input type="text" name="timezone" id="timezone" value="">
  </fieldset>
  <fieldset>
  <legend>Безжична мрежа</legend>
    <label for="ssid">SSID:</label><br>
    <input type="text" name="ssid" id="ssid" value="" list="wifis"><br>
    <datalist id="wifis">
    </datalist>
    <label for="password">Парола:</label><br>
    <input type="text" name="password" id="password" value="">
  </fieldset>
  <fieldset>
  <legend>MQTT</legend>
    <label for="mqtt_server">MQTT сървър:</label><br>
    <input type="text" name="mqtt_server" id="mqtt_server" value=""><br>
    <label for="mqtt_serverport">MQTT порт [без SSL]:</label><br>
    <input type="text" name="mqtt_serverport" id="mqtt_serverport" value=""><br>
    <label for="mqtt_username">MQTT потребител:</label><br>
    <input type="text" name="mqtt_username" id="mqtt_username" value=""><br>
    <label for="mqtt_key">MQTT парола:</label><br>
    <input type="text" name="mqtt_key" id="mqtt_key" value=""></td></tr>
  </fieldset>
  </div>
  <div style="text-align:center;padding:15px;">
  <input type="submit" value=" ЗАПИС ">
  </div>
  </form>
  <div style="padding:2px;">
  <fieldset style="border: solid 1px #ff0000;">
  <legend>Важно</legend>
  Моля, имайте предвид, че проверки за грешки на въведените данни не се правят! Ако въведете грешни данни може да се наложи да рестартирате устройството към заводски настройки чрез задържане на GPIO0 към маса за 10 секунди.
  </fieldset>
  <fieldset style="border: solid 1px #11ff00;">
  <legend>Статус</legend>
  Версия: <span class="data" id="version"></span><br>
  Ниво на wifi сигнал: <span class="data" id="rssi"></span><span class="data">dBi</span><br>
  Захранващо напрежение: <span class="data" id="vcc"></span><span class="data">V</span><br>
  Час и дата: <span class="data" id="time"></span><br>
  </fieldset>
  <fieldset style="border: solid 1px #11ff00;">
  <legend>MQTT информация</legend>
  MQTT данните са достъпни в тема <span id="h1"></span>/#<br>
  За управление на изходите ползвайте публикации във формат: <span id="h2"></span>/set/pin1=1 за включване, и съответно 0 за изключване. Веднага след манипулация устройството ще върне статус на съответния изход във формат <span id="h3"></span>/status/pin1=1, ако е включен, или 0, ако е изключен.<br>
  </fieldset>
</div>
  <script type="text/javascript">
  function updateData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var data = JSON.parse(this.responseText);
        if (data.hostname!="") document.getElementById("hostname").value = data.hostname;
        if (data.admin_password!="") document.getElementById("admin_password").value = data.admin_password;
        if (data.ntp_server!="") document.getElementById("ntp_server").value = data.ntp_server;
        if (data.timezone!="") document.getElementById("timezone").value = data.timezone;
        if (data.ssid!="") document.getElementById("ssid").value = data.ssid;
        if (data.password!="") document.getElementById("password").value = data.password;
        if (data.mqtt_server!="") document.getElementById("mqtt_server").value = data.mqtt_server;
        if (data.mqtt_serverport!="") document.getElementById("mqtt_serverport").value = data.mqtt_serverport;
        if (data.mqtt_username!="") document.getElementById("mqtt_username").value = data.mqtt_username;
        if (data.mqtt_key!="") document.getElementById("mqtt_key").value = data.mqtt_key;
        if (data.version!="") document.getElementById("version").textContent = data.version;
        if (data.rssi!="") document.getElementById("rssi").textContent = data.rssi;
        if (data.vc!="") document.getElementById("vcc").textContent = data.vcc;
        if (data.time!="") document.getElementById("time").textContent = data.time;
        if (data.hostname!="") document.getElementById("h1").textContent = data.hostname;
        if (data.hostname!="") document.getElementById("h2").textContent = data.hostname;
        if (data.hostname!="") document.getElementById("h3").textContent = data.hostname;
        if (data.hostname!="") document.title = data.hostname;
        var opts='';
        for(var w in data.wifis)
          opts+='<option value="' + w + '">' + w + ' [' + data.wifis[w] + '] </option>';
        document.getElementById("wifis").innerHTML=opts;
      }
    };
    xhttp.open("GET", "/data", true);
    xhttp.send();
  }
  function updateDynamicData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var data = JSON.parse(this.responseText);
        if (data.rssi!="") document.getElementById("rssi").textContent = data.rssi;
        if (data.vc!="") document.getElementById("vcc").textContent = data.vcc;
        if (data.time!="") document.getElementById("time").textContent = data.time;
      }
    };
    xhttp.open("GET", "/data", true);
    xhttp.send();
  }
  updateData();
  window.setInterval(updateDynamicData,10000);
  </script>
<div style="position: fixed; bottom: 0; width: 100%; text-align: right;">
&copy; <a class="cc" href="http://kandev.com" title="kandev.com" target="_blank">kandev.com</a>
</div>
  </body></html>
  )=====";

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
    Serial.println(F("[ERR] Няма намерена конфигурация."));
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println(F("[ERR] Конфигурационния файл е твърде голям."));
    return false;
  }
  
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println(F("[ERR] Не може да бъде разпознат конфигурационния файл."));
    return false;
  }
  const char* ssid = json["ssid"];
  const char* pass = json["pass"];
  const char* hostname = json["hostname"];
  const char* mqtt_server = json["mqtt_server"];
  const char* mqtt_serverport = json["mqtt_serverport"];
  const char* mqtt_user = json["mqtt_username"];
  const char* mqtt_key = json["mqtt_key"];
  const char* admin_pass = json["admin_password"];
  const char* ntp_server = json["ntp_server"];
  const char* timezone = json["timezone"];

  if (String(hostname) != "")
    _HOSTNAME = hostname;
  _SSID = ssid;
  _PASS = pass;
  _MQTT_SERVER = mqtt_server;
  _MQTT_SERVERPORT = mqtt_serverport;
  _MQTT_USERNAME = mqtt_user;
  _MQTT_KEY = mqtt_key;
  _ADMIN_PASS = admin_pass;
  _NTP_SERVER = ntp_server;
  _TIMEZONE = timezone;

  if (_MQTT_SERVER.length() == 0) _MQTT_SERVER = "";
  if (_MQTT_SERVERPORT.length() == 0) _MQTT_SERVERPORT = "";
  if (_MQTT_USERNAME.length() == 0) _MQTT_USERNAME = "";
  if (_MQTT_KEY.length() == 0) _MQTT_KEY = "";
  if (_ADMIN_PASS.length() == 0) _ADMIN_PASS = "";
  if (_SSID.length() == 0) _SSID = "";
  if (_PASS.length() == 0) _PASS = "";
  if (_NTP_SERVER.length() == 0) _NTP_SERVER="time.nist.gov";
  if (_TIMEZONE.length() == 0) _TIMEZONE="+3";

  Serial.print(F("SSID: "));
  Serial.println(_SSID);
  closeFS();
  return true;
}

void get_data() {
  if ((!server.authenticate("admin", _ADMIN_PASS.c_str())) && (_CLIENT))
    server.requestAuthentication();
    int w = WiFi.scanNetworks();
    String wifis="{";
    byte i;
    for (i=0;i<w;i++) {
      wifis+="\"" + WiFi.SSID(i) + "\":\"" + WiFi.RSSI(i) + "\"";
      if (i<w-1) wifis+=", ";
    }
    wifis+="}";
    server.send ( 200, F("application/json"), \
      "{\"hostname\":\"" + String(_HOSTNAME) + "\", \
        \"ssid\":\"" + String(_SSID) + "\", \
        \"ntp_server\":\"" + _NTP_SERVER + "\", \
        \"timezone\":\"" + _TIMEZONE + "\", \
        \"password\":\"" + String(_PASS) + "\", \
        \"mqtt_server\":\"" + _MQTT_SERVER + "\", \
        \"mqtt_serverport\":\"" + _MQTT_SERVERPORT + "\", \
        \"mqtt_username\":\"" + _MQTT_USERNAME + "\", \
        \"mqtt_key\":\"" + _MQTT_KEY + "\", \
        \"admin_password\":\"" + _ADMIN_PASS + "\", \
        \"version\":\"" + String(_VERSION) + "\", \
        \"rssi\":\"" + String(WiFi.RSSI()) + "\", \
        \"time\":\"" + NTP.getTimeDateString() + "\", \
        \"vcc\":\"" + String(float(ESP.getVcc() / 1000.0)) + "\", \
        \"wifis\":" + wifis + \ 
        "}" );
}

void html_root(){
  server.send(200, F("text/html"), PAGE_root);
}

void html_favicon(){
  server.send(200, F("image/svg+xml"), PAGE_favicon);
}

void handle_configure() {
  if ((!server.authenticate("admin", _ADMIN_PASS.c_str())) && (_CLIENT))
    server.requestAuthentication();
  for (int i = 0; i < server.args(); i++) {
    if (server.argName(i) == "ssid") _SSID = server.arg(i);
    if (server.argName(i) == "password") _PASS = server.arg(i);
    if (server.argName(i) == "hostname") _HOSTNAME = server.arg(i);
    if (server.argName(i) == "mqtt_server") _MQTT_SERVER = server.arg(i);
    if (server.argName(i) == "mqtt_serverport") _MQTT_SERVERPORT = server.arg(i);
    if (server.argName(i) == "mqtt_username") _MQTT_USERNAME = server.arg(i);
    if (server.argName(i) == "mqtt_key") _MQTT_KEY = server.arg(i);
    if (server.argName(i) == "admin_password") _ADMIN_PASS = server.arg(i);
    if (server.argName(i) == "timezone") _TIMEZONE = server.arg(i).toInt();
    if (server.argName(i) == "ntp_server") _NTP_SERVER = server.arg(i);
  }
  StaticJsonBuffer<500> jsonBuffer;
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
  openFS();
  File configFile = SPIFFS.open(_CONFIG, "w");
  int error = 0;
  if (!configFile) {
    Serial.println(F("[ERR] Конфигурацията не може да бъде съхранена."));
    error = 1;
  }
  json.printTo(configFile);
  closeFS();
  if (error == 0) {
    server.send(200, F("text/plain"), F("[OK]"));
  } else {
    server.send(200, F("text/plain"), F("[ERR]"));
  }
  ESP.restart();
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
    Serial.println(F("[ERR] Файловата система не може да бъде форматирана."));
  else
    Serial.println(F("[OK] Файловата система е форматирана успешно."));
  Serial.println(F("Рестарт..."));
  ESP.restart();
}

void checkforupdate() {
  float vcc = ESP.getVcc() / 1000.0; // supply voltage
  Serial.print(F("Проверка за актуализация... "));
  auto ret = ESPhttpUpdate.update(_UPDATE_SERVER, _UPDATE_PORT, _UPDATE_URL, _VERSION);
  Serial.println(String(ret));
  Serial.println(NTP.getTimeDateString());
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
  server.send(200, F("text/plain"), F("Проверка за актуализация..."));
}

void switchpin(int pin, int state) {
  digitalWrite(pin, state);
}

void onMqttConnect(bool sessionPresent) {
  String subs = _HOSTNAME + "/set/#";
  mqttClient.subscribe(subs.c_str(), 2);
  Serial.print(String("Subscribing to: [" + subs + "]... "));
}
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  unsigned long currentMillis = millis();
  if ((currentMillis - mqtt_lastReconnectAttempt) / 1000 >= 30) {
    IPAddress mqttIP;
    WiFi.hostByName(_MQTT_SERVER.c_str(), mqttIP);
    Serial.println(String("Опит за връзка с MQTT брокер "+_MQTT_SERVER+", ИП адрес: "+mqttIP.toString()));
    mqttClient.setServer(mqttIP, _MQTT_SERVERPORT.toInt());
    mqtt_lastReconnectAttempt = currentMillis;
    mqttClient.connect();
  }
}
void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println(F("готово!"));
}
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  String msg = String(topic);
  msg = msg.substring(msg.indexOf('/set/'));
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
    led_delay = 500;  //if in mode AP blink twice in a second
    Serial.printf("SSID: %s\r\n", _SSID.c_str());
    if (_PASS!="") Serial.printf("Парола: %s\r\n", _PASS.c_str());
    WiFi.mode(WIFI_AP);
    WiFi.softAP(_SSID.c_str(), _PASS.c_str(), _CHANNEL);
    WiFi.softAPConfig(_IP, _GATE, _MASK);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print(F("За да конфигурирате, моля закачете се за безжичната мрежа и отворете http://"));
    Serial.println(myIP.toString().c_str());
  } else {
    led_delay = 10000;  //if client - blink once every 10 seconds
    Serial.print(F("Опит за връзка с "));
    Serial.println(_SSID.c_str());
    WiFi.mode(WIFI_STA);
    WiFi.begin(_SSID.c_str(), _PASS.c_str());
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED) {
      delay(200);
      Serial.print(".");
      digitalWrite(_PIN_LED, LOW);
      delay(20);
      digitalWrite(_PIN_LED, HIGH);
      yield();
      if ((millis() - start) / 1000 >= _WIFI_TIMEOUT) {
        Serial.println(F("[ERR] Май няма смисъл. Рестарт!"));
        ESP.restart();
      }
    }
    Serial.println("OK");
  }
}

void setup()
{
  //watchdog init
  secondTick.attach(1, watchdog);
  //onboard led
  pinMode(_PIN1, OUTPUT); digitalWrite(_PIN1, LOW);
  pinMode(_PIN2, OUTPUT); digitalWrite(_PIN2, LOW);
  pinMode(_PIN3, OUTPUT); digitalWrite(_PIN3, LOW);
  pinMode(_PIN4, OUTPUT); digitalWrite(_PIN4, LOW);
  pinMode(_PIN_LED, OUTPUT); digitalWrite(_PIN_LED, LOW);
  pinMode(_PIN_RESET, INPUT_PULLUP);  //factory reset

  Serial.begin(115200);Serial.println();
  Serial.print(_PRODUCT);
  Serial.print(F(", "));
  Serial.println(_VERSION);
  _CLIENT = loadConfig();
  wifi_connect();
  server.on("/configure", handle_configure);
  server.on("/config.json", handle_showconfig);
  server.on("/check", checkforupdate);
  server.on("/data", get_data);
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
    mqttClient.setServer(mqttIP, _MQTT_SERVERPORT.toInt());
    mqttClient.setKeepAlive(5).setCleanSession(false).setWill(String(_HOSTNAME + "/status/online").c_str(), 2, true, "0").setCredentials(_MQTT_USERNAME.c_str(), _MQTT_KEY.c_str()).setClientId(_HOSTNAME.c_str());
    Serial.print(F("Connecting to MQTT broker "));
    Serial.print(_MQTT_SERVER);
    Serial.print(F(", resolved to "));
    Serial.println(mqttIP.toString());
    mqttClient.connect();
    if (NTP.begin(String(_NTP_SERVER), _TIMEZONE.toInt(), true))
      Serial.println(F("[OK] NTP..."));
    else
      Serial.println(F("[ERR] Инициализацията на NTP клиента не се получи."));
    NTP.setInterval(3600);  //ntp sync once per hour
  } else {
    dnsServer.setTTL(300);
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    dnsServer.start(53, "*", _IP);
  }
}


void loop() {
  unsigned long currentMillis = millis();
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
    Serial.println(F("!"));
    if ((millis() - switch_press >= 100)and (!switch_press_done)) {
      (digitalRead(_PIN1)==0)?(switchpin(_PIN1,1)):(switchpin(_PIN1,0));
      mqttClient.publish(String(_HOSTNAME + "/status/pin1").c_str(), 1, true, String(digitalRead(_PIN1)).c_str());
      switch_press_done=true;
    }
  } else {
    reset_hold = 0;
    switch_press_done=false;
  }

  //handle blinking frequency
  if ((currentMillis - blink_millis) >= led_delay) {
    digitalWrite(_PIN_LED, LOW);
    blink_millis = currentMillis;
  }
  if (currentMillis-blink_millis>1) {
    digitalWrite(_PIN_LED, HIGH);
    blink_millis = currentMillis;
  }    

  if (_CLIENT) {
    //handle disconnect event
    if (WiFi.status() == WL_DISCONNECTED) {
      Serial.println(F("[ERR] Wifi disconnected!"));
      WiFi.disconnect();
      wifi_connect();
    }

    //update check
    if ((currentMillis - last_update_check) / 1000 >= _UPDATE_CHECK_INTERVAL) {
      last_update_check = currentMillis;
      checkforupdate();
    }
  } else {
    dnsServer.processNextRequest();
  }

  if (Serial.available()) {
    char c = Serial.read();
    if (c=='1') {
      (digitalRead(_PIN1)==0)?(switchpin(_PIN1,1)):(switchpin(_PIN1,0));
      mqttClient.publish(String(_HOSTNAME + "/status/pin1").c_str(), 1, true, String(digitalRead(_PIN1)).c_str());
    }
    if (c=='2') {
      (digitalRead(_PIN2)==0)?(switchpin(_PIN2,1)):(switchpin(_PIN2,0));
      mqttClient.publish(String(_HOSTNAME + "/status/pin2").c_str(), 1, true, String(digitalRead(_PIN2)).c_str());
    }
    if (c=='3') {
      (digitalRead(_PIN3)==0)?(switchpin(_PIN3,1)):(switchpin(_PIN3,0));
      mqttClient.publish(String(_HOSTNAME + "/status/pin3").c_str(), 1, true, String(digitalRead(_PIN3)).c_str());
    }
    if (c=='4') {
      (digitalRead(_PIN4)==0)?(switchpin(_PIN4,1)):(switchpin(_PIN4,0));
      mqttClient.publish(String(_HOSTNAME + "/status/pin4").c_str(), 1, true, String(digitalRead(_PIN4)).c_str());
    }
    if (c=='w') {
      int w = WiFi.scanNetworks();
      if (w == 0)
        Serial.println("no wifi networks found");
      byte i;
      for (i=0;i<w;i++) {
        Serial.print(WiFi.SSID(i));
        Serial.print(F(" ["));
        Serial.print(WiFi.RSSI(i));
        Serial.println("dBi]");
      }
    }
  }
  watchdog_counter = 0;
}
