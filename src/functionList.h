#pragma once


std::string getPrefetchFiles();
std::vector<std::filesystem::directory_entry> sortPrefetchFiles(const std::string& condensedFiles);
void displayPrefetchFiles(const std::vector<std::filesystem::directory_entry>& files);
std::wstring findExe(const std::wstring& exeName, const std::wstring& root = L"C:\\");
bool isFileSigned(const std::wstring& path);