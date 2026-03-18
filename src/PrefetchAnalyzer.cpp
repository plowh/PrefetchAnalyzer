#include <windows.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>

#include "functionList.h"

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
        std::cout << file.path() << '\n';
    }
}