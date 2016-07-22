#include <EEPROM.h>
#include "Arduino.h"
#include "M62493FP.h"



M62493PF:: M62493PF(int la, int da, int cl,int eeprom_addr)
{
  _LA = la;
  _DA = da;
  _CL = cl;
  rom_addr = eeprom_addr;
}
//初始化
void M62493PF::init(int delays)
{
  pinMode(_LA, OUTPUT);
  pinMode(_DA, OUTPUT);
  pinMode(_CL, OUTPUT);
  digitalWrite(_LA, HIGH);
 
  readRomData();
  Serial.println("initM62493PF_delays");
  Serial.println(delays);
  delay(600);
  setData(251);  //静音
  setData(0); 
  setData(1); //声道选择
  setData(7);
 
  refushSetting(); 
}

void M62493PF::setToneData(char currSettingTone,char value)
{
  Serial.println("toneSetData");
  toneSetting[currSettingTone] = value;

  char tc = currSettingTone + 1;

  //char index = toneSetting[currSettingTone];
  //char tbc = toneData[index];
  char tbc = toneData[value];
  Serial.println(tc, BIN);
  Serial.println(tbc, BIN);
  tc = tc << 5;
  tbc = tbc << 2;
  char data = tc | tbc;
  bitWrite(data, 1, 1);
  bitWrite(data, 0, 0);
  setData(data);
  saveRomData();
}

char M62493PF::getToneData(char currSettingTone)
{
	return toneSetting[currSettingTone];
}

void M62493PF::setLowBoost(byte b){
	lowBoostSetting = b;
	char data = 1;
   if (b)
   {
    bitWrite(data, 4, 1);
   }else{
   	bitWrite(data, 4, 0);
   }
    setData(data);
    saveRomData();
}

void M62493PF::setHiBooset(byte b){
	hiBoostSetting = b;
	char data = 1;
   if (b)
   {
    bitWrite(data, 3, 1);
   }else{
   	bitWrite(data, 3, 0);
   }
    setData(data);
    saveRomData();
}

void M62493PF::setSurround(byte b)
{
	surroundSetting = b;
	char data = 128;
   if (b)
   {
    bitWrite(data, 3, 1);
   }else{
   	bitWrite(data, 3, 0);
   }
    setData(data);
    saveRomData();
}

byte M62493PF::getSurroundData()
{
	return surroundSetting;
}

//
void  M62493PF::setVolume(int volume)
{
  volumeSetting = volume;

  int value = 31 - volume;
  if(value <0){value =0;}
  if(value >31){value =31;}
  Serial.print("M62493PF_setVolume");
  Serial.println(value);
  byte data = dataReverse(value);
  bitWrite(data, 2, 0);
  bitWrite(data, 1, 1);
  bitWrite(data, 0, 1);
  setData(data);
  saveRomData();
 
}

int M62493PF::getVolume()
{
	return volumeSetting;
}


void  M62493PF::saveRomData()
{
	if (rom_addr > -1)
	{ 
	  EEPROM.update(0,volumeSetting);
      //EEPROM.put(rom_addr+1,toneSetting);
  	EEPROM.update(rom_addr + 1, toneSetting[0]);
  	EEPROM.update(rom_addr + 2, toneSetting[1]);
  	EEPROM.update(rom_addr + 3, toneSetting[2]);
  	EEPROM.update(rom_addr + 4, toneSetting[3]);
  	EEPROM.update(rom_addr + 5, toneSetting[4]);
  	EEPROM.update(rom_addr + 6, surroundSetting);
  	EEPROM.update(rom_addr + 7, lowBoostSetting);
  	EEPROM.update(rom_addr + 8, hiBoostSetting);
 	}
}

void M62493PF::readRomData()
{	
	if (rom_addr > -1)
	{ 
		volumeSetting = EEPROM.read(rom_addr); 
	    EEPROM.get(rom_addr+1,toneSetting);
	    surroundSetting = EEPROM.read(rom_addr + 6);
	    lowBoostSetting = EEPROM.read(rom_addr + 7);
	    hiBoostSetting  = EEPROM.read(rom_addr + 8);
	}
}


void M62493PF::refushSetting()
{
  setVolume(volumeSetting);
  setSurround(surroundSetting);
  setLowBoost(lowBoostSetting);
  setHiBooset(hiBoostSetting);
  for (char i = 0; i < 5; ++i)
  {
  	 char val = toneSetting[i];
  	 setToneData(i,val);
  }
}



//写入数据
void  M62493PF::setData(byte data)
{
  Serial.println(data, BIN);
  digitalWrite(_LA, LOW);
  for (char n = 7; n > -1; n --)
  {
    char b = bitRead(data, n);
    setOneBit(b);
  }
  setOneBit(1);
  setOneBit(1);
  digitalWrite(_LA, HIGH);
  
}

//写入一枚数据
void M62493PF::setOneBit(byte b)
{
  digitalWrite(_DA, LOW);
  digitalWrite(_CL, LOW);
  digitalWrite(_DA, b);
  digitalWrite(_CL, HIGH);
  digitalWrite(_DA, LOW);
  digitalWrite(_CL, LOW);
}


//反转
byte  M62493PF::dataReverse(byte data) {
  byte temp = 0;
  for (char n = 7; n > -1; n --)
  {
    byte b = bitRead(data, n);
    bitWrite(temp, (7 - n), b);
  }
  return temp;
}

