#include <windows.h>
#include <iostream>
#include <string>
#include <filesystem>

#include "functionList.h"

void displayASCII() {

    std::cout << R"(
                      _..-'(                       )`-.._
                   ./'. '||\\.       (\_/)       .//||` .`\.
                ./'.|'.'||||\\|..    )O O(    ..|//||||`.`|.`\.
             ./'..|'.|| |||||\`````` '`"'` ''''''/||||| ||.`|..`\.
           ./'.||'.|||| ||||||||||||.     .|||||||||||| |||||.`||.`\.
          /'|||'.|||||| ||||||||||||{     }|||||||||||| ||||||.`|||`\
         '.|||'.||||||| ||||||||||||{     }|||||||||||| |||||||.`|||.`
        '.||| ||||||||| |/'   ``\||``     ''||/''   `\| ||||||||| |||.`
        |/' \./'     `\./         \!|\   /|!/         \./'     `\./ `\|
        V    V         V          }' `\ /' `{          V         V    V
        `    `         `               V               '         '    '

)" << '\n';
}

int main() {

    displayASCII();
    std::string path = getPrefetchFiles(); //setting the returned value to the path variable
    auto files = sortPrefetchFiles(path);
    displayPrefetchFiles(files);

    system("pause");
    return 0;
}
