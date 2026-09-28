#pragma once

#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <map>
#include "card.h"

class Account{
    private:
        std::string password;
        std::map<std::string,Card> card_list;
        std::string name;
        bool registred = false;
    
    public:
        void regAccount(
            const std::string& name_,
            const std::string& password_);
        void addCard(Card& card);
        void printInfo();
        std::string GetPass();
        std::string GetName();
        bool isRegistred(){return registred;}

        std::string getCard_list();
        std::map<std::string,Card> getCard_map();
        Card regNewCard(BankName Bankname_,std::string& data_ ,std::string& pincode_);
        bool isCard_listNull() {return card_list.size() == 0;}
};
