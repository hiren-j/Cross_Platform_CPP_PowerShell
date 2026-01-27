ADD DEMO VIDEO - URGENT
---

# 🛠️ Custom PowerShell (C++)

A **cross-platform-powershell** built in **C++ (C++17+)**, designed to **overcome key limitations of Windows PowerShell** by supporting **batch operations, relaxed command parsing, and enhanced command behavior** with **Windows and Linux** Compatibility.

---

## ✨ Key Highlights

* ⚙️ **30+ shell commands implemented**
* 🧠 **Dynamic command parsing engine**

  * Flexible spacing
  * Relaxed syntax
  * Many-to-many input/output handling
* 📂 **Batch file & directory operations**

  * Create, delete, rename, copy multiple files/folders in a single command
* 🔁 **Enhanced PowerShell compatibility**

  * Removes single-target limitations of Windows PowerShell
* 🖥️ **Cross-platform**

  * Works on **Windows** and **Linux**
  * Portable, standard, and modern
* 🧠 **Technical Strengths**

   * OS-aware command behavior  
   * Strong validation and safety checks  
   * Memory-efficient parsing  
   * Clean, modular **C++17 implementation (LLD-focused)**
  
---

## 🎯 Motivation

Windows PowerShell often restricts:

* Creating only **one file or directory per command**
* Rigid command syntax
* Limited batch operations

This shell was built to **remove those restrictions**, enabling:

* Multi-file & multi-directory commands
* Flexible input syntax
* Powerful batch execution — all in a lightweight C++ program

---

## ⚠️ Shell Usage Constraints

```
------------------ SHELL USAGE CONSTRAINTS ------------------
=> Dynamic command parsing with flexible spacing
=> Total input length is limited to 32000 characters per line
=> Path-based arguments are NOT supported
=> Provide names only (run shell from target directory)
=> Ensure C++17 or newer for the standard `std::filesystem` support
-------------------------------------------------------------
```

---

## 🧩 Supported Commands & Enhancements

| Category                | Commands                               | Improvement Compared to Windows PowerShell                                               |
| ----------------------- | -------------------------------------- | ---------------------------------------------------------------------------------------- |
| Exit                    | `exit`                                 | Same behavior                                                                            |
| Working Directory       | `pwd`, `cd`, `cd~`                     | Same behavior                                                                            |
| Change Directory        | `cd ..`, `cd <dir>`                    | Same behavior                                                                            |
| Create Directory        | `md`, `mkdir`                          | Create **multiple directories** in one command                                           |
| List Directory          | `dir`, `ls`                            | Same behavior                                                                            |
| Create File             | `type nul`, `touch`, `echo`            | Create **multiple files** in a single command                                            |
| View / Merge Files      | `type`, `cat`                          | View multiple files; when redirection (>) provided, merge content of multiple files and create **multiple new files**   |
| Rename                  | `move`, `mv`, `rename`                 | Rename **multiple files** in one command (auto indexing)                                 |
| Copy File               | `copy`, `cp`                           | Create **multiple copies** of a single file                                              |
| Delete File / Directory | `del`, `rmdir`, `rm`                   | Batch deletion of files and directories                                                  |
| List Processes          | `tasklist`, `ps`                       | Same behavior                                                                            |
| Kill Process            | `taskkill`, `kill`, `pkill`, `killall` | Accepts **multiple PIDs and/or process names** in one command                            |
| Clear Screen            | `cls`, `clear`                         | Same behavior                                                                            |

---

## 🛠️ Build & Run [PREFER VIDEO](link)
> **Build & Execution Guidelines**
> - Before compiling, ensure all header files (`.hpp`), implementation files (`.cpp`), and `main.cpp` are located in the **same directory** to keep the build process simple.
> - Use **`g++`** for compilation, as it is dedicated to C++ builds. Using `gcc` may lead to linking or standard library errors.
> - If anything is unclear or you face issues while running the shell, **refer to the demo video first** for a quick walkthrough.
### 🔹 Requirements

* **C++17 or later**
* GCC / MinGW / Clang
* Linux or Windows terminal

---

### 🪟 Windows (MinGW / MSYS2 / g++)

```bash
g++ *.cpp -o shell.exe
shell.exe
```

Works in **Command Prompt**, **PowerShell**, **MSYS2**, and **VS Code terminal**.

---

### 🐧 Linux (GCC)

```bash
g++ *.cpp -o shell
./shell
```
---
## 📘 Build Command Explanation (g++)

## Line 1
```
g++ *.cpp -o shell.exe
```

### Meaning of Each Part
- g++ → C++ compiler  
- *.cpp → Compiles all C++ source files in the current directory  
- -o → Output flag  
- shell.exe → Name of the generated executable  

This command builds the application.

---

## Line 2 runs the compiled program

```
shell.exe
```

---

## -o (Output Flag)

The -o flag instructs the compiler where and with what name to generate the final executable.

If -o is omitted:
- Windows → a.exe  
- Linux → a.out  

Using -o ensures clarity and avoids default executable names.
---

## 📂 Project Structure

```
.
├── Header Files/
│   ├── file_directory_management.hpp
│   ├── process_management.hpp
│   └── terminal.hpp
│
├── Implementation Files/
│   ├── file_directory_management.cpp
│   ├── process_management.cpp
│   └── terminal.cpp
│
└── main.cpp
```

---

## 🐞 Bug Reports & Feedback

Found a bug, unexpected behavior, or have a suggestion?

* Open a **GitHub Issue**
* Clearly describe the problem
* Include:

  * OS (Windows/Linux)
  * Command used
  * Expected vs actual behavior

Contributions, improvements, and discussions are **welcome**.

---

## 🙌 Author

Developed with a strong focus on **systems programming**, **CLI design**, and **removing real-world PowerShell limitations** using modern **C++**.

---
