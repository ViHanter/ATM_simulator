#pragma once

#include <string>
#include <random>
#include <format>

enum class BankName {
    RofloBank,
    SbeerBank,
    Tankoff,
    MusorBank
};

class Card {
    private:
        std::string card_num;
        std::string bank_name;
        long long balance = 0;
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
        std::string getBankName() {return bank_name;}
        const std::string& getPin() const { return pin_code; }
        const std::string& getCVV() const { return cvv; }
        const std::string& getData() const { return data; }
        bool registered() const { return isRegistered; }

        long long chekBalance() {return balance;}

        bool checkPin(const std::string& input) const { return input == pin_code; }
        int push_request_tupup(long long money);
        std::string getCardInfo() const;
        void withdrew(long long money);
};