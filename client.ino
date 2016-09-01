/*
  Repeating Wifi Web Client

  This sketch connects to a a web server and makes a request
  using an Arduino Wifi shield.

  Circuit:
   WiFi shield attached to pins SPI pins and pin 7

  created 23 April 2012
  modified 31 May 2012
  by Tom Igoe
  modified 13 Jan 2014
  by Federico Vanzati

  http://www.arduino.cc/en/Tutorial/WifiWebClientRepeating
  This code is in the public domain.
*/

#include <SPI.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <DS1302.h>
//boolean for the relays

boolean rA = 1, rB = 1, rC = 1, rD = 1;
//asignacion relays
int relayA = 34;
//string con el json recibido del servidor
String captura = "";
boolean push = true;
boolean startCapture;

char ssid[] = "l33t";      //  your network SSID (name)
char pass[] = "darkside";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

// Initialize the Wifi client library
//cliente 1 y 2 para get y push
WiFiClient client;
WiFiClient client2;

// server address:
IPAddress server(192, 168, 0, 10);
char server2[] = "con100t.herokuapp.com";

unsigned long lastConnectionTime = 0;            // last time you connected to the server, in milliseconds
unsigned long lastConnectionTime2 = 0;            // last time you connected to the server, in milliseconds
const unsigned long getingInterval = 5L * 1000L; // delay between updates, in milliseconds
const unsigned long postingInterval = 30L * 1000L; // delay between updates, in milliseconds

DS1302 rtc(18, 17, 16);
Time t;
int x;
int a, b, c, d;
String string1, string2, string3, string4, string5, string6;
String fp1, fp2, fp3, fp4, fp5;
String p1, p2, p3, p4, p5;
String sumA, sumB, sumC, sumD, sumT;
String potmax1, potmax2, potmax3, potmax4, potmax5;
String dd, mm, yy, com;
String hora, minuto, segundo, com2;

float potsumA = 0, potsumB = 0, potsumC = 0, potsumD = 0;
float potmaxA = 0, potmaxB = 0, potmaxC = 0, potmaxD = 0;

/////variables globales sensor 1
int raw_c, raw_v, sample, sample2, alerta;
float con_v, con_c, pot_inst, v2, sum_v, c2, sum_c, sum_pot_inst, Vrms, Irms, T_pot_inst, Pot_Apa, Factor, Potencia_RMS;
float Sum_VrmsT, Sum_IrmsT, Sum_FactorT, Sum_Potencia_RMST, VrmsT, IrmsT, FactorT, Potencia_RMST;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////variables globales sensor 2
int raw_cB, sampleB, sample2B, alertaB;
float con_cB, pot_instB, c2B, sum_cB, sum_pot_instB, IrmsB, T_pot_instB, Pot_ApaB, FactorB, Potencia_RMSB;
float Sum_IrmsTB, Sum_FactorTB, Sum_Potencia_RMSTB, IrmsTB, FactorTB, Potencia_RMSTB;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////variables globales sensor 3
int raw_cC, sampleC, sample2C, alertaC;
float con_cC, pot_instC, c2C, sum_cC, sum_pot_instC, IrmsC, T_pot_instC, Pot_ApaC, FactorC, Potencia_RMSC;
float Sum_IrmsTC, Sum_FactorTC, Sum_Potencia_RMSTC, IrmsTC, FactorTC, Potencia_RMSTC;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////variables globales sensor 4
int raw_cD, sampleD, sample2D, alertaD;
float con_cD, pot_instD, c2D, sum_cD, sum_pot_instD, IrmsD, T_pot_instD, Pot_ApaD, FactorD, Potencia_RMSD;
float Sum_IrmsTD, Sum_FactorTD, Sum_Potencia_RMSTD, IrmsTD, FactorTD, Potencia_RMSTD;






