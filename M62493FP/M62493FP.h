

#ifndef M62493PF_h
#define M62493PF_h
#include "Arduino.h" 

class M62493PF
{
 
 private : int _LA ;
 private : int _DA ; 
 private : int _CL ;
 private : int rom_addr =-1;
 public  : char toneSetting[5] = {3, 3, 3, 3, 3};
 public  : char toneData[8]  = {3, 2, 1, 0, 4, 5, 6, 7}; 
 public	 : int  volumeSetting =0;
 public  : byte surroundSetting = 0;
 public  : byte lowBoostSetting = 0;
 public  : byte hiBoostSetting = 0;

 public  : M62493PF(int LA,int DA,int CL,int eeprom_addr);
 public  : void init(int delays);
 public  : void setToneData(char currSettingTone,char value);
 public  : char getToneData(char currSettingTone);
 public  : void setSurround(byte b);
 public  : byte getSurroundData();
 public  : void setLowBoost(byte b);
 public  : void setHiBooset(byte b);
 public  : void setVolume(int volume);
 public  : int  getVolume();
 public  : void refushSetting();
 public  : void setData(byte data);
 private : void setOneBit(byte b);
 private : void saveRomData();
 private : void readRomData();

 private : byte dataReverse(byte data);
 
};
#endif





