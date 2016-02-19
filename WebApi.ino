// @auth Alex Suslov <suslov@me.com>
// @auth Victor Brutskiy <4refr0nt@gmail.com>

// return "name":"value" from name, value
String json_string(  String name, String value  ) {
  return "\"" + name + "\"" + ":" + "\"" + value + "\"";
}

// return "name":value from name, value
String json_number(  String name, String value) {
  return "\"" + name + "\"" + ":" +  value;
}
// return "name":value from name, value
String json_number(  String name, int value) {
  return "\"" + name + "\"" + ":" +  value;
}

// return "name":{value} from name, value
String json_object(  String name, String value) {
  return "\"" + name + "\"" + ":{" + value + "}";
}

// return "name":[values] from name, values
String json_array(  String name, String values)  {
  return "\"" + name + "\"" + ":[" + values + "]";
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
  if (n == 0) {
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

// const char option_response[] PROGMEM = "GET,HEAD,POST\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept\r\nAccess-Control-Allow-Methods: GET, POST\r\n";

const char option_response[] PROGMEM = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: POST, GET, OPTIONS\r\nAccess-Control-Allow-Headers: Content-Type, X-Requested-With\r\nAccess-Control-Max-Age: 2048\r\nVary: Accept-Encoding, Origin\r\nContent-Encoding: gzip\r\nContent-Length: 0\r\nKeep-Alive: timeout=2, max=100\r\nConnection: Keep-Alive\r\nContent-Type: text/plain\r\n";

void handle_api_config_options() {
  WebServer.sendContent_P(option_response);
}


//********************************************************************************
// api [GET] hardware json
// @return json
//********************************************************************************
void handle_api_hardware_json() {
  if (!isLoggedInApi()) return;

  // open json
  String reply = F("{");

  reply += json_string( F("psda"), String(Settings.Pin_i2c_sda) );
  reply += "," + json_string( F("pscl"), String(Settings.Pin_i2c_scl) );
  for (byte x = 0; x < 17; x++) {
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

  // open json
  String reply = F("{");
  struct EventStruct TempEvent;
  String taskindex = WebServer.arg("index");

  byte index = taskindex.toInt();


  if (ExtraTaskSettings.TaskDeviceValueNames[0][0] == 0)

    PluginCall(PLUGIN_GET_DEVICEVALUENAMES, &TempEvent, dummyString);

  reply += json_number( "taskdevicenumber", String( Settings.TaskDeviceNumber[index]) );
  reply += ", " + json_number("taskindex", taskindex );
  reply += ", " + json_number("taskdeviceid", String(Settings.TaskDeviceID[index]) );
  reply += ", " +  json_number("taskdevicepin1", Settings.TaskDevicePin1[index]) ;
  reply += ", " + json_number("taskdevicepin2", Settings.TaskDevicePin2[index]) ;
  reply += ", " + json_number("taskdevicepin3", Settings.TaskDevicePin3[index]) ;
  reply += ", " + json_number("taskdevicepin1pullup", Settings.TaskDevicePin1PullUp[index]) ;
  reply += ", " + json_number("taskdevicepin1inversed", Settings.TaskDevicePin1Inversed[index]) ;
  reply += ", " + json_string("taskdevicepin1inversed", ExtraTaskSettings.TaskDeviceName) ;
  reply += ", " + json_number("taskdeviceport", Settings.TaskDevicePort[index]) ;
  reply += ", " + json_number("taskdevicesenddata", Settings.TaskDeviceSendData[index]) ;
  reply += ", " + json_number("taskdeviceglobalsync", Settings.TaskDeviceGlobalSync[index]) ;
  // reply += ", " + json_number("taskdeviceformula", Settings.TaskDevicePort[index]) ;
  // reply += ", " + json_number("taskdevicevaluename", Settings.TaskDevicePort[index]) ;


  /*
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
  reply += F("}");
  // debug
  Serial.println(reply);
  // send to client
  WebServer.send(200, F("application/json"), reply);

}
//********************************************************************************
// API [GET] root
// @return [json] info
// {
//    Name: "newdevice"
//    FreeMem: 20000
//    Uptime: 5
//    IP: "192.168.1.100"
//    Gateway: "192.168.1.1"
//    Build: "80"
//    Unit: "0"
//    STA_MAC: "18:fe:34:da:80:1b"
//    AP_MAC: "1a:fe:34:da:80:1b"
//    Chip_id: "14319643"
//    Flash_Chip_id: "1458400"
//    Flash_Size: "4194304"
//    Boot_cause: 0
//    NodeList: [ ]
// }
//********************************************************************************
void handle_api_root() {
  if (!isLoggedInApi()) return;

  // open json
  String reply = F("{");
  String comma = ",";

  reply += json_string( F("Name"), Settings.Name );

  int freeMem = ESP.getFreeHeap();
  reply += comma;
  reply += json_number( F("FreeMem"), freeMem );


#if FEATURE_TIME
  if (Settings.UseNTP) {
    String time = String(hour());
    time += ":";
    if (minute() < 10)
      time += "0";
    time += minute();
    reply += comma;
    reply += json_string( F("Time"), time );
  }
#endif

  reply += comma;
  reply += json_number( F("Uptime"), wdcounter / 2);

  IPAddress ip = WiFi.localIP();
  char str[20];
  sprintf_P(str, PSTR("%u.%u.%u.%u"), ip[0], ip[1], ip[2], ip[3]);
  reply += comma;
  reply += json_string( F("IP"), str);

  IPAddress gw = WiFi.gatewayIP();
  sprintf_P(str, PSTR("%u.%u.%u.%u"), gw[0], gw[1], gw[2], gw[3]);
  reply += comma;
  reply += json_string( F("Gateway"), str);

  reply += comma;
  reply += json_string( F("Build"), String(BUILD));

  reply += comma;
  reply += json_string( F("Unit"), String(Settings.Unit));

  uint8_t mac[] = {0, 0, 0, 0, 0, 0};
  uint8_t* macread = WiFi.macAddress(mac);
  char macaddress[20];
  sprintf_P(macaddress, PSTR("%02x:%02x:%02x:%02x:%02x:%02x"), macread[0], macread[1], macread[2], macread[3], macread[4], macread[5]);

  reply += comma;
  reply += json_string( F("STA_MAC"), macaddress);

  macread = WiFi.softAPmacAddress(mac);
  sprintf_P(macaddress, PSTR("%02x:%02x:%02x:%02x:%02x:%02x"), macread[0], macread[1], macread[2], macread[3], macread[4], macread[5]);
  reply += comma;
  reply += json_string( F("AP_MAC"), macaddress);

  reply += comma;
  reply += json_string( F("Chip_id"), String(ESP.getChipId()));

  reply += comma;
  reply += json_string( F("Flash_Chip_id"), String(ESP.getFlashChipId()));

  reply += comma;
  reply += json_string( F("Flash_Size"), String(ESP.getFlashChipRealSize()));

  reply += comma;
  reply += json_number( F("Boot_cause"), lastBootCause );

  reply += comma;
  reply += F("\"NodeList\":[");
  comma  = F("{");
  for (byte x = 0; x < UNIT_MAX; x++)  {
    if (Nodes[x].ip[0] != 0)    {
      char url[80];
      sprintf_P(url, PSTR("<a href='http://%u.%u.%u.%u'>%u.%u.%u.%u</a>"), Nodes[x].ip[0], Nodes[x].ip[1], Nodes[x].ip[2], Nodes[x].ip[3], Nodes[x].ip[0], Nodes[x].ip[1], Nodes[x].ip[2], Nodes[x].ip[3]);
      reply += comma;
      reply += F("\"Unit\":\"");
      reply += x;
      reply += F("\",\"Url\":\"");
      reply += url;
      reply += F("\",\"Age\":\"");
      reply += Nodes[x].age;
      reply += F("\"}");
      comma  = F(",{");
    }
  }

  // close json
  reply += F("]}");
  // debug
  Serial.println(reply);
  // send to client
  WebServer.send(200, F("application/json"), reply);
}
