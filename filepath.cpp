#include <chrono>
#include <filesystem>  
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    try {
        const fs::path pathToShow{ argc >= 2 ? argv[1] : fs::current_path() };

        for (const auto& entry : fs::directory_iterator(pathToShow)) {
            const auto filenameStr = entry.path().filename().string();
            if (entry.is_directory()) {
                std::cout << "dir:  " << filenameStr << '\n';
            }
            else if (entry.is_regular_file()) {
                std::cout << "file: " << filenameStr << '\n';
            }
            else
                std::cout << "??    " << filenameStr << '\n';
        }
    }
    catch (const fs::filesystem_error& err) {
        std::cerr << "filesystem error! " << err.what() << '\n';
        if (!err.path1().empty())
            std::cerr << "path1: " << err.path1().string() << '\n';
        if (!err.path2().empty())
            std::cerr << "path2: " << err.path2().string() << '\n';
    }
    catch (const std::exception& ex) {
        std::cerr << "general exception: " << ex.what() << '\n';
    }
}