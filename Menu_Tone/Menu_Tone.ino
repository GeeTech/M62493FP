#include <EEPROM.h>
#include "U8glib.h"
#include "menu_table.h"
#include "M62493FP.h"
U8GLIB_NHD_C12864 u8g(13, 12, 11, 10, 9);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
M62493PF mpf(0, 1, 2, 0);

#define KEY_NONE 0
#define KEY_PREV 1
#define KEY_NEXT 2
#define KEY_SELECT 3
#define KEY_BACK 4

uint8_t uiKeyPrev = 5;
uint8_t uiKeyNext = 8;
uint8_t uiKeySelect = 6;
uint8_t uiKeyBack = 7;
uint8_t ledLight = 3;

uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;

uint8_t menu_redraw_required = 0;
uint8_t last_key_code = KEY_NONE;

uint8_t is_home = 1;
MenuItem *current_menu = &topMenu;
  

void uiStep(void) {
  uiKeyCodeSecond = uiKeyCodeFirst;
  if ( digitalRead(uiKeyPrev) == LOW )
    uiKeyCodeFirst = KEY_PREV;
  else if ( digitalRead(uiKeyNext) == LOW )
    uiKeyCodeFirst = KEY_NEXT;
  else if ( digitalRead(uiKeySelect) == LOW )
    uiKeyCodeFirst = KEY_SELECT;
  else if ( digitalRead(uiKeyBack) == LOW )
    uiKeyCodeFirst = KEY_BACK;
  else
    uiKeyCodeFirst = KEY_NONE;

  if ( uiKeyCodeSecond == uiKeyCodeFirst )
    uiKeyCode = uiKeyCodeFirst;
  else
    uiKeyCode = KEY_NONE;
}

void drawHomePage(void)
{
  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();

  uint8_t h = u8g.getFontAscent() - u8g.getFontDescent();
  
  if(mpf.surroundSetting == 1)
  {
      char *str = "Surround";
      uint8_t w = u8g.getStrWidth(str);
      u8g.drawFrame(2, 0, w + 4, h + 4);
      u8g.drawStr(4, 2, str);
  }
  
  int volSetting = mpf.getVolume();
  uint8_t x, y;
  x = 12; y = 42;
  u8g.drawBox(x, y + 4, 3, 4);
  u8g.drawLine(x + 3, y + 4, x + 6, y);
  u8g.drawLine(x + 6, y, x + 6, y + 12);
  u8g.drawLine(x + 3, y + 8, x + 6, y + 12);
  char *str =""; 
  sprintf(str, "%d", volSetting);
  u8g.drawStr(x + 12, y,str);
  if (volSetting > 0)
  {
    u8g.drawBox(x + 28, y, volSetting * 2, 12);
  }
}

//检查当前菜单 所选中的值
void chkCurrMenu()
{
   char *_name =current_menu->menu_name;
   MenuItem *selectItem = getSelectItem(current_menu);
  
  if(_name =="tone")
  {
    char index = current_menu->data;
    char tbc = mpf.getToneData(index);
    selectItemBydata(current_menu,tbc); 

  }else if(_name == "surround")
  {
    char b = mpf.getSurroundData();
    selectItemBydata(current_menu,b); 
    
  }else if(_name == "lowboost")
  {
    char b = mpf.lowBoostSetting;
    selectItemBydata(current_menu,b); 
    
  }else if(_name == "hiboost")
  {
    //char b = mpf.hiBoostSetting;
    //selectItemBydata(current_menu,b); 
  }
}

//检查选中菜单并 执行操作数据
void chkSelectMenu()
{
  char *_name =current_menu->menu_name;
  MenuItem *selectItem = getSelectItem(current_menu);
  
  if(_name =="tone")
  {
    char index = current_menu->data;
    char val  = selectItem->data;//current_menu->curr;
    mpf.setToneData(index,val);
    
  }else if(_name == "surround")
  {
    mpf.setSurround(selectItem->data);
    
  }else if(_name == "lowboost")
  {
    mpf.setLowBoost(selectItem->data);
    
  }else if(_name == "hiboost")
  {
    mpf.setHiBooset(selectItem->data);
    
  }else if(_name == "topMenu")
  {
    if(selectItem->menu_name == "test")
    {
       mpf.setData(5);
    } 
  }
  
}


