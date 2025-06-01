#include <bits/stdc++.h>
#include <thread>
#include <chrono>
using namespace std;

// Clear screen function (cross-platform)
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");

#endif
}

// Function to simulate loading effect
void showLoadingEffect(const string &message, int duration = 1)
{
    cout << message;
    for (int i = 0; i < 3; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(300 * duration));
        cout << ".";
        cout.flush();
    }
    cout << endl;
    this_thread::sleep_for(chrono::milliseconds(300 * duration));
}

// User structure to store account information
struct User
{
    int accountNumber;
    string name;
    string pin;
    double balance;

    // Default constructor
    User() : accountNumber(0), name(""), pin(""), balance(0.0) {}

    // Parameter wala constructor
    User(int accNo, const string &userName, const string &userPin, double initialBalance)
        : accountNumber(accNo), name(userName), pin(userPin), balance(initialBalance) {}
};

// Loan er information store koar jonno structure
struct Loan
{
    int loanId;
    int accountNumber;
    double amount;
    double interestRate;
    int termMonths;
    double monthlyPayment;
    double remainingAmount;
    string applicationDate;
    bool approved;

    // Default constructor
    Loan() : loanId(0), accountNumber(0), amount(0.0), interestRate(0.0),
             termMonths(0), monthlyPayment(0.0), remainingAmount(0.0),
             applicationDate(""), approved(false) {}

    // Calculate monthly payment (principal + interest)
    void calculateMonthlyPayment()
    {
        // Simple interest calculation for monthly payment
        double totalAmount = amount + (amount * interestRate * termMonths / 12.0 / 100.0);
        monthlyPayment = totalAmount / termMonths;
        remainingAmount = totalAmount;
    }

    //  loan ke string e nitesi  for storage
    string toString() const
    {
        stringstream ss;
        ss << loanId << "|"
           << accountNumber << "|"
           << amount << "|"
           << interestRate << "|"
           << termMonths << "|"
           << monthlyPayment << "|"
           << remainingAmount << "|"
           << applicationDate << "|"
           << (approved ? "1" : "0");
        return ss.str();
    }
};

// Transaction enum to represent transaction types
enum class TransactionType
{
    DEPOSIT,
    WITHDRAWAL,
    TRANSFER_SENT,
    TRANSFER_RECEIVED,
    PIN_CHANGE,
    LOAN_APPLICATION,
    LOAN_PAYMENT
};

// Transaction structure struct Transaction
struct Transaction
{
    int accountNumber;
    TransactionType type;
    double amount;
    string timestamp;
    string description;

    //  transaction ke string e nitesi  for storage
    string toString() const
    {
        stringstream ss;

        // Format based on transaction type
        switch (type)
        {
        case TransactionType::DEPOSIT:
            ss << "+" << amount << " | " << timestamp << " | " << description;
            break;
        case TransactionType::WITHDRAWAL:
            ss << "-" << amount << " | " << timestamp << " | " << description;
            break;
        case TransactionType::TRANSFER_SENT:
            ss << "-" << amount << " | " << timestamp << " | " << description;
            break;
        case TransactionType::TRANSFER_RECEIVED:
            ss << "+" << amount << " | " << timestamp << " | " << description;
            break;
        case TransactionType::PIN_CHANGE:
            ss << "PIN | " << timestamp << " | " << description;
            break;
        case TransactionType::LOAN_APPLICATION:
            ss << "LOAN | " << timestamp << " | " << description;
            break;
        case TransactionType::LOAN_PAYMENT:
            ss << "-" << amount << " | " << timestamp << " | " << description;
            break;
        }

        return ss.str();
    }
};

// Global variables
map<int, User> users;              // Map to store all users
map<int, Loan> loans;              // Map to store all loans
string USERS_FILE = "users.txt";   // File path to store user data
string LOANS_FILE = "loans.txt";   // File path to store loan data
const string ADMIN_PIN = "0000";   // Admin PIN
double ATM_CASH_LIMIT = 50000.0;   // ATM cash limit
double CURRENT_ATM_CASH = 50000.0; // Current cash in ATM
double MAX_LOAN_LIMIT = 100000.0;  // Maximum loan amount allowed
double MIN_LOAN_AMOUNT = 1000.0;   // Minimum loan amount

// Function to trim whitespace from strings
string trim(const string &str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos)
    {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

// Function to split string by delimiter
vector<string> split(const string &str, char delimiter)
{
    vector<string> tokens;
    stringstream ss(str);
    string token;

    while (getline(ss, token, delimiter))
    {
        tokens.push_back(trim(token));
    }

    return tokens;
}

// Function to get current timestamp
string getCurrentTimestamp()
{
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);

    tm *now_tm = localtime(&now_time);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now_tm);

    return string(buffer);
}

// Function to load users from file
bool loadUsers()
{
    ifstream file(USERS_FILE);

    if (!file.is_open())
    {
        // If file doesn't exist, create an empty one
        ofstream newFile(USERS_FILE);
        newFile.close();
        return true;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        vector<string> parts = split(line, '|');
        if (parts.size() >= 4)
        {
            int accountNumber = stoi(parts[0]);
            string name = parts[1];
            string pin = parts[2];
            double balance = stod(parts[3]);

            users[accountNumber] = User(accountNumber, name, pin, balance);
        }
    }

    file.close();
    return true;
}

