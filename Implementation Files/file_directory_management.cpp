#include "file_directory_management.hpp"

// ---------------- File Directory Management ----------------

void FileDirectoryManagement::parseToLowercase(std::string& name) {
    for(char& ch : name) {
        if(std::isupper(static_cast<unsigned char>(ch))) {
            ch = std::tolower(static_cast<unsigned char>(ch));
        }
    }
}

bool FileDirectoryManagement::hasOnlyDots(const std::string& name) {
    for(const char& ch : name) 
        if(ch != '.') 
            return false;
    return true;
}

bool FileDirectoryManagement::isValid(const std::string& name) {
    if(hasOnlyDots(name))
        return false;

    static const std::unordered_set<char> invalidSymbols = {
        '<', '>', ':', 
        '"', '/', '\\', 
        '|', '?', '*'
    };

    static const std::unordered_set<std::string> invalidNames = {
        "con", "prn", "aux", "nul",
        "com1", "com2", "com3", "com4", "com5", "com6", "com7", "com8", "com9", "com0",
        "lpt1", "lpt2", "lpt3", "lpt4", "lpt5", "lpt6", "lpt7", "lpt8", "lpt9", "lpt0"
    };

    std::string str = name;
    parseToLowercase(str);
    if(invalidNames.count(str)) return false;

    for(const char& ch : name) {
        #ifdef _WIN32
            if(invalidSymbols.count(ch)) return false; 
        #else
            if(ch == '/' || ch == '\0')  return false;
        #endif 
    }
        
    return true;
}

void FileDirectoryManagement::__delete(const std::vector<std::string>& fileDirs, const std::string& cmd) {
    std::size_t dir_number = 1;

    for(std::size_t i = 0; i < fileDirs.size(); ++i) {
        const std::string& str = fileDirs[i];

        if(!isValid(str) || (!isDirectory(str) && !isFile(str))) {
            std::cerr << dir_number 
                      << ". Error: '" << str << "' is invalid or not a file nor directory\n";
            dir_number++;
            continue;
        }
        else if(cmd == "rmdir") {
            if(isFile(str)) {
                std::cerr << dir_number 
                          << ". Error: '" << str << "' is a file (rmdir requires directory)\n"; 
                dir_number++;
                continue;
            }
            else {
                std::error_code ec;
                fs::directory_iterator it(str, ec);

                if(ec || it != fs::directory_iterator{}) {
                    std::cerr << dir_number
                         << ". Error: Directory '" << str
                         << "' is not empty (rmdir requires empty directory)\n";
                    dir_number++;
                    continue;
                }
            }
        }       

        std::error_code ec;
        fs::remove_all(str, ec);
        if(ec) std::cerr << dir_number << ". Error: Failed to delete '" << str << "'\n";
        dir_number++;
    }
}

std::vector<std::string> FileDirectoryManagement::getNames(const std::vector<std::string>& args, std::size_t start, std::size_t end, char skipChar) {
    std::vector<std::string> listOfNames;

    const std::string& str = args[start];
    if(str[0] == skipChar) {
        std::string name;
        for(std::size_t i = 1; i < str.size(); ++i) name += str[i];    
        if(!name.empty()) listOfNames.push_back(name);
        start++;
    }
    
    while(start < end) {
        listOfNames.push_back(args[start]);
        start++;
    }
    return listOfNames;
}

// ---------------- File Management ----------------

void FileManagement::updateLastWriteTime(const std::string& filename, std::size_t filenumber) {
    std::error_code ec;
    fs::last_write_time(filename, fs::file_time_type::clock::now(), ec);

    std::cout << filenumber << ". ";
    if(ec) {    
        std::cerr << "Error: Failed to update time of file '" << filename << "'\n";
    } 
    else {
        std::cout << "Write time updated of file '" << filename << "'\n";
    }
}

void FileManagement::writeFileContent(const std::string& filename, std::string& filecontent, std::size_t filenumber) {
    if(filecontent.empty()) 
        filecontent = "This file is created by you";
            
    std::ofstream file(filename);
    if(!file.is_open()) { 
        std::cerr << filenumber << ". Error: Failed to write file '" << filename << "'\n";
    }
    else {
        file << filecontent; 
    }
}

void FileManagement::eraseFileContent(const std::string& filename, std::size_t filenumber) {
    std::ofstream file(filename, std::ios::trunc);

    if(file.fail()) {   
        std::cerr << filenumber << ". Error: Failed to erase content of file '" << filename << "'\n";
    }
}

void FileManagement::getFileContent(const std::string& filename, std::string& filecontent) {
    std::ifstream file(filename);
    std::string line;

    while(file && std::getline(file, line)) {
        if(filecontent.size() + line.size() > MAX_CHAR_PER_LINE) { 
            break;
        }
        filecontent += line + "\n";
    }
}

void FileManagement::makeFile(const std::vector<std::string>& files, std::string& filecontent, int query) {
    std::size_t filenumber = 1;

    for(std::size_t i = 0; i < files.size(); ++i) {
        const std::string& filename = files[i];
        
        if(!isValid(filename)) {
            std::cerr << filenumber 
                      << ". Error: The filename '" 
                      << filename << "' is not valid. Specify another file name\n";
        } 
        else if(exists(filename)) { // If file already exists
            if(query == 1) { 
                updateLastWriteTime(filename, filenumber);
            }
            else if(query == 2) {
                eraseFileContent(filename, filenumber);
            }
            else {
                writeFileContent(filename, filecontent, filenumber); 
            }
        }
        else {
            writeFileContent(filename, filecontent, filenumber);    
        }

        filenumber++;
    }
}

