#include <windows.h>
#include <iostream>
#include <string>
#include <filesystem>

#include "functionList.h"



int main() {

    std::string path = getPrefetchFiles(); //setting the returned value to the path variable
    displayPrefetchFiles(path);

    system("pause");
    return 0;
}