// Function to load loans from file
bool loadLoans()
{
    ifstream file(LOANS_FILE);

    if (!file.is_open())
    {
        // If file doesn't exist, create an empty one
        ofstream newFile(LOANS_FILE);
        newFile.close();
        return true;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        vector<string> parts = split(line, '|');
        if (parts.size() >= 9)
        {
            Loan loan;
            loan.loanId = stoi(parts[0]);
            loan.accountNumber = stoi(parts[1]);
            loan.amount = stod(parts[2]);
            loan.interestRate = stod(parts[3]);
            loan.termMonths = stoi(parts[4]);
            loan.monthlyPayment = stod(parts[5]);
            loan.remainingAmount = stod(parts[6]);
            loan.applicationDate = parts[7];
            loan.approved = (parts[8] == "1");

            loans[loan.loanId] = loan;
        }
    }

    file.close();
    return true;
}

// Function to save users to file
bool saveUsers()
{
    ofstream file(USERS_FILE);

    if (!file.is_open())
    {
        cerr << "Error opening file for writing: " << USERS_FILE << endl;
        return false;
    }

    for (const auto &pair : users)
    {
        const User &user = pair.second;
        file << user.accountNumber << "|"
             << user.name << "|"
             << user.pin << "|"
             << user.balance << endl;
    }

    file.close();
    return true;
}

// Function to save loans to file
bool saveLoans()
{
    ofstream file(LOANS_FILE);

    if (!file.is_open())
    {
        cerr << "Error opening file for writing: " << LOANS_FILE << endl;
        return false;
    }

    for (const auto &pair : loans)
    {
        const Loan &loan = pair.second;
        file << loan.toString() << endl;
    }

    file.close();
    return true;
}

// Function to sanitize filename (remove invalid characters)
string sanitizeFilename(const string &filename)
{
    string sanitized = filename;
    // Replace spaces and invalid characters with underscores
    for (char &c : sanitized)
    {
        if (c == ' ' || c == '/' || c == '\\' || c == ':' || c == '*' ||
            c == '?' || c == '"' || c == '<' || c == '>' || c == '|')
        {
            c = '_';
        }
    }
    return sanitized;
}

// Function to get transaction filename with name and account number
string getTransactionFilename(int accountNumber)
{
    auto it = users.find(accountNumber);
    if (it != users.end())
    {
        string sanitizedName = sanitizeFilename(it->second.name);
        return sanitizedName + "_" + to_string(accountNumber) + "_transactions_.txt";
    }
    // Fallback to old naming if user not found
    return to_string(accountNumber) + "transactions_.txt";
}

// Updated function to get transaction history for an account
vector<string> getTransactionHistory(int accountNumber)
{
    vector<string> transactions;
    string filename = getTransactionFilename(accountNumber);

    ifstream file(filename);
    if (!file.is_open())
    {
        return transactions; // Return empty vector if file doesn't exist
    }

    string line;
    while (getline(file, line))
    {
        if (!line.empty())
        {
            transactions.push_back(line);
        }
    }

    file.close();
    return transactions;
}

// Updated function to save a transaction to file
bool saveTransaction(int accountNumber, const Transaction &transaction)
{
    string filename = getTransactionFilename(accountNumber);

    // Open in append mode
    ofstream file(filename, ios::app);

    if (!file.is_open())
    {
        cerr << "Error opening transaction file: " << filename << endl;
        return false;
    }

    file << transaction.toString() << endl;
    file.close();
    return true;
}
// Function to get recent transactions as a stack
stack<string> getRecentTransactions(int accountNumber, int count)
{
    vector<string> allTransactions = getTransactionHistory(accountNumber);
    stack<string> recentTransactions;

    // Start from the most recent (end of vector) and go backwards
    int start = static_cast<int>(allTransactions.size()) - 1;
    int end = max(0, start - count + 1);

    for (int i = end; i <= start; i++)
    {
        recentTransactions.push(allTransactions[i]);
    }

    return recentTransactions;
}

// Function to generate a new account number
int generateAccountNumber()
{
    // Find the highest account number and add 1
    int maxAccountNumber = 1000; // Start with 1001 as the first account

    for (const auto &pair : users)
    {
        if (pair.first > maxAccountNumber)
        {
            maxAccountNumber = pair.first;
        }
    }

    return maxAccountNumber + 1;
}

// Function to generate a new loan ID
int generateLoanId()
{
    // Find the highest loan ID and add 1
    int maxLoanId = 1000; // Start with 1001 as the first loan

    for (const auto &pair : loans)
    {
        if (pair.first > maxLoanId)
        {
            maxLoanId = pair.first;
        }
    }

    return maxLoanId + 1;
}

// Function to create a new user
bool createUser(const string &name, const string &pin, double initialBalance, int &accountNumber)
{
    accountNumber = generateAccountNumber();

    // Validate initial deposit
    if (initialBalance < 0)
    {
        cout << "❌ ERROR: Initial balance cannot be negative." << endl;
        return false;
    }

    // Create user and add to map
    users[accountNumber] = User(accountNumber, name, pin, initialBalance);

    // Save users to file
    if (!saveUsers())
    {
        cout << "❌ ERROR: Failed to save user data to file." << endl;
        return false;
    }

    // Create transaction record for initial deposit (if any)
    if (initialBalance > 0)
    {
        Transaction transaction;
        transaction.accountNumber = accountNumber;
        transaction.type = TransactionType::DEPOSIT;
        transaction.amount = initialBalance;
        transaction.timestamp = getCurrentTimestamp();
        transaction.description = "Initial deposit";

        saveTransaction(accountNumber, transaction);
    }

    return true;
}

