#include "lcdmenu.h"

using namespace lcdgui;

lcdgui::OptionList::OptionList(const char** arr, const uint8_t &count) {
  arr_ = arr;
  count_ = count;
}

const char* lcdgui::OptionList::option(const uint8_t &index) {
  return arr_[index];
}

const char* OptionList::option(const uint8_t &index) const {
  return  arr_[index] ;
}

uint8_t lcdgui::OptionList::count() const {
  return count_;
}

const char* lcdgui::OptionList::operator[](const uint8_t &index) {
  return option(index);
}