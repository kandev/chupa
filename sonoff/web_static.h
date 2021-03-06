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
  width: 150px;
}
select {
  background-color: #000;
  border: 1px solid #ffa600;
  border-radius: 4px;
  color: #fff;
  margin:0;
  margin-left:2px;
  width: 156px;
}
textarea {
  background-color: #000;
  border: 1px solid #ffa600;
  border-radius: 4px;
  color: #fff;
  margin:0;
  margin-left:2px;
  width: 156px;
}
input[type=submit] {
  background-color: #ffa600;
  border: 1px solid #000;
  border-radius: 4px;
  padding: 5px;
  padding-left: 30px;
  padding-right: 30px;
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
.button {
  color: #aaa;
  background-color: #333;
  border: 1px solid #aaa;
  border-radius: 4px;
  padding: 0px;
  padding-left: 20px;
  padding-right: 20px;
  font-weight: bold;
  margin: 5px;
  height: 20px;
  width: 110px;
}
.button:hover {
  border: 1px solid #fff;
}
.gpio_off {
  color: #aaa;
  background-color: #000;
  border: 1px solid #aaa;
  border-radius: 4px;
  padding: 4px;
  padding-left: 20px;
  padding-right: 20px;
  font-weight: bold;
  margin: 5px;
  width: 110px;
}
.gpio_on {
  color: #fff;
  background-color: #ff0000;
  border: 1px solid #000;
  border-radius: 4px;
  padding: 4px;
  padding-left: 20px;
  padding-right: 20px;
  font-weight: bold;
  margin: 5px;
  width: 110px;
}
.gpio_on:hover {
  border: 1px solid #fff;
}
.gpio_off:hover {
  border: 1px solid #fff;
}
.small_button {
  color: #aaa;
  background-color: #000;
  border: 1px solid #ffa600;
  border-radius: 3px;
  padding:1px;
  padding-left:4px;
  padding-right:4px;
  margin:1px;
}
.small_button:hover {
  color: #fff;
}
.sched {
  background-color: #000;
  border: 1px solid #ffa600;
  border-radius: 4px;
  color: #fff;
  margin: 2px;
  width:40px;
}
</style>

  </head><body>
  <h3>Configuration menu</h3>
  <div style="margin:10px; width: *; overflow:hidden;"><form action="/configure" method="post">
  <fieldset style="height:180px;">
  <legend>General</legend>
    <label for="hostname">Hostname:</label><br>
    <input type="text" name="hostname" id="hostname" value=""><br>
    <label for="dmin_password">Admin password:</label><br>
    <input type="text" name="admin_password" id="admin_password" value=""><br>
    <label for="ntp_server">NTP server:</label><br>
    <input type="text" name="ntp_server" id="ntp_server" value=""><br>
    <label for="timezone">Timezone [UTC=0]:</label><br>
    <input type="text" name="timezone" id="timezone" value="">
  </fieldset>
  <fieldset style="height:180px;">
  <legend>Wifi network</legend>
    <label for="ssid">SSID:</label><br>
    <input type="text" name="ssid" id="ssid" value="" style="width:110px;"><input type="button" id="scan" value="Scan" onclick="wifi_scan();" class="small_button"><br>
    <select id="wifis" size="4" onchange="document.getElementById('ssid').value=this.value;">
    </select><br>
    <label for="password">Password:</label><br>
    <input type="text" name="password" id="password" value="">
  </fieldset>
  <fieldset style="height:180px;">
  <legend>MQTT</legend>
    <label for="mqtt_server">MQTT server:</label><br>
    <input type="text" name="mqtt_server" id="mqtt_server" value=""><br>
    <label for="mqtt_serverport">MQTT port [no SSL]:</label><br>
    <input type="text" name="mqtt_serverport" id="mqtt_serverport" value=""><br>
    <label for="mqtt_username">MQTT username:</label><br>
    <input type="text" name="mqtt_username" id="mqtt_username" value=""><br>
    <label for="mqtt_key">MQTT password:</label><br>
    <input type="text" name="mqtt_key" id="mqtt_key" value=""></td></tr>
  </fieldset>

  <fieldset style="height:180px;">
  <legend>RFid tags</legend>
    <textarea name="rfids" id="rfids" rows="8" cols="19"></textarea><br>
    123456789:X:M (X - pin, Master)
  </fieldset>

  <fieldset style="height:180px;">
  <legend>Daily schedule</legend>
  <label><span style="width:40px;margin-left:2px;">ON [HH:MM]</span><span style="width:40px;margin-left:34px;">OFF [HH:MM]</span></label><br>
  <input type="number" min="0" max="23" name="sched1_h_on" id="sched1_h_on" class="sched" style="margin-right:-2px;">
  <input type="number" min="0" max="59" name="sched1_m_on" id="sched1_m_on" class="sched" style="margin-left:-2px;">
  <input type="number" min="0" max="23" name="sched1_h_off" id="sched1_h_off" class="sched" style="margin-right:-2px;">
  <input type="number" min="0" max="59" name="sched1_m_off" id="sched1_m_off" class="sched" style="margin-left:-2px;">
<br>
  <input type="number" min="0" max="23" name="sched2_h_on" id="sched2_h_on" class="sched" style="margin-right:-2px;">
  <input type="number" min="0" max="59" name="sched2_m_on" id="sched2_m_on" class="sched" style="margin-left:-2px;">
  <input type="number" min="0" max="23" name="sched2_h_off" id="sched2_h_off" class="sched" style="margin-right:-2px;">
  <input type="number" min="0" max="59" name="sched2_m_off" id="sched2_m_off" class="sched" style="margin-left:-2px;">
  </fieldset>

  <fieldset style="height:180px;">
  <legend>Controls</legend>
  <input type="button" class="gpio_off" name="pin1" id="pin1" value="Output 1" onclick="switch_pin(this.id);"><br>
  <input type="button" class="gpio_off" name="pin2" id="pin2" value="Output 2" onclick="switch_pin(this.id);"><br>
  <input type="button" class="button" name="btn_syslog" id="btn_syslog" value="Syslog" onclick="button_click(this.id);"><br>
  <input type="button" class="button" name="btn_syslog_delete" id="btn_syslog_delete" value="Erase log" onclick="button_click(this.id);"><br>
  <input type="button" class="button" name="btn_restart" id="btn_restart" value="Restart" onclick="button_click(this.id);"><br>
  </fieldset>

  <fieldset style="border: solid 1px #11ff00; height:180px;">
    <legend>Status</legend>
    Version: <span class="data" id="version"></span><br>
    MAC: <span class="data" id="mac"></span><br>
    RSSI: <span class="data" id="rssi"></span><span class="data"></span><br>
    Vcc: <span class="data" id="vcc"></span><span class="data"></span><br>
    Time and date: <span class="data" id="time"></span><br>
    Flash chip size: <span class="data" id="flash_size"></span><br>
    Temperature: <span class="data" id="temperature"></span><br>
    Humidity: <span class="data" id="humidity"></span><br>
    MQTT status: <span class="data" id="mqtt_status"></span><br>
  </fieldset>
  </div>
  <div style="text-align:center;padding:15px;">
  <input type="submit" value=" SAVE ">
  </div>
  </form>
  <div style="padding:2px;">
  <fieldset style="border: solid 1px #ff0000;">
  <legend>Important</legend>
  Please verify all the data before you click SAVE as there is no error correction. If you enter wrong data and the device disappears from your sight - hold GPIO0 to ground for 10 seconds (this will do factory reset).
  </fieldset>
  <fieldset style="border: solid 1px #11ff00;">
  <legend>MQTT details</legend>
  MQTT data is accessible at topic <span style="font-weight:bold;" id="h1"></span><b>/#</b><br>
  For managing output ports, publish like this: <span style="font-weight:bold;" id="h2"></span><b>/set/pin1=1</b> for switch ON, and 0 for switch OFF. Immediate status update will be published at topic  <span style="font-weight:bold;" id="h3"></span><b>/status/pin1=1</b>, if it's ON, or 0, if it's OFF.<br>
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
        if (data.mac!="") document.getElementById("mac").textContent = data.mac;
        if (data.rssi!="") document.getElementById("rssi").textContent = data.rssi + 'dBi';
        if (data.vc!="") document.getElementById("vcc").textContent = data.vcc + 'V';
        if (data.time!="") document.getElementById("time").textContent = data.time;
        if (data.flash_size!="") document.getElementById("flash_size").textContent = data.flash_size;
        if (data.hostname!="") document.getElementById("h1").textContent = data.hostname;
        if (data.hostname!="") document.getElementById("h2").textContent = data.hostname;
        if (data.hostname!="") document.getElementById("h3").textContent = data.hostname;
        if (data.hostname!="") document.title = data.hostname;
        if (data.temperature!="") document.getElementById("temperature").textContent = data.temperature + String.fromCharCode(176) + 'C';
        if (data.humidity!="") document.getElementById("humidity").textContent = data.humidity + '%';
        if (data.mqtt_status!="") document.getElementById("mqtt_status").textContent = data.mqtt_status;
        if (data.rfids!="") document.getElementById("rfids").textContent = data.rfids;
        var opts='';
        var array=[];
        for(var w in data.wifis){
          array.push({ssid:w,rssi:data.wifis[w]})
        }
        array.sort(function(a,b){return a.rssi - b.rssi});
        array.reverse();
        for(var i in array) {
          var col='#ff0000';
          if (array[i].rssi>-90) col='#ffaa00';
          if (array[i].rssi>-80) col='#ffff00';
          if (array[i].rssi>-65) col='#00ff00';
          opts+='<option value="' + array[i].ssid + '" style="color: ' + col + ';" ';
          if (array[i].ssid==data.ssid) opts+=' selected';
          opts+='>' + array[i].ssid + ' [' + array[i].rssi + '] ';
          if (array[i].ssid==data.ssid) opts+='<-';
          opts+='</option>';
        }
        document.getElementById("wifis").innerHTML=opts;
        if (data.pin1=="1") 
          document.getElementById("pin1").className = "gpio_on";
        else
          document.getElementById("pin1").className = "gpio_off";
        if (data.pin2=="1")
          document.getElementById("pin2").className = "gpio_on";
        else
          document.getElementById("pin2").className = "gpio_off";
          
        if (data.time_on_1!="") {
          var minutes = Math.floor(data.time_on_1 / 60);
          var h = Math.floor(minutes / 60);
          var m = minutes - h * 60;
          document.getElementById("sched1_h_on").value = h;
          document.getElementById("sched1_m_on").value = m;
        }
        if (data.time_off_1!="") {
          var minutes = Math.floor(data.time_off_1 / 60);
          var h = Math.floor(minutes / 60);
          var m = minutes - h * 60;
          document.getElementById("sched1_h_off").value = h;
          document.getElementById("sched1_m_off").value = m;
        }

        if (data.time_on_1!="") {
          var minutes = Math.floor(data.time_on_2 / 60);
          var h = Math.floor(minutes / 60);
          var m = minutes - h * 60;
          document.getElementById("sched2_h_on").value = h;
          document.getElementById("sched2_m_on").value = m;
        }
        if (data.time_off_1!="") {
          var minutes = Math.floor(data.time_off_2 / 60);
          var h = Math.floor(minutes / 60);
          var m = minutes - h * 60;
          document.getElementById("sched2_h_off").value = h;
          document.getElementById("sched2_m_off").value = m;
        }
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
        if (data.temperature!="") document.getElementById("temperature").textContent = data.temperature + String.fromCharCode(176) + 'C';
        if (data.humidity!="") document.getElementById("humidity").textContent = data.humidity + '%';
        if (data.mqtt_status!="") document.getElementById("mqtt_status").textContent = data.mqtt_status;
        if (data.pin1=="1") 
          document.getElementById("pin1").className = "gpio_on";
        else
          document.getElementById("pin1").className = "gpio_off";
        if (data.pin2=="1")
          document.getElementById("pin2").className = "gpio_on";
        else
          document.getElementById("pin2").className = "gpio_off";
      }
    };
    xhttp.open("GET", "/data", true);
    xhttp.send();
  }
  
  function wifi_scan() {
    document.getElementById("scan").style.backgroundColor="#444";
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var data = JSON.parse(this.responseText);
        if (data.ssid!="") document.getElementById("ssid").value = data.ssid;
        var opts='';
        var array=[];
        for(var w in data.wifis){
          array.push({ssid:w,rssi:data.wifis[w]})
        }
        array.sort(function(a,b){return a.rssi - b.rssi});
        array.reverse();
        for(var i in array) {
          var col='#ff0000';
          if (array[i].rssi>-90) col='#ffaa00';
          if (array[i].rssi>-85) col='#ffff00';
          if (array[i].rssi>-65) col='#00ff00';
          opts+='<option value="' + array[i].ssid + '" style="color: ' + col + ';" ';
          opts+='>' + array[i].ssid + ' [' + array[i].rssi + '] ';
          if (array[i].ssid==data.ssid) opts+='<-';
          opts+='</option>';
        }
        document.getElementById("wifis").innerHTML=opts;
        document.getElementById("scan").style.backgroundColor="";
      }
    };
    xhttp.open("GET", "/scan", true);
    xhttp.send();
  }
  updateData();
  wifi_scan();
  window.setInterval(updateDynamicData,10000);
  
  function switch_pin(pin) {
    document.getElementById(pin).style.backgroundColor="#444";
    var xhttp = new XMLHttpRequest();
    if (document.getElementById(pin).className=="gpio_on")
      state="0";
    else
      state="1";
    var params = pin + '=' + state;
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var data = JSON.parse(this.responseText);
        if (data.pin1=="1") 
          document.getElementById("pin1").className = "gpio_on";
        else
          document.getElementById("pin1").className = "gpio_off";
        if (data.pin2=="1")
          document.getElementById("pin2").className = "gpio_on";
        else
          document.getElementById("pin2").className = "gpio_off";
        if (data.time!="") 
          document.getElementById("time").textContent = data.time;
        document.getElementById(pin).style.backgroundColor="";
      }
    }
    xhttp.open("POST", "/gpio", true);
    xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xhttp.send(params);
  }

  function button_click(btn) {
    if (btn == 'btn_restart') {
      window.location.href = "/reboot";
    }
    if (btn == 'btn_syslog') {
      window.location.href = "/syslog";
    }
    if (btn == 'btn_syslog_delete') {
      window.location.href = "/syslog_delete";
    }
  }
  </script>
