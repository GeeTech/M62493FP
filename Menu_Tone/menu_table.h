 
struct MenuItem{
  char* menu_name;
	char *str;
	uint8_t sub_count;
	uint8_t curr;
	MenuItem *subMenu;
	MenuItem *supMenu;
  char data;
	//void (*action)(char opt);
};

//公用开关菜单
MenuItem swichMenu[]{
   {"swich","On",0,0,NULL,NULL,1},
   {"swich","Close",0,0,NULL,NULL,0},
};

//公用tone 值
MenuItem ToneValMenu[]{
  {"","Boost 3",0,0,NULL,NULL,0},
  {"","Boost 2",0,0,NULL,NULL,1},
  {"","Boost 1",0,0,NULL,NULL,2},
  {"","Boost 0",0,0,NULL,NULL,3},
  {"","Cut   0",0,0,NULL,NULL,4},
  {"","Cut   1",0,0,NULL,NULL,5},
  {"","Cut   2",0,0,NULL,NULL,6},
  {"","Cut   3",0,0,NULL,NULL,7},
};

MenuItem topMenu = {"topMenu","", 4, 0, NULL,NULL};

MenuItem MainMenu[]{ 
	{"eq","EQ Setting",5,0,NULL,&topMenu},
	{"boost","Boost Setting",2,0,NULL,&topMenu},
  {"surround","Surround",2,0,&(swichMenu[0]),&topMenu},
  {"test","test bowl",0,0,NULL,&topMenu},
};
 


MenuItem ToneMenu[]{
  {"tone","Tone 1",8,0,&(ToneValMenu[0]),&(MainMenu[0]),0},
  {"tone","Tone 2",8,0,&(ToneValMenu[0]),&(MainMenu[0]),1},
  {"tone","Tone 3",8,0,&(ToneValMenu[0]),&(MainMenu[0]),2},
  {"tone","Tone 4",8,0,&(ToneValMenu[0]),&(MainMenu[0]),3},
  {"tone","Tone 5",8,0,&(ToneValMenu[0]),&(MainMenu[0]),4},
};


MenuItem BoostMenu[]{
  {"lowboost","Low Boost",2,0,&(swichMenu[0]),&(MainMenu[1]),0},
  {"hiboost","Hight Boost",2,0,&(swichMenu[0]),&(MainMenu[1]),1},
};


void makeUpMenu()
{
  topMenu.subMenu = &(MainMenu[0]); 
  MainMenu[0].subMenu = &(ToneMenu[0]);
  MainMenu[1].subMenu = &(BoostMenu[0]);
  
}

MenuItem* getSelectItem(MenuItem *curr)
{
  MenuItem *selectItem = (*curr).subMenu + curr->curr;

  return selectItem;
}

void selectItemBydata(MenuItem *curr,char data)
{
  for(char i =0;i<curr->sub_count;i++)
  {
    MenuItem *selectItem = (*curr).subMenu + i;
    if(selectItem->data == data)
    {
      curr->curr = i;
      break;
    }
  }
}


