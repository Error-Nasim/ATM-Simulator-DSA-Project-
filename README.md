# ATM Simulator

A comprehensive C++ application that simulates the functionality of an Automated Teller Machine (ATM) with user and administrative capabilities.

## Table of Contents
- [Features](#features)
- [Data Structures and Algorithms](#data-structures-and-algorithms)
- [Real-life Applications](#real-life-applications)
- [Setup Instructions](#setup-instructions)
- [Usage Guide](#usage-guide)
- [System Requirements](#system-requirements)
- [Security Features](#security-features)
- [Future Enhancements](#future-enhancements)

## Features

### User Features
- **Account Creation**: Create new bank accounts with a unique account number, name, PIN, and initial balance
- **Authentication**: Secure login using account number and PIN
- **Balance Inquiry**: Check current account balance
- **Cash Deposit**: Add funds to your account with real-time balance update
- **Cash Withdrawal**: Withdraw funds with balance and ATM cash availability checks
- **Fund Transfer**: Transfer money between accounts with transaction recording
- **Transaction History**: View recent account activities (deposits, withdrawals, transfers)
- **PIN Management**: Securely change your PIN with confirmation

### Admin Features
- **Account Management**: View all registered accounts and their details
- **Account Deletion**: Remove user accounts from the system
- **ATM Cash Management**: Monitor and set ATM cash limits
- **System Balance View**: Check total funds within the ATM system

### System Features
- **Cross-Platform Compatibility**: Works on both Windows and Unix-based systems
- **File Persistence**: All user data and transactions are stored in files
- **Interactive UI**: Clear console-based user interface with intuitive navigation
- **Input Validation**: Robust error checking for user inputs
- **Loading Animations**: Visual feedback during processing operations

## Data Structures and Algorithms

### Data Structures Used
1. **Map (std::map)**: Used to store user accounts with account number as key for O(log n) lookup time
2. **Stack (std::stack)**: Implements transaction history retrieval in LIFO order
3. **Vector (std::vector)**: Used for storing and processing transaction records
4. **Struct (User)**: Custom data structure to encapsulate user account information
5. **Struct (Transaction)**: Custom data structure to represent transaction details
6. **Enum Class (TransactionType)**: Type-safe enumeration for categorizing transactions

### Algorithms and Techniques
1. **Authentication Algorithm**: PIN-based verification system
2. **File I/O Operations**: Reading from and writing to files for data persistence
3. **String Tokenization**: Parsing data using split and trim functions
4. **Account Number Generation**: Incremental algorithm to ensure unique identifiers
5. **Input Validation**: Regular expression-like validation for PIN (numeric check)
6. **Transaction Recording**: Timestamp-based chronological logging
7. **Error Handling**: Comprehensive input validation and error recovery mechanisms

## Real-life Applications

This ATM simulator demonstrates key concepts used in actual banking systems:

1. **Banking Software Training**: Can be used to train new bank employees on ATM operations
2. **Educational Tool**: Teaches students about banking systems and financial transaction processing
3. **Financial Software Development**: Demonstrates core banking software principles
4. **Banking System Prototyping**: Provides a foundation for more complex banking systems
5. **System Design Learning**: Illustrates encapsulation, persistence, and user interface design principles

## Setup Instructions

1. **Clone or download** the project files to your local machine
2. **Compile the code** using a C++ compiler:
   ```
   g++ -std=c++11 atm_simulator.cpp -o atm_simulator
   ```
3. **Run the executable**:
   - On Windows: `atm_simulator.exe`
   - On Linux/Mac: `./atm_simulator`

4. **First-time setup**:
   - The program will automatically create necessary data files
   - Default admin PIN is `0000`

## Usage Guide

### Main Menu Options
1. **Create New Account**: Register as a new user
2. **Login to Existing Account**: Access your account with account number and PIN
3. **Admin Login**: Access administrative functions using admin PIN
4. **Exit**: Close the application

### User Menu Options
1. **Check Balance**: View your current account balance
2. **Deposit Money**: Add funds to your account
3. **Withdraw Money**: Take out cash from your account
4. **Transfer Money**: Send money to another account
5. **View Transaction History**: See your recent transactions
6. **Change PIN**: Update your security PIN
7. **Logout**: Return to main menu

### Admin Menu Options
1. **View All Accounts**: See details of all registered users
2. **Delete User Account**: Remove a user from the system
3. **View Total ATM Balance**: Check system financial status
4. **Set ATM Cash Limit**: Configure maximum cash capacity
5. **Logout**: Return to main menu

## System Requirements

- C++11 compatible compiler
- Terminal or Command Prompt
- Minimum 64MB RAM
- 5MB free disk space

## Security Features

- **PIN Protection**: 4-digit numeric PIN for account access
- **Transaction Logging**: All financial activities are recorded with timestamps
- **Admin Access Control**: Separate authentication for administrative functions
- **Input Validation**: Guards against invalid or malicious inputs
- **Confirmation Steps**: Critical actions require user confirmation

## Future Enhancements

Potential improvements for future versions:

1. **Encryption**: Add data encryption for stored files
2. **Multi-currency Support**: Handle different types of currencies
3. **Account Types**: Add support for savings, checking, and credit accounts
4. **Interest Calculation**: Implement interest accrual algorithms
5. **GUI Interface**: Create a graphical user interface
6. **Multi-user Support**: Allow concurrent access from multiple users
7. **Biometric Authentication**: Add support for fingerprint or facial recognition
8. **Transaction Fees**: Implement service charge calculations
9. **Notifications**: Add SMS or email alerts for transactions
10. **Card Number Support**: Implement credit/debit card functionality
