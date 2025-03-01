#include "SmartHome.h"
#include "SmartLight.h"
#include "TempHumiditySensor.h"
#include "SmartSpeaker.h"
#include "Thermostat.h"
#include "SmartPlug.h"
#include "RadiatorValve.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// Singleton instance getter for the SmartHome class.
// Ensures there is only one instance of the SmartHome object in the program.
SmartHome& SmartHome::getInstance() {
    static SmartHome instance;  // Static instance, initialized only once
    return instance;
}

// Constructor: Initializes the SmartHome object.
// Automatically loads devices from the saved file into the devices vector.
SmartHome::SmartHome() {
    loadDevices();  // Load devices from "smart_home.txt"
}

// Destructor: Ensures the current state of devices is saved to the file when the object is destroyed.
SmartHome::~SmartHome() {
    saveDevices();  // Save devices to "smart_home.txt"
}

// Loads devices from the "smart_home.txt" file.
// Reads the serialized data line by line, creates appropriate device objects, and stores them in the devices vector.
void SmartHome::loadDevices() {
    ifstream file("smart_home.txt");  // Open the file for reading
    if (!file) return;               // Exit if the file does not exist

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string type;
        getline(ss, type, '|');

        unique_ptr<SmartDevice> device;  // Pointer to hold the created device
        if (type == "LIGHT") device = make_unique<SmartLight>("");
        else if (type == "TEMP_HUMIDITY") device = make_unique<TempHumiditySensor>("");
        else if (type == "SPEAKER") device = make_unique<SmartSpeaker>("");
        else if (type == "THERMOSTAT") device = make_unique<Thermostat>("");
        else if (type == "PLUG") device = make_unique<SmartPlug>("");
        else if (type == "RADIATOR") device = make_unique<RadiatorValve>("");

        if (device) {
            device->deserialize(line);  // Restore device state from serialized data
            devices.push_back(move(device));  // Add the device to the list
        }
    }
}

// Saves the current state of all devices to the "smart_home.txt" file.
// Iterates through the devices vector and writes serialized data for each device to the file.
void SmartHome::saveDevices() {
    ofstream file("smart_home.txt");  // Open the file for writing
    for (const auto& device : devices) {
        file << device->serialize() << "\n";  // Serialize each device and write to the file
    }
}

// Lists all devices currently stored in the devices vector.
// Calls the getQuickView() method of each device to display its status.
void SmartHome::listDevices() const {
    if (devices.empty()) {
        cout << "No devices found.\n";  // Inform the user if there are no devices
        return;
    }

    for (const auto& device : devices) {
        cout << device->getQuickView() << "\n";  // Display each device's quick view
    }
}

// Helper function: Performs a case-insensitive comparison of two strings for sorting.
bool caseInsensitiveSortCompare(const string& a, const string& b) {
    string lowerA = a, lowerB = b;
    transform(lowerA.begin(), lowerA.end(), lowerA.begin(), ::tolower);
    transform(lowerB.begin(), lowerB.end(), lowerB.begin(), ::tolower);
    return lowerA < lowerB;  // Compare lowercase versions of the strings
}

// Sorts devices in the devices vector alphabetically by name, ignoring case.
void SmartHome::sortByName() {
    sort(devices.begin(), devices.end(),
        [](const unique_ptr<SmartDevice>& a, const unique_ptr<SmartDevice>& b) {
            return caseInsensitiveSortCompare(a->getName(), b->getName());
        });
    cout << "Devices sorted by name.\n";
}

// Sorts devices in the devices vector by type, and then by name within each type.
// Sorting is case-insensitive for both type and name.
void SmartHome::sortByType() {
    sort(devices.begin(), devices.end(),
        [](const unique_ptr<SmartDevice>& a, const unique_ptr<SmartDevice>& b) {
            string typeA = a->getDeviceType();
            string typeB = b->getDeviceType();
            if (caseInsensitiveSortCompare(typeA, typeB)) {
                return true;
            }
            if (caseInsensitiveSortCompare(typeB, typeA)) {
                return false;
            }
            return caseInsensitiveSortCompare(a->getName(), b->getName());
        });
    cout << "Devices sorted by type and name.\n";
}

// Helper function: Compares two strings case-insensitively to check if they are equal.
bool caseInsensitiveCompare(const string& a, const string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (tolower(a[i]) != tolower(b[i])) {
            return false;
        }
    }
    return true;
}

