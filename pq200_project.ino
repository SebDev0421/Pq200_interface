#include<LiquidCrystal.h>

#define periodClock  59000
#define periodSampler 500


const int rs=12,rw=9,en1=11,en2=10,d4=2,d5=3,d6=4,d7=5;
const int on=6,up=7,sel=8,down=13,ex=A5;
 
unsigned long timeClock = 0;
unsigned long timeSampler = 0;


int tempAmbPin = A0;
int tempFilPin = A1;
int pressBarPin = A2;
int pressSysPin = A3;


//A4 && A5 i2c comunication interface

//clock variables
int hour = 23;
int minutes = 0;
int day = 30;
int year = 2021;
int month = 0;

int periodSaveSampler = 5000;
int flag_menu = 0;
int index_menu = 0;

LiquidCrystal lcd(rs,rw,en1,en2,d4,d5,d6,d7);

//map character specials
byte arrow[8] = {
  0b11000,
  0b11100,
  0b11110,
  0b11111,
  0b11110,
  0b11100,
  0b11000,
  0b00000
};

byte arrow_up[8] = {
  0b00100,
  0b00100,
  0b01110,
  0b01110,
  0b01110,
  0b11111,
  0b11111,
  0b11111
};

byte arrow_down[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b01110,
  0b01110,
  0b00100,
  0b00100
};

