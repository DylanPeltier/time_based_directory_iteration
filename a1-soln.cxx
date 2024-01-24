//
//  main.cpp
//  a1
//
//  Created by Dylan Peltier on 2024-01-24.
//

#include <iostream>
#include <chrono>
#include <filesystem>
#include <regex>

using namespace std;
using namespace std::literals::chrono_literals;

namespace fs = std::filesystem;
namespace chr = std::chrono;

int main(int argc, const char * argv[]) {
    
    auto const cur_time = chr::file_clock::now();
    auto min = cur_time - (7 * 24h);
    auto max= cur_time - 24h;
    
    fs::path currentPath;
    
    if (argc >= 2) {
        currentPath = argv[1];
    } else {
        currentPath = fs::current_path();
    }
    
    try {
        for (const auto& entry : fs::directory_iterator(currentPath, fs::directory_options::skip_permission_denied)) {
            static const std::regex regex("[[:alnum:]]+\\.txt", std::regex_constants::ECMAScript | std::regex_constants::icase);
            
            std::string name = entry.path().filename().string();
            
            if (std::regex_match(name, regex)) {
                auto fileTime = fs::last_write_time(entry);
                
                auto epoch = fileTime.time_since_epoch();
                auto seconds = chr::duration_cast<chr::seconds>(epoch);
                std::time_t time = seconds.count();
                
                if (fileTime >= min && fileTime <= max) {
                    cout << "FOUND: " << entry.path().string() << " " << std::ctime(&time);
                }
            }
        }
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
