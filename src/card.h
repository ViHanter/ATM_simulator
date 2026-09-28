#pragma once

#include <string>
#include <random>
#include <format>
#include "bank.h"

class Card {
    private:
        std::string card_num;
        Bank_account bank;
        std::string pin_code;
        std::string cvv;
        std::string data;
        bool isRegistered = false;

        static std::mt19937& rng();
        static std::string generateDigits(int digitsCnt);
    public:
        void regCard(BankName           bank_,
                     const std::string& data_,
                     const std::string& pin_code_);

        const std::string& getCardNum() const { return card_num; }
        const Bank_account& getBank() const { return bank; }
        std::string getBankName() {return bank.getName();}
        const std::string& getPin() const { return pin_code; }
        const std::string& getCVV() const { return cvv; }
        const std::string& getData() const { return data; }
        bool registered() const { return isRegistered; }

        long long chekBalance() {return bank.getBalance();}

        bool checkPin(const std::string& input) const { return input == pin_code; }
        std::string getCardInfo() const;
};