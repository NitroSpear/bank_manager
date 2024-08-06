#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

// Abstract Base Class (Interface) for Account
class Account {
protected:
    int accountNumber;
    std::string accountHolderName;
    double balance;

public:
    Account(int accNum, std::string holderName, double bal)
        : accountNumber(accNum), accountHolderName(holderName), balance(bal) {}

    virtual ~Account() = default;

    virtual void deposit(double amount) {
        balance += amount;
    }

    virtual bool withdraw(double amount) = 0;

    virtual void printAccountDetails() const {
        std::cout << "Account Number: " << accountNumber << "\n"
                  << "Account Holder: " << accountHolderName << "\n"
                  << "Balance: " << balance << "\n";
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }
};

// Derived class for SavingsAccount
class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(int accNum, std::string holderName, double bal, double rate)
        : Account(accNum, holderName, bal), interestRate(rate) {}

    void applyInterest() {
        balance += balance * interestRate;
    }

    bool withdraw(double amount) override {
        if (amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }

    void printAccountDetails() const override {
        Account::printAccountDetails();
        std::cout << "Interest Rate: " << interestRate << "\n";
    }
};

// Derived class for CheckingAccount
class CheckingAccount : public Account {
private:
    double overdraftLimit;

public:
    CheckingAccount(int accNum, std::string holderName, double bal, double overdraft)
        : Account(accNum, holderName, bal), overdraftLimit(overdraft) {}

    bool withdraw(double amount) override {
        if (amount <= balance + overdraftLimit) {
            balance -= amount;
            return true;
        }
        return false;
    }

    void printAccountDetails() const override {
        Account::printAccountDetails();
        std::cout << "Overdraft Limit: " << overdraftLimit << "\n";
    }
};

// Class for managing Bank
class Bank {
private:
    std::vector<std::shared_ptr<Account>> accounts;

public:
    void addAccount(const std::shared_ptr<Account>& account) {
        accounts.push_back(account);
    }

    void removeAccount(int accNum) {
        accounts.erase(std::remove_if(accounts.begin(), accounts.end(),
                                      [accNum](const std::shared_ptr<Account>& acc) {
                                          return acc->getAccountNumber() == accNum;
                                      }),
                       accounts.end());
    }

    std::shared_ptr<Account> findAccount(int accNum) {
        for (const auto& acc : accounts) {
            if (acc->getAccountNumber() == accNum) {
                return acc;
            }
        }
        return nullptr;
    }

    void printAllAccounts() const {
        for (const auto& acc : accounts) {
            acc->printAccountDetails();
            std::cout << "-------------------\n";
        }
    }
};

// Main function demonstrating the usage of the system
int main() {
    Bank bank;
    auto savAcc = std::make_shared<SavingsAccount>(101, "P1", 1000.0, 0.05);
    auto chkAcc = std::make_shared<CheckingAccount>(102, "P2", 500.0, 200.0);

    bank.addAccount(savAcc);
    bank.addAccount(chkAcc);

    bank.printAllAccounts();

    savAcc->deposit(500);
    savAcc->applyInterest();
    chkAcc->withdraw(600);

    bank.printAllAccounts();

    return 0;
}