// Function to authenticate user
bool authenticateUser(int accountNumber, const string &pin)
{
    auto it = users.find(accountNumber);

    if (it == users.end())
    {
        return false; // Account not found
    }

    return it->second.pin == pin;
}

// Function to check balance
double checkBalance(int accountNumber)
{
    auto it = users.find(accountNumber);

    if (it == users.end())
    {
        return -1; // Account not found
    }

    return it->second.balance;
}

// Function to deposit money
bool deposit(int accountNumber, double amount)
{
    if (amount <= 0)
    {
        cout << "❌ ERROR: Deposit amount must be positive." << endl;
        return false;
    }

    auto it = users.find(accountNumber);
    if (it == users.end())
    {
        cout << "❌ ERROR: Account not found." << endl;
        return false;
    }

    // Check if adding this amount would exceed ATM cash limit
    if (CURRENT_ATM_CASH + amount > ATM_CASH_LIMIT)
    {
        cout << "❌ ERROR: Deposit rejected - ATM cash limit would be exceeded." << endl;
        return false;
    }

    // Update user balance
    it->second.balance += amount;
    CURRENT_ATM_CASH += amount;

    // Create and save transaction
    Transaction transaction;
    transaction.accountNumber = accountNumber;
    transaction.type = TransactionType::DEPOSIT;
    transaction.amount = amount;
    transaction.timestamp = getCurrentTimestamp();
    transaction.description = "Deposit";

    saveTransaction(accountNumber, transaction);

    // Save updated user data
    saveUsers();

    // Success message
    cout << "✅ TRANSACTION SUCCESSFUL!" << endl;
    cout << "💰 Amount Deposited: $" << fixed << setprecision(2) << amount << endl;
    cout << "💳 New Balance: $" << fixed << setprecision(2) << it->second.balance << endl;

    return true;
}

// Function to withdraw money
bool withdraw(int accountNumber, double amount)
{
    if (amount <= 0)
    {
        cout << "❌ ERROR: Withdrawal amount must be positive." << endl;
        return false;
    }

    auto it = users.find(accountNumber);
    if (it == users.end())
    {
        cout << "❌ ERROR: Account not found." << endl;
        return false;
    }
    if (it->second.balance <= amount + 500)
    {
        cout << "❌ TRANSACTION FAILED!" << endl;
        cout << "🚫 Not enough balance!" << endl;
        cout << "Minimum Required Balence 500$" << endl;
        cout << "💳 Your current balance: $" << fixed << setprecision(2) << it->second.balance << endl;
        cout << "💸 Requested amount: $" << fixed << setprecision(2) << amount << endl;
        return false;
    }

    // Check if ATM has sufficient cash
    if (CURRENT_ATM_CASH < amount)
    {
        cout << "❌ ERROR: ATM does not have sufficient cash." << endl;
        return false;
    }

    // Update user balance
    it->second.balance -= amount;
    CURRENT_ATM_CASH -= amount;

    // Create and save transaction
    Transaction transaction;
    transaction.accountNumber = accountNumber;
    transaction.type = TransactionType::WITHDRAWAL;
    transaction.amount = amount;
    transaction.timestamp = getCurrentTimestamp();
    transaction.description = "Withdrawal";

    saveTransaction(accountNumber, transaction);

    // Save updated user data
    saveUsers();

    // Success message
    cout << "✅ TRANSACTION SUCCESSFUL!" << endl;
    cout << "💸 Amount Withdrawn: $" << fixed << setprecision(2) << amount << endl;
    cout << "💳 New Balance: $" << fixed << setprecision(2) << it->second.balance << endl;

    return true;
}

// Function to transfer money between accounts
bool transfer(int senderAccount, int receiverAccount, double amount)
{
    if (amount <= 0)
    {
        cout << "❌ ERROR: Transfer amount must be positive." << endl;
        return false;
    }

    if (senderAccount == receiverAccount)
    {
        cout << "❌ ERROR: Cannot transfer to the same account." << endl;
        return false;
    }

    auto senderIt = users.find(senderAccount);
    auto receiverIt = users.find(receiverAccount);

    if (senderIt == users.end())
    {
        cout << "❌ ERROR: Sender account not found." << endl;
        return false;
    }

    if (receiverIt == users.end())
    {
        cout << "❌ ERROR: Receiver account not found." << endl;
        return false;
    }

    // Check if sender has sufficient balance
    if (senderIt->second.balance < amount + 500)
    {
        cout << "❌ TRANSFER FAILED!" << endl;
        cout << "🚫 Not enough balance!" << endl;
        cout << "Minimum Required Balence 500$" << endl;
        cout << "💳 Your current balance: $" << fixed << setprecision(2) << senderIt->second.balance << endl;
        cout << "💸 Transfer amount: $" << fixed << setprecision(2) << amount << endl;
        return false;
    }

    // Update balances
    senderIt->second.balance -= amount;
    receiverIt->second.balance += amount;

    // Create and save sender transaction
    Transaction senderTransaction;
    senderTransaction.accountNumber = senderAccount;
    senderTransaction.type = TransactionType::TRANSFER_SENT;
    senderTransaction.amount = amount;
    senderTransaction.timestamp = getCurrentTimestamp();
    senderTransaction.description = "Transfer to Acc#" + to_string(receiverAccount);

    saveTransaction(senderAccount, senderTransaction);

    // Create and save receiver transaction
    Transaction receiverTransaction;
    receiverTransaction.accountNumber = receiverAccount;
    receiverTransaction.type = TransactionType::TRANSFER_RECEIVED;
    receiverTransaction.amount = amount;
    receiverTransaction.timestamp = getCurrentTimestamp();
    receiverTransaction.description = "Transfer from Acc#" + to_string(senderAccount);

    saveTransaction(receiverAccount, receiverTransaction);

    // Save updated user data
    saveUsers();

    // Success message
    cout << "✅ TRANSFER SUCCESSFUL!" << endl;
    cout << "💸 Amount Transferred: $" << fixed << setprecision(2) << amount << endl;
    cout << "📤 To Account: " << receiverAccount << endl;
    cout << "💳 Your New Balance: $" << fixed << setprecision(2) << senderIt->second.balance << endl;

    return true;
}

