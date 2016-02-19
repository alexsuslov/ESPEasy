// @auth Alex Suslov <suslov@me.com>
// @auth Viktor

// return "name":"value" from name , value
String json_string(  String name, String value  ){
  return "\"" + name + "\"" + ":" + "\"" + value + "\"";
}

// return "name":value from name , value
String json_number(  String name, String value){
  return "\"" + name + "\"" + ":" +  value;
}


// return "name":{value} from name , value
String json_object(  String name, String value){
  return "\"" + name + "\"" + ":{" + value + "}";
}


// return "name":[value] from name , value
String json_array(  String name, String value){
  return "\"" + name + "\"" + ":[" + value + "]";
}

//********************************************************************************
// Web Interface Wifi scanner
// API [GET]
// @return [{ SSID:"ssid", RSSI:"rssi"},...]
//********************************************************************************
void handle_api_wifiscanner_json() {
  if (!isLoggedInApi()) return;

  // open json
  String reply = F("{");
  String Networks = "";
  String comma = reply;
  int n = WiFi.scanNetworks();
  if (n == 0){
    reply += json_string( F("error"), F("No Access Points found") );
  } else {
    for (int i = 0; i < n; ++i) {
      Networks += comma +  json_string( F("SSID"), WiFi.SSID(i) ) + ",";
      Networks += json_string( F("RSSI"), String(WiFi.RSSI(i) ) );
      Networks += "}";
      comma = F(",{");
    }
    reply += json_array( F("Networks"), Networks);
  }
  // close json
  reply += F("}\n");
  // send to client
  WebServer.send(200, F("application/json"), reply);

}



//********************************************************************************
// API [GET] config
// @return [json] config
// {
//    ControllerHostName: "mqtt.42do.ru"
//    ControllerPassword: "Adm1n1strat0r"
//    ControllerPort: "20000"
//    ControllerUser: "PassW0rd"
//    Controller_IP: [0, 0, 0, 0]
//    DNS: [0, 0, 0, 0]
//    Delay: "60"
//    IP: [0, 0, 0, 0]
//    Name: "NewTestDevice"
//    Protocol: "2"
//    Subnet: [0, 0, 0, 0]
//    Unit: "0"
//    WifiAPKey: "IoT"
//    WifiKey: "WeriSecurPass"
//    WifiSSID: "Trollstigen"
//    deepSleep: "0"
// }
//********************************************************************************
void handle_api_config_json() {
  if (!isLoggedInApi()) return;
  // open json
  String reply = F("{");

  // Settings
  reply += json_string( F("name"), Settings.Name) +
    ", " + json_string( F("usedns"), String(Settings.UseDNS) ) +
    ", " + json_string( F("unit"), String(Settings.Unit) ) +
    ", " + json_string( F("protocol"), String(Settings.Protocol)) +
    ", " + json_array( F("controllerip"),
      String(Settings.Controller_IP[0]) + "," +
      String(Settings.Controller_IP[1]) + "," +
      String(Settings.Controller_IP[2]) + "," +
      String(Settings.Controller_IP[3])
      ) +
    ", " + json_array( F("espip"),
      String(Settings.IP[0]) + "," +
      String(Settings.IP[1]) + "," +
      String(Settings.IP[2]) + "," +
      String(Settings.IP[3])
      ) +
    ", " + json_array( F("espsubnet"),
      String(Settings.Subnet[0]) + "," +
      String(Settings.Subnet[1]) + "," +
      String(Settings.Subnet[2]) + "," +
      String(Settings.Subnet[3])
      ) +
    ", " + json_array( F("espdns"),
      String(Settings.DNS[0]) + "," +
      String(Settings.DNS[1]) + "," +
      String(Settings.DNS[2]) + "," +
      String(Settings.DNS[3])
      ) +
    ", " + json_array( F("espgateway"),
      String(Settings.Gateway[0]) + "," +
      String(Settings.Gateway[1]) + "," +
      String(Settings.Gateway[2]) + "," +
      String(Settings.Gateway[3])
      ) +
    ", " + json_string( F("controllerhostname"), Settings.ControllerHostName) +
    ", " + json_string( F("delay"), String(Settings.Delay)) +
    ", " + json_string( F("deepsleep"), String(Settings.deepSleep)) +
    ", " + json_string( F("controllerport"), String( Settings.ControllerPort) )
  ;

  // SecuritySettings
  reply += ", " + json_string( F("ssid"), SecuritySettings.WifiSSID) +
    ", " + json_string( F("key"), SecuritySettings.WifiKey) +
    ", " + json_string( F("apkey"), SecuritySettings.WifiAPKey) +
    ", " + json_string( F("controllerpassword"), SecuritySettings.ControllerPassword) +
    ", " + json_string( F("controlleruser"), SecuritySettings.ControllerUser);

  // close json
  reply += F("}\n");

  // debug
  WebServer.send(200, F("application/json"), reply);
}


