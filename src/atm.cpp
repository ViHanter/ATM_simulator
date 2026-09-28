#include "atm.h"

void ATM::insertCard(const Card& card_, const std::string& pin_code_){
    if (!card_.registered()) {}
    if (isCardInsert) {} //TODO: обработать ошибки
    if (!card_.checkPin(pin_code_)) {}

    isCardInsert = true;
    card = card_;  
}
void ATM::eraseCard(){
    isCardInsert = false;
    card = Card();
}

long long ATM::getBalance(){
    if (!isCardInsert) {std::cout << "ATM_SYS: Insert card.\n" ; return -1;}
    
    return card.chekBalance();
}

void ATM::log(Account& account_,const std::string& pass){
    if (account_.GetPass() == pass) {
        account = account_;
        std::cout << "ATM_SYS: Succesful log.\n";
    } else std::cout << "ATM_SYS: Wrong password.\n";
}

std::string ATM::getStatus(){
    std::string result = "Account: " + account.GetName() + '\n';
    if (isCardInsert) {
        result += "Card: " + card.getCardNum() + '\n' + "isCardInsert: True";
    } else result += "isCardInsert: False";

    return result;
}