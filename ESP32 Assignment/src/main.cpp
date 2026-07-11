#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ANITAB";
const char* password = "Akirachix@2011";

WebServer server(80);

const int ledPin = 2;


void handleRoot() {

  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>ESP Information</title>

<style>

body{
font-family:Arial, sans-serif;
text-align:center;
background:#f5f5f5;
}

.container{
display:flex;
justify-content:center;
gap:30px;
margin-top:20px;
}

.box{
width:350px;
min-height:350px;
border:2px solid black;
padding:20px;
background:white;
}

button{
padding:10px 20px;
font-size:16px;
cursor:pointer;
}

input{
padding:10px;
width:180px;
font-size:16px;
}

ol{
text-align:left;
}

</style>

</head>

<body>

<h2>ESP Information</h2>

<div class="container">

<div class="box">

<h3>WiFi Scanner</h3>

<form action="/scan" method="GET">
<button type="submit">Scan Me</button>
</form>

</div>

<div class="box">

<h3>LED Control</h3>

<form action="/control" method="GET">

<input
type="text"
name="cmd"
placeholder="Type on/off">

<br><br>

<button type="submit">
Send
</button>

</form>

</div>

</div>

</body>
</html>

)rawliteral";

  server.send(200, "text/html", page);
}


void handleScan() {

  int n = WiFi.scanNetworks();

  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>ESP Information</title>

<style>

body{
font-family:Arial;
text-align:center;
background:#f5f5f5;
}

.container{
display:flex;
justify-content:center;
gap:30px;
margin-top:20px;
}

.box{
width:350px;
min-height:350px;
border:2px solid black;
padding:20px;
background:white;
}

button{
padding:10px 20px;
font-size:16px;
}

input{
padding:10px;
width:180px;
}

ol{
text-align:left;
}

</style>

</head>

<body>

<h2>ESP Information</h2>

<div class="container">

<div class="box">

<h3>Available Networks</h3>
)rawliteral";

  if (n == 0) {
    page += "<p>No networks found.</p>";
  } else {

    int index[n];

    for(int i=0;i<n;i++)
      index[i]=i;

    for(int i=0;i<n-1;i++){
      for(int j=i+1;j<n;j++){

        if(WiFi.RSSI(index[j]) > WiFi.RSSI(index[i])){

          int temp=index[i];
          index[i]=index[j];
          index[j]=temp;

        }

      }
    }

    page += "<ol>";

    for(int i=0;i<n;i++){

      page += "<li>";
      page += WiFi.SSID(index[i]);
      page += " (";
      page += WiFi.RSSI(index[i]);
      page += " dBm)";
      page += "</li>";

    }

    page += "</ol>";
  }

  page += R"rawliteral(

<form action="/scan">
<button>Scan Again</button>
</form>

</div>

<div class="box">

<h3>LED Control</h3>

<form action="/control">

<input
type="text"
name="cmd"
placeholder="Type on/off">

<br><br>

<button>
Send
</button>

</form>

</div>

</div>

</body>
</html>

)rawliteral";

  server.send(200,"text/html",page);
}


void handleControl() {

  String cmd = server.arg("cmd");

  cmd.toLowerCase();

  if(cmd=="on"){
    digitalWrite(ledPin,HIGH);
  }

  else if(cmd=="off"){
    digitalWrite(ledPin,LOW);
  }

  server.sendHeader("Location","/scan");
  server.send(303);

}

void setup() {

  Serial.begin(115200);

  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid,password);

  Serial.print("Connecting");

  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/",handleRoot);
  server.on("/scan",handleScan);
  server.on("/control",handleControl);

  server.begin();

}

void loop() {

  server.handleClient();

}
