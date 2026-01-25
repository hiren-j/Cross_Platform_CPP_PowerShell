#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <climits>
#include <csignal>
#include <cstddef>      
#include <cctype>
#include <string>
#include <vector>

#if defined(__linux__)
    #include <sys/types.h> // pid_t
    #include <sys/wait.h>  // wait()
    #include <unistd.h>    // fork(), execvp()
    #include <signal.h>    // kill(), SIGTERM
    #include <cstring>     // strerror()
    #include <cerrno>      // errno
    #define LINUX_PID_MAX 4194304UL
    #define LINUX_PID_MAX_DIGIT 7
#else
    #define WINDOWS_PID_MAX 4294967295U
    #define WINDOWS_PID_MAX_DIGIT 10
#endif
    using U_LONG = unsigned long;

class ProcessManagement {
protected:
    bool isPID(const std::string& str);
    bool isDangerousPID(const U_LONG process_id);
    bool isDangerousProcess(std::string& process_name);
    void killProcessById(const U_LONG process_id, const std::size_t process_counter);
    void killProcessByName(const std::string& process_name, const std::size_t process_counter);

public:
    void killProcesses(const std::vector<std::string>& processes);
    void showProcesses();
};

#endif // PROCESS_MANAGEMENT_HPP