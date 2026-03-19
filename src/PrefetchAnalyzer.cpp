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

void displayPrefetchFiles(const std::vector<std::filesystem::directory_entry>& files) {

    //printing the files in the path into console
    for (const auto& file : files) {
        std::cout << file.path().filename() << " | ";
        

        // Print the date modified next to the filenames

        auto ftime = std::filesystem::last_write_time(file); // set the files last write time to ftime variable


        //time_point_cast - function that converts one time format to another
        //system_clock::duration - converts from computer clock to human clock
        auto systemTime = std::chrono::time_point_cast<std::chrono::system_clock::duration>( 
            ftime - std::filesystem::file_time_type::clock::now()
            + std::chrono::system_clock::now()
        );

        std::time_t cftime_s = std::chrono::system_clock::to_time_t(systemTime); //converts system clock to time_t

        std::string timeStr = std::ctime(&cftime_s);
        timeStr.pop_back();
        std::cout << timeStr << " | ";

        std::wstring path = file.path().wstring();

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        if (isFileSigned(path)) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << "[SIGNED]\n\n";
        }
        else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cout << "[UNSIGNED]\n\n";
        }
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}


bool isFileSigned(const std::wstring& filePath) {

    WINTRUST_FILE_INFO fileInfo = {};
    fileInfo.cbStruct = sizeof(fileInfo);
    fileInfo.pcwszFilePath = filePath.c_str();

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