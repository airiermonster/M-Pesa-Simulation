# M-PESA SIMULATION CLI Application

A command-line interface simulation of Tanzania's popular mobile money service, **M-PESA**. This application provides basic M-PESA functionalities in a secure, Fun, and user-friendly terminal environment.

> **❗ Disclaimer:** This project is not affiliated with Vodacom Tanzania or its M-PESA service in any way. It is a simple C++ script designed to mimic M-PESA functionalities for educational and recreational purposes. All trademarks, brand names, and logos are the property of Vodacom Tanzania. I, Maxmillian Urio, have no affiliation with them.

## Features 🚀

- 💰 **Send Money**: Transfer funds to other M-PESA users
- 📱 **Buy Airtime**: Purchase airtime for yourself or others
- 💳 **Cash Withdrawal**: Withdraw money from agents or ATMs
- 🏪 **Lipa na M-PESA**: Make payments to businesses
- 👤 **Account Management**: View balance and transaction history
- ⚙️ **Settings**: Update PIN and personal information

## Technical Features ⚙️

- ✅ Real-time transaction processing
- 🔒 Secure PIN verification system
- 💾 Persistent data storage
- 📶 Network provider validation
- 💰 Transaction fee calculation
- 📜 Government levy integration
- 🔢 Unique transaction ID generation

## Installation 📥

1. **Clone the repository:**
   ```bash
   git clone https://github.com/airiermonster/M-Pesa-Simulation.git
   ```

2. **Navigate to the project directory:**
   ```bash
   cd M-Pesa-Simulation
   ```

3. **Compile the application:**
   ```bash
   g++ main.cpp network.cpp transactions.cpp userdata.cpp menu.cpp utils.cpp -o mpesa.exe
   ```

4. **Run the application:**
   ```bash
   ./mpesa.exe
   ```

## Usage 🛠️

### 1️⃣ First-time Setup
- The application will automatically detect first-time usage
- Follow the prompts to set up your account
- Create a secure PIN
- All your account details, including transactions, PIN, etc., will be saved in a text file named "mpesa_data.txt" within the same folder. Please do not delete it

### 2️⃣ Regular Usage
- Login with your PIN
- Select from the available services
- Follow the on-screen instructions

## Security Features 🔐

- 🔑 **PIN-protected access**
- ✅ **Transaction verification**
- 💰 **Balance validation**
- 🛡️ **Input sanitization**
- 🔏 **Secure data storage heheh 😂**

## Supported Networks 📡

- **Vodacom Tanzania PLC**
- **Airtel Tanzania PLC**
- **Tigo (MIC Tanzania PLC)**
- **TTCL (Tanzania Telecommunications Corporation)**
- **Zantel (Zanzibar Telecom PLC)**
- **Halotel (Viettel Tanzania PLC)**

## Development 🛠️

### Prerequisites

- C++ compiler (**I recommend GCC**)
- Basic understanding of **CLI applications**
- Knowledge of **M-PESA services**
- IDE I recommend **VSCode** or **CLION by JetBrains**

### Project Structure 📂
```plaintext
M-Pesa Simulation/
├── main.cpp          # Application entry point
├── menu.cpp          # Menu system implementation
├── mpesa.h           # Header file with declarations
├── network.cpp       # Network provider validation
├── transactions.cpp  # Transaction processing
├── userdata.cpp      # User data management
└── utils.cpp         # Utility functions
```

## Contributing 🤝

1. **Fork the repository**
2. **Create a feature branch:**
   ```bash
   git checkout -b feature/AmazingFeature
   ```
3. **Commit your changes:**
   ```bash
   git commit -m 'Add some AmazingFeature'
   ```
4. **Push to the branch:**
   ```bash
   git push origin feature/AmazingFeature
   ```
5. **Open a Pull Request**

## License 📜

This project is licensed under the MIT License. For details, see the [LICENSE](LICENSE) file.

## Author ✍️

**Maximillian Urio**

- 📧 Email: [airiermonster@gmail.com](mailto:airiermonster@gmail.com)
- 💻 GitHub: [github.com/airiermonster](https://github.com/airiermonster)

## Acknowledgments 🎖️

- **StackOverflow** for resources and support.

