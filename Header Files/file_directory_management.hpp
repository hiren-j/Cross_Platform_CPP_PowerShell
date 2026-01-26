#ifndef FILE_DIRECTORY_MANAGEMENT_HPP
#define FILE_DIRECTORY_MANAGEMENT_HPP

#include <unordered_set>
#include <system_error>   
#include <filesystem>  
#include <algorithm>     
#include <iostream>
#include <fstream>
#include <cstdlib> // system()
#include <cstddef>      
#include <vector>
#include <string>
#include <cctype>   

namespace fs = std::filesystem;

#if defined(__linux__)
    #include <sys/types.h> // pid_t
    #include <sys/wait.h>  // wait()
    #include <unistd.h>    // fork(), execvp()
    #include <cstring>     // strerror()
    #include <cerrno>      // errno
#endif

#define MAX_CHAR_PER_LINE 32000
#define exists fs::exists
#define isFile fs::is_regular_file
#define isDirectory fs::is_directory
#define createDirectory fs::create_directory

class FileDirectoryManagement {
public:
    void parseToLowercase(std::string& name);
    bool hasOnlyDots(const std::string& name);
    bool isValid(const std::string& name);
    void __delete(const std::vector<std::string>& fileDirs, const std::string& cmd);
    std::vector<std::string> getNames(const std::vector<std::string>& args, std::size_t start, std::size_t end, char skipChar);
};

class FileManagement : private FileDirectoryManagement {
private:
    void updateLastWriteTime(const std::string& filename, std::size_t filenumber);
    void writeFileContent(const std::string& filename, std::string& filecontent, std::size_t filenumber);
    void eraseFileContent(const std::string& filename, std::size_t filenumber);
    void getFileContent(const std::string& filename, std::string& filecontent);

public:    
    void makeFile(const std::vector<std::string>& files, std::string& filecontent, int query = -1);
    void renameFile(const std::vector<std::string>& oldnames, std::string& newname);
    void copyFile(const std::string& sourceFile, const std::vector<std::string>& args);
    void mergeFileContent(const std::vector<std::string>& contentFiles, const std::vector<std::string>& outputFiles);
    void showFileContent(const std::vector<std::string>& files);
};

class DirectoryManagement : private FileDirectoryManagement {
public:
    void showDirectoryPath();
    void changeToDirectory(const std::string& dir);
    void makeDirectory(const std::vector<std::string>& directories);
    void showDirectoryContent(const std::string& dir);
};

#endif // FILE_DIRECTORY_MANAGEMENT_HPP
