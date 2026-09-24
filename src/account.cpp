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
        for (int i=0;i<card_list.size();i++){
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