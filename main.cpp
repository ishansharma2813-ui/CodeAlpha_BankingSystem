// CodeAlpha C++ Internship - Task 4: Banking System
// OOP design: Customer, Account, Transaction + Bank (manager class).
// Features: create customers/accounts, deposit, withdraw, transfer,
// transaction history, account information.

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <limits>

using namespace std;

// ---------- Transaction ----------
class Transaction {
    int id;
    string type;     // Deposit, Withdrawal, Transfer In, Transfer Out
    double amount;
    double balanceAfter;
    string time;

    static string now() {
        time_t t = std::time(nullptr);
        char buf[32];
        strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M:%S", localtime(&t));
        return buf;
    }

public:
    Transaction(int id, const string& type, double amount, double balanceAfter)
        : id(id), type(type), amount(amount), balanceAfter(balanceAfter), time(now()) {}

    void print() const {
        cout << "  #" << setw(3) << id << " | " << time << " | "
             << left << setw(13) << type << right
             << " | " << setw(10) << fixed << setprecision(2) << amount
             << " | Balance: " << balanceAfter << "\n";
    }
};

// ---------- Account ----------
class Account {
    int accountNumber;
    int ownerId;
    double balance;
    vector<Transaction> history;
    int nextTxnId;

public:
    Account(int accNo, int ownerId)
        : accountNumber(accNo), ownerId(ownerId), balance(0.0), nextTxnId(1) {}

    int getAccountNumber() const { return accountNumber; }
    int getOwnerId() const { return ownerId; }
    double getBalance() const { return balance; }

    bool deposit(double amount, const string& type = "Deposit") {
        if (amount <= 0) return false;
        balance += amount;
        history.emplace_back(nextTxnId++, type, amount, balance);
        return true;
    }

    bool withdraw(double amount, const string& type = "Withdrawal") {
        if (amount <= 0 || amount > balance) return false;
        balance -= amount;
        history.emplace_back(nextTxnId++, type, amount, balance);
        return true;
    }

    void showRecentTransactions(int count = 5) const {
        if (history.empty()) {
            cout << "  No transactions yet.\n";
            return;
        }
        int start = history.size() > (size_t)count ? history.size() - count : 0;
        for (size_t i = start; i < history.size(); i++) history[i].print();
    }
};

// ---------- Customer ----------
class Customer {
    int id;
    string name;
    string phone;
    vector<int> accountNumbers;

public:
    Customer(int id, const string& name, const string& phone)
        : id(id), name(name), phone(phone) {}

    int getId() const { return id; }
    const string& getName() const { return name; }
    const string& getPhone() const { return phone; }
    const vector<int>& getAccounts() const { return accountNumbers; }
    void addAccount(int accNo) { accountNumbers.push_back(accNo); }
};

// ---------- Bank ----------
class Bank {
    vector<Customer> customers;
    vector<Account> accounts;
    int nextCustomerId = 1001;
    int nextAccountNo = 50001;

    Account* findAccount(int accNo) {
        for (auto& a : accounts)
            if (a.getAccountNumber() == accNo) return &a;
        return nullptr;
    }

    Customer* findCustomer(int id) {
        for (auto& c : customers)
            if (c.getId() == id) return &c;
        return nullptr;
    }

public:
    int createCustomer(const string& name, const string& phone) {
        customers.emplace_back(nextCustomerId, name, phone);
        return nextCustomerId++;
    }

    int openAccount(int customerId) {
        Customer* c = findCustomer(customerId);
        if (!c) return -1;
        accounts.emplace_back(nextAccountNo, customerId);
        c->addAccount(nextAccountNo);
        return nextAccountNo++;
    }

    bool deposit(int accNo, double amount) {
        Account* a = findAccount(accNo);
        return a && a->deposit(amount);
    }

    bool withdraw(int accNo, double amount) {
        Account* a = findAccount(accNo);
        return a && a->withdraw(amount);
    }

    // Returns: 0 ok, 1 account not found, 2 invalid amount / insufficient funds, 3 same account
    int transfer(int fromNo, int toNo, double amount) {
        if (fromNo == toNo) return 3;
        Account* from = findAccount(fromNo);
        Account* to = findAccount(toNo);
        if (!from || !to) return 1;
        if (!from->withdraw(amount, "Transfer Out")) return 2;
        to->deposit(amount, "Transfer In");
        return 0;
    }

