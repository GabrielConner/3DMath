#include "globalScripts.h"

void dynamicDropDownFunction(void* val) {
	std::pair<DropDownFieldDynamic*, std::string>* pair = (std::pair<DropDownFieldDynamic*, std::string>*)val;
	pair->first->selectOption(pair->second);
}

void dynamicDropDownDelete(void* val) {
	std::pair<DropDownFieldDynamic*, std::string>* pair = (std::pair<DropDownFieldDynamic*, std::string>*)val;
    pair->second.~basic_string();
	delete(pair);
}

void staticDropDownFunction(void* val) {
    std::pair<DropDownFieldStatic*, std::string>* pair = (std::pair<DropDownFieldStatic*, std::string>*)val;
    pair->first->selectOption(pair->second);
}

void staticDropDownDelete(void* val) {
    std::pair<DropDownFieldStatic*, std::string>* pair = (std::pair<DropDownFieldStatic*, std::string>*)val;
    pair->second.~basic_string();
    delete(pair);
}