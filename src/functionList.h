#pragma once

std::string getPrefetchFiles();
std::vector<std::filesystem::directory_entry> sortPrefetchFiles(const std::string& condensedFiles);
void displayPrefetchFiles(const std::vector<std::filesystem::directory_entry>& files);