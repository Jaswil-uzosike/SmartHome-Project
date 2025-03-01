# Smart Home System

## Overview
This project is an **Advanced Programming** assessment. It is a **Smart Home System** developed in **C/C++** using **Visual Studio 2022**. The system allows users to manage various smart home devices using an object-oriented approach while demonstrating good software engineering principles.

## Features
- **Device Management**
  - Add, list, and sort devices.
  - Perform actions such as switching on/off, adjusting brightness, setting schedules, etc.
- **Supported Devices**
  - **Light** (on/off, adjust brightness, sleep timer)
  - **Temperature & Humidity Sensor** (live and historical data)
  - **Speaker** (volume, play/stop)
  - **Heating Thermostat** (on/off, schedule, heating boost)
  - **Socket/Plug** (on/off, schedule, sleep timer, energy usage tracking)
  - **Radiator Valve** (on/off, temperature, schedule)
- **State Persistence**
  - Devices are loaded from a file at startup and saved back at shutdown.
- **Command-Line Interface (CLI)**
  - Intuitive CLI for interaction and device control.

## Usage
Run the program and interact using the following options:
```
[device name]: Perform that device’s one-click action
1: List devices
2: Sort by name
3: Sort by device type (then by name)
4 [device name]: Select device to interact with its full feature set
5: Add device
9: Exit
```
Each device has a **Quick View** that shows its status with a single-action command for ease of use.

## Code Structure
- **Encapsulation & OOP Principles**
  - The program follows **object-oriented design** with well-structured classes and inheritance.
  - Each device type is implemented using **polymorphism** and **runtime type identification**.
- **Memory Management**
  - Efficient use of **dynamic memory** and **smart pointers** to prevent memory leaks.
- **Standard Template Library (STL)**
  - Utilizes STL containers for efficient data handling.
- **Data Persistence**
  - Uses a single file to store device states, ensuring data is retained between sessions.

## Installation & Compilation
### Requirements:
- **Visual Studio 2022**
- **C++17 or later**

### Steps:
1. Clone the repository:
   ```sh
   git clone https://github.com/Jaswil-uzosike/SmartHome-Project.git
   ```
2. Open the project in **Visual Studio 2022**.
3. Build and run the project.

## Best Practices Followed
✔ Proper **object-oriented design** (encapsulation, inheritance, and polymorphism)
✔ Efficient **memory management** (avoiding leaks with smart pointers)
✔ Clean and structured **code separation** using functions and classes
✔ Robust **error handling** for user input
✔ Use of **STL** for optimized data management

## Assessment Guidelines
This project adheres to the assessment brief, demonstrating:
- Strong understanding of **advanced C++ programming concepts**.
- Proper implementation of **object-oriented principles**.
- Efficient management of **device interactions and state persistence**.

## Author
Jaswil Uzosike

## License
This project is for educational purposes and follows university guidelines on academic integrity.