void drawMenu(void) {
  if (is_home == 1)
  {
    drawHomePage();
    return ;
  }
  
  uint8_t i, j, k, h;
  u8g_uint_t w, d;

  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();

  h = u8g.getFontAscent() - u8g.getFontDescent();
  w = u8g.getWidth();

  char menu_count = current_menu->sub_count;
  MenuItem *frist = &(current_menu->subMenu[0]);
  i = 0, j = -1;
  k = ((current_menu->curr) / 6) * 6; //计算从第几屏开始

  for (MenuItem *item = frist; item < frist + menu_count; item++)
  {
    j++;
    if (j < k) {
      continue;
    }

    char *str = item->str;
    d = (w - u8g.getStrWidth(str)) / 2;
    u8g.setDefaultForegroundColor();
    if ( i == current_menu->curr % 6) {
      u8g.drawBox(0, i * h + 1, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(d, i * h, str);
    i++;
  }
}

void updateMenu(void) {
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) {
    return;
  }
  last_key_code = uiKeyCode;

  MenuItem *selectItem = (*current_menu).subMenu + current_menu->curr;

  switch ( uiKeyCode ) {
    case KEY_NEXT:
      if (is_home == 0)
      {
        current_menu->curr++;
        if ( current_menu->curr >= current_menu->sub_count )
        {
          current_menu->curr = 0;
        }
        chkSelectMenu();
      } else {
        setVol(true); //增加音量
      }
      break;
      
    case KEY_PREV:
      if (is_home == 0)
      {
        if ( current_menu->curr == 0 )
        {
          current_menu->curr = current_menu->sub_count;
        }
        current_menu->curr--;
        chkSelectMenu();
      } else {
        setVol(false); //减小音量
      }
      break;
      
    case KEY_SELECT:
      if (is_home == 1)
      { current_menu = &topMenu;
        is_home = 0;
      }
      else if (selectItem != NULL && selectItem->sub_count > 0)
      {
        current_menu = selectItem;
      }
      //检查选中菜单 选中的值
      chkCurrMenu();
      break;
      
    case KEY_BACK:
      if (selectItem == NULL || current_menu->supMenu == NULL)
      { 
        is_home = 1; 
      } else {
        current_menu = current_menu->supMenu;
      }

      break;
  }
  menu_redraw_required = 1;
}

void setup() {
  // rotate screen, if required
   u8g.setRot180();
   Serial.begin(9600);
   pinMode(uiKeyPrev, INPUT_PULLUP);           // set pin to input with pullup
   pinMode(uiKeyNext, INPUT_PULLUP);           // set pin to input with pullup
   pinMode(uiKeySelect, INPUT_PULLUP);           // set pin to input with pullup
   pinMode(uiKeyBack, INPUT_PULLUP);           // set pin to input with pullup
   pinMode(ledLight, OUTPUT);
   analogWrite(ledLight, 1000);
   makeUpMenu();
   
   mpf.init(500); 
  
   menu_redraw_required = 1;     // force initial redraw
}

void loop() {
  uiStep();// check for key press
  if (  menu_redraw_required != 0 ) {
    u8g.firstPage();
    do  {
      drawMenu();
    } while ( u8g.nextPage() );
    menu_redraw_required = 0;
  }
  updateMenu();       // update menu bar
}

//true add ,false sub
void setVol(bool b)
{
  int volSetting = mpf.getVolume();
  if (b)
  {
    volSetting++;
  } else
  {
    volSetting--;
  }
  if (volSetting < 0)
  {
    volSetting = 0;
  }

  if (volSetting > 31)
  {
    volSetting = 31;
  }  
  mpf.setVolume(volSetting);
} 
 
