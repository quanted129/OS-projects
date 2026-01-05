# Marker Random Array Threads

## Short Description
This project is a multithreaded C++ console application designed for the Windows operating system. It demonstrates process synchronization and thread management using the Windows API. The application consists of a main thread that manages a shared array and coordinates several "marker" threads. These marker threads compete to write their unique IDs into random indices of the shared array using Critical Sections and Events to ensure data integrity and safe execution.

## Features
*   **Multithreading Architecture**: Implements a specific synchronization logic between a main control thread and multiple worker threads.
*   **Windows API Integration**: Utilizes native Windows threading primitives including `CreateThread`, `WaitForSingleObject`, and `WaitForMultipleObjects`.
*   **Synchronization**: Ensures thread safety using Critical Sections (`CRITICAL_SECTION`) for memory access and Events (`CreateEvent`) for signaling state changes.
*   **Interactive Control**: Allows the user to dynamically define array size, thread count, and manually select which threads to terminate during execution.
*   **Unit Testing**: Includes a comprehensive test suite using the GoogleTest framework to verify thread termination and array modification logic.
*   **CMake Build System**: Uses CMake for easy project configuration and dependency management (automatically fetches GoogleTest).

## Installation

### Prerequisites
*   Operating System: Microsoft Windows.
*   C++ Compiler supporting C++23 (e.g., MSVC, MinGW).
*   CMake (Version 3.20 or higher).

### Build Steps
1.  **Clone the repository:**
    ```bash
    git clone https://github.com/quanted129/OS-projects/MarkerRandomArrayThreads
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

This process will generate two executables in the build directory:
*   `MarkerThreads.exe`: The main application.
*   `MarkerArrayTests.exe`: The unit test suite.

## Usage

### Running the Main Application
1.  Run the executable `MarkerThreads.exe` from the command line.
2.  **Input Array Size**: When prompted, enter the size of the array (integer).
3.  **Input Thread Count**: Enter the number of marker threads to launch.
4.  **Observation**: The threads will start filling the array. Once all threads are blocked (cannot find an empty cell), the program will pause and display the current state of the array.
5.  **Interaction**:
    *   The console will output the ID of the thread, the number of marked elements, and the index collision.
    *   Enter the **ID** of the marker thread you wish to terminate.
    *   The selected thread will clear its marked numbers and exit.
    *   Remaining threads will resume operation.
6.  **Completion**: The process repeats until all threads have been terminated.

### Running Tests
To verify the integrity of the threading logic, run the test executable:
```bash
./MarkerArrayTests.exe
```
The output will indicate whether the tests for thread termination and memory writing have passed.
