#include <Arduino.h>
#include <../include/lcdgui.h>
#include <SPI.h>

using namespace lcdgui;

#define PIN_CS 5

uint8_t last_keypad_input = 0;

U8G2_ST7920_128X64_1_SW_SPI u8g(U8G2_R0, 14, 27, 26, 12);

option_list step_option_list = OPTION_BUILDER("1", "2", "5", "10");

LiquidCrystalGui lcd_main(u8g);

uint8_t step = 1;

void eventShow3dCube();
void eventChangeStepValue(uint8_t option);
void eventChangeXScaleValue(int16_t value, int8_t offset);
void eventChangeYScaleValue(int16_t value, int8_t offset);
void eventChangeZScaleValue(int16_t value, int8_t offset);

menu_dialog dialog_main = MENU_BUILDER(
    OPTION_MENU_ITEM_VA("Step", step_option_list, OptionItemStyle::Bracketed, eventChangeStepValue),
    COUNTER_MENU_ITEM_VA("X Scale", 10, 0, 30, OptionItemStyle::Bracketed, eventChangeXScaleValue),
    COUNTER_MENU_ITEM_VA("Y Scale", 10, 0, 30, OptionItemStyle::Bracketed, eventChangeYScaleValue),
    COUNTER_MENU_ITEM_VA("Z Scale", 10, 0, 30, OptionItemStyle::Bracketed, eventChangeZScaleValue),
    ACTION_MENU_ITEM_VA("Show 3D Cube", eventShow3dCube)
);

class RenderDialog : public U8G2Dialog {

  typedef int8_t int_type_;

  struct Point2d {
    int_type_ x;
    int_type_ y;
    Point2d(int_type_ x, int_type_ y) : x(x), y(y) {}
    Point2d() : Point2d(0, 0) {}
  };

  struct Point3d {
    float x;
    float y;
    float z;
    Point3d(float x, float y, float z) : x(x), y(y), z(z) {}
    Point3d() : Point3d(0, 0, 0) {}
  };

  Point3d nodes[8];
  uint8_t edges[24]{
      0, 1,1, 2,2, 3,3, 0,4, 5,5, 6,6, 7,7, 4,
      0, 4,1, 5,2, 6,3, 7
  };

  int_type_ cx = 0;
  int_type_ cy = 0;
  int_type_ cz = 0;

 protected:
  void input(LiquidCrystalGui &lcg, const uint8_t &input) override {
    switch (input) {
      case LCD_INPUT_BACK:
        dispose();
        break;
      case LCD_INPUT_LEFT:
        rotateX(PI / -16);
        updateDisplay();
        break;
      case LCD_INPUT_RIGHT:
        rotateX(PI / 16);
        updateDisplay();
        break;
      case LCD_INPUT_UP:
        rotateY(PI / -16);
        updateDisplay();
        break;
      case LCD_INPUT_DOWN:
        rotateY(PI / 16);
        updateDisplay();
        break;
      default:break;
    }
  }

  void render(U8G2 &u8g2, LiquidCrystalGui &lcg) override {
    u8g2.firstPage();
    do {
      for(uint8_t idx = 0; idx < 12; idx++) {
        Point3d first = nodes[edges[idx * 2]];
        Point3d second = nodes[edges[idx * 2 + 1]];
        auto fx = static_cast<uint8_t>(first.x);
        auto fz = static_cast<uint8_t>(first.z);
        auto sx = static_cast<uint8_t>(second.x);
        auto sz = static_cast<uint8_t>(second.z);
        u8g2.drawLine(fx, fz, sx, sz);
      }
    } while(u8g2.nextPage());
    u8g2.sendBuffer();
  }

 public:

  void set(int_type_ x, int_type_ y, int_type_ z, int_type_ x_scale, int_type_ y_scale, int_type_ z_scale) {
    cx = x;
    cy = y;
    cz = z;
    nodes[0] = Point3d(x + x_scale, y + y_scale, z + z_scale);
    nodes[1] = Point3d(x - x_scale, y + y_scale, z + z_scale);
    nodes[2] = Point3d(x - x_scale, y - y_scale, z + z_scale);
    nodes[3] = Point3d(x + x_scale, y - y_scale, z + z_scale);
    nodes[4] = Point3d(x + x_scale, y + y_scale, z - z_scale);
    nodes[5] = Point3d(x - x_scale, y + y_scale, z - z_scale);
    nodes[6] = Point3d(x - x_scale, y - y_scale, z - z_scale);
    nodes[7] = Point3d(x + x_scale, y - y_scale, z - z_scale);
  }

