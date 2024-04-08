#include <mutex>
#include <fstream>
#include <string>

class SafeFile {
private:
    std::mutex mtx;
    std::ofstream file;
public:
    SafeFile(const std::string& filename);
    ~SafeFile();
    void write(const std::string& message);
};