//********************************************************************************
// API [POST] config
// save config
// @return [json] config
//********************************************************************************
void handle_api_config_post() {
  post_config_save();
  handle_api_config_json();
}


//********************************************************************************
// api [GET] hardware json
// @return json
//********************************************************************************
void handle_api_hardware_json() {
  if (!isLoggedInApi()) return;

  // open json
  String reply = F("{");

  reply += json_string( F("Pin_i2c_sda"), String(Settings.Pin_i2c_sda) );
  reply += "," + json_string( F("Pin_i2c_scl"), String(Settings.Pin_i2c_scl) );
  for (byte x=0; x < 17; x++){
    reply += "," + json_string( "p" + String(x), String(Settings.PinStates[x]) );
  }

  // close json
  reply += F("}\n");

  // debug
  // Serial.println(reply);
  // send to client
  WebServer.send(200, F("application/json"), reply);
}

//********************************************************************************
// api [Post] hardware json
// @return json
//********************************************************************************
void handle_api_hardware_post() {
  hardware_save();
  handle_api_hardware_json();
}

//********************************************************************************
// api [GET] devices json
// @return json
//********************************************************************************
void handle_api_devices_json() {
  if (!isLoggedInApi()) return;

  char tmpString[41];
  struct EventStruct TempEvent;

  // open json
  String reply = F("[");
  String comma = "{";

  String deviceName;
  byte DeviceIndex = 0;

  for (byte x = 0; x < TASKS_MAX; x++)
  {
    LoadTaskSettings(x);
    DeviceIndex = getDeviceIndex(Settings.TaskDeviceNumber[x]);
    TempEvent.TaskIndex = x;

    if (ExtraTaskSettings.TaskDeviceValueNames[0][0] == 0)
      PluginCall(PLUGIN_GET_DEVICEVALUENAMES, &TempEvent, dummyString);

    deviceName = "";
    if (Settings.TaskDeviceNumber[x] != 0)
      Plugin_ptr[DeviceIndex](PLUGIN_GET_DEVICENAME, &TempEvent, deviceName);

    if ( deviceName.length() == 0 ) {
      continue;
    }

    reply += comma;
    reply += json_string( F("DeviceId"), String(x) );
    comma = ",";

    reply += comma;
    reply += json_string( F("DeviceName"), deviceName );

    reply += comma;
    reply += json_string( F("TaskDeviceName"), ExtraTaskSettings.TaskDeviceName );

    byte customConfig = false;
    String plgConfig = "";
    customConfig = PluginCall(PLUGIN_WEBFORM_SHOW_CONFIG, &TempEvent, plgConfig);
    if (plgConfig.length() > 0) {
        reply += comma;
        reply += json_string( F("PluginConfig"), plgConfig );
    }
    if (!customConfig)
      if (Device[DeviceIndex].Ports != 0) {
        reply += comma;
        reply += json_string( F("TaskDevicePort"), String(Settings.TaskDevicePort[x]) );
      }

    if (Settings.TaskDeviceID[x] != 0) {
      reply += comma;
      reply += json_string( F("TaskDeviceID"), String(Settings.TaskDeviceID[x]) );
    }

    /////////////// PINS START
    if (Settings.TaskDeviceDataFeed[x] == 0) {
      reply += comma;
      reply += F("\"DevicePins\":[");
      comma = "";
      if (Device[DeviceIndex].Type == DEVICE_TYPE_I2C)  {
        reply += comma;
        reply += Settings.Pin_i2c_sda;
        comma = ",";
        reply += comma;
        reply += Settings.Pin_i2c_scl;
      }
      if (Device[DeviceIndex].Type == DEVICE_TYPE_ANALOG) {
        reply += comma;
        reply += 1; // ADC always 1
        comma = ",";
      }
      if (Settings.TaskDevicePin1[x] != -1)  {
        String s = String(Settings.TaskDevicePin1[x]);
        if (s.length() > 0) {
          reply += comma;
          reply += s;
          comma = ",";
        }
      }
      if (Settings.TaskDevicePin2[x] != -1) {
        reply += comma;
        reply += Settings.TaskDevicePin2[x];
        comma = ",";
      }
      if (Settings.TaskDevicePin3[x] != -1) {
        reply += comma;
        reply += F("\"PIN\":");
        reply += Settings.TaskDevicePin3[x];
        comma = ",";
      }
      reply += "]";
    }
    /////////////// PINS END
    byte customValues = false;
    String plgValues = "";
    customValues = PluginCall(PLUGIN_WEBFORM_SHOW_VALUES, &TempEvent, plgValues);
    if (plgValues.length() > 0) {
        reply += comma;
        reply += json_string( F("PluginCustomValues"), plgValues );
    }
    if (!customValues)
    {
      reply += F(",\"Tasks\":[");
      for (byte varNr = 0; varNr < VARS_PER_TASK; varNr++)
      {
        if ((Settings.TaskDeviceNumber[x] != 0) and (varNr < Device[DeviceIndex].ValueCount))
        {
          if (varNr > 0) {
            reply += comma;
          }
          reply += F("{\"TaskDeviceValueName\":\"");
          reply += ExtraTaskSettings.TaskDeviceValueNames[varNr];
          reply += F("\"");
          reply += comma;
          reply += F("\"TaskDeviceValue\":\"");
          reply += UserVar[x * VARS_PER_TASK + varNr];
          reply += F("\"}");
        }
      }
      reply += "]";
    }
    reply += "}";
    comma = ",{";
  }

  // close json
  reply += F("]");
  // debug
   Serial.println(reply);
  // send to client
  WebServer.send(200, F("application/json"), reply);

}

