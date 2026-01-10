#ifndef SIMPLE_LCD_GUI_SRC_LCDGUIDEF_H_
#define SIMPLE_LCD_GUI_SRC_LCDGUIDEF_H_

//Force to use i2c lcd library (LiquidCrystal_I2C.h)
//#define LCD_OVERRIDE_I2C
//
//Force to use default lcd library (LiquidCrystal.h)
//#define LCD_OVERRIDE_DEFAULT
//
//Force to use U8g2lib lcd library (U8g2lib.h)
//#define LCD_OVERRIDE_U8G2
//

#if (__has_include("LiquidCrystal_I2C.h") && !defined(LCD_OVERRIDE_DEFAULT) && !defined(LCD_OVERRIDE_U8G2)) \
|| defined(LCD_OVERRIDE_I2C)
#define LCD_I2C_ 1
#include <LiquidCrystal_I2C.h>
#elif (__has_include("U8g2lib.h") && !defined(LCD_OVERRIDE_DEFAULT) && !defined(LCD_OVERRIDE_I2C)) \
|| defined(LCD_OVERRIDE_U8G2)
#define LCD_U8G2_ 1
#include "lcdu8g2.h"
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

#define EVENT_SEND                false
#define EVENT_CANCELLED           true

#if defined(LCD_DEFAULT_)
typedef LiquidCrystal* lcd_native_type;
#elif defined(LCD_I2C_)
typedef LiquidCrystal_I2C* lcd_native_type;
#elif defined(LCD_U8G2_)
typedef LiquidCrystalU8GLIB2* lcd_native_type;
#endif

#define LCD_CHAR_SPACE          ' '
#define LCD_CHAR_ARROW_UP       '\u0001'
#define LCD_CHAR_ARROW_DOWN     '\u0002'
#define LCD_CHAR_ARROW_RIGHT    '\u0003'

#define DIALOG_DO_NOTHING       0
#define DIALOG_ONLY_CLOSE       1
#define DIALOG_DISPOSE          2

#define DIALOG_EVENT_CLOSE       0
#define DIALOG_EVENT_OPEN        1

class LiquidCrystalGui;

class GuiDialog;
class MenuDialog;
class ResultDialog;
class InputDialog;

typedef GuiDialog* gui_dialog;
typedef MenuDialog* menu_dialog;
typedef ResultDialog* result_dialog;
typedef InputDialog* input_dialog;

typedef void (*action_event_t)();

typedef void (*counter_change_event_t)(int16_t value, int8_t offset);

typedef void (*option_change_event_t)(uint8_t index);

typedef int (*result_choose_event_t)(uint8_t option);

typedef bool (*input_event_t)(uint8_t input);

typedef void (*dialog_event_t)(gui_dialog dialog,uint8_t event);

struct DisplayInfo {
  const uint8_t columns;
  const uint8_t rows;
  const uint8_t char_size;
};

#define LCD_DATA_ARROW_UP_5x8 { \
0x04, \
0x0E, \
0x1F, \
0x00, \
0x00, \
0x00, \
0x00, \
0x00  \
}

#define LCD_DATA_ARROW_DOWN_5x8 { \
0x00, \
0x00, \
0x00, \
0x00, \
0x00, \
0x1F, \
0x0E, \
0x04  \
}

#define LCD_DATA_ARROW_RIGHT_5x8 { \
0x00, \
0x04, \
0x02, \
0x1F, \
0x02, \
0x04, \
0x00, \
0x00  \
}


#define LCD_DATA_ARROW_UP_6x9 { \
0b000000, \
0b001100, \
0b011110, \
0b011110, \
0b111111, \
0b000000, \
0b000000, \
0b000000, \
0b000000  \
}

#define LCD_DATA_ARROW_DOWN_6x9 { \
0b000000, \
0b000000, \
0b000000, \
0b000000, \
0b111111, \
0b011110, \
0b011110, \
0b001100, \
0b000000  \
}

#define LCD_DATA_ARROW_RIGHT_6x9 { \
0b000000, \
0b001000, \
0b000100, \
0b000010, \
0b011111, \
0b000010, \
0b000100, \
0b001000, \
0b000000  \
}

#define LCD_DATA_ARROW_UP_6x11 { \
0b000000, \
0b001100, \
0b001100, \
0b011110, \
0b011110, \
0b111111, \
0b000000, \
0b000000, \
0b000000, \
0b000000, \
0b000000 \
}

#define LCD_DATA_ARROW_DOWN_6x11 { \
0b000000, \
0b000000, \
0b000000, \
0b000000, \
0b000000, \
0b111111, \
0b011110, \
0b011110, \
0b001100, \
0b001100, \
0b000000 \
}

#define LCD_DATA_ARROW_RIGHT_6x11 { \
0b000000, \
0b000000, \
0b001000, \
0b000100, \
0b000010, \
0b011111, \
0b000010, \
0b000100, \
0b001000, \
0b000000, \
0b000000  \
}

template<typename ...Args>
constexpr size_t vaCount(Args&&...) { return sizeof...(Args); }

}

#endif //SIMPLE_LCD_GUI_SRC_LCDGUIDEF_H_
