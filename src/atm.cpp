#include "atm.h"
#include <vector>

ATM::ATM(){
    init_money_bag();
}

void ATM::init_money_bag(){
    atm_money_bag[50] = 500;
    atm_money_bag[100] = 500;
    atm_money_bag[200] = 500;
    atm_money_bag[500] = 500;
    atm_money_bag[1000] = 500;
    atm_money_bag[2000] = 500;
    atm_money_bag[5000] = 500;
}

int ATM::insertCard(Card& card_, const std::string& pin_code_){
    if (!card_.registered()) {return 2;}
    if (!card_.checkPin(pin_code_)) {return 1;}

    if (isCardInsert) eraseCard();

    isCardInsert = true;
    card = &card_;
    return 0;
}
void ATM::eraseCard(){
    isCardInsert = false;
    card = nullptr;
}

long long ATM::getBalance(){
    if (!isCardInsert || !card) {return -1;}

    return card->chekBalance();
}

void ATM::getCard(Card& card_){
    if (card) card_ = *card;
}

void ATM::log(Account& account_,const std::string& pass){
    if (account_.GetPass() == pass) {
        account = &account_;
    }
}

bool ATM::isLogged(){
    return account != nullptr && account->isRegistred();
}

std::string ATM::getStatus(){
    std::string account_name = account ? account->GetName() : "неавторизован";
    std::string result = "Account: " + account_name + '\n';
    if (isCardInsert && card) {
        result += "Card: " + card->getCardNum() + '\n' + "isCardInsert: True";
    } else result += "isCardInsert: False";

    return result;
}
long long ATM::put_money_transaction(int type_money){
    // купюра вставлена в приёмник, но ещё не проверена
    transaction_map[type_money]++;
    return type_money;
}

long long ATM::topup_account_transaction(){
    if (!isCardInsert || !card) return 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> roll_luck(1, 100);
    long long accepted = 0;

    for (const auto&[type,cnt] : transaction_map){
        if (account) account->take_banknotes(type, cnt); // купюры достаются из кошелька владельца
        for (int i = 0; i < cnt; ++i){
            if (roll_luck(gen) < 6){ // 5% - купюра не распознана
                transaction_error_map[type] += 1;
                if (account) account->add_banknotes(type, 1); // непринятая купюра возвращается
            } else {
                accepted += type;
                atm_money_bag[type] += 1; // принятая купюра попадает в кассету банкомата
            }
        }
    }
    transaction_map.clear();

    if (accepted > 0) card->push_request_tupup(accepted);
    return accepted;
}
void ATM::get_rejected_banknotes(std::string& rejected){
    for (auto[type,cnt] : transaction_error_map){
        rejected += std::to_string(type) + "x" + std::to_string(cnt) + ' ';
    }
    transaction_error_map.clear();
}


int ATM::get_banknote_cnt(int type_money){
    return transaction_map[type_money];
}

bool ATM::pick_banknotes(long long money, std::map<int,int>& out){
    out.clear();

    if (money <= 0) return false;
    if (money % 50 != 0) return false;

    long long total = 0;
    for (const auto&[type,cnt] : atm_money_bag){
        total += 1LL * type * cnt;
    }
    if (money > total) return false;

    // can[s]  - сумму s можно собрать из купюр
    // from[s] - предыдущая сумма на пути к s
    // bill[s] - купюра, которой получили сумму s
    std::vector<bool> can(money + 1, false);
    std::vector<long long> from(money + 1, -1);
    std::vector<int> bill(money + 1, -1);
    can[0] = true;

    // сначала крупные номиналы - чтобы предпочтение отдавалось меньшему числу купюр
    for (auto it = atm_money_bag.rbegin(); it != atm_money_bag.rend(); ++it){
        const auto&[type,avail] = *it;
        // used[s] - сколько купюр данного номинала на пути к сумме s
        std::vector<int> used(money + 1, 0);
        for (long long s = type; s <= money; ++s){
            if (can[s] || !can[s - type]) continue;
            if (used[s - type] >= avail) continue;

            can[s] = true;
            used[s] = used[s - type] + 1;
            from[s] = s - type;
            bill[s] = type;
        }
    }
    if (!can[money]) return false;

    for (long long s = money; s > 0; s = from[s]){
        out[bill[s]]++;
    }

    for (const auto&[type,cnt] : out){
        if (cnt > atm_money_bag.at(type)) {
            out.clear();
            return false;
        }
    }
    return true;
}

bool ATM::can_withdraw(long long money){
    std::map<int,int> tmp;
    return pick_banknotes(money, tmp);
}

WithdrawResult ATM::withdrew_transaction(long long money){
    if (!isCardInsert || !card) return WithdrawResult::NoCard;

    if (money <= 0 || money % 50 != 0) return WithdrawResult::InvalidAmount;
    if (card->chekBalance() < money) return WithdrawResult::NotEnoughBalance;

    std::map<int,int> notes;
    if (!pick_banknotes(money, notes)) return WithdrawResult::NotEnoughCash;

    for (const auto&[type,cnt] : notes){
        atm_money_bag[type] -= cnt;
        if (account) account->add_banknotes(type, cnt); // выданные купюры уходят владельцу
    }

    card->withdrew(money);
    dispensed_map = notes;
    return WithdrawResult::Success;
}

void ATM::get_dispensed_banknotes(std::string& dispensed){
    for (const auto&[type,cnt] : dispensed_map){
        dispensed += std::to_string(type) + "x" + std::to_string(cnt) + ' ';
    }
    dispensed_map.clear();
}

std::string withdrawResultText(WithdrawResult result){
    switch (result){
        case WithdrawResult::Success:
            return "Успешно";
        case WithdrawResult::NoCard:
            return "Карта не вставлена.";
        case WithdrawResult::InvalidAmount:
            return "Некорректная сумма (кратно 50).";
        case WithdrawResult::NotEnoughBalance:
            return "Недостаточно средств на карте.";
        case WithdrawResult::NotEnoughCash:
            return "В банкомате нет нужных купюр.";
    }
    return "Неизвестная ошибка.";
}