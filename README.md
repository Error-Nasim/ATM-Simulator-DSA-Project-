# ATM Management System

## 📋 Table of Contents

- [Overview](#overview)
- [Aim/Objectives](#aimobjectives)
- [Tools](#tools)
- [Methodology](#methodology)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Installation & Setup](#installation--setup)
- [Usage](#usage)
- [Results](#results)
- [Conclusion](#conclusion)
- [References](#references)
- [Author](#author)

## 🎯 Overview

A comprehensive ATM Management System developed in C++ that simulates real-world banking operations. The system provides a complete banking experience with user account management, transaction processing, loan services, and administrative controls.

## 🎯 Aim/Objectives

### Primary Objectives:

1. **User Account Management**: Create and manage customer bank accounts with secure authentication
2. **Transaction Processing**: Enable secure deposits, withdrawals, and fund transfers
3. **Financial Services**: Provide loan application and management capabilities
4. **Administrative Control**: Implement admin panel for system oversight and management
5. **Data Persistence**: Ensure all transactions and user data are stored securely
6. **User Experience**: Create an intuitive interface with loading effects and clear feedback

### Secondary Objectives:

- Implement robust error handling and validation
- Maintain transaction history for audit trails
- Provide real-time balance updates
- Ensure data integrity across all operations
- Simulate real ATM constraints (cash limits, withdrawal limits)

## 🛠️ Tools

### Programming Language:

- **C++17/20** - Core development language

### Libraries Used:

- `<bits/stdc++.h>` - Standard C++ libraries
- `<thread>` - Multi-threading support for loading effects
- `<chrono>` - Time-based operations and timestamps
- `<fstream>` - File I/O operations
- `<sstream>` - String stream operations
- `<iomanip>` - Input/output formatting

### Development Environment:

- **Compiler**: GCC/G++ or Microsoft Visual C++
- **Platform**: Cross-platform (Windows/Linux/macOS)
- **File System**: Text-based data storage

### System Requirements:

- C++17 compatible compiler
- Minimum 512MB RAM
- 50MB disk space for data files

## 📊 Methodology

### Development Approach:

1. **Structured Programming**: Modular design with separate functions for each operation
2. **Object-Oriented Design**: Custom structs for User, Loan, and Transaction entities
3. **File-Based Database**: Persistent storage using text files with delimiter separation
4. **State Management**: Real-time data synchronization between memory and storage
5. **User Interface**: Menu-driven console application with clear navigation

### Data Management Strategy:

- **User Data**: Stored in `users.txt` with pipe-delimited format
- **Loan Data**: Maintained in `loans.txt` with comprehensive loan information
- **Transaction History**: Individual files per user for complete audit trails
- **In-Memory Processing**: STL containers (map, vector, stack) for efficient operations

### Security Implementation:

- PIN-based authentication system
- Admin panel with separate authentication
- Input validation and sanitization
- Safe file operations with error handling

## ✨ Features

### User Features:

- ✅ **Account Creation** - Register new bank accounts with initial deposit
- ✅ **Secure Login** - PIN-based authentication system
- ✅ **Balance Inquiry** - Real-time balance checking
- ✅ **Cash Deposit** - Add funds to account with ATM limit validation
- ✅ **Cash Withdrawal** - Withdraw funds with balance and ATM cash verification
- ✅ **Fund Transfer** - Transfer money between accounts instantly
- ✅ **Transaction History** - View recent transactions with timestamps
- ✅ **PIN Management** - Change account PIN securely
- ✅ **Loan Services** - Apply for loans with automatic calculation
- ✅ **Loan Management** - View active loans and make payments

### Administrative Features:

- 🔧 **User Management** - View and delete user accounts
- 🔧 **ATM Cash Management** - Monitor and set ATM cash limits
- 🔧 **Loan Administration** - Review and approve loan applications
- 🔧 **System Configuration** - Set maximum loan limits and system parameters
- 🔧 **Comprehensive Reporting** - View all accounts and pending loans

### Technical Features:

- 💾 **Data Persistence** - All data saved to files automatically
- 🔒 **Error Handling** - Comprehensive validation and error messages
- 🎨 **User Experience** - Loading animations and formatted output
- 📱 **Cross-Platform** - Works on Windows, Linux, and macOS
- 🔄 **Real-Time Updates** - Immediate reflection of all transactions

## 🏗️ System Architecture

### Core Components:

#### 1. Data Structures:

```cpp
struct User {
    int accountNumber;
    string name;
    string pin;
    double balance;
};

struct Loan {
    int loanId;
    int accountNumber;
    double amount;
    double interestRate;
    int termMonths;
    double monthlyPayment;
    double remainingAmount;
    string applicationDate;
    bool approved;
};

struct Transaction {
    int accountNumber;
    TransactionType type;
    double amount;
    string timestamp;
    string description;
};
```

#### 2. File Management:

- **users.txt**: Account information storage
- **loans.txt**: Loan data persistence
- **[name]_[account]\_transactions_.txt**: Individual transaction histories

#### 3. Session Management:

- User session handling
- Admin session control
- Authentication verification
- Menu navigation system

## 🚀 Installation & Setup

### Prerequisites:

```bash
# Install a C++ compiler (if not already installed)
# For Ubuntu/Debian:
sudo apt-get install g++

# For Windows: Install MinGW or Visual Studio
# For macOS: Install Xcode command line tools
xcode-select --install
```

### Compilation:

```bash
# Clone or download the source code
# Navigate to the project directory

# Compile the program
g++ -std=c++17 -o atm_system atm_system.cpp -pthread

# For Windows (MinGW):
g++ -std=c++17 -o atm_system.exe atm_system.cpp -pthread

# Run the program
./atm_system        # Linux/macOS
atm_system.exe      # Windows
```

### File Structure:

```
ATM_Management_System/
├── atm_system.cpp          # Main source code
├── users.txt               # User accounts data (auto-generated)
├── loans.txt               # Loan information (auto-generated)
├── [user]_transactions_.txt # Transaction histories (auto-generated)
└── README.md               # This file
```

## 💻 Usage

### Getting Started:

1. **Run the Application**: Execute the compiled program
2. **Create Account**: Select option 1 to create a new account
3. **Login**: Use your account number and PIN to access services
4. **Perform Transactions**: Use the menu to access various banking services

### Default Admin Credentials:

- **Admin PIN**: `0000`

### Sample Workflow:

```
1. Start Application → Welcome Screen
2. Create New Account → Enter Details → Get Account Number
3. User Login → Enter Credentials → Access User Menu
4. Perform Operations → Deposit/Withdraw/Transfer
5. View History → Check Transaction Records
6. Apply for Loan → Enter Loan Details → Wait for Approval
7. Admin Login → Manage Users/Loans/ATM Settings
```

## 📈 Results

### Performance Metrics:

- **Response Time**: Instantaneous transaction processing
- **Data Integrity**: 100% accuracy in balance calculations
- **File Operations**: Efficient read/write operations with error handling
- **Memory Usage**: Optimized with STL containers
- **Cross-Platform**: Successfully tested on multiple operating systems

### Test Results:

✅ **Account Creation**: Successfully creates unique account numbers  
✅ **Authentication**: Secure PIN verification system  
✅ **Transactions**: Accurate balance updates and validations  
✅ **File Persistence**: Data survives application restarts  
✅ **Error Handling**: Graceful handling of invalid inputs  
✅ **Loan System**: Complete loan lifecycle management  
✅ **Admin Functions**: Full administrative control capabilities

### System Limitations:

- Text-based storage (not encrypted)
- Single-user access at a time
- No network connectivity
- Console-based interface only

## 🎯 Conclusion

The ATM Management System successfully demonstrates a comprehensive banking solution with the following achievements:

### Key Accomplishments:

1. **Complete Banking Operations**: Successfully implemented all core banking functionalities including account management, transactions, and loan services
2. **Data Persistence**: Robust file-based storage system ensuring data integrity across sessions
3. **User Experience**: Intuitive menu-driven interface with loading effects and clear feedback
4. **Administrative Control**: Comprehensive admin panel for system management and oversight
5. **Error Handling**: Robust validation and error handling throughout the application
6. **Scalability**: Modular design allows for easy feature additions and modifications

### Learning Outcomes:

- **File I/O Operations**: Mastered reading from and writing to files with proper error handling
- **Data Structure Design**: Implemented efficient data structures for complex banking operations
- **User Interface Design**: Created intuitive console-based interfaces with enhanced user experience
- **System Design**: Developed a complete system with multiple user roles and functionalities
- **Code Organization**: Structured code with proper modularity and separation of concerns

### Future Enhancements:

- Database integration (MySQL/PostgreSQL)
- Network capabilities for multi-user access
- Graphical User Interface (GUI) implementation
- Enhanced security with encryption
- Mobile application development
- Real-time notifications and alerts
- Advanced reporting and analytics

The project successfully meets all stated objectives and provides a solid foundation for understanding banking system development and file-based data management in C++.

## 📚 References

### Technical Documentation:

1. **C++ Reference**: https://cppreference.com/
2. **STL Containers**: https://cplusplus.com/reference/stl/
3. **File I/O in C++**: https://cplusplus.com/doc/tutorial/files/
4. **Threading in C++**: https://cplusplus.com/reference/thread/

### Banking System References:

1. Modern Banking Systems and ATM Operations
2. Software Engineering Principles for Financial Applications
3. Data Persistence Patterns in C++ Applications
4. User Interface Design for Console Applications

### Development Resources:

1. **GCC Compiler Documentation**: https://gcc.gnu.org/
2. **Cross-Platform C++ Development**: Best practices and guidelines
3. **Code Organization**: Modular programming in C++
4. **Error Handling**: Exception handling and input validation

---

## 👨‍💻 Author

**_NASIM_**

**Project**: ATM Management System  
**Language**: C++  
**Type**: Console Application  
**Purpose**: Educational/Portfolio Project

---

### 📞 Contact & Support

**phone**:1315365416

**Email**:manasm1213@gmail.com

For questions, suggestions, or contributions to this project, please feel free to reach out or submit issues through the project repository.

---

_This README.md provides comprehensive documentation for the ATM Management System. The system demonstrates practical application of C++ programming concepts in developing real-world banking solutions._