  void rotateX(const float& theta) {
    double sinX = sin(theta);
    double cosX = cos(theta);
    double x;
    double y;
    for(auto &item : nodes) {
      item.x -= cx;
      item.y -= cy;

      x = item.x * cosX - item.y * sinX;
      y = item.x * sinX + item.y * cosX;

      item.x = x + cx;
      item.y = y + cy;
    }
  }

  void rotateY(const float& theta) {
    double sinY = sin(theta);
    double cosY = cos(theta);
    double y;
    double z;
    for(auto &item : nodes) {
      item.z -= cz;
      item.y -= cy;

      z = item.z * cosY - item.y * sinY;
      y = item.z * sinY + item.y * cosY;

      item.z = z + cz;
      item.y = y + cy;
    }
  }

};

void eventShow3dCube() {
  uint8_t xc = u8g.getWidth()/2;
  uint8_t yc = u8g.getHeight()/2;

  uint8_t scale_x = GET_ITEM(dialog_main, 1, counter_menu_item)->value();
  uint8_t scale_y = GET_ITEM(dialog_main, 2, counter_menu_item)->value();
  uint8_t scale_z = GET_ITEM(dialog_main, 3, counter_menu_item)->value();
  auto* dialog = new RenderDialog();
  dialog->set(xc, 0, yc, scale_x, scale_y, scale_z);
  dialog_main->openChild(dialog);
}

void eventChangeStepValue(uint8_t option) {
  switch (option) {
    case 0:
      step = 1;
      break;
    case 1:
      step = 2;
      break;
    case 2:
      step = 5;
      break;
    case 3:
      step = 10;
      break;
    default:break;
  }
}

void eventChangeXScaleValue(int16_t value, int8_t offset) {
  int16_t new_value = (value - offset) + step * offset;
  GET_ITEM(dialog_main, 1, counter_menu_item)->value(new_value);
}

void eventChangeYScaleValue(int16_t value, int8_t offset) {
  int16_t new_value = (value - offset) + step * offset;
  GET_ITEM(dialog_main, 2, counter_menu_item)->value(new_value);
}

void eventChangeZScaleValue(int16_t value, int8_t offset) {
  int16_t new_value = (value - offset) + step * offset;
  GET_ITEM(dialog_main, 3, counter_menu_item)->value(new_value);
}

void initKeypad();

void setup() {
  initKeypad();
  lcd_main.begin(true);
  lcd_main.openDialog(dialog_main);
}

////////////////////////////////////
// End of important example code
////////////////////////////////////

void initKeypad() {
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  digitalWrite(PIN_CS, HIGH);
  pinMode(PIN_CS, OUTPUT);
}

uint8_t readKeypad() {
  digitalWrite(PIN_CS, LOW);
  delayMicroseconds(25);
  uint8_t value = SPI.transfer(0x00);
  digitalWrite(PIN_CS, HIGH);
  return value;
}

void loop() {
  delay(50);
  uint8_t input = readKeypad();
  if(last_keypad_input == input) {
    return;
  }
  last_keypad_input = input;
  switch (input) {
    case 0x12: //Button up
      lcd_main.dispatchInput(LCD_INPUT_UP);
      break;
    case 0x21: //Button left
      lcd_main.dispatchInput(LCD_INPUT_LEFT);
      break;
    case 0x22: //Button next / ok
      lcd_main.dispatchInput(LCD_INPUT_NEXT);
      break;
    case 0x24: //Button right
      lcd_main.dispatchInput(LCD_INPUT_RIGHT);
      break;
    case 0x42: //Button down
      lcd_main.dispatchInput(LCD_INPUT_DOWN);
      break;
    case 0x81: //Button back / cancel
      lcd_main.dispatchInput(LCD_INPUT_BACK);
      break;
    default:return;
  }

}