// Function to change PIN
bool changePin(int accountNumber, const string &oldPin, const string &newPin)
{
    auto it = users.find(accountNumber);

    if (it == users.end())
    {
        cout << "❌ ERROR: Account not found." << endl;
        return false;
    }

    if (it->second.pin != oldPin)
    {
        cout << "❌ ERROR: Incorrect old PIN." << endl;
        return false;
    }

    if (newPin.length() != 4 || !all_of(newPin.begin(), newPin.end(), ::isdigit))
    {
        cout << "❌ ERROR: PIN must be a 4-digit number." << endl;
        return false;
    }

    // Update PIN
    it->second.pin = newPin;

    // Create and save transaction
    Transaction transaction;
    transaction.accountNumber = accountNumber;
    transaction.type = TransactionType::PIN_CHANGE;
    transaction.amount = 0;
    transaction.timestamp = getCurrentTimestamp();
    transaction.description = "PIN changed";

    saveTransaction(accountNumber, transaction);

    // Save updated user data
    saveUsers();

    // Success message
    cout << "✅ PIN CHANGE SUCCESSFUL!" << endl;
    cout << "🔐 Your PIN has been updated successfully." << endl;

    return true;
}

// Function to apply for a loan
int applyForLoan(int accountNumber, double amount, int termMonths)
{
    auto it = users.find(accountNumber);
    if (it == users.end())
    {
        cout << "❌ ERROR: Account not found." << endl;
        return -1;
    }

    // Check if loan amount is within allowed limits
    if (amount < MIN_LOAN_AMOUNT)
    {
        cout << "❌ ERROR: Loan amount must be at least $" << MIN_LOAN_AMOUNT << endl;
        return -1;
    }

    if (amount > MAX_LOAN_LIMIT)
    {
        cout << "❌ ERROR: Loan amount cannot exceed $" << MAX_LOAN_LIMIT << endl;
        return -1;
    }

    // Check if term is valid (between 6 and 60 months)
    if (termMonths < 6 || termMonths > 60)
    {
        cout << "❌ ERROR: Loan term must be between 6 and 60 months." << endl;
        return -1;
    }

    // Create a new loan
    Loan loan;
    loan.loanId = generateLoanId();
    loan.accountNumber = accountNumber;
    loan.amount = amount;
    loan.interestRate = 10.0; // 10% fixed interest rate
    loan.termMonths = termMonths;
    loan.applicationDate = getCurrentTimestamp();
    loan.approved = false; // Loans are pending approval by default

    // Calculate monthly payment
    loan.calculateMonthlyPayment();

    // Add loan to the map
    loans[loan.loanId] = loan;

    // Create transaction record for loan application
    Transaction transaction;
    transaction.accountNumber = accountNumber;
    transaction.type = TransactionType::LOAN_APPLICATION;
    transaction.amount = amount;
    transaction.timestamp = getCurrentTimestamp();
    transaction.description = "Loan application #" + to_string(loan.loanId);

    saveTransaction(accountNumber, transaction);

    // Save all loans to file
    saveLoans();

    // Success message
    cout << "✅ LOAN APPLICATION SUCCESSFUL!" << endl;
    cout << "📝 Loan ID: " << loan.loanId << endl;
    cout << "💰 Loan Amount: $" << fixed << setprecision(2) << amount << endl;
    cout << "📅 Term: " << termMonths << " months" << endl;
    cout << "💳 Monthly Payment: $" << fixed << setprecision(2) << loan.monthlyPayment << endl;
    cout << "⏳ Status: Pending Approval" << endl;

    return loan.loanId;
}

// Function to approve a loan (admin only)
bool approveLoan(int loanId)
{
    auto it = loans.find(loanId);
    if (it == loans.end())
    {
        cout << "❌ ERROR: Loan not found." << endl;
        return false;
    }

    // Check if loan is already approved
    if (it->second.approved)
    {
        cout << "❌ ERROR: Loan is already approved." << endl;
        return false;
    }

    // Approve the loan
    it->second.approved = true;

    // Add loan amount to user's balance
    auto userIt = users.find(it->second.accountNumber);
    if (userIt != users.end())
    {
        userIt->second.balance += it->second.amount;

        // Create transaction for loan disbursement
        Transaction transaction;
        transaction.accountNumber = it->second.accountNumber;
        transaction.type = TransactionType::DEPOSIT;
        transaction.amount = it->second.amount;
        transaction.timestamp = getCurrentTimestamp();
        transaction.description = "Loan disbursement #" + to_string(loanId);

        saveTransaction(it->second.accountNumber, transaction);

        // Save updated user data
        saveUsers();
    }

    // Save loans data
    saveLoans();

    // Success message
    cout << "✅ LOAN APPROVED SUCCESSFULLY!" << endl;
    cout << "📝 Loan ID: " << loanId << endl;
    cout << "💰 Amount: $" << fixed << setprecision(2) << it->second.amount << endl;
    cout << "📤 Funds have been disbursed to account #" << it->second.accountNumber << endl;

    return true;
}

