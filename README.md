ADD DEMO VIDEO - URGENT

---

# 🚀 Custom PowerShell-Like Shell (C++)

A **cross-platform-powershell** built in **C++ (C++17+)**, designed to **overcome key limitations of Windows PowerShell** by supporting **batch operations, relaxed command parsing, and enhanced command behavior** across **Windows and Linux**.

---

## ✨ Key Highlights

* ⚙️ **30+ shell commands implemented**
* 🧠 **Dynamic command parsing engine**

  * Flexible spacing
  * Relaxed syntax
  * Many-to-many input/output handling
* 📂 **Batch file & directory operations**

  * Create, delete, rename, copy multiple files in one command
* 🔁 **Enhanced PowerShell compatibility**

  * Removes single-target limitations of Windows PowerShell
* 🖥️ **Cross-platform**

  * Works on **Windows** and **Linux**
* 🧩 **C++17+ filesystem API**

  * Portable, standard, and modern

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

## 🧩 Supported Command Categories

| Category                | Examples                                                 |
| ----------------------- | -------------------------------------------------------- |
| Directory Management    | `md`, `mkdir`, `cd`, `pwd`, `dir`, `ls`                  |
| File Creation           | `touch`, `type nul`, `echo`                              |
| File Viewing & Merging  | `type`, `cat`, redirection (`>`)                         |
| File Rename / Move      | `move`, `mv`, `rename`                                   |
| File Copy               | `copy`, `cp`                                             |
| File / Directory Delete | `del`, `rmdir`, `rm`                                     |
| Process Management      | `tasklist`, `ps`, `taskkill`, `kill`, `pkill`, `killall` |
| Screen Control          | `cls`, `clear`                                           |

> Many commands support **multiple inputs and outputs in a single execution**.

---

## ⚠️ System Usage Constraints

```
------------------ SHELL USAGE CONSTRAINTS ------------------
=> Dynamic command parsing with flexible spacing
=> Maximum input length: 32000 characters per line
=> Path-based arguments are NOT supported
=> Provide names only (run shell from target directory)
-------------------------------------------------------------
```

---

## 🛠️ Build & Run

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

> Works in **Command Prompt**, **PowerShell**, **MSYS2**, or **VS Code terminal**

---

### 🐧 Linux (GCC)

```bash
g++ *.cpp -o shell
./shell
```

---

## 🎥 Video Demos

* ▶️ **Windows Demo:**
  👉 *[Add Windows demo video link here]*

* ▶️ **Linux Demo:**
  👉 *[Add Linux demo video link here]*

*(You can upload demo videos to YouTube or GitHub Releases and paste links here.)*

---

## 🧠 Technical Details

* Uses `std::filesystem` (C++17+) for portable filesystem operations
* OS-aware behavior (Windows vs Linux)
* Strong validation & safety checks
* Memory-efficient parsing
* Clean modular architecture (headers + implementations)

---

## 📌 Platform-Specific Notes

* **Windows**

  * Supports drive switching (`C:`, `D:`)
  * `taskkill` behavior replicated and enhanced
* **Linux**

  * Unified filesystem model
  * Process safety rules enforced for critical processes

---

## 📂 Project Structure (example)

```
.
├── main.cpp
├── file_directory_management.hpp / .cpp
├── process_management.hpp / .cpp
├── terminal.hpp / .cpp
└── README.md
```

---

## 🏷️ Topics / Tags

```
cpp cxx17 shell powershell custom-shell cli
filesystem process-management cross-platform
windows linux systems-programming
```

---

## 📜 License

This project is released under the **MIT License**.
Feel free to use, modify, and learn from it.

---

## 🙌 Author

Developed with a focus on **systems programming**, **CLI design**, and **overcoming real PowerShell limitations** using modern C++.

---

Just tell me 👍