byte celcius[8] = {
  0b11100,
  0b10100,
  0b11100,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

//options menu

char *options[15] = {
  "Muestrear Desde las 00:01 - 24:00",
  "Muestrear c/ Usuario- Arrancar / Parar",
  "Continuar Corrida Muestra",
  "Reset Datos y Programacion",
  "Seleccionar Fecha y Tiempo",
  "El Sitio & Filtra Informacion",
  "Establecer Intervalo Grabadora",
  "Prueba Fugas",
  "Selecion De Flujo",
  "Calibrar/Verificar El Flujo",
  "Calibre de Presion y Temperatura",
  "Autodiagnostico",
  "Bajar Ahora"
  };

char *events[4] = {
  "LISTO PARA NUEVO RUN!    ",
  "DETENIDO POR OPERADOR!   ",
  "MAX EXCESO de CARGA!     ",
  "ESPERA TIEMPO DE EVENTOS!"
  };


char *months[12]={
  "jan",
  "feb",
  "mar",
  "apr",
  "may",
  "jun",
  "jul",
  "aug",
  "sep",
  "oct",
  "nov",
  "dec"
  };

// screens

void initial_screen(){
  
  lcd.setCursor(0,0);
  lcd.print("PQ200 Air Sampling System");
  lcd.setCursor(0,1);
  lcd.print("(c)Copyright 1997 BGI Incorporated");
  lcd.setCursor(0,2);
  lcd.print("All Rights Reserved");
  lcd.setCursor(0,3);
  lcd.print("   Version:2.56   Serial Number:2030");

}

void events_main(String event){


  //list Events -- "LISTO PARA NUEVO RUN!    "
  //list Events -- "DETENIDO POR OPERADOR!   "
  //list Events -- "MAX EXCESO de CARGA!     "
  //list Events -- "ESPERA TIEMPO DE EVENTOS!"
  
  lcd.setCursor(0,0);
  lcd.print(event);

  
  }

void year_main(String year){
  lcd.setCursor(35,1);
  lcd.print(year);
  }


void day_main(String month){
  lcd.setCursor(34,2);
  lcd.print(month);
  }


void hour_main(String hour){
  lcd.setCursor(34,3);
  lcd.print(hour);
  }


void sensors_main(String tempA, String tempF, String pressBar){

  lcd.setCursor(1,3);
  lcd.print(pressBar);
  lcd.setCursor(4,3);
  lcd.print("mmHg");
  
  lcd.setCursor(9,3);
  lcd.print("A");
  lcd.setCursor(10,3);
  lcd.print(tempA);
  lcd.setCursor(15,3);
  lcd.write(3);
  lcd.setCursor(16,3);
  lcd.print("C");

  lcd.setCursor(19,3);
  lcd.print("F");
  lcd.setCursor(20,3);
  lcd.print(tempF);
  lcd.setCursor(25,3);
  lcd.write(3);
  lcd.setCursor(26,3);
  lcd.print("C");
  
  
  }
  
  

void main_screen(){
  lcd.clear();
  
  
  lcd.setCursor(25,0);
  lcd.print("[DC In]");
  lcd.setCursor(35,0);
  lcd.print("P");
  lcd.setCursor(36,0);
  lcd.print("Q");
  lcd.setCursor(37,0);
  lcd.print("F");
  lcd.setCursor(38,0);
  lcd.print("T");
  lcd.setCursor(39,0);
  lcd.print("M");

  events_main(events[0]);
  String day_format = String(day);
  if(day < 10 ){
     day_format = "0"+day_format;
  }
  
  String day_show = day_format+months[month];
          
  day_main(day_show);
  year_main(String(year));
  
  hour_main("11:23");
  sensors_main(" 18.5"," 17.8","556");
}

void menu_screen(int index_show){
  lcd.clear();
  //arrow select
  int index_arrow = index_show;
  int index_navigation = 0;
  if(index_arrow >= 3){
    index_navigation = index_show - 3;
    index_arrow = 3;
   }


  lcd.setCursor(0,index_arrow);
  lcd.write(0);
  
  //arrow up
  if(index_menu){
  lcd.setCursor(39,0);
  lcd.write(1);
    }

  
  if(index_show < 12){
  lcd.setCursor(39,3);
  lcd.write(2); 
   }
  //arrow down
  

  //options

  lcd.setCursor(1,0);
  lcd.print(options[0+index_navigation]);
  lcd.setCursor(1,1);
  lcd.print(options[1+index_navigation]);
  lcd.setCursor(1,2);
  lcd.print(options[2+index_navigation]);
  lcd.setCursor(1,3);
  lcd.print(options[3+index_navigation]);
  
}


int define_max_month(int month,int modBiciest){
  int sizeMonth = 0;
          switch(month){
            case 0:
                    //janury
                    sizeMonth = 31;
                    break;
            case 1:

                  //feb
                  if(!modBiciest){
                      sizeMonth = 29;
                  }else{
                     sizeMonth = 28;  
                  }
                  break;
            
            case 2: 
                    //march
                    sizeMonth = 31;
                    break;
            case 3: 
                    //april
                    sizeMonth = 30;
                    break;
            case 4: 
                    //may
                    sizeMonth = 31;
                    break;
            case 5: 
                    //jun
                    sizeMonth = 30;
                    break;

            case 6: 
                    //jul
                    sizeMonth = 31;
                    break;

            case 7: 
                    //aug
                    sizeMonth = 31;
                    break;
                    
            case 8: 
                    //sept
                    sizeMonth = 30;
                    break;

            case 9: 
                    //oct
                    sizeMonth = 31;
                    break;
            case 10: 
                    //nov
                    sizeMonth = 30;
                    break;
            case 11: 
                    //dec
                    sizeMonth = 31;
                    break;
            }
            return sizeMonth;
 };



void setup() {
  //keyBoard setup


 Serial.begin(9600);

 
 
 

 pinMode(on,0);
 pinMode(up,0);
 pinMode(sel,0);
 pinMode(down,0);
 pinMode(ex,0);
 

  
  
  lcd.createChar(0, arrow);
  lcd.createChar(1, arrow_up);
  lcd.createChar(2, arrow_down);
  lcd.createChar(3, celcius);
  lcd.begin(40,4,LCD_5x8DOTS);

  initial_screen();

  delay(2000);

  //get status from eeprom

  main_screen();
}

void loop() {

  if(digitalRead(sel)){
    delay(200);
    if(!flag_menu){
      menu_screen(0);
    }else{
        switch(index_menu){
          case 0: 
           
          break; 
          }
    }
    flag_menu = 1;
    
  }

  if(digitalRead(down)){
    delay(300);
    if(flag_menu);
    
    
    if(index_menu < 12){
     index_menu ++;
     menu_screen(index_menu);
      }
    
    }

    if(digitalRead(up)){
    delay(300);
    if(flag_menu);
    
    
    if(index_menu > 0){
     index_menu --;
     menu_screen(index_menu);
      }
    }


    if(digitalRead(ex)){
    delay(300);
    index_menu = 0;
    flag_menu = 0;
    main_screen();
    }

    if(!flag_menu){
      
     //run clock hour/date/year
     
     
     //read sensors
     //get status power

     
    }

    if(millis() > (timeClock + periodClock )){
       timeClock= millis();
       minutes ++;
       if(minutes > 59){
        minutes = 0;
        hour ++;
        if(hour > 23){
          hour = 0;
          day ++;
          int modBiciest = year%4;
          
          if (day > define_max_month(month,modBiciest)){
            day = 1;
            month ++;
            if(month > 11){
              month = 0;
              year ++;
              
              }
            }

          String day_format = String(day);
          if(day < 10 ){
            day_format = "0"+day_format;
            }
          String day_show = day_format+months[month];

          if(!flag_menu){
             day_main(day_show);
             year_main(String(year));  
            }
          
          }
        }
       Serial.println(hour);
       Serial.println(minutes);

       String formatHour = String(hour);
       String formatMinutes = String(minutes);

       if(hour<10){
        formatHour = "0"+formatHour;
        }

       if(minutes<10){
        formatMinutes = "0"+formatMinutes;
        }

       String showHour = formatHour+":"+formatMinutes;
       if(!flag_menu){
         hour_main(showHour);
        }
       

       

       //calculated biciest year
       
      }


      //sampler sensors statics temps, pressures

      if(millis() > (timeSampler + periodSampler )){

          
          timeSampler = millis();
          
          int ADtempFil = analogRead(tempFilPin);

          float tempFil = (ADtempFil*(-0.29))+85.6087;
          
          Serial.println(tempFil);
          
          String tempFilShow = String(tempFil);
          if(tempFil > 0){
            tempFilShow = " "+tempFilShow;
            }
          if(!flag_menu){
            sensors_main(" 18.5",tempFilShow,"556");  
            }
          
      }

      
}
