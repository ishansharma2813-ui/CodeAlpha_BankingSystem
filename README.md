# CodeAlpha_BankingSystem

Console-based Banking System in C++ built for the **CodeAlpha C++ Programming Internship (Task 4)**.

## OOP design
| Class | Responsibility |
|-------|----------------|
| `Transaction` | One record: id, type, amount, balance after, timestamp |
| `Account` | Balance, deposit/withdraw with validation, transaction history |
| `Customer` | Customer details and the accounts they own |
| `Bank` | Creates customers/accounts, handles transfers, shows account info |

## Features
- Create customers and open accounts (a customer can have multiple accounts)
- Deposit, withdraw, and transfer funds between accounts
- Validation: invalid amounts, insufficient balance, unknown accounts, same-account transfer
- Transaction history with the last 5 transactions shown per account
- Account information view (holder, phone, balance)

## Run
```bash
g++ -std=c++17 -o bank main.cpp
./bank
```

## Author
Ishan Sharma