void setup() {
  pinMode(relayA, OUTPUT);
  digitalWrite(relayA,HIGH); 
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // The following lines can be commented out to use the values already stored in the DS1302
 rtc.setDOW(SATURDAY);        // Set Day-of-Week 
 rtc.setTime(02, 32,10);     // Set the time to 12:00:00 (24hr format)
 rtc.setDate(27, 8, 2016);   // Set the date to JULY 18, 2016

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  while (client.available()) {
    char c = client.read();
  

    captura += c;

  }

  // Get data from the DS1302
  t = rtc.getTime();

  // muestreo I,V,P SENSOR 1
  for (sample = 0; sample < 240; sample++)
  {

    raw_v = analogRead(A11);
    raw_c = analogRead(A7);
    raw_cB = analogRead(A8);
    raw_cC = analogRead(A9);
    raw_cD = analogRead(A10);

    con_v = ((5 * raw_v / 1023) - 2.5) * 75;

    con_c = ((.0264 * raw_c ) - 14.279) * 1.28;
    pot_inst = con_v * con_c;

    con_cB = ((.0264 * raw_cB ) - 14.279) * 1.28;
    pot_instB = con_v * con_cB;

    con_cC = ((.0264 * raw_cC ) - 14.279) * 1.28;
    pot_instC = con_v * con_cC;

    con_cD = ((.0264 * raw_cD ) - 14.279) * 1.28;
    pot_instD = con_v * con_cD;

    v2 = con_v * con_v;
    sum_v = v2 + sum_v;

    c2 = con_c * con_c;
    sum_c = c2 + sum_c;
    sum_pot_inst = pot_inst + sum_pot_inst;

    c2B = con_cB * con_cB;
    sum_cB = c2B + sum_cB;
    sum_pot_instB = pot_instB + sum_pot_instB;

    c2C = con_cC * con_cC;
    sum_cC = c2C + sum_cC;
    sum_pot_instC = pot_instC + sum_pot_instC;

    c2D = con_cD * con_cD;
    sum_cD = c2D + sum_cD;
    sum_pot_instD = pot_instD + sum_pot_instD;


    delay(.34);
  }

  Vrms = sqrt(sum_v / 240);

  Irms = sqrt(sum_c / 240) ;
  T_pot_inst = (sum_pot_inst / 240);
  Pot_Apa = Vrms * Irms + 10;
  Factor = (abs((T_pot_inst / Pot_Apa)));
  Potencia_RMS = Pot_Apa * Factor;

  IrmsB = sqrt(sum_cB / 240) ;
  T_pot_instB = (sum_pot_instB / 240);
  Pot_ApaB = Vrms * IrmsB + 10;
  FactorB = (abs((T_pot_instB / Pot_ApaB))) ;
  Potencia_RMSB = Pot_ApaB * FactorB;

  IrmsC = sqrt(sum_cC / 240) ;
  T_pot_instC = (sum_pot_instC / 240);
  Pot_ApaC = Vrms * IrmsC + 10;
  FactorC = (abs((T_pot_instC / Pot_ApaC))) ;
  Potencia_RMSC = Pot_ApaC * FactorC;

  IrmsD = sqrt(sum_cD / 240) ;
  T_pot_instD = (sum_pot_instD / 240);
  Pot_ApaD = Vrms * IrmsD + 10;
  FactorD = (abs((T_pot_instD / Pot_ApaD)));
  Potencia_RMSD = Pot_ApaD * FactorD;


  Sum_VrmsT = Vrms + Sum_VrmsT;

  Sum_IrmsT = Irms + Sum_IrmsT;
  Sum_FactorT = Sum_FactorT + Factor;
  Sum_Potencia_RMST = Sum_Potencia_RMST + Potencia_RMS;

  Sum_IrmsTB = IrmsB + Sum_IrmsTB;
  Sum_FactorTB = Sum_FactorTB + FactorB;
  Sum_Potencia_RMSTB = Sum_Potencia_RMSTB + Potencia_RMSB;

  Sum_IrmsTC = IrmsC + Sum_IrmsTC;
  Sum_FactorTC = Sum_FactorTC + FactorC;
  Sum_Potencia_RMSTC = Sum_Potencia_RMSTC + Potencia_RMSC;

  Sum_IrmsTD = IrmsD + Sum_IrmsTD;
  Sum_FactorTD = Sum_FactorTD + FactorD;
  Sum_Potencia_RMSTD = Sum_Potencia_RMSTD + Potencia_RMSD;


  if (sample2 == 10)
  {
    VrmsT = Sum_VrmsT / 10;
    IrmsT = Sum_IrmsT / 10;
    FactorT = Sum_FactorT / 10;
    Potencia_RMST = (Sum_Potencia_RMST / 10) / 1000;
    Sum_IrmsT = 0;
    Sum_FactorT = 0;
    Sum_Potencia_RMST = 0;

    IrmsTB = Sum_IrmsTB / 10;
    FactorTB = (Sum_FactorTB / 10);
    Potencia_RMSTB = (Sum_Potencia_RMSTB / 10) / 1000;
    Sum_IrmsTB = 0;
    Sum_FactorTB = 0;
    Sum_Potencia_RMSTB = 0;

    IrmsTC = Sum_IrmsTC / 10;
    FactorTC = (Sum_FactorTC / 10);
    Potencia_RMSTC = (Sum_Potencia_RMSTC / 10) / 1000;
    Sum_IrmsTC = 0;
    Sum_FactorTC = 0;
    Sum_Potencia_RMSTC = 0;

    IrmsTD = Sum_IrmsTD / 10;
    FactorTD = (Sum_FactorTD / 10);
    Potencia_RMSTD = (Sum_Potencia_RMSTD / 10) / 1000;
    Sum_IrmsTD = 0;
    Sum_FactorTD = 0;
    Sum_Potencia_RMSTD = 0;

    Sum_VrmsT = 0;
    sample2 = 0;
  }

  sample2++;

  if (sample == 240)
  {
    sample = 0;
    sum_v = 0;
    sum_c = 0;
    sum_pot_inst = 0;

    sum_cB = 0;
    sum_pot_instB = 0;

    sum_cC = 0;
    sum_pot_instC = 0;

    sum_cD = 0;
    sum_pot_instD = 0;
  }


  if (VrmsT <= 39.3)
  {
    VrmsT = 0;
  }

  if (IrmsT <= .25)
  {
    IrmsT = 0;
    FactorT = 0;
    Potencia_RMST = 0;
    a = 1;
  }
  else {
    a = 0;
  }

  if (IrmsTB <= .24)
  {
    IrmsTB = 0;
    FactorTB = 0;
    Potencia_RMSTB = 0;
    b = 1;
  }
  else {
    b = 0;
  }

  if (IrmsTC <= .24)
  {
    IrmsTC = 0;
    FactorTC = 0;
    Potencia_RMSTC = 0;
    c = 1;
  }
  else {
    c = 0;
  }


  if (IrmsTD <= .24)
  {
    IrmsTD = 0;
    FactorTD = 0;
    Potencia_RMSTD = 0;
    d = 1;
  }
  else {
    d = 0;
  }


  potsumA = potsumA + Potencia_RMST;
  potsumB = potsumB + Potencia_RMSTB;
  potsumC = potsumC + Potencia_RMSTC;
  potsumD = potsumD + Potencia_RMSTD;

  if (Potencia_RMST > potmaxA)
  {
    potmaxA = Potencia_RMST;
  }


  if (Potencia_RMSTB > potmaxB)
  {
    potmaxB = Potencia_RMSTB;
  }

  if (Potencia_RMSTC > potmaxC)
  {
    potmaxC = Potencia_RMSTC;
  }

  if (Potencia_RMSTD > potmaxD)
  {
    potmaxD = Potencia_RMSTD;
  }

  //String de las corrientes
  string1 = String(IrmsT);
  string2 = String(IrmsTB);
  string3 = String(IrmsTC);
  string4 = String(IrmsTD);
  string5 = String(" : ");

  //String de los factores de potencia

  fp1 = String(FactorT);
  fp2 = String(FactorTB);
  fp3 = String(FactorTC);
  fp4 = String(FactorTD);

  //String de potencia

  p1 = String(Potencia_RMST);
  p2 = String(Potencia_RMSTB);
  p3 = String(Potencia_RMSTC);
  p4 = String(Potencia_RMSTD);

  //Suma de Potencias

  sumA = String(potsumA);
  sumB = String(potsumB);
  sumC = String(potsumC);
  sumD = String(potsumD);

  //String de Valor Maximo Registrado

  potmax1 = String(potmaxA);
  potmax2 = String(potmaxB);
  potmax3 = String(potmaxC);
  potmax4 = String(potmaxD);

  //String Fecha

  dd = String(t.date);
  mm = String(rtc.getMonthStr());
  yy = String(t.year);

  //String Hora

  hora = String(t.hour);
  minuto = String(t.min);
  segundo = String(t.sec);




  ////////////////////////////////////////////////////////
  // String voltaje
  string6 = string1 + string5 + string2 + string5 + string3 + string5 + string4;

  //String de factor de potencia
  fp5 = fp1 + string5 + fp2 + string5 + fp3 + string5 + fp4;

  //String de potencia
  p5 = p1 + string5 + p2 + string5 + p3 + string5 + p4;

  //String suma de las potencias individual
  sumT = sumA + string5 + sumB + string5 + sumC + string5 + sumD;

  //Valor maximo de potencia registrado
  potmax5 = potmax1 + string5 + potmax2 + string5 + potmax3 + string5 + potmax4;

  //String de fecha
  com = dd + string5 + mm + string5 + yy;

  //String de Hora

  com2 = hora + string5 + minuto + string5 + segundo;











  //manejar repeticionnes de peticiones
  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > getingInterval) {
    relayRequest();
  }
  if (millis() - lastConnectionTime2 > postingInterval) {
    pushData();
  }

}  

