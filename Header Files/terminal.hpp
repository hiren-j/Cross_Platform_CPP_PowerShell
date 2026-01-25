#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <iostream>
#include <cstdlib>
#include <string>

namespace Terminal {
    void showWindowSpecificUse(std::string windows_cmd, std::string linux_cmd);
    void showLinuxSpecificUse(std::string windows_cmd, std::string linux_cmd);
    void clearScreen(const std::string& cmd);
    void showShellCommands();    
    void showBanner();
    void showMeta();
}

#endif // TERMINAL_HPP