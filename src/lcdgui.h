#ifndef SIMPLE_LCD_MENU_SRC_LCDGUI_H_
#define SIMPLE_LCD_MENU_SRC_LCDGUI_H_
#include "lcddialog.h"
#include "lcdmenu.h"
#include "lcdcore.h"

namespace lcdgui {

#define MENU_BUILDER(...) new MenuDialog(new menu_item[ \
lcdgui::vaCount(__VA_ARGS__)]{__VA_ARGS__}, lcdgui::vaCount(__VA_ARGS__))
#define CHILD_MENU_BUILDER(parent,...) new MenuDialog(parent, new menu_item[ \
lcdgui::vaCount(__VA_ARGS__)]{__VA_ARGS__}, lcdgui::vaCount(__VA_ARGS__))
#define OPTION_BUILDER(...) new OptionList(new const char*[lcdgui::vaCount(__VA_ARGS__)]{__VA_ARGS__}, \
lcdgui::vaCount(__VA_ARGS__))

#define DUMMY_MENU_ITEM(text) new DummyMenuItem(text)
#define ACTION_MENU_ITEM(text,action) new ActionMenuItem(text, action)
#define COUNTER_MENU_ITEM(text,value,min,max) new CounterMenuItem(text,value,min,max)
#define COUNTER_MENU_ITEM_EXTENDED(text,value,min,max,style,event) \
new CounterMenuItem(text,value,min,max,style,event)
#define OPTION_MENU_ITEM(text,option_list) new OptionMenuItem(text,option_list)
#define OPTION_MENU_ITEM_EXTENDED(text,option_list,style,event) new OptionMenuItem(text,option_list,style,event)

#define DUMMY_MENU_ITEM_VA(...) new DummyMenuItem(__VA_ARGS__)
#define ACTION_MENU_ITEM_VA(...) new ActionMenuItem(__VA_ARGS__)
#define COUNTER_MENU_ITEM_VA(...) new CounterMenuItem(__VA_ARGS__)
#define OPTION_MENU_ITEM_VA(...) new OptionMenuItem(__VA_ARGS__)

#define GET_ITEM(dialog,index,type) (reinterpret_cast<type>((reinterpret_cast<menu_dialog>(dialog))->itemAt(index)))

#define RESULT_DIALOG(message,options,event) new ResultDialog(message,options,event)
#define CHILD_RESULT_DIALOG(parent,message,options,event) new ResultDialog(parent,message,options,event)

#define INPUT_DIALOG(message,event,input) new InputDialog(message,event,input)
#define CHILD_INPUT_DIALOG(parent,message,event,input) new InputDialog(parent,message,event,input)

}

#endif //SIMPLE_LCD_MENU_SRC_LCDGUI_H_
