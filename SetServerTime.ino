#include <WiFi.h>
#include <WebServer.h>
#include "html.h"
#include "zones.h"

WebServer server(80);

char ssid[] = "myssid";
char passwd[] = "mypasswd";

void handleTZGet() {
  String html = String(htmlConfigTZ);
  html.reserve(15000);
  for (int x=0; x < sizeof(timezones)/8; x++) {
    String tzreplace = "<option value='";
    tzreplace += timezones[(x*2)+1];
    tzreplace += "'>";
    tzreplace += timezones[x*2];
    tzreplace += "</option>";
    tzreplace += "%tz_options%";
    html.replace("%tz_options%", tzreplace);
  }
  html.replace("%tz_options%","");
  time_t now = time(NULL);
  char outstr[200];
  strftime(outstr, sizeof(outstr), "%a, %d %b %Y %T %z", localtime(&now));
  html.replace("%server_time%", outstr);
  server.send(200, "text/html", html);
}

void handleTZPost() {
  if (server.hasArg("timezone")) {
    setenv("TZ", server.arg("timezone").c_str(), 1);
    tzset();
  }
  if (server.hasArg("utctime")) {
    String utctime = server.arg("utctime");
    // convert to epoch time & set
    timeval epoch;
    epoch.tv_sec = utctime.substring(0,utctime.length()-3).toInt();
    epoch.tv_usec = utctime.substring(utctime.length()-3).toInt();
    settimeofday(&epoch, 0);
  }
  server.sendHeader("Refresh", "10");
  server.sendHeader("Location","/");
  server.send(302);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, passwd);
  WiFi.waitForConnectResult();
  Serial.println(WiFi.localIP());
  server.on("/", []() {handleTZGet();});
  server.on("/configTZ", HTTP_GET, []() {handleTZGet();});
  server.on("/configTZ", HTTP_POST, []() {handleTZPost();});
  server.begin();
}

void loop() {
  server.handleClient();
  delay(79);
}