<div style="position: fixed; bottom: 0; width: 100%; text-align: right;">
&copy; <a class="cc" href="http://kandev.com" title="kandev.com" target="_blank">kandev.com</a>
</div>
  </body></html>
  )=====";

void get_data() {
  if ((!server.authenticate("admin", _ADMIN_PASS.c_str())) && (_CLIENT))
    server.requestAuthentication();
  if (mqttClient.connected()) {
    mqtt_status = "connected";
  } else {
    mqtt_status = "disconnected";
  }
  server.send ( 200, F("application/json"), "{ \
        \"hostname\":\"" + String(_HOSTNAME) + "\", \
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
        \"flash_size\":\"" + ESP.getFlashChipRealSize() / 1024 + "KB\", \
        \"mac\":\"" + getMacAddress() + "\", \
        \"vcc\":\"" + String(float(ESP.getVcc() / 1000.0)) + "\", \
        \"pin1\":\"" + digitalRead(_PIN1) + "\", \
        \"pin2\":\"" + digitalRead(_PIN2) + "\", \
        \"rfids\":\"" + _RFIDS + "\", \
        \"temperature\":\"n/a\", \
        \"humidity\":\"n/a\", \
        \"time_on_1\":\"" + schedule[0].time_on + "\", \
        \"time_off_1\":\"" + schedule[0].time_off + "\", \
        \"time_on_2\":\"" + schedule[1].time_on + "\", \
        \"time_off_2\":\"" + schedule[1].time_off + "\", \
        \"mqtt_status\":\"" + String(mqtt_status) + "\" \
                }" );
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
  const char* rfids = json["rfids"];
  schedule[0].time_on = json["time_on_1"];
  schedule[0].time_off = json["time_off_1"];
  schedule[1].time_on = json["time_on_2"];
  schedule[1].time_off = json["time_off_2"];

  if (String(hostname) != "")
    _HOSTNAME = hostname;
  _SSID = ssid;
  _PASS = pass;
  _MQTT_SERVER = mqtt_server;
  _MQTT_USERNAME = mqtt_user;
  _MQTT_KEY = mqtt_key;
  _ADMIN_PASS = admin_pass;
  //  _NTP_SERVER = ntp_server;
  //  _TIMEZONE = timezone;
  _RFIDS = rfids;

  if (_MQTT_SERVER.length() == 0) _MQTT_SERVER = "";
  if (_MQTT_USERNAME.length() == 0) _MQTT_USERNAME = "";
  if (_MQTT_KEY.length() == 0) _MQTT_KEY = "";
  if (_ADMIN_PASS.length() == 0) _ADMIN_PASS = "";
  if (_SSID.length() == 0) _SSID = "";
  if (_PASS.length() == 0) _PASS = "";
  //  if (_NTP_SERVER.length() == 0) _NTP_SERVER = "bg.pool.ntp.org";
  //  if (_TIMEZONE.length() == 0) _TIMEZONE = "2";
  if (_MQTT_SERVERPORT == 0) _MQTT_SERVERPORT = 1883;
  if (_RFIDS.length() == 0) _RFIDS = "";

  Serial.print(F("SSID: "));
  Serial.println(_SSID);
  closeFS();
  return true;
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
    //    if (server.argName(i) == "timezone") _TIMEZONE = server.arg(i).toInt();
    //    if (server.argName(i) == "ntp_server") _NTP_SERVER = server.arg(i);
    if (server.argName(i) == "rfids") _RFIDS = server.arg(i);
    if (server.argName(i) == "sched1_h_on") schedule[0].time_on = server.arg(i).toInt() * 3600;
    if (server.argName(i) == "sched1_m_on") schedule[0].time_on += server.arg(i).toInt() * 60;
    if (server.argName(i) == "sched1_h_off") schedule[0].time_off = server.arg(i).toInt() * 3600;
    if (server.argName(i) == "sched1_m_off") schedule[0].time_off += server.arg(i).toInt() * 60;
    if (server.argName(i) == "sched2_h_on") schedule[1].time_on = server.arg(i).toInt() * 3600;
    if (server.argName(i) == "sched2_m_on") schedule[1].time_on += server.arg(i).toInt() * 60;
    if (server.argName(i) == "sched2_h_off") schedule[1].time_off = server.arg(i).toInt() * 3600;
    if (server.argName(i) == "sched2_m_off") schedule[1].time_off += server.arg(i).toInt() * 60;
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
  json["rfids"] = _RFIDS;
  json["time_on_1"] = schedule[0].time_on;
  json["time_off_1"] = schedule[0].time_off;
  json["time_on_2"] = schedule[1].time_on;
  json["time_off_2"] = schedule[1].time_off;

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
  write_log(F("Configuraton saved."));
  if ((newssid) && (newpass)) ESP.restart();
}

void handle_showconfig() {
  if (!server.authenticate("admin", _ADMIN_PASS.c_str()))
    server.requestAuthentication();
  openFS();
  if (SPIFFS.exists(_CONFIG)) {
    File file = SPIFFS.open(_CONFIG, "r");
    size_t sent = server.streamFile(file, F("application/json"));
    file.close();
  } else {
    server.send(200, F("text/plain"), F("Config file not found."));
  }
  closeFS();
}
