#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <wintrust.h>
#include <Softpub.h>

#include "functionList.h"

#pragma comment(lib, "wintrust.lib")


std::string getPrefetchFiles() {

    return R"(C:\Windows\Prefetch)"; //returning the path i wanna look thru
}


std::vector<std::filesystem::directory_entry> sortPrefetchFiles(const std::string& path) {

    std::vector<std::filesystem::directory_entry> files; 

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        files.push_back(entry); //read the files and put them into the vector

    }


    std::sort(files.begin(), files.end(), [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
        return std::filesystem::last_write_time(a) > std::filesystem::last_write_time(b); //return newest files first
        });

    return files;
}

#include <unordered_map>

void displayPrefetchFiles(const std::vector<std::filesystem::directory_entry>& files) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Build a map of all EXEs on C:\ once
    std::unordered_map<std::wstring, std::wstring> exeMap;
    std::wcout << L"Scanning C:\\ for EXE files...\n";
    for (auto& p : std::filesystem::recursive_directory_iterator(L"C:\\", std::filesystem::directory_options::skip_permission_denied)) {
        try {
            if (p.is_regular_file() && p.path().extension() == L".exe") {
                // lowercase filename for case-insensitive matching
                std::wstring filename = p.path().filename().wstring();
                std::transform(filename.begin(), filename.end(), filename.begin(), ::towlower);
                exeMap[filename] = p.path().wstring();
            }
        }
        catch (...) {
            continue; // skip inaccessible files
        }
    }
    std::wcout << L"EXE scan complete.\n\n";

    // Loop through each PF file
    for (const auto& file : files) {
        std::wcout << file.path().filename().wstring() << L" | ";

        // Print last modified time
        auto ftime = std::filesystem::last_write_time(file);
        auto systemTime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()
        );
        std::time_t cftime_s = std::chrono::system_clock::to_time_t(systemTime);
        std::wstring timeStr = std::wstring(std::ctime(&cftime_s), std::ctime(&cftime_s) + 24); // trim newline
        std::wcout << timeStr << L" | ";

        // Extract EXE name
        std::wstring pfFile = file.path().filename().wstring();
        size_t dashPos = pfFile.find(L'-');
        std::wstring exeName = (dashPos != std::wstring::npos) ? pfFile.substr(0, dashPos) : pfFile;

        // lowercase for lookup
        std::wstring exeNameLower = exeName;
        std::transform(exeNameLower.begin(), exeNameLower.end(), exeNameLower.begin(), ::towlower);

        // Look up full path in map
        std::wstring exePath;
        auto it = exeMap.find(exeNameLower);
        if (it != exeMap.end()) exePath = it->second;

        // Check signing
        if (!exePath.empty() && isFileSigned(exePath)) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::wcout << L"[SIGNED]\n\n";
        }
        else if (exePath.empty()) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED);
            std::wcout << L"[EXE NOT FOUND]\n\n";
        }
        else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::wcout << L"[UNSIGNED]\n\n";
        }

        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}

std::wstring findExe(const std::wstring& exeName, const std::wstring& root) {
    for (auto& p : std::filesystem::recursive_directory_iterator(root)) {
        if (p.is_regular_file() && p.path().filename() == exeName) {
            return p.path().wstring();
        }
    }
    return L""; // not found
}


bool isFileSigned(const std::wstring& exeName) {

    WINTRUST_FILE_INFO fileInfo = {};
    fileInfo.cbStruct = sizeof(fileInfo);
    fileInfo.pcwszFilePath = exeName.c_str();

    WINTRUST_DATA winTrustData = {};
    winTrustData.cbStruct = sizeof(winTrustData);
    winTrustData.dwUIChoice = WTD_UI_NONE;
    winTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
    winTrustData.dwUnionChoice = WTD_CHOICE_FILE;
    winTrustData.pFile = &fileInfo;

    GUID policyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;

    LONG status = WinVerifyTrust(
        NULL,
        &policyGUID,
        &winTrustData
    );

    return status == ERROR_SUCCESS;

}