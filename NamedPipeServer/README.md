# Named Pipe Process Synchronization

This project is a C++ application designed to demonstrate inter-process communication (IPC) and synchronization using Windows Named Pipes. It implements a client-server architecture where a central Server process manages concurrent access to a binary file containing employee records. The system handles the "Reader-Writer" problem by enforcing record-level locking: allowing multiple clients to read a specific record simultaneously while requiring exclusive access for modification.

## Features
*   **Client-Server Architecture:** Utilizes Windows Named Pipes for reliable communication between the server and multiple client processes.
*   **Concurrent Access Control:** Implements a custom synchronization mechanism to manage parallel requests.
    *   **Read Lock:** Allows multiple clients to view the same record without blocking each other.
    *   **Write Lock:** Ensures exclusive access for a client modifying a record, blocking other readers and writers until the operation is complete.
*   **Binary File Management:** Stores and retrieves structured data (`employee` struct) from a binary file.
*   **Automatic Process Management:** The server is responsible for spawning the specified number of client processes.
*   **Unit Testing:** Includes a test suite (using a custom test runner) to verify data structure integrity and locking logic.

## Installation

### Prerequisites
*   **Operating System:** Windows (The project uses the Windows API headers `<windows.h>`).
*   **Compiler:** C++ compiler supporting C++23 (or compatible with the provided CMake configuration). MSVC (Visual Studio) is recommended.
*   **Build System:** CMake (Version 3.20 or higher).

### Build Steps
1.  Clone the repository:
    ```bash
    git clone https://github.com/quanted129/OS-projects/NamedPipeServer
    cd ToDoManager
    ```
2.  Create a build directory:
    ```bash
    mkdir build
    cd build
    ```
3.  Generate the build files using CMake:
    ```bash
    cmake ..
    ```
4.  Compile the project:
    ```bash
    cmake --build .
    ```
    This will generate the executables (`Server.exe`, `Client.exe`, and `Tests.exe`) in the `bin` directory (or `Debug`/`Release` subdirectories depending on your generator).

## Usage

### 1. Starting the Server
Run the `Server` executable from the command line. You will be prompted to perform the initial setup:
1.  Enter the filename for the database (e.g., `data.bin`).
2.  Enter the number of employee records to create.
3.  Input the initial data for each employee (ID, Name, Hours).
4.  The server will display the initial file content.
5.  Enter the number of Client processes to launch.

The server will automatically spawn the specified number of `Client` console windows and wait for connections.

### 2. Client Operations
In each Client window, a menu will appear allowing the following operations:
*   **Modify record:**
    1.  Enter the Employee ID.
    2.  The client requests a **Write Lock**. If another process is reading or writing this record, the client waits.
    3.  Once locked, the current data is displayed.
    4.  Enter new Name and Hours.
    5.  Confirm sending the update to the server.
    6.  Press a key to finish and release the lock.
*   **Read record:**
    1.  Enter the Employee ID.
    2.  The client requests a **Read Lock**. This will succeed even if other clients are reading the same record, but will wait if a writer is active.
    3.  The data is displayed.
    4.  Press a key to finish and release the lock.
*   **Exit:** Terminates the client process.

### 3. Shutting Down
*   Close clients using the "Exit" menu option.
*   Once all interactions are complete, return to the Server console window.
*   Press any key to stop the server. The program will display the final state of the modified file before exiting.

## Running Tests
To verify the internal logic of data structures and the synchronization algorithms, run the `Tests` executable generated during the build process:

```bash
./Tests.exe
```

This will execute the defined test cases (DataStructs/Employee and Concurrency/Locks) and report the results.
