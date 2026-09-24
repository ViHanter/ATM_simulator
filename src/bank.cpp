#include "bank.h"

void Bank::setName(std::string name_) {name = name_;}
void Bank::setBalance(int cash){balance = cash;}
int Bank::getBalance(){return balance;}