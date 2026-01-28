#include "file_directory_management.hpp"
#include "process_management.hpp"

bool ProcessManagement::isPID(const std::string& str) {
    #ifdef _WIN32
        if(str.size() > WINDOWS_PID_MAX_DIGIT) return false;
    #else
        if(str.size() > LINUX_PID_MAX_DIGIT)   return false;
    #endif

    // Check has only digits
    for(const char ch : str) { 
        if(!std::isdigit(static_cast<unsigned char>(ch))) {
            return false;
        }
    }

    const U_LONG val = std::stoul(str);
    #ifdef _WIN32
        return val <= WINDOWS_PID_MAX; 
    #else
        return val <= LINUX_PID_MAX;   
    #endif
}

bool ProcessManagement::isDangerousPID(const U_LONG process_id) {
    #ifdef _WIN32
        return process_id == 0 || process_id == 4;
    #else
        return process_id <= 2;
    #endif
}

bool ProcessManagement::isDangerousProcess(std::string& process_name) {
    #ifdef _WIN32
        // Process names in windows are case-insensitive
        FileDirectoryManagement fdManager;
        fdManager.parseToLowercase(process_name);

        static const std::unordered_set<std::string> windowsCriticals = {
            "csrss", "winlogon", "services", "lsass", "smss",
            "system", "wininit", "explorer", "svchost",
            "dwm", "lsm", "fontdrvhost", "registry",
            "systemidleprocess", "runtimebroker", "taskhostw", "logonui"
        };
        return windowsCriticals.count(process_name);
    #else
        if(process_name.size() > 1 && process_name[0] == '[' && process_name.back() == ']') {
            return true;
        }

        static const std::unordered_set<std::string> linuxCriticals = {
            "systemd", "init", "kthreadd", "ksoftirqd", "rcu_sched", "rcu_bh",
            "migration", "watchdog", "cpuset", "khelper", "kdevtmpfs", "netns",
            "perf", "khungtaskd", "writeback", "crypto", "bioset", "kblockd",
            "ata_sff", "md", "devfreq_wq", "kswapd0", "vmstat", "fsnotify_mark",
            "ecryptfs-kthrea", "kthrotld", "acpi_thermal_pm", "kworker", 
            "ipv6_addrconf", "kauditd", "khugepaged", "kstrp", "zswap-shrink",
            "kcompactd0", "irq", "scsi_eh", "scsi_tmf", "ttm_swap", "drm-rendernode",
            "udevd", "systemd-udevd", "dbus-daemon", "networkmanager", "cron", 
            "crond", "sshd"
        };
        return linuxCriticals.count(process_name);
    #endif
}

void ProcessManagement::killProcessById(const U_LONG process_id, const std::size_t process_counter) {
    #ifdef _WIN32
        std::string cmd = "taskkill /F /PID " + std::to_string(process_id) + " > nul 2>&1";

        if(std::system(cmd.c_str()) != 0) {
            std::cerr << process_counter 
                      << ". Error: Process not found or failed to kill " << process_id << '\n';
        }
    #else
        // TERMINATION SIGNAL for process to exit while cleaning up resources
        int signal = SIGTERM; 
        
        if(kill(process_id, signal) != 0) {
            std::cerr << process_counter 
                      << ". Error: Failed to kill process " << process_id << '\n'
                      << "   Description: " << strerror(errno) << '\n';
        }
    #endif  
}

void ProcessManagement::killProcessByName(const std::string& process_name, const std::size_t process_counter) {
   #ifdef _WIN32
        std::string cmd = "taskkill /F /IM " + process_name + ".exe > nul 2>&1";

        if(std::system(cmd.c_str()) != 0) {
            std::cerr << process_counter 
                      << ". Error: Process not found or failed to kill '" << process_name << "'\n";
        }
    #else
        pid_t pid = fork();

        if(pid == 0) {
            const char* c_args[] = {
                "killall", 
                "-q",
                process_name.c_str(),
                nullptr
            };

        if(execvp("killall", (char* const*)c_args) == -1) { // Execute command
                std::cerr << process_counter 
                          << ". Error: Failed to kill process '" << process_name << "'\n" 
                          << "   Description: " << strerror(errno) << '\n';
                exit(EXIT_FAILURE);
            }
        }
        else if(pid > 0) {
            wait(nullptr);
        }
        else {
            std::cerr << process_counter
                      << ". Error: fork() failed, can't kill '" << process_name << "'\n"
                      << "   Description: " << strerror(errno) << '\n';
        }
    #endif
}

void ProcessManagement::killProcesses(const std::vector<std::string>& processes) {
    std::size_t process_counter = 1;

    for(std::size_t i = 0; i < processes.size(); ++i) {
        const std::string& str = processes[i];

        if(isPID(str)) {
            U_LONG process_id = std::stoul(str); 

            if(isDangerousPID(process_id)) {
                std::cerr << process_counter 
                          << ". Error: PID " << process_id << " may be a critical system process\n"
                          << "   Note : This could crash your system - deletion not allowed\n";
            }
            else {
                killProcessById(process_id, process_counter);
            }
        }
        else {
            std::string process_name = str;

            if(isDangerousProcess(process_name)) {
                std::cerr << process_counter 
                          << ". Error: Process '" << process_name << "' may be a critical system process\n"
                          << "   Note : This could crash your system - deletion not allowed\n";
            }
            else {
                killProcessByName(process_name, process_counter);
            }
        }

        process_counter++;
    }
}

void ProcessManagement::showProcesses() {
    #ifdef _WIN32
        std::string cmd = "tasklist 2> nul";

        if(std::system(cmd.c_str()) != 0) {
            std::cerr << "Error: Failed to list processes\n";
        }
    #else
        pid_t pid = fork();

        if(pid == 0) {
            std::vector<char*> c_args;
            std::string cmd = "ps";
            c_args.push_back(const_cast<char*>(cmd.c_str()));
            c_args.push_back(nullptr);

            if(execvp("ps", c_args.data()) == -1) { 
                std::cerr << "Error: Failed to list processes\n"
                          << "Description: " << strerror(errno) << '\n';
                exit(EXIT_FAILURE);
            }
        }
        else if(pid > 0) {
            wait(nullptr);
        }
        else {
            std::cerr << "Error: fork() failed, can't list processes\n"
                      << "Description: " << strerror(errno) << '\n';
        }
    #endif
}