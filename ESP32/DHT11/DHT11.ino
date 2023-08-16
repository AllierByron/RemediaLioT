/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       -
 *    D3       SS
 *    CMD      MOSI
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      SCK
 *    VSS      GND
 *    D0       MISO
 *    D1       -
 */
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <string>
#include<iostream>
#include<cstring>
#include "DHT.h"
#include <HTTPClient.h>
using namespace std; 

//definicion de variables para el DHT11
#define DHTPIN 2 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

// WiFiClient client;
HTTPClient http;

//url de API
//pongan su IP AQUI       |
//                        |
//                        v
#define sensorAPI "http://192.168.100.9:3000/sensor"

const char* ssid = "Totalplay-E3A8";
const char* password = "E3A8A63Ed3bg987G";

// #define pin 2


void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}

void setup(){
  // delay(2000);
  Serial.begin(115200);
  if(!SD.begin()){
      Serial.println("Card Mount Failed");
      return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
      Serial.println("No SD card attached");
      return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
      Serial.println("MMC");
  } else if(cardType == CARD_SD){
      Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
      Serial.println("SDHC");
  } else {
      Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  listDir(SD, "/", 0);
  createDir(SD, "/mydir");
  // listDir(SD, "/", 0);
  // removeDir(SD, "/mydir");
  // listDir(SD, "/", 2);

  //string para mediciones del sensor, puede variar el numero de mediciones
  String datos = "No, Reflectividad \n";

  //se convierte el archivo a .csv
  // writeFile(SD, "/mydir/datalog.csv", datos.c_str());
  writeFile(SD, "/mydir/datalog.csv", "");

  // appendFile(SD, "/hello.txt", "World!\n");
  readFile(SD, "/mydir/datalog.csv");
  // deleteFile(SD, "/foo.txt");
  // renameFile(SD, "/hello.txt", "/foo.txt");
  // readFile(SD, "/foo.txt");
  testFileIO(SD, "/mydir/datalog.csv");
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
  WiFi.disconnect(true);
  Serial.println("POST test program");
  WiFi.mode(WIFI_STA);

  //para ip estatica
  //WiFi.config(Server_ip, dns, gateway, subnet);  // forces to use the fixed IP

  //config normal, IP dinamica
  WiFi.begin(ssid, password);
  delay(1000);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.print("Connecting to:   ");
  Serial.print(WiFi.SSID());
  Serial.print("\n");
  Serial.print("This device IP address: ");
  Serial.println(WiFi.localIP());
  /*******FIN DE LA CONFIGURACION DE WIFI**********/

  dht.begin();// se inicia el DHT11
    
}

String medi;
int no = 0;
String postData;
boolean activarSensor;
void loop(){
  onOrOff();
  if(activarSensor){
    if(no == 0){
      medi = "No, Humedad, Temperatura \n";  
    }else{
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      SendSensorData(h,t, String("true"));
      medi = String(no) + ", "+ String(h)+", "+String(t)+"  \r\n";
      appendFile(SD, "/mydir/datalog.csv", medi.c_str());
      delay(1000);
    }
    Serial.println(medi);
    //se convierte el archivo a .csv
    no++;
  }
}

void SendSensorData(float humedad, float temperatura, String msj) {

  //se inicializa el cliente (esp32) con la url de la API
  http.begin(sensorAPI);
  // http.begin(sensorAPI, root_ca);

  //creacion de string de datos a insertar
  postData = "temperatura="+ String(temperatura)+ "&humedad=" + String(humedad)+"&msj="+msj ;

  //visualizacion de los datos a ingresar en el body
  Serial.println("datos: "+postData);

  //es el header, lo que encripta el body del request
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(postData);  //Send the request (hacemos el request de POST)
  delay(2000);                         //do we need to wait longer?
  String payload = http.getString();   //Get the response payload (obtenemos la confirmacion de la info)

  Serial.println(httpCode);  //Print HTTP return code (el codigo http que indica el estado de request)
  Serial.println(payload);   //Print request response payload (es lo de la respuesta, la devolucion o confirmacion)

  http.end();  //Close connection
}

void onOrOff(){
  http.begin(sensorAPI);

  //es el header, lo que encripta el body del request
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.GET();  //Send the request (hacemos el request de POST)
  delay(2000);                         //do we need to wait longer?
  String payload = http.getString();   //Get the response payload (obtenemos la confirmacion de la info)
  if(payload == "true"){
    activarSensor = true;
  }else{
    activarSensor = false;
  }
  Serial.println(httpCode);  //Print HTTP return code (el codigo http que indica el estado de request)
  Serial.println(payload);   //Print request response payload (es lo de la respuesta, la devolucion o confirmacion)

  http.end();  //Close connection
}
