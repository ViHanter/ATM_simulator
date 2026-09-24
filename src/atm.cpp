#include "atm.h"

void ATM::insertCard(const Card& card_, const std::string& pin_code_){
    if (!card_.registered()) {std::cout << "ATM_SYS: Card not registered.\n" ; return;}
    if (isCardInsert) {std::cout << "ATM_SYS: Another card was inserted.\n";return;}
    if (!card_.checkPin(pin_code_)) {std::cout << "ATM_SYS: Wrong pin.\n";return;}

    isCardInsert = true;
    card = card_;  
}

int ATM::getBalance(){
    if (!isCardInsert) {std::cout << "ATM_SYS: Insert card.\n" ; return -1;}
    Bank card_bank = card.getBank();
    
    return card_bank.getBalance();
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