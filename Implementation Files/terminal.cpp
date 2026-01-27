#include "terminal.hpp"

namespace Terminal {
    void showWindowSpecificUse(std::string windows_cmd, std::string linux_cmd) {
        std::cerr << "Note: '" << linux_cmd << "' is Linux-only. Use '" << windows_cmd << "' on Windows\n";
    }

    void showLinuxSpecificUse(std::string windows_cmd, std::string linux_cmd) {   
        std::cerr << "Note: '" << windows_cmd << "' is Windows-only. Use '" << linux_cmd << "' on Linux\n";
    }

    void clearScreen(const std::string& cmd) { 
        if(cmd == "cls") {
            #if defined(__linux__) 
                showLinuxSpecificUse("cls", "clear");
                return;
            #endif
        }   
        #ifdef _WIN32
            std::system("cls");
        #else  
            std::system("clear");
        #endif
    }

    void showShellCommands() {
        std::cout << "+----------------------------------------------------- SHELL USAGE CONSTRAINTS -----------------------------------------------------+\n";
        std::cout << "|                                                                                                                                   |\n";
        std::cout << "|                              1. This PowerShell uses a dynamic command parsing engine with flexible spacing                       |\n";
        std::cout << "|                              2. Total input length is limited to 32000 characters per line                                        |\n";
        std::cout << "|                              3. Path-based arguments are not supported. Provide names only, not paths                             |\n";
        std::cout << "|                              4. Run shell from the target directory                                                               |\n";
        std::cout << "|                                                                                                                                   |\n";
        std::cout << "+----------------------------------------------------- SHELL USAGE CONSTRAINTS -----------------------------------------------------+\n\n\n\n";

        std::cout << "+----------------------+-----------------------------------------------+------------------------------------------------------------+\n";
        std::cout << "| Category             | Commands                                      | Improvement Compared to Windows PowerShell                 |\n";
        std::cout << "+----------------------+-----------------------------------------------+------------------------------------------------------------+\n";

        std::cout << "| Exit                 | exit                                          | Same behavior                                              |\n";
        std::cout << "+----------------------+-----------------------------------------------+------------------------------------------------------------+\n";

        std::cout << "| Working Directory    | pwd, cd, cd~                                  | Same behavior                                              |\n";
        std::cout << "+----------------------+-----------------------------------------------+------------------------------------------------------------+\n";

        std::cout << "| Change Directory     | cd .. , cd <dir>                              | Same behavior                                              |\n";
        std::cout << "+----------------------+-----------------------------------------------+------------------------------------------------------------+\n";

        std::cout << "| Create Directory     | md, mkdir                                     | Create multiple directories in single command              |\n";
        std::cout << "+----------------------+-----------------------------------------------+------------------------------------------------------------+\n";

        std::cout << "| List Directory       | dir, ls                                       | Same behavior                                              |\n";
        std::cout << "+----------------------+-----------------------------------------------+------------------------------------------------------------+\n";

        std::cout << "| Create File          | type nul, touch, echo                         | Create multiple files in single command                    |\n";
        std::cout << "|                      |                                               |------------------------------------------------------------|\n";
        std::cout << "|                      |                                               | Echo-specific behavior:                                    |\n";
        std::cout << "|                      |                                               | Extra spaces in input are ignored, only a single space is  |\n";
        std::cout << "|                      |                                               | preserved between tokens to reduce memory usage            |\n";
        std::cout << "|                      |                                               | Output content is normalized to single-space tokens        |\n";
        std::cout << "+----------------------+-----------------------------------------------+------------------------------------------------------------+\n";

        std::cout << "| View / Merge Files   | type, cat                                     | View content of multiple files;                            |\n";
        std::cout << "|                      |                                               | when redirection (>) is used, merge content of multiple    |\n";
        std::cout << "|                      |                                               | files and create any number of new files;                  |\n";
        std::cout << "|                      |                                               | Achieve all of this in single command                      |\n";
        std::cout << "+----------------------+-----------------------------------------------+------------------------------------------------------------+\n";

        std::cout << "| Rename File          | move, mv, rename                              | Rename multiple files in one command using a specified     |\n";
        std::cout << "|                      |                                               | base name (automatic indexing applied)                     |\n";
        std::cout << "+----------------------+-----------------------------------------------+------------------------------------------------------------+\n";

        std::cout << "| Copy File            | copy, cp                                      | Create multiple copies of a file in single command         |\n";
        std::cout << "+----------------------+-----------------------------------------------+------------------------------------------------------------+\n";

        std::cout << "| Delete File / Dir    | del, rmdir, rm                                | Batch deletion of multiple files and directories together  |\n";
        std::cout << "+----------------------+-----------------------------------------------+------------------------------------------------------------+\n";

        std::cout << "| List Processes       | tasklist, ps                                  | Same behavior                                              |\n";
        std::cout << "+----------------------+-----------------------------------------------+------------------------------------------------------------+\n";

        std::cout << "| Kill Process         | taskkill, kill/pkill/killall                  | Accepts any number of PIDs and/or process names in single  |\n";
        std::cout << "|                      |                                               | command (mixed usage supported)                            |\n";
        std::cout << "|                      |                                               |------------------------------------------------------------|\n";
        std::cout << "|                      |                                               | Windows-specific behavior:                                 |\n";
        std::cout << "|                      |                                               | Do not provide process names with the .exe extension       |\n";
        std::cout << "|                      |                                               | Avoid writing the extension                                |\n";
        std::cout << "|                      |                                               | Example (invalid): chrome.exe                              |\n";
        std::cout << "|                      |                                               |                                                            |\n";
        std::cout << "|                      |                                               | taskkill <process_name>  terminates all instances          |\n";
        std::cout << "|                      |                                               | taskkill <PID>           terminates one instance           |\n";
        std::cout << "|                      |                                               |                                                            |\n";
        std::cout << "|                      |                                               | Windows-specific PID limits:                               |\n";
        std::cout << "|                      |                                               | Maximum PID value: 4294967295                              |\n";
        std::cout << "|                      |                                               | Maximum PID digits: 10                                     |\n";
        std::cout << "|                      |                                               |------------------------------------------------------------|\n";
        std::cout << "|                      |                                               | Linux-specific process behavior:                           |\n";
        std::cout << "|                      |                                               | Any process name enclosed in [ ] is treated as a           |\n";
        std::cout << "|                      |                                               | critical system process and will be rejected               |\n";
        std::cout << "|                      |                                               | Example (invalid): [bash]                                  |\n";
        std::cout << "|                      |                                               |                                                            |\n";
        std::cout << "|                      |                                               | kill <PID>             terminates one instance             |\n";
        std::cout << "|                      |                                               | pkill <process_name>   terminates all instances            |\n";
        std::cout << "|                      |                                               | killall <process_name> terminates all instances            |\n";
        std::cout << "|                      |                                               |                                                            |\n";
        std::cout << "|                      |                                               | Linux-specific PID limits:                                 |\n";
        std::cout << "|                      |                                               | Maximum PID value: 4194304                                 |\n";
        std::cout << "|                      |                                               | Maximum PID digits: 7                                      |\n";
        std::cout << "+----------------------+-----------------------------------------------+------------------------------------------------------------+\n";

        std::cout << "| Clear Screen         | cls, clear                                    | Same behavior                                              |\n";
        std::cout << "+----------------------+-----------------------------------------------+------------------------------------------------------------+\n";

    }  