void FileManagement::renameFile(const std::vector<std::string>& oldnames, std::string& newname) {
    if(!isValid(newname) || isDirectory(newname) || isFile(newname)) {
        std::cerr << "Error: Cannot accept newname '" 
                  << newname << "' (invalid name or file / directory already exists)\n";
    }
    else {
        std::size_t count_renamed = 1; // Counts how many files renamed
        std::size_t filenumber    = 1; 

        for(std::size_t i = 0; i < oldnames.size(); ++i) {
            const auto& filename = oldnames[i];
                
            std::string count = std::to_string(count_renamed);
            if(i > 0) { // Ignore first file, Append count_renamed to filename
                newname += "_" + count;
            }

            if(isFile(filename)) {
                fs::rename(filename, newname);
                count_renamed++;
            }
            else {  
                std::cerr << filenumber << ". Error: '" 
                          << filename << "' is not a file or not present\n"; 
            }
                
            if(i > 0) { // Ignore first file, Remove count_renamed from filename
                std::size_t j = count.size();
                while(j--) newname.pop_back();
                newname.pop_back();
            }

            filenumber++;
        }
    }
}

void FileManagement::copyFile(const std::string& sourceFile, const std::vector<std::string>& args) {
    if(!isValid(sourceFile) || isDirectory(sourceFile)) {
        std::cerr << "Error: Cannot accept source name '" 
                  << sourceFile << "' (invalid name or directory already exists)\n";
    }
    else if(!isFile(sourceFile)) {
        std::cerr << "Error: File with source name '" 
                  << sourceFile << "' not present. Specify another file name\n";
    }
    else {
        // Fetch content of source file
        std::string filecontent;       
        getFileContent(sourceFile, filecontent);     

        std::vector<std::string> newFiles = getNames(args, 2, args.size(), '\0');
        std::size_t filenumber = 1;

        for(const auto& filename : newFiles) {
            if(!isValid(filename) || isDirectory(filename)) {
                std::cerr << filenumber << ". Error: Cannot accept new name '" 
                          << filename << "' (invalid name or directory already exists)\n";
            }
            else if(isFile(filename)) {
                std::cerr << filenumber << ". Error: File with new name '" 
                          << filename << "' already exists. Specify another file name\n";
            }
            else {
               writeFileContent(filename, filecontent, filenumber); 
            }
            filenumber++;
        }
    }
}

void FileManagement::mergeFileContent(const std::vector<std::string>& contentFiles, const std::vector<std::string>& outputFiles) {
    std::string overallContent;

    for(const auto& filename : contentFiles) {
        getFileContent(filename, overallContent);
    }            

    makeFile(outputFiles, overallContent);
}

void FileManagement::showFileContent(const std::vector<std::string>& files) {
    std::size_t filenumber = 1;

    for(std::size_t i = 0; i < files.size(); ++i) {
        const std::string& filename = files[i];
        std::ifstream file(filename);    

        if(file) {
            std::cout << '\n' << filenumber << ". Content of file '" << filename << "': \n";
            std::string filecontent;
            getFileContent(filename, filecontent);
            std::cout << filecontent;
        }

        filenumber++;
    }
}

// ---------------- Directory Management ----------------

void DirectoryManagement::showDirectoryPath() {
    std::cout << fs::current_path().string() << '\n';
}

void DirectoryManagement::changeToDirectory(const std::string& dir) {
    auto parentSpecified = [] (const std::string& name) -> bool {
        std::size_t dots = 0;
        for(const char& ch : name) dots += (ch == '.');
        return dots == 2;
    };

    if(hasOnlyDots(dir) && !parentSpecified(dir)) {
        std::cerr << "Error: parent reference must be '..'\n";
    }
    else if(!isDirectory(dir)) {
        std::cerr << "Error: " << "No such directory\n";
    }
    else {
        std::error_code ec;
        fs::current_path(dir, ec);
        if(ec) std::cerr << "Error: Failed to change directory\n";
    }
}

void DirectoryManagement::makeDirectory(const std::vector<std::string>& directories) {
    std::size_t dir_number = 1;

    for(std::size_t i = 0; i < directories.size(); ++i) {
        const std::string& dir = directories[i];

        if(!isValid(dir) || isFile(dir) || !createDirectory(dir)) {
            std::cerr << dir_number << ". Error: cannot create directory '" 
                      << dir << "' (invalid name or already exists)\n";
        } 

        dir_number++;
    }
}

void DirectoryManagement::showDirectoryContent(const std::string& dir) {
    if((dir != "." && !isValid(dir)) || !isDirectory(dir)) {
        std::cerr << "Error: " << "No such directory\n";
        return;
    }
        
    #ifdef _WIN32
        // 2> nul; Extract all error messages from stderror and redirect them to nothing
        // 2 stands for stderror file descriptor
        std::string cmd = "dir \"" + dir + "\" 2> nul";

        if(std::system(cmd.c_str()) != 0) {
            std::cerr << "Error: Failed to list directories\n";
        }
    #else
        pid_t pid = fork();
    
        if(pid == 0) { // Child process is created and currently executing           
            std::vector<char*> c_args; 
            std::string cmd = "ls";
            c_args.push_back(const_cast<char*>(cmd.c_str()));
            c_args.push_back(const_cast<char*>(dir.c_str()));
            c_args.push_back(nullptr);

            if(execvp("ls", c_args.data()) == -1) {  // Execute ls command
                std::cerr << "Error: Failed to list directories\n"
                          << "Description: " << strerror(errno) << '\n';
                exit(EXIT_FAILURE);
            }
        }
        else if(pid > 0) { // Child process is created and parent is executing, wait till child terminates
            wait(nullptr);  
        }
        else { // Process creation failed then print description of failure
            std::cerr << "Error: fork() failed, can't list directories\n"
                      << "Description: " << strerror(errno) << '\n';
        }
    #endif
}