    void showAccountInfo(int accNo) {
        Account* a = findAccount(accNo);
        if (!a) { cout << "Account not found.\n"; return; }
        Customer* c = findCustomer(a->getOwnerId());
        cout << "\n--- Account Information ---\n";
        cout << "Account No : " << a->getAccountNumber() << "\n";
        cout << "Holder     : " << c->getName() << " (ID " << c->getId() << ")\n";
        cout << "Phone      : " << c->getPhone() << "\n";
        cout << "Balance    : " << fixed << setprecision(2) << a->getBalance() << "\n";
        cout << "\nRecent transactions (last 5):\n";
        a->showRecentTransactions(5);
    }

    void listCustomers() {
        if (customers.empty()) { cout << "No customers yet.\n"; return; }
        cout << "\n--- Customers ---\n";
        for (const auto& c : customers) {
            cout << c.getId() << " | " << c.getName() << " | Accounts:";
            if (c.getAccounts().empty()) cout << " none";
            for (int n : c.getAccounts()) cout << " " << n;
            cout << "\n";
        }
    }
};

// ---------- Input helpers ----------
int readInt(const string& prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x) return x;
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid number.\n";
    }
}

double readDouble(const string& prompt) {
    double x;
    while (true) {
        cout << prompt;
        if (cin >> x) return x;
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid amount.\n";
    }
}

string readLine(const string& prompt) {
    string s;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (true) {
        cout << prompt;
        if (!getline(cin, s)) exit(0);
        if (!s.empty()) return s;
        cout << "This cannot be empty.\n";
    }
}

void printMenu() {
    cout << "\n=========== BANKING SYSTEM ===========\n";
    cout << "1. Create customer\n";
    cout << "2. Open account for customer\n";
    cout << "3. Deposit\n";
    cout << "4. Withdraw\n";
    cout << "5. Transfer funds\n";
    cout << "6. View account info & recent transactions\n";
    cout << "7. List all customers\n";
    cout << "0. Exit\n";
    cout << "======================================\n";
}

int main() {
    Bank bank;
    while (true) {
        printMenu();
        int choice = readInt("Enter choice: ");

        if (choice == 0) {
            cout << "Thank you for banking with us. Goodbye!\n";
            break;
        } else if (choice == 1) {
            string name = readLine("Customer name: ");
            string phone;
            cout << "Phone number: ";
            cin >> phone;
            int id = bank.createCustomer(name, phone);
            cout << "Customer created. Customer ID: " << id << "\n";
        } else if (choice == 2) {
            int cid = readInt("Customer ID: ");
            int acc = bank.openAccount(cid);
            if (acc == -1) cout << "Customer not found.\n";
            else cout << "Account opened. Account number: " << acc << "\n";
        } else if (choice == 3) {
            int acc = readInt("Account number: ");
            double amt = readDouble("Amount to deposit: ");
            cout << (bank.deposit(acc, amt) ? "Deposit successful.\n"
                                             : "Failed: invalid account or amount.\n");
        } else if (choice == 4) {
            int acc = readInt("Account number: ");
            double amt = readDouble("Amount to withdraw: ");
            cout << (bank.withdraw(acc, amt) ? "Withdrawal successful.\n"
                                              : "Failed: invalid account, amount or insufficient balance.\n");
        } else if (choice == 5) {
            int from = readInt("From account number: ");
            int to = readInt("To account number: ");
            double amt = readDouble("Amount to transfer: ");
            switch (bank.transfer(from, to, amt)) {
                case 0: cout << "Transfer successful.\n"; break;
                case 1: cout << "Failed: one of the accounts was not found.\n"; break;
                case 2: cout << "Failed: invalid amount or insufficient balance.\n"; break;
                case 3: cout << "Failed: cannot transfer to the same account.\n"; break;
            }
        } else if (choice == 6) {
            bank.showAccountInfo(readInt("Account number: "));
        } else if (choice == 7) {
            bank.listCustomers();
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