    void showBanner() {
        std::string banner = R"(                                   
         `-._:  .:'   `:::  .:\           |\__/|           /::  .:'   `:::  .:.-'
             \      :          \          |:   |          /         :       /    
              \     ::    .     `-_______/ ::   \_______-'   .      ::   . /      
               |  :   :: ::'  :   :: ::'  :   :: ::'      :: ::'  :   :: :|       
               /     :           :           :           :           :    \
              /______::_____     ::    .     ::    .     ::   _____._::____\
                            `----._:: ::'  :   :: ::'  _.----'                    
                                   `--.       ;::  .--'                           
                                       `-. .:'  .-'                               
                                          \    /                            
                                           \  /                                   
                                            \/)";
        std::cout << banner << "\n\n";
    }

    void showMeta() {
        std::cout << "\033[31mVersion : \033[32mv1.0 (beta release)\033[0m\n"
                  << "\033[31mProject : \033[32mPowerShell 2.0 (Windows & Linux Compatible)\033[0m\n"
                  << "\033[31mAuthor  : \033[32mvHiren\033[0m\n"
                  << "\033[31mPurpose : \033[32movercomes Windows-PowerShell limitations\033[0m\n"
                  << "\033[31mStartup : \033[32mtype 'help' for commands\033[0m\n\n";
    }
}
