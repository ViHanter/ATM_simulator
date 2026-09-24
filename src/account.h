#pragma once

#include <string>
#include <vector>
#include <random>
#include <iostream>
#include "card.h"

class Account{
    private:
        std::string password;
        std::vector<Card> card_list;
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
};
