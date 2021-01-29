#include <IRremote.h>
#include <Wire.h>
#include "RTClib.h"
#include <TM1637Display.h>
// Define the connections pins:
#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>

#define CLK 6
#define DIO 5
#define MAXVOL 6
#define IRSENSOR 7
#define Boton_1 0xBF828CD5
#define Boton_2 0xC8FB13D9
#define Boton_3 0xE2D94FD
#define Boton_4 0xCB9C6577
#define Boton_5 0x9926853B
#define Boton_Sig 0xC35E80DF
#define Boton_Menu 0x70BDB29B //inicio cuentos

#define SSpin 10

#define Boton_volumemas 0xDD996F5B
#define Boton_volumemenos 0xAAF603FB
DateTime ahora;
uint32_t horas=0;  
uint32_t minutos=0;
byte pista=0;
// static uint32_t oldtime=millis();
RTC_DS3231 rtc;

TM1637Display display = TM1637Display(CLK, DIO);

char *file[] ={  "01bds.wav", //0
                "01LUNES.wav", //1
               "02MARTES.wav", //2,
                "03MIERC.wav", //3,
                 "04JUEV.wav", //4,
                  "05VIER.wav", 
                   "06SAB.wav", 
                  "07DOM.wav", 
                  "00BD-L-V.wav", //8
               "00BD-S-D.wav", //9
                "08RECOM.wav", //10,
                  "1.WAV", //11
                  "c01RyP.wav" //12
                };


//Se declara un objeto del tipo de la librería
TMRpcm tmrpcm;
byte volumen=3;
IRrecv irrecv(IRSENSOR);
decode_results codigo; //instancia 
 
void setup() {
 
  // put your setup code here, to run once:
Serial.begin(9600);
irrecv.enableIRIn();//se inicializa la recepción de datos desde el módulo
iniciarSD();
//se define el pin del altavoz
tmrpcm.speakerPin=9;
tmrpcm.quality(1);
   tmrpcm.setVolume(volumen);


  
    

display.clear();
delay(1000);
 display.setBrightness(1);

 iniciarRTC();
   display.showNumberDecEx(obtenerHora(), 0b11100000, true, 4, 0);
    Serial.print(F("Fecha es "));
   Serial.print(ahora.day());
       Serial.print(F(" de "));
   Serial.print(ahora.month());
   Serial.print(F(" de "));
   Serial.println(ahora.year(),DEC);
   Serial.print(F(" Día de la semana (0 domingo, 6 sábado):  "));
    Serial.println(ahora.dayOfTheWeek());
    if (ahora.dayOfTheWeek() !=0)
    reproducirFichero(file[ahora.dayOfTheWeek()]);
    else
    reproducirFichero(file[7]);

    delay(2000);
    switch (ahora.dayOfTheWeek()){
    case 1:
       reproducirFichero(file[8]);
       break;
    case 2:
       reproducirFichero(file[8]);
       break;
    case 3:
       reproducirFichero(file[8]);
       break;
    case 4:
       reproducirFichero(file[8]);
       break;
    case 5:
      
       reproducirFichero(file[8]);
       break;
    case 6:
       reproducirFichero(file[1]);
       break;
    case 0:
       reproducirFichero(file[1]);
       break;
    }
}

void loop() {

recibirpista();
  display.showNumberDecEx(obtenerHora(), 0b11100000, true, 4, 0);
}

void iniciarRTC(){
  
  //modulo reloj
   Serial.println(F("Cargando RTC..."));
delay(1000);
 if (!rtc.begin()) {
      Serial.println(F("Módulo RTC no cargado!"));
      while(1);
   }
   else{
       Serial.println(F("Módulo RTC Cargado..."));
      // if (rtc.lostPower()) {
      // Fijar a fecha y hora de compilacion
       // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      
      // Fijar a fecha y hora específica. En el ejemplo, 29 de Enero de 2021 a las 00:30:00
      // rtc.adjust(DateTime(2021, 1, 29, 0, 30, 0));
     // }
      // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   //rtc.adjust(DateTime(2021, 1, 29, 0, 30, 0));
  
   
    
   }
   delay(1000);
}
void recibirpista(){


if(irrecv.decode(&codigo)){
  Serial.println(F("Se está recibiendo código"));
  Serial.println(codigo.value,HEX);
  if(codigo.value == Boton_1){
    //Serial.println(F("Se ha pulsado botón 1"));
    pista=1;
    reproducirFichero(file[pista]);
  }
  
  irrecv.resume(); //esta listo para recibir nuevos datos
  if(codigo.value == Boton_2){
   // Serial.println(F("Se ha pulsado botón 2"));
    pista =2;
    reproducirFichero(file[pista]);
  }
   if(codigo.value == Boton_3){
    //Serial.println(F("Se ha pulsado botón 3"));
    pista = 3;
    reproducirFichero(file[pista]);
  }
   if(codigo.value == Boton_4){
    pista = 4;
   // Serial.println(F("Se ha pulsado botón 4"));
    reproducirFichero(file[pista]);
  }
 if(codigo.value == Boton_5){
   // Serial.println(F("Se ha pulsado botón 5"));
    pista=5;
    reproducirFichero(file[pista]);
  }
   if(codigo.value == Boton_volumemas){
     reproducirFichero(file[11]);
    volumen=volumen+1;
    if(volumen>=MAXVOL) //limitamos el volumen debido a las limitaciones del sistema
     volumen=MAXVOL;
     //    Serial.print(F("Se ha pulsado botón mas volumen, valor: "));
      //   Serial.println(volumen);
   
     tmrpcm.setVolume(volumen);
  }
   if(codigo.value == Boton_volumemenos){
       reproducirFichero(file[11]);
    volumen=volumen-1;
    if(volumen<=1)
     volumen=1;
     //     Serial.print(F("Se ha pulsado botón menos volumen, valor: "));
       //  Serial.println(volumen);
 
     tmrpcm.setVolume(volumen);
  }
  // Boton_Sig
   if(codigo.value == Boton_Sig){
   // Serial.println(F("Se ha pulsado botón Sig"));
    pista  = pista + 1;
    if(pista >10)
    pista=10;
    reproducirFichero(file[pista]);
  }

//boton cuentos
 if(codigo.value == Boton_Menu){
    //Serial.println(F("Se ha pulsado botón Menu"));
    pista = 12;
    reproducirFichero(file[pista]);
  }
  
  irrecv.resume(); //esta listo para recibir nuevos datos

}
delay(100);
}

void reproducirFichero(char *file){
  if( !tmrpcm.isPlaying() )
    {
     
      Serial.println(F("Se reproduce la música"));
     tmrpcm.play(file);
    }
}

int obtenerHora()
{
  ahora = rtc.now();
 int displaytime = (ahora.hour() * 100) + ahora.minute();
   delay(1000);
  return displaytime;
 
}

void iniciarSD()
{
 
 if (!SD.begin(SSpin))
  {
    Serial.println(F("Intentando iniciar SD.. Error al iniciar"));
     return; 
  }
  Serial.println(F(" SD iniciada correctamente"));
}
