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
  
 /*if( (millis()-oldtime) > 60000){
  oldtime = millis(); //ha pasado un minuto, actualizamos
  //cada minuto actualizamos
 
  minutos++;
  }*/
      //display.showNumberDecEx(obtenerHora(), 0b11100000, true, 4, 0);
        //   delay(1000);

// display.showNumberDec(obtenerHora(), true);
recibirpista();
  // Print 1234 with the center colon:
  //display.showNumberDecEx(obtenerHora(), 0b11100000, false, 4, 0);
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
      
      // Fijar a fecha y hora específica. En el ejemplo, 21 de Enero de 2016 a las 03:00:00
      // rtc.adjust(DateTime(2021, 1, 29, 0, 30, 0));
     // }
      // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   //rtc.adjust(DateTime(2021, 1, 29, 0, 30, 0));
  
   
 //  horas = ahora.hour();
   //minutos = ahora.minute();
 

   
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


//millis devuelve los milisegundos que han pasado desde que se inicia, mientras no dispongamos del reloj RTC nos permitirá
//comprobar si la pantalla 4 digit se actualiza correctamente
//millis()/3600000 nos daría las horas
//millis()/60000 nos daría los minutos
//if(millis()/60000 > 1)
/*https://arduino.stackexchange.com/questions/25939/using-two-digits-to-display-a-number-using-tm1637-and-a-4-digit-display
 * tm1637.display(0, hour/10);
tm1637.display(1, hour%10);
tm1637.display(2, mins/10);
tm1637.display(3, mins%10);

*/

/*
if (horas > 24) 
  horas = 0;
if (minutos >59){
  minutos = 0;
  horas = horas + 1;
}

*/


  
   // Serial.print(F("minutos"));
   // Serial.println(millis()/60000);
  //return 1234;
    
 // return (horas*100 + minutos);

  //Código cuando tengamos el módulo RTC
   // Get current date and time:
  ahora = rtc.now();
  // Create time format to display:
  int displaytime = (ahora.hour() * 100) + ahora.minute();
  // Print displaytime to the Serial Monitor:
  //Serial.println(displaytime);

 /*  Serial.print(F("Fecha es "));
   Serial.print(ahora.day());
       Serial.print(F(" de "));
   Serial.print(ahora.month());
   Serial.print(F(" de "));
   Serial.println(ahora.year(),DEC);
   Serial.print(F(" Día de la semana (0 domingo, 6 sábado):  "));
    Serial.println(ahora.dayOfTheWeek());
    */
  delay(1000);
  return displaytime;
  
}

void iniciarSD()
{
 // Serial.print(F("Iniciando SD ..."));
 if (!SD.begin(SSpin))
  {
    Serial.println(F("Intentando iniciar SD.. Error al iniciar"));
     return; 
  }
  Serial.println(F(" SD iniciada correctamente"));
}
