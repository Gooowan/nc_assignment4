#include "SafeFile.hpp"

SafeFile::SafeFile(const std::string& filename) {
    file.open(filename, std::ofstream::app);
}

SafeFile::~SafeFile() {
    if (file.is_open()) {
        file.close();
    }
}

void SafeFile::write(const std::string& message) {
    std::lock_guard<std::mutex> guard(mtx);
    file << message << std::endl;
}