void pushData() {
  client2.stop();
  if (client2.connect(server2, 80)) {
    push = true;
    Serial.println("-> Connected to post");
    // Make a HTTP request:
    char test[20];
    String v(floatToString(test, VrmsT  , 3, 0));
    String corrienteA(floatToString(test, IrmsT  , 3, 0));
    String corrienteB(floatToString(test, IrmsTB  , 3, 0));
    String corrienteC(floatToString(test, IrmsTC  , 3, 0));
    String corrienteD(floatToString(test, IrmsTD  , 3, 0));
    String corrienteTotal(floatToString(test, IrmsTD + IrmsTB + IrmsTC + IrmsT , 3, 0));
    Serial.println("-> corriente a");
    Serial.println(corrienteA);
    Serial.println("-> corriente b");
    Serial.println(corrienteB);
    Serial.println("-> corriente c");
    Serial.println(corrienteC);
    Serial.println("-> corriente d");
    Serial.println(corrienteD);
    Serial.println("-> corriente t");
    Serial.println(corrienteTotal);




    //String Fecha

    dd = String(t.date);
    mm = String(t.mon);
    yy = String(t.year);



    if (dd.length() == 1) {
      dd = "0" + dd;
    }



    if (mm.length() == 1) {
      mm = "0" + mm;
    }

    Serial.println("-> fecha");
    String fechaTS = yy + mm + dd;
    Serial.println(fechaTS);
    String fechaFormat = yy + "-" + mm + "-" + dd;




    hora = String(t.hour);
    minuto = String(t.min);
    segundo = String(t.sec);


    if (segundo.length() == 1) {
      segundo = "0" + segundo;
    }



    if (minuto.length() == 1) {
      minuto = "0" + minuto;
    }

    if (hora.length() == 1) {
      hora = "0" + hora;
    }




    Serial.println("-> hora");
    String horaTS = hora + minuto + segundo;
    Serial.println(horaTS);
    Serial.println(hora + ":" + minuto);
    String horaData =  hora + ":" + minuto + ":" + segundo;






   // client2.println("GET /push?ft=" + horaData + "&cA=" + corrienteA + "&cB=" + corrienteB + "&cC=" + corrienteC + "&cD=" + corrienteD + "&vt=" + v + "&fF=" + fechaFormat + "&cT=" + corrienteTotal);
   client2.print("GET /push");
   client2.print("?ft=" + horaData + "&cA=" + corrienteA + "&cB=" + corrienteB + "&cC="+corrienteC + "&cD=" + corrienteD + "&vt=" + v + "&fF=" + fechaFormat + "&cT=" + corrienteTotal);
   client2.println(" HTTP/1.0");
   
    client2.println("Host: con100t.herokuapp.com");
client2.println("Authorization: Basic ---------------");
    // client2.println("GET /hola?");
    //  client2.println("id=");
    //   client2.println(VrmsT);
     

    client2.println();





    // note the time that the connection was made:
    lastConnectionTime2 = millis();
 } else {
    // if you couldn't make a connection:
   Serial.println("connection failed");
  }
}




