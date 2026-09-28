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
    card_list[card.getCardNum()] = card;
}
void Account::printInfo(){
    std::cout << "Name: " << name << "\n"
            << "Password: " << password << "\n"
            << "Card list: \n";
    if (card_list.size() != 0) {
        for (const auto&[card_num,card] : card_list){
            std::cout << card.getCardInfo() << "\n";
        }
    } else std::cout << "WTH, u're looser, bruh.\n";
}
std::string Account::GetPass(){
    return password;
}
std::string Account::GetName(){
    return name;
}
std::map<std::string,Card> Account::getCard_map(){
    return card_list;
}
std::string Account::getCard_list(){
    std::string result = "";
    size_t i = 0;
    for (const auto&[card_num,card] : card_list) {
        if (i++ != card_list.size()-1){
            result += card.getCardNum();
            result += " | ";
        } else result += card.getCardNum();
    }

    if (result == "") {result = "У вас нет карт.";}
    return result;
}

Card Account::regNewCard(BankName Bankname_,std::string& data_ ,std::string& pincode_){
    Card new_card;
    new_card.regCard(Bankname_,data_,pincode_);
    return new_card;
}