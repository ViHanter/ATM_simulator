#include "bank.h"

void Bank_account::setName(std::string name_) {name = name_;}
void Bank_account::setBalance(int cash){balance = cash;}
int Bank_account::getBalance(){return balance;}