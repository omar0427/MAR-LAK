/*************************************************************************
** Author      : OMAR H
** Program     : HOMEWORK 2
** Date Created: February 20, 2025
** Date Last Modified : February 21, 2025
** Usage       : No command line arguments
**
** Problem:
** Create a CheckBook program with a Check struct and CheckBook class.
** The Check struct includes CheckNum, CheckMemo, and CheckAmount.
** It overloads an operator to compare CheckAmount with a float and the << operator
** to output check details in one line.
**
** The CheckBook class maintains a balance, an array of Checks, and functions to deposit,
** write checks (with auto-assigned random memo), automatically expand the check array,
** and display all checks in reverse order. It includes default and parameterized constructors,
** a copy constructor, an assignment operator, and a destructor.
*************************************************************************/

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Check struct
struct Check {
    int checkNum;
    string checkMemo;
    float checkAmount;

    // Overloaded operator to compare CheckAmount with a float
    // Returns true if checkAmount is greater than the float value.
    bool operator>(float value) const {
        return checkAmount > value;
    }

    // Overload << operator to print all details in a single line
    friend ostream& operator<<(ostream &os, const Check &c) {
        os << "CheckNum: " << c.checkNum
           << ", Memo: " << c.checkMemo
           << ", Amount: " << c.checkAmount;
        return os;
    }
};

// (b) Define CheckBook class
class CheckBook {
private:
    float balance;
    float lastDeposit;
    int numOfChecks;
    int checkBookSize;
    Check *chkPtr; // pointer to Check array

public:
    // Getters (const functions)
    float getBalance() const { return balance; }
    float getLastDeposit() const { return lastDeposit; }
    int getNumOfChecks() const { return numOfChecks; }
    int getCheckBookSize() const { return checkBookSize; }

    // Setters
    void setBalance(float b) { balance = b; }
    void setLastDeposit(float d) { lastDeposit = d; }
    void setNumOfChecks(int n) { numOfChecks = n; }
    void setCheckBookSize(int s) { checkBookSize = s; }

    // Default constructor: balance = 0, numOfChecks = 0, checkBookSize = 4
    CheckBook() : balance(0), lastDeposit(0), numOfChecks(0), checkBookSize(4) {
        chkPtr = new Check[checkBookSize];
    }

    // Second constructor: takes initial balance, numOfChecks = 0, checkBookSize = 2
    CheckBook(float initBalance) : balance(initBalance), lastDeposit(initBalance), numOfChecks(0), checkBookSize(2) {
        chkPtr = new Check[checkBookSize];
    }

    // Copy constructor
    CheckBook(const CheckBook &other) {
        balance = other.balance;
        lastDeposit = other.lastDeposit;
        numOfChecks = other.numOfChecks;
        checkBookSize = other.checkBookSize;
        chkPtr = new Check[checkBookSize];
        for (int i = 0; i < numOfChecks; i++) {
            chkPtr[i] = other.chkPtr[i];
        }
    }

    // Assignment operator
    CheckBook& operator=(const CheckBook &other) {
        if(this != &other) {
            delete[] chkPtr;
            balance = other.balance;
            lastDeposit = other.lastDeposit;
            numOfChecks = other.numOfChecks;
            checkBookSize = other.checkBookSize;
            chkPtr = new Check[checkBookSize];
            for (int i = 0; i < numOfChecks; i++) {
                chkPtr[i] = other.chkPtr[i];
            }
        }
        return *this;
    }

    // Destructor
    ~CheckBook() {
        delete[] chkPtr;
    }

    // Deposit function
    void deposit(float amount) {
        balance += amount;
        lastDeposit = amount;
    }

    // Write check function
    // Returns true if check is successfully written; false if checkAmount is greater than balance.
    bool writeCheck(Check c) {
        // Use overloaded operator from Check (without dot operator in main comparison)
        if(c > balance) { // if check amount is greater than balance
            return false;
        }

        // Check if half of the check book is used -> double the array size.
        if(numOfChecks >= checkBookSize/2) {
            // Create new array with double size
            int newSize = checkBookSize * 2;
            Check *newChkPtr = new Check[newSize];
            // Copy old checks
            for (int i = 0; i < numOfChecks; i++) {
                newChkPtr[i] = chkPtr[i];
            }
            delete[] chkPtr;
            chkPtr = newChkPtr;
            checkBookSize = newSize;
            cout << "Warning: Check book is half used. A new check book has been ordered (size doubled to " << checkBookSize << ")." << endl;
        }

        // Update check details
        c.checkNum = numOfChecks + 1;
        // Randomly select a memo from a predefined list
        const string memos[] = {"wedding", "baby shower", "pizza", "children hospital", "barber shop", "plumber", "hobbit", "trudo"};
        int numMemos = sizeof(memos) / sizeof(memos[0]);
        c.checkMemo = memos[rand() % numMemos];

        // Deduct amount from balance and update check info
        balance -= c.checkAmount;
        chkPtr[numOfChecks] = c;
        numOfChecks++;

        return true;
    }

    // Display checks in reverse chronological order (most recent first)
    void displayChecks() const {
        cout << "Displaying checks (most recent first):" << endl;
        for (int i = numOfChecks - 1; i >= 0; i--) {
            cout << chkPtr[i] << endl;
        }
    }
};

// (f) Test function: writes checks until balance is exhausted
void checkTest(CheckBook &cb, float smallCheckAmount) {
    // Loop until a check fails to write (balance exhausted)
    Check temp;
    temp.checkAmount = smallCheckAmount; // using small checks
    while(cb.writeCheck(temp)) {
        // Continue writing checks; note that each check uses the same small amount.
        // You can adjust smallCheckAmount for testing.
    }
    cb.displayChecks();
}

int main() {
    // Seed random number generator
    srand(static_cast<unsigned int>(time(0)));

    // (g) Initialize a CheckBook with a high balance and call checkTest
    CheckBook myCheckBook(100.0f); // starting with a balance of 100
    // Write small checks of 10 each so that doubling is observed.
    checkTest(myCheckBook, 10.0f);

    return 0;
}
