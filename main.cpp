#include "file_directory_management.hpp"
#include "process_management.hpp"
#include "terminal.hpp"

class PowerShell {
public:
    void run() {
        while(true) {
            std::cout<<"$ ";
            std::string line;
            while(!std::getline(std::cin, line)) break;
            
            if(line.size() > MAX_CHAR_PER_LINE) {
                std::cerr << "Error: Command line exceeds maximum length limit ("
                          << MAX_CHAR_PER_LINE << " characters)\n"
                          << "Suggestion: Split into multiple commands\n\n";
            }   
            else {
                auto args = splitLineGetTokens(line);
                if(args.empty()) continue;

                std::string empty;
                parseCommandsToLowercase(args[0], (args.size() > 1 ? args[1] : empty));

                if(args[0] == "exit") break;
                executeCommand(args); 
                std::cout << '\n';
            }
        }
    }

private:
    bool isNull(const std::string& name) {
        return name.size() >= 3 && ((name[0] == 'n' || name[0] == 'N')
                                &&  (name[1] == 'u' || name[1] == 'U')
                                &&  (name[2] == 'l' || name[2] == 'L'));
    }

    void parseCommandsToLowercase(std::string& arg1, std::string& arg2) {
        FileDirectoryManagement fdManager;
        fdManager.parseToLowercase(arg1);

        if(arg1 == "type" && isNull(arg2)) { // Handle 'type nul' specially
            fdManager.parseToLowercase(arg2);
        }
    }

    std::vector<std::string> splitLineGetTokens(const std::string& line) {
        if(line.empty())
            return {};

        std::vector<std::string> listOfTokens;
        std::string token;
        std::size_t idx = 0;

        while(idx < line.size()) {
            if(std::isspace(line[idx])) {
                if(!token.empty()) {
                    listOfTokens.push_back(token);
                }
                token = "";
            }
            else {
                token.push_back(line[idx]);
            }
            idx++;
        }

        // Handle "cd.." as a special case
        if(token == "cd.." || token == "CD.." || token == "Cd.." || token == "cD..") { 
            #ifdef _WIN32
                listOfTokens.push_back("cd");
                listOfTokens.push_back("..");
            #else
                Terminal::showLinuxSpecificUse("cd..", "cd ..");
            #endif
        }
        else if(!token.empty()) {
            listOfTokens.push_back(token); 
        }

        return listOfTokens;
    }

