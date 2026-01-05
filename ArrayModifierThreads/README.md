# ArrayModifierThreads

ArrayModifierThreads is a C++ console application that demonstrates multithreading capabilities using the Windows API. The program processes an array of integers using concurrent threads to calculate statistics and modify the data based on the results. The application creates an array of integers based on user input and spawns two worker threads. One thread identifies the minimum and maximum elements, while the other calculates the arithmetic mean of the array. Upon completion of these tasks, the main thread replaces all instances of the minimum and maximum values with the calculated integer average and displays the result.

## Features

*   **Multithreading:** Utilizes the Windows API (`CreateThread`) to perform array operations concurrently.
*   **Synchronization:** Implements `WaitForMultipleObjects` to ensure the main thread waits for worker threads to complete before proceeding.
*   **Simulated Latency:** Incorporates `Sleep` delays within worker threads (7ms for comparisons, 12ms for summation) to simulate complex processing and demonstrate thread concurrency.
*   **Unit Testing:** Includes a comprehensive test suite using Google Test to verify array logic and edge cases.
*   **C++23 Standard:** Built using modern C++ standards.

## Installation

To build this project, you need a Windows environment with a C++ compiler (MSVC or MinGW) and CMake installed.

### Prerequisites

*   Windows OS (due to `<windows.h>` dependency)
*   CMake 3.20 or higher
*   C++ Compiler supporting C++23

### Build Steps

1.  Clone the repository:
    ```bash
    git clone https://github.com/quanted129/OS-projects/ArrayModifierThreads
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

## Usage

After compiling, the executable can be found in the build directory (e.g., `Debug/ArrayModifierThreads.exe` or `./ArrayModifierThreads.exe` depending on the generator).

1.  Run the application from the console.
2.  Enter the size of the array when prompted.
3.  Enter the integer elements of the array.
4.  The program will display the output from the worker threads and the final modified array.

### Example

```text
Enter array size: 5
Element 1: 10
Element 2: 50
Element 3: 10
Element 4: 30
Element 5: 20

Found min (10) and max (50).
Found average: 24
Min element: 10
Max element: 50
Average value: 24 (applied as 24)
New array: { 24 24 24 30 20 }
```

### Running Tests

To run the unit tests included in the project:

1.  Navigate to the build directory.
2.  Run the test executable (usually named `Testing.exe`):
    ```bash
    ./Testing.exe
    ```
