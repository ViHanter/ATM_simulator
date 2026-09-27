#include "account.h"

void Account::regAccount(
    const std::string& name_,
    const std::string& password_) 
    {
        if (!registred){
            name = name_;
            password = password_;
            registred = true;
        }
    }
void Account::addCard(Card& card){
    card_list.push_back(card);
}
void Account::printInfo(){
    std::cout << "Name: " << name << "\n"
            << "Password: " << password << "\n"
            << "Card list: \n";
    if (card_list.size() != 0) {
        for (size_t i=0;i<card_list.size();i++){
            std::cout << card_list[i].getCardInfo() << "\n";
        }
    } else std::cout << "WTH, u're looser, bruh.\n";
}
std::string Account::GetPass(){
    return password;
}
std::string Account::GetName(){
    return name;
}

std::string Account::getCard_list(){
    std::string result = "";
    size_t i = 0;
    for (const Card& card : card_list) {
        if (i++ != card_list.size()-1){
            result += card.getCardNum();
            result += " | ";
        } else result += card.getCardNum();
    }

    if (result == "") {result = "У вас нет карт.";}
    return result;
}