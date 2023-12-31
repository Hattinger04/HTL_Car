#include <FahrzeugLib.h>
#include <SoftwareSerial.h>

char BT_Char=' ';
char BT_ID=' ';
char BT_ID_temp= ' ';
bool BT_ID_expected=true;
bool BT_newReading=false;
String BT_ValueStr="";
float BT_Value;
float einschlag, geschw=0;bool vorwaerts;
float einschlag_wert;float einschlag_vz;
bool running=false;
long int lastBTtime=0;


SoftwareSerial BT(10, 11);
CMotor MotorLinks(5, 4);
CMotor MotorRechts(3, 2);
CAntrieb Antrieb(&MotorLinks, &MotorRechts);

void BT_check()
{
  BT_newReading=false;
  if(BT.available())
  {
    Serial.println("Available!");
    BT_Char=BT.read();
    Serial.print("Char: "); 
    Serial.write(BT_Char);
    /*
    if(BT_ID_expected) { BT_ID_temp=BT_Char;BT_ID_expected=false;BT_ValueStr="";}
    else 
    {
      if((BT_Char!=' ')&&(BT_Char!='\n')) BT_ValueStr+=BT_Char;
      if(BT_Char=='\n'){BT_ID=BT_ID_temp;BT_Value=BT_ValueStr.toFloat();BT_ValueStr="";BT_ID_expected=true;BT_newReading=true;}
    }*/
  }
}

void BT_setValue()
{
  switch(BT_ID) 
  {
    case 'G': geschw=BT_Value;break;
    case 'Y': einschlag=BT_Value;break;
    case 'V': running=true;vorwaerts=true;break;
    case 'S': running=false;break;
    case 'Z': running=true;vorwaerts=false;break;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BT.begin(9600);
  pinMode(13,OUTPUT);
}

void loop() {
  BT_check();
  if(BT_newReading) { BT_setValue();lastBTtime=millis();}

  if(geschw>255)geschw=255;if(geschw<0)geschw=0; 

  if(einschlag>0){einschlag_wert=einschlag;einschlag_vz=1;}
  else {einschlag_wert=-einschlag;einschlag_vz=-1;}

  running=running&&(millis()<lastBTtime+500);
  if (running)
  { 
    if (BT_newReading) 
      {Antrieb.Kurve(einschlag_vz*sqrt(einschlag_wert),abs(geschw),vorwaerts);BT_newReading=false;}
  }
  else {Antrieb.Stop();}
}
