#include "account.h"
#include "db.h"

Account::Account(){
    init_money_bag();
}

void Account::reset(){
    password.clear();
    name.clear();
    card_list.clear();
    registred = false;
    db_id_ = -1;
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

// ==================
// Работа с БД
// ==================

bool Account::dbRegister(Database& db, const std::string& name_, const std::string& password_){
    if (!db.ok()) return false;

    Database::Result res = db.exec(
        "INSERT INTO accounts(name, password) VALUES ("
        + db.quote(name_) + ", " + db.quote(password_) + ") "
        "ON CONFLICT (name) DO NOTHING RETURNING id;");

    if (!res.ok || res.empty()) return false;

    card_list.clear();
    db_id_    = std::stoi(res.scalar());
    name      = name_;
    password  = password_;
    registred = true;

    init_money_bag();
    dbSaveMoneyBag(db);
    return true;
}

bool Account::dbLogin(Database& db, const std::string& name_, const std::string& password_){
    if (!db.ok()) return false;

    Database::Result acc = db.exec(
        "SELECT id FROM accounts WHERE name = " + db.quote(name_)
        + " AND password = " + db.quote(password_) + ";");

    if (!acc.ok || acc.empty()) return false;

    reset();
    db_id_    = std::stoi(acc.scalar());
    name      = name_;
    password  = password_;
    registred = true;

    Database::Result cards = db.exec(
        "SELECT card_num, cvv, data, bank, pin_code, balance FROM cards "
        "WHERE account_id = " + std::to_string(db_id_) + " ORDER BY id;");

    if (cards.ok) {
        for (const auto& row : cards.rows) {
            Card card;
            card.restore(row[0], row[3], row[1], row[2],
                         row[5].empty() ? 0 : std::stoll(row[5]), row[4]);
            card_list[card.getCardNum()] = card;
        }
    }

    dbLoadMoneyBag(db);
    return true;
}

bool Account::dbAddCard(Database& db, const Card& card){
    if (!db.ok() || db_id_ < 0) return false;

    Database::Result res = db.exec(
        "INSERT INTO cards(account_id, card_num, cvv, data, bank, pin_code, balance) VALUES ("
        + std::to_string(db_id_) + ", "
        + db.quote(card.getCardNum()) + ", "
        + db.quote(card.getCVV()) + ", "
        + db.quote(card.getData()) + ", "
        + db.quote(card.getBankName()) + ", "
        + db.quote(card.getPin()) + ", "
        + std::to_string(card.chekBalance()) + ");");

    return res.ok;
}

bool Account::dbUpdateBalance(Database& db, const std::string& card_num, long long balance){
    if (!db.ok()) return false;

    Database::Result res = db.exec(
        "UPDATE cards SET balance = " + std::to_string(balance)
        + " WHERE card_num = " + db.quote(card_num) + ";");

    return res.ok;
}

bool Account::dbLoadMoneyBag(Database& db){
    if (!db.ok() || db_id_ < 0) return false;

    Database::Result res = db.exec(
        "SELECT nominal, count FROM money_bag WHERE account_id = "
        + std::to_string(db_id_) + ";");

    if (!res.ok) return false;

    // Нет записей (например, старый аккаунт) — сохраняем кошелёк по умолчанию.
    if (res.empty()) return dbSaveMoneyBag(db);

    for (const auto& row : res.rows) {
        money_bag[std::stoi(row[0])] = std::stoi(row[1]);
    }
    return true;
}

bool Account::dbSaveMoneyBag(Database& db){
    if (!db.ok() || db_id_ < 0) return false;

    std::string sql = "INSERT INTO money_bag(account_id, nominal, count) VALUES ";
    size_t i = 0;
    for (const auto& [nominal, cnt] : money_bag) {
        if (i++) sql += ", ";
        sql += "(" + std::to_string(db_id_) + ", "
             + std::to_string(nominal) + ", " + std::to_string(cnt) + ")";
    }
    sql += " ON CONFLICT (account_id, nominal) DO UPDATE SET count = EXCLUDED.count;";

    return db.exec(sql).ok;
}

bool Account::dbAddTransaction(Database& db, const std::string& card_num,
                               const std::string& type, long long amount,
                               const std::string& detail){
    if (!db.ok() || db_id_ < 0) return false;

    Database::Result res = db.exec(
        "INSERT INTO transactions(account_id, card_num, type, amount, detail) VALUES ("
        + std::to_string(db_id_) + ", "
        + db.quote(card_num) + ", "
        + db.quote(type) + ", "
        + std::to_string(amount) + ", "
        + db.quote(detail) + ");");

    return res.ok;
}

std::vector<TransactionHistoryEntry> Account::getTransactionHistory(Database& db){
    std::vector<TransactionHistoryEntry> result;
    if (!db.ok() || db_id_ < 0) return result;

    Database::Result res = db.exec(
        "SELECT to_char(created_at, 'DD.MM.YYYY HH24:MI'), type, card_num, amount, detail "
        "FROM transactions WHERE account_id = " + std::to_string(db_id_)
        + " ORDER BY id DESC;");

    if (!res.ok) return result;

    for (const auto& row : res.rows) {
        TransactionHistoryEntry entry;
        entry.created_at = row[0];
        entry.type       = row[1];
        entry.card_num   = row[2];
        entry.amount     = row[3].empty() ? 0 : std::stoll(row[3]);
        entry.detail     = row[4];
        result.push_back(std::move(entry));
    }
    return result;
}

