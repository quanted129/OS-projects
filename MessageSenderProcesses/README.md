# Inter-Process Message Queue

This project implements an inter-process communication (IPC) system on Windows using C++. It facilitates message transfer between a single Receiver process and multiple Sender processes through a shared binary file (memory-mapped file). The system utilizes a circular FIFO (First-In-First-Out) queue protected by synchronization primitives (Mutexes and Events) to ensure thread-safe data transfer.

## Features
*   **One-to-Many Architecture:** Supports one Receiver process managing connections from multiple Sender processes.
*   **Shared Memory Communication:** Uses Windows File Mapping to create a shared binary buffer accessible by distinct processes.
*   **Circular FIFO Queue:** Implements a ring buffer algorithm to manage message storage efficiently.
*   **Synchronization:**
    *   **Mutexes:** Ensure exclusive access to the shared memory during read/write operations.
    *   **Events:** Handle synchronization for "Queue Full" and "Queue Empty" states, putting processes in a waiting state when necessary to prevent busy waiting.
*   **Automatic Process Management:** The Receiver application automatically spawns the specified number of Sender processes.
*   **Unit Testing:** Includes comprehensive tests using the Google Test framework to verify queue logic, overflow handling, and data integrity.

## Installation

### Prerequisites
*   **Operating System:** Windows (The project relies on the Windows API).
*   **Compiler:** C++ compiler supporting C++23 (or compatible with the provided source).
*   **Build System:** CMake.

### Build Steps
1.  **Clone the repository:**
    ```bash
    git clone https://github.com/quanted129/OS-projects/MessageSenderProcesses
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

This process will generate the executables: `Receiver.exe`, `Sender.exe`, and `Tests.exe`.

## Usage

### Running the Application
1.  Navigate to the directory containing the compiled executables (usually inside `build/Debug` or `build/Release`).
2.  Launch the **Receiver** application.

### Interaction Guide
Once the `Receiver.exe` is running, follow the on-screen prompts:

1.  **Configuration:**
    *   Enter the name for the binary file (e.g., `data.bin`).
    *   Enter the capacity of the queue (number of messages).
    *   Enter the number of Sender processes to launch.

2.  **Process Initialization:**
    *   The Receiver will create the shared memory and automatically launch the specified number of `Sender.exe` windows.
    *   The Receiver waits until all Senders signal they are ready.

3.  **Sending Messages (Sender Window):**
    *   In a Sender window, type `send` to initiate a transfer.
    *   Enter the message text (maximum 20 characters).
    *   If the queue is full, the Sender will wait until space becomes available.
    *   Type `exit` to close the Sender process.

4.  **Receiving Messages (Receiver Window):**
    *   In the Receiver window, type `read` to retrieve the next message from the queue.
    *   If the queue is empty, the Receiver will wait until a new message arrives.
    *   Type `exit` to terminate the Receiver and close all child processes.

### Running Tests
To verify the internal logic of the queue (creation, full/empty behavior, truncation):
1.  Run the test executable from the build directory:
    ```bash
    ./Tests.exe
    ```
