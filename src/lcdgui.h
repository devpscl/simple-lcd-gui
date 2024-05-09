#ifndef SIMPLE_LCD_MENU_SRC_LCDGUI_H_
#define SIMPLE_LCD_MENU_SRC_LCDGUI_H_
#include "lcddialog.h"
#include "lcdmenu.h"
#include "lcdcore.h"

namespace lcdgui {

#define MENU_BUILDER(...) new MenuDialog(new menu_item[ \
lcdgui::vaCount(__VA_ARGS__)]{__VA_ARGS__}, lcdgui::vaCount(__VA_ARGS__))
#define OPTION_BUILDER(...) new OptionList(new String[lcdgui::vaCount(__VA_ARGS__)]{__VA_ARGS__}, \
lcdgui::vaCount(__VA_ARGS__))

#define DUMMY_MENU_ITEM(text) new DummyMenuItem(text)
#define ACTION_MENU_ITEM(text,action) new ActionMenuItem(text, action)
#define COUNTER_MENU_ITEM(text,value,min,max) new CounterMenuItem(text,value,min,max)
#define COUNTER_MENU_ITEM_EXTENDED(text,value,min,max,style,event) \
new CounterMenuItem(text,value,min,max,style,event)
#define OPTION_MENU_ITEM(text,option_list) new OptionMenuItem(text,option_list)
#define OPTION_MENU_ITEM_EXTENDED(text,option_list,style,event) new OptionMenuItem(text,option_list,style,event)



}

#endif //SIMPLE_LCD_MENU_SRC_LCDGUI_H_
