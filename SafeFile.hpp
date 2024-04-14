#include <mutex>
#include <fstream>
#include <string>

class SafeFile {
public:
    SafeFile(const std::string& filename);
    ~SafeFile();
    void write(const std::string& message);
private:
    std::mutex mtx;
    std::ofstream file;
};
