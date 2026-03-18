#pragma once

std::string getPrefetchFiles();
std::vector<std::filesystem::directory_entry> sortPrefetchFiles(const std::string& path);
void displayPrefetchFiles(const std::vector<std::filesystem::directory_entry>& files);