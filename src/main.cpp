#include <windows.h>
#include <iostream>
#include <string>
#include <filesystem>

#include "functionList.h"



int main() {

    std::string path = getPrefetchFiles();
    displayPrefetchFiles(path);

    system("pause");
    return 0;
}


std::string getPrefetchFiles() {

  return R"(C:\Windows\Prefetch)"; //defining the path i wanna look thru
}

void displayPrefetchFiles(const std::string& path) {

    //printing the files in the path into console
    for (const auto& file : std::filesystem::directory_iterator(path)) std::cout << file.path() << std::endl; 
}