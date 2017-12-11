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
}
.gpio_on {
  color: #000;
  background-color: #aaa;
  border: 1px solid #000;
  border-radius: 4px;
  padding: 4px;
  padding-left: 20px;
  padding-right: 20px;
  font-weight: bold;
  margin: 5px;
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

  <fieldset style="height:260px;">
  <legend>SMTP</legend>
    <label for="smtp_server">Server:</label><br>
    <input type="text" name="smtp_server" id="smtp_server" value=""><br>
    <label for="smtp_serverport">Port:</label><br>
    <input type="text" name="smtp_serverport" id="smtp_serverport" value=""><br>
    <label for="smtp_username">Username:</label><br>
    <input type="text" name="smtp_username" id="smtp_username" value=""><br>
    <label for="smtp_password">Password:</label><br>
    <input type="text" name="smtp_password" id="smtp_password" value=""><br>
    <label for="smtp_from">Mail From:</label><br>
    <input type="text" name="smtp_from" id="smtp_from" value=""><br>
    <label for="smtp_to">Mail To:</label><br>
    <input type="text" name="smtp_to" id="smtp_to" value=""></td></tr>
  </fieldset>

  <fieldset style="height:180px;">
  <legend>Daily schedule</legend>
  <label><span style="width:40px;margin-left:2px;">ON [HH:MM]</span><span style="width:40px;margin-left:32px;">OFF [HH:MM]</span><span style="width:40px;margin-left:26px;">Output</span></label><br>

  <input type="number" min="0" max="23" name="sched1_h_on" id="sched1_h_on" class="sched" style="margin-right:-2px;">
  <input type="number" min="0" max="59" name="sched1_m_on" id="sched1_m_on" class="sched" style="margin-left:-2px;">
  <input type="number" min="0" max="23" name="sched1_h_off" id="sched1_h_off" class="sched" style="margin-right:-2px;">
  <input type="number" min="0" max="59" name="sched1_m_off" id="sched1_m_off" class="sched" style="margin-left:-2px;">
  <select name="sched1_pin" id="sched1_pin" class="sched">
  <option value="0">off</option><option value="1">1</option><option value="2">2</option><option value="3">3</option><option value="4">4</option>
  </select>
<br>
  <input type="number" min="0" max="23" name="sched2_h_on" id="sched2_h_on" class="sched" style="margin-right:-2px;">
  <input type="number" min="0" max="59" name="sched2_m_on" id="sched2_m_on" class="sched" style="margin-left:-2px;">
  <input type="number" min="0" max="23" name="sched2_h_off" id="sched2_h_off" class="sched" style="margin-right:-2px;">
  <input type="number" min="0" max="59" name="sched2_m_off" id="sched2_m_off" class="sched" style="margin-left:-2px;">
  <select name="sched2_pin" id="sched2_pin" class="sched">
  <option value="0">off</option><option value="1">1</option><option value="2">2</option><option value="3">3</option><option value="4">4</option>
  </select>
<br>
  <input type="number" min="0" max="23" name="sched3_h_on" id="sched3_h_on" class="sched" style="margin-right:-2px;">
  <input type="number" min="0" max="59" name="sched3_m_on" id="sched3_m_on" class="sched" style="margin-left:-2px;">
  <input type="number" min="0" max="23" name="sched3_h_off" id="sched3_h_off" class="sched" style="margin-right:-2px;">
  <input type="number" min="0" max="59" name="sched3_m_off" id="sched3_m_off" class="sched" style="margin-left:-2px;">
  <select name="sched3_pin" id="sched3_pin" class="sched">
  <option value="0">off</option><option value="1">1</option><option value="2">2</option><option value="3">3</option><option value="4">4</option>
  </select>
<br>
  <input type="number" min="0" max="23"  name="sched4_h_on" id="sched4_h_on" class="sched" style="margin-right:-2px;">
  <input type="number" min="0" max="59" name="sched4_m_on" id="sched4_m_on" class="sched" style="margin-left:-2px;">
  <input type="number" min="0" max="23" name="sched4_h_off" id="sched4_h_off" class="sched" style="margin-right:-2px;">
  <input type="number" min="0" max="59" name="sched4_m_off" id="sched4_m_off" class="sched" style="margin-left:-2px;">
  <select name="sched4_pin" id="sched4_pin" class="sched">
  <option value="0">off</option><option value="1">1</option><option value="2">2</option><option value="3">3</option><option value="4">4</option>
  </select>
<br>
  <input type="number" min="0" max="23" name="sched5_h_on" id="sched5_h_on" class="sched" style="margin-right:-2px;">
  <input type="number" min="0" max="59" name="sched5_m_on" id="sched5_m_on" class="sched" style="margin-left:-2px;">
  <input type="number" min="0" max="23" name="sched5_h_off" id="sched5_h_off" class="sched" style="margin-right:-2px;">
  <input type="number" min="0" max="59" name="sched5_m_off" id="sched5_m_off" class="sched" style="margin-left:-2px;">
  <select name="sched5_pin" id="sched5_pin" class="sched">
  <option value="0">off</option><option value="1">1</option><option value="2">2</option><option value="3">3</option><option value="4">4</option>
  </select>

  </fieldset>
  <fieldset style="height:180px;">
  <legend>Output Control</legend>
  <input type="button" class="gpio_off" name="pin1" id="pin1" value="Output 1" onclick="switch_pin(this.id);"><br>
  <input type="button" class="gpio_off" name="pin2" id="pin2" value="Output 2" onclick="switch_pin(this.id);"><br>
  <input type="button" class="gpio_off" name="pin3" id="pin3" value="Output 3" onclick="switch_pin(this.id);"><br>
  <input type="button" class="gpio_off" name="pin4" id="pin4" value="Output 4" onclick="switch_pin(this.id);"><br>
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

        if (data.smtp_server!="") document.getElementById("smtp_server").value = data.smtp_server;
        if (data.smtp_serverport!="") document.getElementById("smtp_serverport").value = data.smtp_serverport;
        if (data.smtp_username!="") document.getElementById("smtp_username").value = data.smtp_username;
        if (data.smtp_password!="") document.getElementById("smtp_password").value = data.smtp_password;
        if (data.smtp_from!="") document.getElementById("smtp_from").value = data.smtp_from;
        if (data.smtp_to!="") document.getElementById("smtp_to").value = data.smtp_to;

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
        if (data.pin3=="1")
          document.getElementById("pin3").className = "gpio_on";
        else
          document.getElementById("pin3").className = "gpio_off";
        if (data.pin4=="1")
          document.getElementById("pin4").className = "gpio_on";
        else
          document.getElementById("pin4").className = "gpio_off";
        if (data.sched1_h_on!="") document.getElementById("sched1_h_on").value = data.sched1_h_on;
        if (data.sched1_m_on!="") document.getElementById("sched1_m_on").value = data.sched1_m_on;
        if (data.sched1_h_off!="") document.getElementById("sched1_h_off").value = data.sched1_h_off;
        if (data.sched1_m_off!="") document.getElementById("sched1_m_off").value = data.sched1_m_off;
        if (data.sched1_pin!="") document.getElementById("sched1_pin").value = data.sched1_pin;

        if (data.sched2_h_on!="") document.getElementById("sched2_h_on").value = data.sched2_h_on;
        if (data.sched2_m_on!="") document.getElementById("sched2_m_on").value = data.sched2_m_on;
        if (data.sched2_h_off!="") document.getElementById("sched2_h_off").value = data.sched2_h_off;
        if (data.sched2_m_off!="") document.getElementById("sched2_m_off").value = data.sched2_m_off;
        if (data.sched2_pin!="") document.getElementById("sched2_pin").value = data.sched2_pin;

        if (data.sched3_h_on!="") document.getElementById("sched3_h_on").value = data.sched3_h_on;
        if (data.sched3_m_on!="") document.getElementById("sched3_m_on").value = data.sched3_m_on;
        if (data.sched3_h_off!="") document.getElementById("sched3_h_off").value = data.sched3_h_off;
        if (data.sched3_m_off!="") document.getElementById("sched3_m_off").value = data.sched3_m_off;
        if (data.sched3_pin!="") document.getElementById("sched3_pin").value = data.sched3_pin;

        if (data.sched4_h_on!="") document.getElementById("sched4_h_on").value = data.sched4_h_on;
        if (data.sched4_m_on!="") document.getElementById("sched4_m_on").value = data.sched4_m_on;
        if (data.sched4_h_off!="") document.getElementById("sched4_h_off").value = data.sched4_h_off;
        if (data.sched4_m_off!="") document.getElementById("sched4_m_off").value = data.sched4_m_off;
        if (data.sched4_pin!="") document.getElementById("sched4_pin").value = data.sched4_pin;

        if (data.sched5_h_on!="") document.getElementById("sched5_h_on").value = data.sched5_h_on;
        if (data.sched5_m_on!="") document.getElementById("sched5_m_on").value = data.sched5_m_on;
        if (data.sched5_h_off!="") document.getElementById("sched5_h_off").value = data.sched5_h_off;
        if (data.sched5_m_off!="") document.getElementById("sched5_m_off").value = data.sched5_m_off;
        if (data.sched5_pin!="") document.getElementById("sched5_pin").value = data.sched5_pin;
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
        if (data.pin3=="1")
          document.getElementById("pin3").className = "gpio_on";
        else
          document.getElementById("pin3").className = "gpio_off";
        if (data.pin4=="1")
          document.getElementById("pin4").className = "gpio_on";
        else
          document.getElementById("pin4").className = "gpio_off";
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
        if (data.pin3=="1")
          document.getElementById("pin3").className = "gpio_on";
        else
          document.getElementById("pin3").className = "gpio_off";
        if (data.pin4=="1")
          document.getElementById("pin4").className = "gpio_on";
        else
          document.getElementById("pin4").className = "gpio_off";
        if (data.time!="") 
          document.getElementById("time").textContent = data.time;
        document.getElementById(pin).style.backgroundColor="";
      }
    }
    xhttp.open("POST", "/gpio", true);
    xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xhttp.send(params);
  }
  </script>
<div style="position: fixed; bottom: 0; width: 100%; text-align: right;">
&copy; <a class="cc" href="http://kandev.com" title="kandev.com" target="_blank">kandev.com</a>
</div>
  </body></html>
  )=====";

