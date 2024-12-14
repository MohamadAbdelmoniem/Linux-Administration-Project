# Linux Administration Project

## Overview
This repository contains solutions to various Linux administration and programming challenges, including Bash scripting, Makefiles, CMake, and process management. The project demonstrates skills in automation, software development, and system-level programming.

## Folder Structure
- **Task1/**: Contains Bash scripts and related files for system automation tasks.
- **Task2/**: Includes Makefiles, CMakeLists, and source files for cryptographic modules.
- **Task3/**: Hosts the source code for the Enhanced Remote Command Execution Application (ERCEA).

---

## Task Details

### Task 1: Bash Scripting
**Objective**: Automate Linux administrative tasks with Bash scripts.

#### Scripts:
1. **`userdef`**: 
   - Creates a new user and group, assigns the user to the group, and customizes user settings.
   - Key Features:
     - Validates arguments and ensures script runs with `sudo`.
     - Sets user home directory, default shell, and assigns a password.
     - Configures user ID (UID=1600) and sets primary group.
     - Makes the script globally accessible.

2. **`file_manager`**:
   - Automates directory and file management tasks.
   - Key Features:
     - Creates a directory and files, writes content, compresses the directory, and moves it to the new user's home.
     - Performs recursive operations like ownership changes, file searches, and selective file deletions.

---

### Task 2: Cryptographic Modules with Makefiles & CMake
**Objective**: Develop Caesar and XOR cipher modules with reusable Makefiles and CMake configurations.

#### Features:
- **Project Structure**:
  - Cryptographic algorithms are modularized.
  - Object files and libraries are stored in designated directories (`gen/` and `libs/`).
- **Makefiles**:
  - Automates building, cleaning, and managing dependencies for the project.
  - Generates:
    - Static library for Caesar cipher.
    - Dynamic library for XOR cipher.
  - Produces a final executable (`output`).
- **CMake**:
  - Provides equivalent functionality to Makefiles with a structured approach.
  - Three `CMakeLists.txt` files are used for:
    - Application.
    - Caesar cipher module.
    - XOR cipher module.

#### Testing:
Run the command: `./output 3 K "aaa"`.  
Expected output:
```
text encrypted with caesar: ddd
text decrypted with caesar: aaa
text encrypted with xor: ***
text decrypted with xor: aaa
```

---

### Task 3: Enhanced Remote Command Execution Application (ERCEA)
**Objective**: Implement a client-server application for remote command execution.

#### Features:
- **Architecture**: 
  - Client-server communication using TCP.
  - Server handles multiple clients using child processes.
- **Client**:
  - Sends commands to the server for remote execution.
  - Prints the exit status of commands executed on the server.
  - Stops execution upon receiving the `stop` command.
- **Server**:
  - Accepts commands from multiple clients.
  - Executes commands and returns the exit status.
  - Handles `SIGINT` gracefully without leaving zombie processes.

#### Scenario:
1. Client 1: Creates a directory (`mkdir exam_dir`) and lists contents (`ls`).
2. Client 2: Creates a file (`touch exam_file`) and lists contents (`ls`).
3. Both clients terminate using the `stop` command.
4. Server terminates upon receiving `Ctrl+C`.

---

## Prerequisites
- Linux OS with Bash shell.
- GCC and Make for building C projects.
- CMake (for Task 2).

---

## Setup Instructions
1. **Clone the Repository**:
   ```bash
   git clone <https://github.com/MohamadAbdelmoniem/Linux-Administration-Project>
   cd <Linux-Administration-Project>
   ```
2. **Run Scripts**:
   - Navigate to `Task1` and execute the Bash scripts:
     ```bash
     sudo ./userdef.sh <username> <password> <groupname>
     sudo ./file.sh
     ```
3. **Build Cryptographic Modules**:
   - Using Makefiles:
     ```bash
     cd Task2
     make all
     ./output 3 K "aaa"
     make clean
     ```
   - Using CMake:
     ```bash
     mkdir build && cd build
     cmake ..
     make
     ./output 3 K "aaa"
     ```
4. **Run ERCEA**:
   - Compile:
     ```bash
     cd Task3
     gcc server.c -o server
     gcc client.c -o client
     ```
   - Start Server:
     ```bash
     ./server
     ```
   - Start Clients:
     ```bash
     ./client <server-ip> <port>
     ```
     
---
## Notes
- Ensure scripts are run with appropriate permissions.
- Test on a Linux system for optimal compatibility.
- Follow the provided hints and implementation details for accurate results.
## License
This project is licensed under the MIT License. See the LICENSE file for details.

---

Would you like any specific customizations or additional sections for this README?