/////////////////////////////////////////////////////////////
void handle_api_device_json() {
  if (!isLoggedInApi()) return;

  char tmpString[41];
  struct EventStruct TempEvent;

  // open json
  String reply = F("[");
  String comma = "{";
/*
  String deviceName;
  byte DeviceIndex = 0;

  String taskindex = WebServer.arg("index");


  if (ExtraTaskSettings.TaskDeviceValueNames[0][0] == 0)
      PluginCall(PLUGIN_GET_DEVICEVALUENAMES, &TempEvent, dummyString);


    reply += F("<TR><TD>Device:<TD>");
    addDeviceSelect(reply, "taskdevicenumber", Settings.TaskDeviceNumber[index - 1]);

    if (Settings.TaskDeviceNumber[index - 1] != 0 )
    {
      reply += F("<a class=\"button-link\" href=\"http://www.esp8266.nu/index.php/plugin");
      reply += Settings.TaskDeviceNumber[index - 1];
      reply += F("\" target=\"_blank\">?</a>");

      reply += F("<TR><TD>Name:<TD><input type='text' maxlength='25' name='taskdevicename' value='");
      reply += ExtraTaskSettings.TaskDeviceName;
      reply += F("'>");


      if (!Device[DeviceIndex].Custom)
      {
        reply += F("<TR><TD>IDX / Var:<TD><input type='text' name='taskdeviceid' value='");
        reply += Settings.TaskDeviceID[index - 1];
        reply += F("'>");
      }

      if (!Device[DeviceIndex].Custom && Settings.TaskDeviceDataFeed[index - 1] == 0)
      {
        if (Device[DeviceIndex].Ports != 0)
        {
          reply += F("<TR><TD>Port:<TD><input type='text' name='taskdeviceport' value='");
          reply += Settings.TaskDevicePort[index - 1];
          reply += F("'>");
        }

        if (Device[DeviceIndex].Type == DEVICE_TYPE_SINGLE || Device[DeviceIndex].Type == DEVICE_TYPE_DUAL)
        {
          reply += F("<TR><TD>1st GPIO:<TD>");
          addPinSelect(false, reply, "taskdevicepin1", Settings.TaskDevicePin1[index - 1]);
        }
        if (Device[DeviceIndex].Type == DEVICE_TYPE_DUAL)
        {
          reply += F("<TR><TD>2nd GPIO:<TD>");
          addPinSelect(false, reply, "taskdevicepin2", Settings.TaskDevicePin2[index - 1]);
        }

        if (Device[DeviceIndex].PullUpOption)
        {
          reply += F("<TR><TD>Pull UP:<TD>");
          if (Settings.TaskDevicePin1PullUp[index - 1])
            reply += F("<input type=checkbox name=taskdevicepin1pullup checked>");
          else
            reply += F("<input type=checkbox name=taskdevicepin1pullup>");
        }

        if (Device[DeviceIndex].InverseLogicOption)
        {
          reply += F("<TR><TD>Inversed:<TD>");
          if (Settings.TaskDevicePin1Inversed[index - 1])
            reply += F("<input type=checkbox name=taskdevicepin1inversed checked>");
          else
            reply += F("<input type=checkbox name=taskdevicepin1inversed>");
        }
      }

      PluginCall(PLUGIN_WEBFORM_LOAD, &TempEvent, reply);

      if (Device[DeviceIndex].SendDataOption)
      {
        reply += F("<TR><TD>Send Data:<TD>");
        if (Settings.TaskDeviceSendData[index - 1])
          reply += F("<input type=checkbox name=taskdevicesenddata checked>");
        else
          reply += F("<input type=checkbox name=taskdevicesenddata>");
      }

      if (Device[DeviceIndex].GlobalSyncOption && Settings.TaskDeviceDataFeed[index - 1] == 0 && Settings.UDPPort != 0)
      {
        reply += F("<TR><TD>Global Sync:<TD>");
        if (Settings.TaskDeviceGlobalSync[index - 1])
          reply += F("<input type=checkbox name=taskdeviceglobalsync checked>");
        else
          reply += F("<input type=checkbox name=taskdeviceglobalsync>");
      }

      if (!Device[DeviceIndex].Custom)
      {
        reply += F("<TR><TH>Optional Settings<TH>Value");

        if (Device[DeviceIndex].FormulaOption)
        {
          for (byte varNr = 0; varNr < Device[DeviceIndex].ValueCount; varNr++)
          {
            reply += F("<TR><TD>Formula ");
            reply += ExtraTaskSettings.TaskDeviceValueNames[varNr];
            reply += F(":<TD><input type='text' maxlength='25' name='taskdeviceformula");
            reply += varNr + 1;
            reply += F("' value='");
            reply += ExtraTaskSettings.TaskDeviceFormula[varNr];
            reply += F("'>");
            if (varNr == 0)
              reply += F("<a class=\"button-link\" href=\"http://www.esp8266.nu/index.php/EasyFormula\" target=\"_blank\">?</a>");
          }
        }

        for (byte varNr = 0; varNr < Device[DeviceIndex].ValueCount; varNr++)
        {
          reply += F("<TR><TD>Value Name ");
          reply += varNr + 1;
          reply += F(":<TD><input type='text' maxlength='25' name='taskdevicevaluename");
          reply += varNr + 1;
          reply += F("' value='");
          reply += ExtraTaskSettings.TaskDeviceValueNames[varNr];
          reply += F("'>");
        }
      }

    }

*/
  // close json
  reply += F("]");
  // debug
   Serial.println(reply);
  // send to client
  WebServer.send(200, F("application/json"), reply);

}