// Function to get active loans for an account
vector<Loan> getActiveLoans(int accountNumber)
{
    vector<Loan> activeLoans;

    for (const auto &pair : loans)
    {
        const Loan &loan = pair.second;
        if (loan.accountNumber == accountNumber && loan.approved && loan.remainingAmount > 0)
        {
            activeLoans.push_back(loan);
        }
    }

    return activeLoans;
}

// Function to make a loan payment
bool makeLoanPayment(int accountNumber, int loanId)
{
    auto loanIt = loans.find(loanId);
    if (loanIt == loans.end())
    {
        cout << "❌ ERROR: Loan not found." << endl;
        return false;
    }

    Loan &loan = loanIt->second;

    // Check if the loan belongs to this account
    if (loan.accountNumber != accountNumber)
    {
        cout << "❌ ERROR: This loan does not belong to your account." << endl;
        return false;
    }

    // Check if the loan is approved
    if (!loan.approved)
    {
        cout << "❌ ERROR: This loan is not yet approved." << endl;
        return false;
    }

    // Check if there's anything left to pay
    if (loan.remainingAmount <= 0)
    {
        cout << "❌ ERROR: This loan has already been fully paid." << endl;
        return false;
    }

    auto userIt = users.find(accountNumber);
    if (userIt == users.end())
    {
        cout << "❌ ERROR: Account not found." << endl;
        return false;
    }

    // Check if user has enough balance to make the payment
    if (userIt->second.balance < loan.monthlyPayment + 500)
    {
        cout << "❌ LOAN PAYMENT FAILED!" << endl;
        cout << "🚫 Not enough balance!" << endl;
        cout << "Minimum Required Balence 500$" << endl;
        cout << "💳 Your current balance: $" << fixed << setprecision(2) << userIt->second.balance << endl;
        cout << "💸 Required payment: $" << fixed << setprecision(2) << loan.monthlyPayment << endl;
        return false;
    }

    // Make the payment
    double paymentAmount = min(loan.monthlyPayment, loan.remainingAmount);

    // Update user balance
    userIt->second.balance -= paymentAmount;

    // Update loan remaining amount
    loan.remainingAmount -= paymentAmount;

    // Create transaction for loan payment
    Transaction transaction;
    transaction.accountNumber = accountNumber;
    transaction.type = TransactionType::LOAN_PAYMENT;
    transaction.amount = paymentAmount;
    transaction.timestamp = getCurrentTimestamp();
    transaction.description = "Payment for loan #" + to_string(loanId);

    saveTransaction(accountNumber, transaction);

    // Save updated user and loan data
    saveUsers();
    saveLoans();

    // Success message
    cout << "✅ LOAN PAYMENT SUCCESSFUL!" << endl;
    cout << "💸 Payment Amount: $" << fixed << setprecision(2) << paymentAmount << endl;
    cout << "📝 Loan ID: " << loanId << endl;
    cout << "💰 Remaining Loan Amount: $" << fixed << setprecision(2) << loan.remainingAmount << endl;
    cout << "💳 Your New Balance: $" << fixed << setprecision(2) << userIt->second.balance << endl;

    if (loan.remainingAmount <= 0)
    {
        cout << "🎉 Congratulations! Your loan has been fully paid off!" << endl;
    }

    return true;
}

// Function to display welcome screen
void displayWelcomeScreen()
{
    clearScreen();
    cout << "=======================================" << endl;
    cout << "||                                   ||" << endl;
    cout << "||        WELCOME TO THE ATM         ||" << endl;
    cout << "||                                   ||" << endl;
    cout << "=======================================" << endl;
    cout << endl;

    showLoadingEffect("Initializing ATM", 2);
    cout << "ATM Ready!" << endl;
    this_thread::sleep_for(chrono::seconds(1));
}

// Function to display user menu
void displayUserMenu()
{
    clearScreen();
    cout << "=======================================" << endl;
    cout << "||           USER MENU               ||" << endl;
    cout << "=======================================" << endl;
    cout << "1. Check Balance" << endl;
    cout << "2. Deposit Money" << endl;
    cout << "3. Withdraw Money" << endl;
    cout << "4. Transfer Money" << endl;
    cout << "5. View Transaction History" << endl;
    cout << "6. Change PIN" << endl;
    cout << "7. Apply for Loan" << endl;
    cout << "8. View Active Loans" << endl;
    cout << "9. Make Loan Payment" << endl;
    cout << "10. Logout" << endl;
    cout << "=======================================" << endl;
    cout << "Enter your choice: ";
}

// Function to display admin menu
void displayAdminMenu()
{
    clearScreen();
    cout << "=======================================" << endl;
    cout << "||           ADMIN MENU              ||" << endl;
    cout << "=======================================" << endl;
    cout << "1. View All Accounts" << endl;
    cout << "2. Delete User Account" << endl;
    cout << "3. View Total ATM Balance" << endl;
    cout << "4. Set ATM Cash Limit" << endl;
    cout << "5. View Pending Loans" << endl;
    cout << "6. Approve Loan" << endl;
    cout << "7. Set Max Loan Limit" << endl;
    cout << "8. Logout" << endl;
    cout << "=======================================" << endl;
    cout << "Enter your choice: ";
}

