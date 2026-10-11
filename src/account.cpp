#include "account.h"

Account::Account(){
    init_money_bag();
}

void Account::init_money_bag(){
    money_bag[50] = 5;
    money_bag[100] = 5;
    money_bag[200] = 5;
    money_bag[500] = 5;
    money_bag[1000] = 5;
    money_bag[2000] = 5;
    money_bag[5000] = 5;
}
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
const std::map<std::string,Card>& Account::getCard_map(){
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

int Account::get_banknote_cnt(int type_money){
    auto it = money_bag.find(type_money);
    return it == money_bag.end() ? 0 : it->second;
}
void Account::take_banknotes(int type_money, int cnt_banknotes){
    int cur = get_banknote_cnt(type_money);
    money_bag[type_money] = (cur > cnt_banknotes) ? cur - cnt_banknotes : 0;
}
void Account::add_banknotes(int type_money, int cnt_banknotes){
    money_bag[type_money] += cnt_banknotes;
}
std::string Account::getMoneyBagList(){
    std::string result;
    for (const auto&[type,cnt] : money_bag){
        if (cnt > 0) result += std::to_string(type) + "x" + std::to_string(cnt) + ' ';
    }
    if (result.empty()) result = "нет купюр";
    return result;
}