// Removes a device from the devices vector based on its name.
// If the device is found, it is deleted from the list and memory is cleaned up.
// If not found, an error message is displayed.
// The delete completes after the program ends well. 
void SmartHome::removeDevice(const string& deviceName) {
    auto it = find_if(devices.begin(), devices.end(),
        [&deviceName](const unique_ptr<SmartDevice>& device) {
            return caseInsensitiveCompare(device->getName(), deviceName);
        });

    if (it != devices.end()) {
        cout << "Device \"" << (*it)->getName() << "\" is being deleted.\n";
        devices.erase(it);  // Safely erase the device from the list
    }
    else {
        cout << "Error: Device \"" << deviceName << "\" not found in the system.\n";
    }
}

// Adds a new device to the devices vector based on user input.
// Prompts the user to select the type of device and provide its name.
// Creates the device and adds it to the devices list.
// The added device saves when the program is ended well.
// so if you use the nuclear stop button after adding a device, when you reboot the program, it won't be there.
void SmartHome::addDevice() {
    cout << "\nAvailable device types:\n";
    cout << "1: Light\n";
    cout << "2: Temperature & Humidity Sensor\n";
    cout << "3: Speaker\n";
    cout << "4: Thermostat\n";
    cout << "5: Smart Plug\n";
    cout << "6: Radiator Valve\n";

    int choice;
    cout << "Select device type: ";
    cin >> choice;
    cin.ignore();

    string name;
    cout << "Enter device name: ";
    getline(cin, name);

    unique_ptr<SmartDevice> device;
    switch (choice) {
    case 1: device = make_unique<SmartLight>(name); break;
    case 2: device = make_unique<TempHumiditySensor>(name); break;
    case 3: device = make_unique<SmartSpeaker>(name); break;
    case 4: device = make_unique<Thermostat>(name); break;
    case 5: device = make_unique<SmartPlug>(name); break;
    case 6: device = make_unique<RadiatorValve>(name); break;
    default:
        cout << "Invalid choice.\n";
        return;
    }

    devices.push_back(move(device));  // Add the new device to the list
    cout << "Device added successfully.\n";
}

// Executes the one-click action for a specified device by name.
// Finds the device and calls its oneClickAction() method.
void SmartHome::handleOneClickAction(const string& name) {
    auto it = find_if(devices.begin(), devices.end(),
        [&name](const unique_ptr<SmartDevice>& device) {
            return caseInsensitiveCompare(device->getName(), name);
        });

    if (it != devices.end()) {
        (*it)->oneClickAction();  // Perform the device's one-click action
    }
    else {
        cout << "Device not found.\n";
    }
}

// Allows the user to interact with a specific device by name.
// Displays the device's menu and handles user input for its options.
void SmartHome::interactWithDevice(const string& name) {
    auto it = find_if(devices.begin(), devices.end(),
        [&name](const unique_ptr<SmartDevice>& device) {
            return caseInsensitiveCompare(device->getName(), name);
        });

    if (it != devices.end()) {
        while (true) {
            (*it)->showMenu();
            int choice;
            cout << "Enter choice: ";
            cin >> choice;
            cin.ignore();

            if (choice == 9) break;  // Exit the menu
            if (choice == 5) {       // Edit the device name
                (*it)->editName();
                break;
            }

            (*it)->handleMenuChoice(choice);  // Execute the selected option
        }
    }
    else {
        cout << "Device not found.\n";
    }
}

// Main loop of the SmartHome system.
// Displays the main menu and handles user input for listing devices, sorting, adding devices,
// and interacting with devices by name.
void SmartHome::run() {
    while (true) {
        cout << "\nMenu:\n";
        cout << "[device name]: Perform device's one-click action\n";
        cout << "1: List devices\n";
        cout << "2: Sort by name\n";
        cout << "3: Sort by device type\n";
        cout << "4 [device name]: Select device to interact with\n";
        cout << "5: Add device\n";
        cout << "9: Exit\n";

        string input;
        cout << "Enter choice: ";
        getline(cin, input);

        if (input == "1") {
            listDevices();
        }
        else if (input == "2") {
            sortByName();
        }
        else if (input == "3") {
            sortByType();
        }
        else if (input == "5") {
            addDevice();
        }
        else if (input == "9") {
            break;  // Exit the program
        }
        else if (input.substr(0, 2) == "4 ") {
            interactWithDevice(input.substr(2));  // Interact with a specific device
        }
        else {
            handleOneClickAction(input);  // Perform a one-click action
        }
    }
}