// Function to display main menu
void displayMainMenu()
{
    clearScreen();
    cout << "=======================================" << endl;
    cout << "||           MAIN MENU               ||" << endl;
    cout << "=======================================" << endl;
    cout << "1. Create New Account" << endl;
    cout << "2. User Login" << endl;
    cout << "3. Admin Login" << endl;
    cout << "4. Exit" << endl;
    cout << "=======================================" << endl;
    cout << "Enter your choice: ";
}

// Function to handle user session
void userSession(int accountNumber)
{
    int choice;

    while (true)
    {
        displayUserMenu();
        cin >> choice;

        switch (choice)
        {
        case 1: // Check Balance
        {
            clearScreen();
            double balance = checkBalance(accountNumber);
            cout << "=======================================" << endl;
            cout << "||        BALANCE INQUIRY            ||" << endl;
            cout << "=======================================" << endl;
            cout << "💳 Account Number: " << accountNumber << endl;
            cout << "💰 Current Balance: $" << fixed << setprecision(2) << balance << endl;
            cout << "=======================================" << endl;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 2: // Deposit Money
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||           DEPOSIT MONEY           ||" << endl;
            cout << "=======================================" << endl;
            double amount;
            cout << "Enter amount to deposit: $";
            cin >> amount;

            showLoadingEffect("Processing deposit");
            deposit(accountNumber, amount);

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 3: // Withdraw Money
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||          WITHDRAW MONEY           ||" << endl;
            cout << "=======================================" << endl;
            double amount;
            cout << "Enter amount to withdraw: $";
            cin >> amount;

            showLoadingEffect("Processing withdrawal");
            withdraw(accountNumber, amount);

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 4: // Transfer Money
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||          TRANSFER MONEY           ||" << endl;
            cout << "=======================================" << endl;
            int receiverAccount;
            double amount;

            cout << "Enter receiver account number: ";
            cin >> receiverAccount;
            cout << "Enter amount to transfer: $";
            cin >> amount;

            showLoadingEffect("Processing transfer");
            transfer(accountNumber, receiverAccount, amount);

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 5: // View Transaction History
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||      TRANSACTION HISTORY          ||" << endl;
            cout << "=======================================" << endl;

            stack<string> recentTransactions = getRecentTransactions(accountNumber, 10);

            if (recentTransactions.empty())
            {
                cout << "No transactions found." << endl;
            }
            else
            {
                cout << "Recent Transactions (Latest First):" << endl;
                cout << "-----------------------------------" << endl;

                while (!recentTransactions.empty())
                {
                    cout << recentTransactions.top() << endl;
                    recentTransactions.pop();
                }
            }

            cout << "=======================================" << endl;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 6: // Change PIN
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||            CHANGE PIN             ||" << endl;
            cout << "=======================================" << endl;

            string oldPin, newPin;
            cout << "Enter current PIN: ";
            cin >> oldPin;
            cout << "Enter new PIN (4 digits): ";
            cin >> newPin;

            showLoadingEffect("Updating PIN");
            changePin(accountNumber, oldPin, newPin);

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 7: // Apply for Loan
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||          APPLY FOR LOAN           ||" << endl;
            cout << "=======================================" << endl;

            double amount;
            int termMonths;

            cout << "Loan Information:" << endl;
            cout << "- Interest Rate: 10% per annum" << endl;
            cout << "- Min Amount: $" << MIN_LOAN_AMOUNT << endl;
            cout << "- Max Amount: $" << MAX_LOAN_LIMIT << endl;
            cout << "- Term: 6-60 months" << endl;
            cout << "-----------------------------------" << endl;

            cout << "Enter loan amount: $";
            cin >> amount;
            cout << "Enter term (in months): ";
            cin >> termMonths;

            showLoadingEffect("Processing loan application");
            applyForLoan(accountNumber, amount, termMonths);

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 8: // View Active Loans
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||          ACTIVE LOANS             ||" << endl;
            cout << "=======================================" << endl;

            vector<Loan> activeLoans = getActiveLoans(accountNumber);

            if (activeLoans.empty())
            {
                cout << "No active loans found." << endl;
            }
            else
            {
                for (const auto &loan : activeLoans)
                {
                    cout << "Loan ID: " << loan.loanId << endl;
                    cout << "Amount: $" << fixed << setprecision(2) << loan.amount << endl;
                    cout << "Monthly Payment: $" << fixed << setprecision(2) << loan.monthlyPayment << endl;
                    cout << "Remaining: $" << fixed << setprecision(2) << loan.remainingAmount << endl;
                    cout << "Application Date: " << loan.applicationDate << endl;
                    cout << "-----------------------------------" << endl;
                }
            }

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 9: // Make Loan Payment
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||         LOAN PAYMENT              ||" << endl;
            cout << "=======================================" << endl;

            vector<Loan> activeLoans = getActiveLoans(accountNumber);

            if (activeLoans.empty())
            {
                cout << "No active loans found." << endl;
            }
            else
            {
                cout << "Your Active Loans:" << endl;
                for (const auto &loan : activeLoans)
                {
                    cout << "ID: " << loan.loanId << " | Payment: $" << fixed << setprecision(2)
                         << loan.monthlyPayment << " | Remaining: $" << fixed << setprecision(2)
                         << loan.remainingAmount << endl;
                }

                int loanId;
                cout << "Enter Loan ID to make payment: ";
                cin >> loanId;

                showLoadingEffect("Processing loan payment");
                makeLoanPayment(accountNumber, loanId);
            }

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 10: // Logout
            cout << "Logging out..." << endl;
            return;

        default:
            cout << "❌ Invalid choice! Please try again." << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
}

// Function to handle admin session
void adminSession()
{
    int choice;

    while (true)
    {
        displayAdminMenu();
        cin >> choice;

        switch (choice)
        {
        case 1: // View All Accounts
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||         ALL ACCOUNTS              ||" << endl;
            cout << "=======================================" << endl;

            if (users.empty())
            {
                cout << "No accounts found." << endl;
            }
            else
            {
                cout << left << setw(10) << "Acc No" << setw(20) << "Name"
                     << setw(15) << "Balance" << endl;
                cout << "-------------------------------------------" << endl;

                for (const auto &pair : users)
                {
                    const User &user = pair.second;
                    cout << left << setw(10) << user.accountNumber
                         << setw(20) << user.name
                         << "$" << fixed << setprecision(2) << user.balance << endl;
                }
            }

            cout << "=======================================" << endl;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 2: // Delete User Account
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||         ALL ACCOUNTS              ||" << endl;
            cout << "=======================================" << endl;

            if (users.empty())
            {
                cout << "No accounts found." << endl;
                cout << "=======================================" << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            else
            {
                cout << left << setw(10) << "Acc No" << setw(20) << "Name"
                     << setw(15) << "Balance" << endl;
                cout << "-------------------------------------------" << endl;

                for (const auto &pair : users)
                {
                    const User &user = pair.second;
                    cout << left << setw(10) << user.accountNumber
                         << setw(20) << user.name
                         << "$" << fixed << setprecision(2) << user.balance << endl;
                }
            }
            cout << "=======================================" << endl;
            cout << "||        DELETE ACCOUNT             ||" << endl;
            cout << "=======================================" << endl;

            int accountNumber;

            string accInput;
            while (true)
            {
                cout << "Enter account number to delete: ";
                cin >> accInput;

                if (all_of(accInput.begin(), accInput.end(), ::isdigit))
                {
                    accountNumber = stoi(accInput); // Safe conversion after validation
                    break;                          // Valid input, break the loop
                }
                else
                {
                    cout << "❌ ERROR: Loan Id must be a number." << endl;
                }
            }
            auto it = users.find(accountNumber);
            if (it == users.end())
            {
                cout << "❌ Account not found." << endl;
            }
            else
            {
                cout << "Account Details:" << endl;
                cout << "Name: " << it->second.name << endl;
                cout << "Balance: $" << fixed << setprecision(2) << it->second.balance << endl;

                char confirm;
                cout << "Are you sure you want to delete this account? (y/n): ";
                cin >> confirm;

                if (confirm == 'y' || confirm == 'Y')
                {
                    users.erase(it);
                    saveUsers();
                    cout << "✅ Account deleted successfully." << endl;
                }
                else
                {
                    cout << "❌ Account deletion cancelled." << endl;
                }
            }

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 3: // View Total ATM Balance
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||        ATM CASH STATUS            ||" << endl;
            cout << "=======================================" << endl;
            cout << "💰 Current ATM Cash: $" << fixed << setprecision(2) << CURRENT_ATM_CASH << endl;
            cout << "🏦 ATM Cash Limit: $" << fixed << setprecision(2) << ATM_CASH_LIMIT << endl;
            cout << "📊 Usage: " << fixed << setprecision(1)
                 << (CURRENT_ATM_CASH / ATM_CASH_LIMIT * 100) << "%" << endl;
            cout << "=======================================" << endl;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 4: // Set ATM Cash Limit
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||       SET ATM CASH LIMIT          ||" << endl;
            cout << "=======================================" << endl;
            cout << "Current ATM Cash Limit: $" << fixed << setprecision(2) << ATM_CASH_LIMIT << endl;

            double newLimit;
            cout << "Enter new ATM cash limit: $";
            cin >> newLimit;

            if (newLimit >= CURRENT_ATM_CASH)
            {
                ATM_CASH_LIMIT = newLimit;
                cout << "✅ ATM cash limit updated successfully." << endl;
            }
            else
            {
                cout << "❌ New limit cannot be less than current cash amount." << endl;
            }

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 5: // View Pending Loans
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||        PENDING LOANS              ||" << endl;
            cout << "=======================================" << endl;

            bool hasPendingLoans = false;

            for (const auto &pair : loans)
            {
                const Loan &loan = pair.second;
                if (!loan.approved)
                {
                    hasPendingLoans = true;
                    cout << "Loan ID: " << loan.loanId << endl;
                    cout << "Account: " << loan.accountNumber << endl;
                    cout << "Amount: $" << fixed << setprecision(2) << loan.amount << endl;
                    cout << "Term: " << loan.termMonths << " months" << endl;
                    cout << "Monthly Payment: $" << fixed << setprecision(2) << loan.monthlyPayment << endl;
                    cout << "Application Date: " << loan.applicationDate << endl;
                    cout << "-----------------------------------" << endl;
                }
            }

            if (!hasPendingLoans)
            {
                cout << "No pending loans found." << endl;
            }

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 6: // Approve Loan
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||        PENDING LOANS              ||" << endl;
            cout << "=======================================" << endl;

            bool hasPendingLoans = false;

            for (const auto &pair : loans)
            {
                const Loan &loan = pair.second;
                if (!loan.approved)
                {
                    hasPendingLoans = true;
                    cout << "Loan ID: " << loan.loanId << endl;
                    cout << "Account: " << loan.accountNumber << endl;
                    cout << "Amount: $" << fixed << setprecision(2) << loan.amount << endl;
                    cout << "Term: " << loan.termMonths << " months" << endl;
                    cout << "Monthly Payment: $" << fixed << setprecision(2) << loan.monthlyPayment << endl;
                    cout << "Application Date: " << loan.applicationDate << endl;
                    cout << "-----------------------------------" << endl;
                }
            }

            if (!hasPendingLoans)
            {
                cout << "No pending loans found." << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }

            cout << "=======================================" << endl;
            cout << "||         APPROVE LOAN              ||" << endl;
            cout << "=======================================" << endl;

            int loanId;

            string accInput;
            while (true)
            {
                cout << "Enter loan ID to approve: ";
                cin >> accInput;

                if (all_of(accInput.begin(), accInput.end(), ::isdigit))
                {
                    loanId = stoi(accInput); // Safe conversion after validation
                    break;                   // Valid input, break the loop
                }
                else
                {
                    cout << "❌ ERROR: Loan Id must be a number." << endl;
                }
            }
            showLoadingEffect("Processing loan approval");
            approveLoan(loanId);

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 7: // Set Max Loan Limit
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||      SET MAX LOAN LIMIT           ||" << endl;
            cout << "=======================================" << endl;
            cout << "Current Max Loan Limit: $" << fixed << setprecision(2) << MAX_LOAN_LIMIT << endl;

            double newLimit;
            cout << "Enter new maximum loan limit: $";
            cin >> newLimit;

            if (newLimit >= MIN_LOAN_AMOUNT)
            {
                MAX_LOAN_LIMIT = newLimit;
                cout << "✅ Maximum loan limit updated successfully." << endl;
            }
            else
            {
                cout << "❌ Maximum limit cannot be less than minimum loan amount." << endl;
            }

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 8: // Logout
            cout << "Logging out from admin panel..." << endl;
            return;

        default:
            cout << "❌ Invalid choice! Please try again." << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
}

// Main function
int main()
{
    // Load data from files
    if (!loadUsers() || !loadLoans())
    {
        cout << "❌ Error loading data files!" << endl;
        return 1;
    }

    displayWelcomeScreen();

    int choice;

    while (true)
    {
        displayMainMenu();
        cin >> choice;

        switch (choice)
        {
        case 1: // Create New Account
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||       CREATE NEW ACCOUNT          ||" << endl;
            cout << "=======================================" << endl;

            string name, pin;
            double initialBalance;

            cout << "Enter your name: ";
            cin.ignore();
            getline(cin, name);

            cout << "Enter 4-digit PIN: ";
            cin >> pin;

            if (pin.length() != 4 || !all_of(pin.begin(), pin.end(), ::isdigit))
            {
                cout << "❌ ERROR: PIN must be a 4-digit number." << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }

            cout << "Enter initial deposit amount: $";
            cin >> initialBalance;

            int accountNumber;
            showLoadingEffect("Creating account");

            if (createUser(name, pin, initialBalance, accountNumber))
            {
                cout << "🎉 ACCOUNT CREATED SUCCESSFULLY!" << endl;
                cout << "📋 Your Account Number: " << accountNumber << endl;
                cout << "👤 Name: " << name << endl;
                cout << "💰 Initial Balance: $" << fixed << setprecision(2) << initialBalance << endl;
                cout << "⚠️  Please remember your account number and PIN!" << endl;
            }

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 2: // User Login
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||           USER LOGIN              ||" << endl;
            cout << "=======================================" << endl;

            int accountNumber;
            string pin;
            string accInput;

            // Loop until a valid 4-digit numeric account number is entered
            while (true)
            {
                cout << "Enter Account Number (4-digit): ";
                cin >> accInput;

                if (all_of(accInput.begin(), accInput.end(), ::isdigit))
                {
                    accountNumber = stoi(accInput); // Safe conversion after validation
                    break;                          // Valid input, break the loop
                }
                else
                {
                    cout << "❌ ERROR: Account number must be a number." << endl;
                }
            }

            // Now take PIN input
            cout << "Enter PIN (4-digit): ";
            cin >> pin;

            showLoadingEffect("Authenticating");

            // Validate PIN format
            if (pin.length() != 4 || !all_of(pin.begin(), pin.end(), ::isdigit))
            {
                cout << "❌ ERROR: PIN must be a 4-digit number." << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }

            // Authenticate user
            if (authenticateUser(accountNumber, pin))
            {
                cout << "✅ Login successful!" << endl;
                this_thread::sleep_for(chrono::seconds(1));
                userSession(accountNumber);
            }
            else
            {
                cout << "❌ Invalid account number or PIN!" << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
            }

            break;
        }

        case 3: // Admin Login
        {
            clearScreen();
            cout << "=======================================" << endl;
            cout << "||          ADMIN LOGIN              ||" << endl;
            cout << "=======================================" << endl;

            string adminPin;
            cout << "Enter Admin PIN: ";
            cin >> adminPin;

            showLoadingEffect("Verifying admin credentials");

            if (adminPin == ADMIN_PIN)
            {
                cout << "✅ Admin login successful!" << endl;
                this_thread::sleep_for(chrono::seconds(1));
                adminSession();
            }
            else
            {
                cout << "❌ Invalid admin PIN!" << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
            }
            break;
        }

        case 4: // Exit
            cout << "Thank you for using our ATM!" << endl;
            cout << "Goodbye! 👋" << endl;
            return 0;

        default:
            cout << "❌ Invalid choice! Please try again." << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
    }

    return 0;
}
