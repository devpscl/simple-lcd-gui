#ifndef SIMPLE_LCD_GUI_SRC_LCDGUIDEF_H_
#define SIMPLE_LCD_GUI_SRC_LCDGUIDEF_H_

//Force to use i2c lcd library (LiquidCrystal_I2C.h)
//#define LCD_OVERRIDE_I2C
//
//Force to use default lcd library (LiquidCrystal.h)
//#define LCD_OVERRIDE_DEFAULT
//

#if (__has_include("LiquidCrystal_I2C.h") && (!defined(LCD_OVERRIDE_DEFAULT))) || defined(LCD_OVERRIDE_I2C)
#define LCD_I2C_ 1
#include <LiquidCrystal_I2C.h>
#else
#define LCD_DEFAULT_ 1
#include <LiquidCrystal.h>
#endif

namespace lcdgui {

#define BIT_INUM(x,bit) ((x & bit) ? 1 : 0)

#define LCD_INPUT_UP              0x0
#define LCD_INPUT_DOWN            0x1
#define LCD_INPUT_LEFT            0x2
#define LCD_INPUT_RIGHT           0x3
#define LCD_INPUT_NEXT            0x4
#define LCD_INPUT_BACK            0x5

#define LCD_INPUT_OK              LCD_INPUT_NEXT
#define LCD_INPUT_CANCEL          LCD_INPUT_BACK

#define EVENT_SEND                0
#define EVENT_CANCELLED           1

#if defined(LCD_DEFAULT_)
typedef LiquidCrystal* lcd_native_type;
#elif defined(LCD_I2C_)
typedef LiquidCrystal_I2C* lcd_native_type;
#endif

#define LCD_CHAR_SPACE          ' '
#define LCD_CHAR_ARROW_UP       '\u0001'
#define LCD_CHAR_ARROW_DOWN     '\u0002'
#define LCD_CHAR_ARROW_RIGHT    '\u0003'

class LiquidCrystalGui;

class GuiDialog;
class MenuDialog;

typedef GuiDialog *gui_dialog;
typedef MenuDialog *menu_dialog;

typedef void (*action_event_t)();

typedef void (*counter_change_event_t)(int16_t value, int8_t offset);

typedef void (*option_change_event_t)(uint8_t index);

typedef void (*result_choose_event_t)(uint8_t option);

typedef bool (*input_event_t)(uint8_t input);

struct DisplayInfo {
  const uint8_t columns;
  const uint8_t rows;
  const uint8_t char_size;
};


#define LCD_DATA_ARROW_UP { \
0x04, \
0x0E, \
0x1F, \
0x00, \
0x00, \
0x00, \
0x00, \
0x00  \
}     \

#define LCD_DATA_ARROW_DOWN { \
0x00, \
0x00, \
0x00, \
0x00, \
0x00, \
0x1F, \
0x0E, \
0x04  \
}     \

#define LCD_DATA_ARROW_RIGHT { \
0x00, \
0x04, \
0x02, \
0x1F, \
0x02, \
0x04, \
0x00, \
0x00  \
}     \

template<typename ...Args>
constexpr size_t vaCount(Args&&...) { return sizeof...(Args); }

}

#endif //SIMPLE_LCD_GUI_SRC_LCDGUIDEF_H_
