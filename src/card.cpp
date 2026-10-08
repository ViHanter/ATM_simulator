#include "card.h"

std::mt19937& Card::rng() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

std::string Card::generateDigits(int digitsCnt) {
    std::string result;
    std::uniform_int_distribution<int> dist_num(0, 9);
    for (int i = 0; i < digitsCnt; ++i)
        result += std::to_string(dist_num(rng()));
    return result;
}

void Card::regCard(BankName           bank_,
                   const std::string& data_,
                   const std::string& pin_code_)
{
    if (isRegistered) return;

    switch (bank_) {
        case BankName::RofloBank:
            bank_name = "RofloBank";
            card_num = "6767";
            break;
        case BankName::SbeerBank:
            bank_name = "SbeerBank";
            card_num = "9999";
            break;
        case BankName::Tankoff:
            bank_name = "Tankoff";
            card_num = "1444";
            break;
        case BankName::MusorBank:
            bank_name = "MusorBank";
            card_num = "7777";
            break;
    }

    card_num += generateDigits(6);
    data      = data_;
    cvv       = generateDigits(3);
    pin_code  = pin_code_;
    isRegistered = true;
    balance = 0;
}
std::string Card::getCardInfo() const {
    if (!isRegistered) return "Not registered.";
    std::string result = card_num + " | " + data + " | " + cvv;
    return result;
}

int Card::push_request_tupup(long long money){
    //TODO: мб шанс ошибки пополнения
    balance += money*100;
    return 0;
}