    void executeCommand(const std::vector<std::string>& args) {
        const std::string& cmd = args[0];

        // Displays path of working directory
        if(cmd == "pwd" || (cmd == "cd" && (args.size() == 1 || (args.size() == 2 && args[1] == "~")))) {
            if(cmd == "cd" && args.size() == 1) { 
                #if defined(__linux__)
                    const char* home = std::getenv("HOME");
                    if(home != nullptr) chdir(home); // change to home directory
                #endif
            }
            DirectoryManagement dirManager;
            dirManager.showDirectoryPath();
        }
        
        // Handles directory changing 
        else if(cmd == "cd") {  
            if(args.size() > 2) {
                std::cerr << "Error: Too many arguments. Expected 1 directory\n"
                          << "Usage: cd [directory]\n"
                          << "Example: cd Documents\n";
            }
            else {
                DirectoryManagement dirManager;
                dirManager.changeToDirectory(args[1]); // Handles [cd   ..], [cd foldername]
            }
        }

        // Handle directories making 
        else if(cmd == "md" || cmd == "mkdir") {
            if(cmd == "md") {
                #if defined(__linux__) 
                    Terminal::showLinuxSpecificUse("md", "mkdir");
                    return;
                #endif
            }
            
            if(args.size() == 1) {
                std::cerr << "Error: Missing directory name(s)\n"
                          << "Usage: " << cmd << " [dir1 dir2 dir3 ...]\n"
                          << "Example: " << cmd << " Movies\n";
            }
            else {
                FileDirectoryManagement  fdManager;
                std::vector<std::string> directories = fdManager.getNames(args, 1, args.size(), '\0');
                DirectoryManagement dirManager;
                dirManager.makeDirectory(directories);
            }
        } 

        // Displays directory content
        else if(cmd == "dir" || cmd == "ls") { 
            if(cmd == "dir") {
                #if defined(__linux__) 
                    Terminal::showLinuxSpecificUse("dir", "ls");
                    return;
                #endif
            }

            if(args.size() > 2) {
                std::cerr << "Error: Too many arguments. Expected 1 directory\n"
                          << "Usage: " << cmd << " [directory]\n"
                          << "Example: " << cmd << " Documents\n";
            }
            else {
                DirectoryManagement dirManager;
                std::string targetDir = (args.size() == 2) ? args[1] : "."; // Use current dir if none specified
                dirManager.showDirectoryContent(targetDir);
            }
        }

        // Handles files creation
        else if(cmd == "touch" || cmd == "echo" || (args.size() > 1 && cmd == "type" && isNull(args[1]))) {    
            if(cmd == "touch") {
                #ifdef _WIN32
                    Terminal::showWindowSpecificUse("type nul", "touch");
                    return;
                #endif
                    if(args.size() == 1) {
                        std::cerr << "Error: Missing file name(s)\n"
                                  << "Usage: " << cmd << " [file1 file2 ...]\n"
                                  << "Example: " << cmd << " Document.txt\n";
                    }
                    else {
                        FileDirectoryManagement fdManager;
                        std::vector<std::string> files = fdManager.getNames(args, 1, args.size(), '\0');
                        std::string empty;
                        FileManagement fileManager;
                        fileManager.makeFile(files, empty, 1);
                    }
                return;
            }
            if(cmd == "type") { // type nul
                #if defined(__linux__) 
                    Terminal::showLinuxSpecificUse("type nul", "touch");
                    return;
                #endif
            }

            bool isFirstChar  = true;
            bool seenFileChar = false;    // Tracks where the file name begins
            bool seenContentChar = false; // Tracks where the filecontent begins
            bool seenRedirection = false;
            std::size_t redirectionOperator = 0;                

            std::vector<std::string> files;
            std::string filecontent;

            for(std::size_t i = 1; i < args.size(); ++i) {
                const std::string& str = args[i];
                std::string filename;

                for(std::size_t j = (cmd == "type" && i == 1 ? 3 : 0); j < str.size(); ++j) {
                    if(cmd == "type") { // type nul
                        if(isFirstChar && str[j] != '>') { 
                            std::cerr << "Error: Do not write anything between 'nul' and '>'\n"
                                      << "Usage: type nul > [file1 file2 ...]\n"
                                      << "Example: type nul > Students.txt\n";
                            return;
                        }   
                        isFirstChar = false;
                        if(str[j] != '>') seenFileChar = true;
                        redirectionOperator += (!seenFileChar && str[j] == '>');       
                        if(seenFileChar) filename += str[j];
                    }
                    else { // echo
                        if(str[j] == '>') seenRedirection = true;

                        if(!seenRedirection) {
                            filecontent += str[j];      
                        }
                        else {
                            redirectionOperator += (!seenFileChar && str[j] == '>');
                            if(redirectionOperator > 0 && str[j] != '>') seenFileChar = true; 
                            if(seenFileChar) filename += str[j]; 
                        } 
                    }

                    if(redirectionOperator > 1) {
                        std::cerr << "Error: Multiple '>' operators detected (only 1 allowed)\n";

                        if(cmd == "type") { // type nul
                            std::cerr << "Usage: type nul > [file1 file2 ...]\n"
                                      << "Example: type nul > Students.txt\n";   
                        }
                        else {
                            std::cerr << "Usage: echo content > [file1 file2 ...]\n"
                                      << "Example: echo \"Hello World!\" > Greetings.txt\n";
                        }
                        return;
                    }
                }
                if(!filecontent.empty()) filecontent.push_back(' ');
                if(!filename.empty()) files.push_back(filename);
            }
            if(!filecontent.empty()) filecontent.pop_back();  // Remove extra white space

            if(files.empty()) {
                std::cerr << "Error: Missing file name(s) with redirection operator '>'\n";

                if(cmd == "type") { // type nul
                    std::cerr << "Usage: type nul > [file1 file2 ...]\n"
                              << "Example: type nul > Students.txt\n";
                }
                else { // echo
                    std::cerr << "Usage: echo content > [file1 file2 ...]\n"
                              << "Example: echo \"Hello World!\" > Greetings.txt\n";
                }
            }
            else {
                FileManagement fileManager;
                fileManager.makeFile(files, filecontent, (cmd == "type" ? 2 : -1));
            }
        }           

        // Handles content merging of multiple files to new file, displays file content
        else if(cmd == "type" || cmd == "cat") {
            if(args.size() == 1) {
                std::cerr << "Error: Missing file name(s)\n"
                          << "Usage: " << cmd << " [file1 file2 ...]\n"
                          << "Example: " << cmd << " Document.txt\n";
                return;
            }

            // Find position of redirection operator
            int redirection_pos = -1;
            for(std::size_t i = 1; i < args.size(); ++i) {
                if(args[i][0] == '>') {
                    redirection_pos = i;
                    break;
                }
            }

            FileDirectoryManagement fdManager;    
            FileManagement fileManager;    

            if(redirection_pos != -1) {
                std::vector<std::string> contentFiles = fdManager.getNames(args, 1, redirection_pos, '\0');
                std::vector<std::string> outputFiles  = fdManager.getNames(args, redirection_pos, args.size(), '>');

                if(outputFiles.empty() || outputFiles[0][0] == '>') {
                    std::cerr << "Error: Missing file name(s) or Multiple '>' operators detected\n"
                              << "Usage: " << cmd << " [file1 file2 ...] > [merged1 ...]\n"
                              << "Example: " << cmd << " Teachers.txt Students.txt > Classrooms.txt\n";
                }
                else {
                    fileManager.mergeFileContent(contentFiles, outputFiles);
                }
            }
            else {
                if(cmd == "type") {
                    #if defined(__linux__)
                        Terminal::showLinuxSpecificUse("type", "cat");
                        return;
                    #endif
                }
                std::vector<std::string> filenames = fdManager.getNames(args, 1, args.size(), '\0');
                fileManager.showFileContent(filenames);
            }
        }

        // Handles renaming of files
        else if(cmd == "move" || cmd == "rename" || cmd == "mv") {    
            if(cmd == "move") {
                #if defined(__linux__)
                    Terminal::showLinuxSpecificUse("move", "mv");
                    return;
                #endif
            }

            if(args.size() < 3) {
                std::cerr << "Error: Missing file name(s) with newfile name\n"
                          << "Usage: " << cmd << " [file1 file2 ...] [newfile]\n"
                          << "Example: " << cmd << " Teachers.txt Students.txt Humans.txt\n";
            }
            else {
                FileDirectoryManagement fdManager;
                std::string newname               = args[args.size() - 1];
                std::vector<std::string> oldnames = fdManager.getNames(args, 1, args.size() - 1, '\0');
                FileManagement fileManager;
                fileManager.renameFile(oldnames, newname);
            }
        }

        // Handles copy creation of files
        else if(cmd == "copy" || cmd == "cp") {
            if(cmd == "copy") {
                #if defined(__linux__)
                    Terminal::showLinuxSpecificUse("copy", "cp");
                    return;
                #endif
            }

            if(args.size() < 3) {
                std::cerr << "Error: Missing file name(s)\n"
                          << "Usage: " << cmd << " [file] [newfile1 newfile2 ...]\n"
                          << "Example: " << cmd << " Old-Data.txt New-Data1.txt New-Data2.txt\n";
            }
            else {
                FileManagement fileManager;
                fileManager.copyFile(args[1], args);
            }
        }

        // Handles file, directory deletions
        else if(cmd == "del" || cmd == "rmdir" || cmd == "rm") {
            if(args.size() == 1) {
                std::cerr << "Error: Missing target argument(s)\n"
                          << "Usage: " << cmd << " [arg1 arg2 ...]\n"
                          << "Example: " << cmd << " Movies\n";
                return;
            }
            if(cmd == "del") {
                #if defined(__linux__)
                    Terminal::showLinuxSpecificUse("del", "rm");
                    return;
                #endif
            }
            
            FileDirectoryManagement fdManager;
            std::vector<std::string> fileDirs = fdManager.getNames(args, 1, args.size(), '\0');
            fdManager.__delete(fileDirs, cmd);
        }

        // Displays processes
        else if(cmd == "tasklist" || cmd == "ps") {
            if(cmd == "tasklist") {
                #if defined(__linux__) 
                    Terminal::showLinuxSpecificUse("tasklist", "ps");
                    return;
                #endif
            }

            if(args.size() > 1) {
                std::cerr << "Error: Too many arguments. Expected command only\n"
                          << "Usage: " << cmd << '\n';
            }
            else {
                ProcessManagement processManager;
                processManager.showProcesses();
            }
        }

        // Handles process killing
        else if(cmd == "taskkill" || cmd == "kill" || cmd == "pkill" || cmd == "killall") {
            if(cmd == "taskkill") {
                #if defined(__linux__) 
                    Terminal::showLinuxSpecificUse(cmd, "kill, pkill, killall"); 
                    return;
                #endif
            }
            else {
                #ifdef _WIN32 
                    Terminal::showWindowSpecificUse("taskkill", cmd);
                    return;
                #endif
            }   
            
            if(args.size() == 1) {
                std::cerr << "Error: Missing target argument(s)\n"
                          << "Usage: " << cmd << " process_name\n"
                          << "Example: " << cmd << " firefox\n"
                          << "Usage: " << cmd << " PID\n"
                          << "Example: " << cmd << " 1234\n";
            }
            else {
                FileDirectoryManagement fdManager;
                std::vector<std::string> processes = fdManager.getNames(args, 1, args.size(), '\0');
                ProcessManagement processManager;
                processManager.killProcesses(processes);
            }
        }

        // Handles screen cleaning
        else if(args.size() == 1 && (cmd == "cls" || cmd == "clear")) {
            Terminal::clearScreen(cmd);
        }
        
        // Lists out shell commands with their information
        else if(args.size() == 1 && cmd == "help") {
            Terminal::showShellCommands();
        }

        else {
            std::cerr << "Error: Command not found\n"
                      << "Use 'help' to list all available commands in this shell\n";
        }
    }
};

// Driver code
int main() {
    // Terminal::showBanner();
    Terminal::showMeta();
    
    PowerShell shell;
    shell.run();

    return 0;
}