// this method makes a HTTP connection to the server:
void relayRequest() {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();
  getDataRelays();





  // if there's a successful connection:
  if (client.connect(server2, 80)) {
    push = false;
    Serial.println("-> Connected to get data ");
    // Make a HTTP request:
    client.println("GET /enchufes HTTP/1.0");

   
    client.println("Host: con100t.herokuapp.com");
client.println("Authorization: Basic ---------------");

    client.println();



   

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}

void getDataRelays() {

  String stringOne = captura;
  int firstClosingBracket = stringOne.indexOf('{');

  if (firstClosingBracket > 0) {
    Serial.println(firstClosingBracket);
    String onlyjson = stringOne.substring(firstClosingBracket, stringOne.length());

    Serial.println(onlyjson);



    // Length (with one extra character for the null terminator)
    int str_len = onlyjson.length() + 1;

    // Prepare the character array (the buffer)
    char json[str_len];

    // Copy it over
    onlyjson.toCharArray(json, str_len);

    //
    // Step 1: Reserve memory space
    //
    StaticJsonBuffer<200> jsonBuffer;

    //
    // Step 2: Deserialize the JSON string
    //
    JsonObject& root = jsonBuffer.parseObject(json);

    if (!root.success())
    {
      Serial.println("parseObject() failed");
      return;
    }

    //
    // Step 3: Retrieve the values
    //
    rA      = root["a"];
    rB      = root["b"];
    rC      = root["c"];
    rD      = root["d"];

     Serial.println(rA);
      Serial.println(rB);
       Serial.println(rC);
  

   apagarRelays();


  }
  captura = "";
}



char * floatToString(char * outstr, double val, byte precision, byte widthp) {
  char temp[70]; //increase this if you need more digits than 15
  byte i;

  temp[0] = '\0';
  outstr[0] = '\0';

  if (val < 0.0) {
    strcpy(outstr, "-\0"); //print "-" sign
    val *= -1;
  }

  if ( precision == 0) {
    strcat(outstr, ltoa(round(val), temp, 10)); //prints the int part
  }
  else {
    unsigned long frac, mult = 1;
    byte padding = precision - 1;

    while (precision--)
      mult *= 10;

    val += 0.5 / (float)mult;    // compute rounding factor

    strcat(outstr, ltoa(floor(val), temp, 10)); //prints the integer part without rounding
    strcat(outstr, ".\0"); // print the decimal point

    frac = (val - floor(val)) * mult;

    unsigned long frac1 = frac;

    while (frac1 /= 10)
      padding--;

    while (padding--)
      strcat(outstr, "0\0");   // print padding zeros

    strcat(outstr, ltoa(frac, temp, 10)); // print fraction part
  }

  // generate width space padding
  if ((widthp != 0) && (widthp >= strlen(outstr))) {
    byte J = 0;
    J = widthp - strlen(outstr);

    for (i = 0; i < J; i++) {
      temp[i] = ' ';
    }

    temp[i++] = '\0';
    strcat(temp, outstr);
    strcpy(outstr, temp);
  }

  return outstr;
}


void apagarRelays(){
  if(rA){
    digitalWrite(relayA,HIGH); 
  }else{
    digitalWrite(relayA,LOW); 
  }
  




}




