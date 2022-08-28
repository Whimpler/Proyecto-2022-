#include <NexButton.h>
#include <NexText.h>
#include <Nextion.h>
#include <NexTouch.h>
#include <NexUpload.h>
#include <NexVariable.h>
#include <NexWaveform.h>
#include <stdlib.h>

#define sensor_humedad      A0
#define sensor_temperatura  A1
#define salida_digital      8


char buffer[7];         //the ASCII of the integer will be stored in this char array
uint32_t temperatura;
uint32_t humedad;
uint32_t bomba=0;
uint32_t modo_act=0;
uint32_t max_temp=34;
uint32_t min_temp=15;
uint32_t max_hum=80;
uint32_t min_hum=60;
uint32_t temperatura_a;
const unsigned long eventInterval = 5000;
unsigned long previousTime = 0;

NexText   modo=NexText(0,25,"modo");
NexText   tempval=NexText(0,5,"tempval");
NexText   humval=NexText(0,6,"humval");
NexText   maxvaltemp=NexText(0,11,"maxvaltemp");
NexText   maxvalhum=NexText(0,12,"maxvalhum");
NexText   minvaltemp=NexText(0,10,"minvaltemp");
NexText   minvalhum=NexText(0,13,"minvalhum");
NexButton b0=NexButton(0,14,"b0");
NexButton b1=NexButton(0,15,"b1");
NexButton b2=NexButton(0,16,"b2");
NexButton b3=NexButton(0,17,"b3");
NexButton b4=NexButton(0,18,"b4");
NexButton b5=NexButton(0,19,"b5");
NexButton b6=NexButton(0,20,"b6");
NexButton b7=NexButton(0,21,"b7");
NexButton b8=NexButton(0,22,"b8");
NexButton b9=NexButton(0,23,"b9");
NexButton b10=NexButton(0,24,"b10");

NexTouch *nex_listen_list[]={
  &b0,
  &b1,
  &b2,
  &b3,
  &b4,
  &b5,
  &b6,
  &b7,
  &b8,
  &b9,
  &b10,
  NULL
  };


void b0upcount(void *ptr){
  max_temp+=1;
  itoa(max_temp,buffer,10); 
  maxvaltemp.setText(buffer);
  
  }

void b1countdown(void *ptr){
  max_temp-=1;
  itoa(max_temp,buffer,10); 
  maxvaltemp.setText(buffer);
  
  }

void b2upcount(void *ptr){
  max_hum+=1;
  itoa(max_hum,buffer,10); 
  maxvalhum.setText(buffer);
  
  }

void b3countdown(void *ptr){
  max_hum-=1;
  itoa(max_hum,buffer,10); 
  maxvalhum.setText(buffer);
  
  }
  
void b4upcount(void *ptr){
  min_temp+=1;
  itoa(min_temp,buffer,10); 
  minvaltemp.setText(buffer);
  
  }
  
void b5countdown(void *ptr){
  min_temp-=1;
 itoa(min_temp,buffer,10); 
 minvaltemp.setText(buffer);
  
  }
      
void b6upcount(void *ptr){
  min_hum+=1;
  itoa(min_hum,buffer,10); 
  minvalhum.setText(buffer);
  
  }

void b7countdown(void *ptr){
  min_hum-=1;
  itoa(min_hum,buffer,10); 
  minvalhum.setText(buffer);
  
  }    




void b8updatevalues(void *ptr){
    
    temperatura_a=analogRead(sensor_temperatura);
    temperatura=((temperatura_a*5000.0)/1023)/10;
    humedad=map(analogRead(sensor_humedad),1024,0,0,99);
    delay(10);
    itoa(temperatura,buffer,10);
    tempval.setText(buffer);
    delay(10);
    itoa(humedad,buffer,10);
    humval.setText(buffer);
  
  }    

void b9bombaact(void *ptr){
    if (bomba==0){
        bomba=1;
      }
    else{
       bomba=0; 
    }
  }

void b10changemode(void *ptr){
    if (modo_act==0){
        modo.setText("modo:automatico");
        modo_act=1;
      }
    else{
        modo.setText("modo:manual");
        modo_act=0;
      }
  }

void setup() {
  pinMode(sensor_humedad,INPUT);
  pinMode(sensor_temperatura,INPUT);
  pinMode(salida_digital,OUTPUT);
  Serial.begin(115200);
  delay(500);
  nexInit();
  delay(100);
  modo.setText("modo:manual");
  delay(10);
  itoa(max_temp,buffer,10); 
  maxvaltemp.setText(buffer);
  delay(10);
  itoa(max_hum,buffer,10); 
  maxvalhum.setText(buffer);
  delay(10);
  itoa(min_temp,buffer,10); 
  minvaltemp.setText(buffer);
  delay(10);
  itoa(min_hum,buffer,10); 
  minvalhum.setText(buffer);
  delay(10);
  b0.attachPop(b0upcount,&b0);
  b1.attachPop(b1countdown,&b1);
  b2.attachPop(b2upcount,&b2);
  b3.attachPop(b3countdown,&b3);
  b4.attachPop(b4upcount,&b4);
  b5.attachPop(b5countdown,&b5);
  b6.attachPop(b6upcount,&b6);
  b7.attachPop(b7countdown,&b7);
  b9.attachPop(b9bombaact,&b9);
  b8.attachPop(b8updatevalues,&b8);
  b10.attachPop(b10changemode,&b10);
}

void loop() {
   
   nexLoop(nex_listen_list);
   /* Updates frequently */
  unsigned long currentTime = millis();

   
   temperatura=map(analogRead(sensor_temperatura),70,100,0,40);
   humedad=map(analogRead(sensor_humedad),1024,0,0,99);
   
   if(modo_act==0 && bomba==1){
      digitalWrite(salida_digital,LOW);
    }
    else if(modo_act==0 && bomba==0){
        digitalWrite(salida_digital,HIGH);
      }
    else if (modo_act==1 &&  humedad<min_hum){
        digitalWrite(salida_digital,LOW);
        bomba=1;
      }
    else{
          digitalWrite(salida_digital,HIGH);
          bomba=0;
      }


  
  /* This is the event */
  if (currentTime - previousTime >= eventInterval) {
    /* Event code */
    Serial.print(temperatura);
    Serial.print(",");
    Serial.print(humedad);
    Serial.print(",");
    Serial.print(bomba);
    Serial.print(",");
    Serial.println(modo_act  );
   /* Update the timing for the next time around */
    previousTime = currentTime;
  }
  
          
}
