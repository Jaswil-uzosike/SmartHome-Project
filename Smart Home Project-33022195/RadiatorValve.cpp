#include "RadiatorValve.h"
#include "SmartHome.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

// Constructor: Initializes a RadiatorValve object and loads schedule data from a file.
RadiatorValve::RadiatorValve(const string& name) : SmartDevice(name) {
    loadScheduleFromFile();  // Load schedule data on initialization
}

// Destructor: Cleans up the RadiatorValve object and saves schedule data to a file.
RadiatorValve::~RadiatorValve() {
    saveScheduleToFile();  // Save schedule when the object is destroyed
}

// Displays the menu options for controlling the RadiatorValve.
void RadiatorValve::showMenu() const {
    cout << "\nHeating Controls for " << name << ":\n";
    cout << "1: Toggle On/Off (Currently " << (isOn ? "On" : "Off") << ")\n";
    cout << "2: Set Target Temperature\n";
    cout << "3: Manage Schedule\n";
    cout << "4: View Schedule\n";
    cout << "5: Edit Device Name\n";
    cout << "6: Delete Schedule\n";
    cout << "7: Delete Device\n";  // New delete option
    cout << "9: Back to Main Menu\n";
}

// Handles user input for the menu options.
void RadiatorValve::handleMenuChoice(int choice) {
    switch (choice) {
    case 1:
        oneClickAction();
        break;
    case 2:
        float temp;
        cout << "Enter target temperature: ";
        cin >> temp;
        cout << "Target temperature set to " << temp << "C.\n";
        break;
    case 3:
        manageSchedule();
        break;
    case 4:
        viewSchedule();
        break;
    case 5:
        editName();
        break;
    case 6:
        deleteSchedule();
        break;
    case 7:  // Delete device
        cout << "\nAre you sure you want to delete this device?\n";
        cout << "1: Yes, delete\n";
        cout << "2: No, cancel\n";
        cout << "Enter your choice: ";

        int confirmChoice;
        cin >> confirmChoice;

        if (confirmChoice == 1) {
            SmartHome::getInstance().removeDevice(name);  // Pass the device name
        }
        else if (confirmChoice == 2) {
            cout << "Deletion cancelled.\n";
        }
        else {
            cout << "Invalid choice. Returning to menu.\n";
        }
        break;

    default:
        cout << "Invalid choice.\n";
    }
}

// Allows the user to add, view, or delete schedules for the RadiatorValve.
// Schedules are stored in a vector of structures with hour, minute, and state (ON/OFF).
// The schedule does not turn the device on or off at the scheduled time. It basically just saves and displays.
// The same applies to all classes with the schedule functionality. That was all I could manage.
void RadiatorValve::manageSchedule() {
    int choice;
    cout << "\nManage Schedule:\n";
    cout << "1: Schedule ON\n";
    cout << "2: Schedule OFF\n";
    cout << "3: Back to Device Menu\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1 || choice == 2) {
        int hour, minute;
        cout << "Enter time in 24-hour format (HH MM): ";
        cin >> hour >> minute;

        if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60) {
            Schedule newSchedule = { hour, minute, (choice == 1) ? "ON" : "OFF" };
            schedules.push_back(newSchedule);  // Add the new schedule to the vector
            cout << "Schedule added: " << setw(2) << setfill('0') << hour << ":"
                << setw(2) << setfill('0') << minute << " -> " << newSchedule.state << "\n";
            saveScheduleToFile();  // Save the updated schedule to the file
        }
        else {
            cout << "Invalid time. Please enter a valid time in 24-hour format.\n";
        }
    }
}

// Displays all the schedules for the RadiatorValve.
// Iterates over the vector of schedules and prints each schedule.
void RadiatorValve::viewSchedule() const {
    if (schedules.empty()) {
        cout << "No schedules set.\n";
        return;
    }

    cout << "\nScheduled Times:\n";
    for (const auto& schedule : schedules) {
        cout << setw(2) << setfill('0') << schedule.hour << ":"
            << setw(2) << setfill('0') << schedule.minute
            << " -> " << schedule.state << "\n";
    }
}

// Deletes a schedule based on user input.
// Prompts the user to select a schedule by its index and removes it from the vector.
void RadiatorValve::deleteSchedule() {
    if (schedules.empty()) {
        cout << "No schedules to delete.\n";
        return;
    }

    viewSchedule();  // Show the user the current schedules
    int index;
    cout << "Enter the schedule number to delete (1-" << schedules.size() << "): ";
    cin >> index;

    if (index >= 1 && index <= static_cast<int>(schedules.size())) {
        schedules.erase(schedules.begin() + index - 1);  // Erase the selected schedule
        cout << "Schedule deleted successfully.\n";
        saveScheduleToFile();  // Save the updated schedule to the file
    }
    else {
        cout << "Invalid schedule number.\n";
    }
}

// Saves the current schedules to a file.
// Appends each schedule in the vector to "smart_home.txt" in a specific format.
void RadiatorValve::saveScheduleToFile() const {
    ofstream outFile("smart_home.txt", ios::app);  // Append schedules to file
    if (outFile) {
        for (const auto& schedule : schedules) {
            outFile << name << "|" << schedule.hour << "|" << schedule.minute
                << "|" << schedule.state << "\n";
        }
        outFile.close();
    }
}

// Loads schedules for the RadiatorValve from "smart_home.txt".
// Goes through each line of the file to extract and store schedules for the current device
void RadiatorValve::loadScheduleFromFile() {
    ifstream inFile("smart_home.txt");
    string line, deviceName, state;
    int hour, minute;

    schedules.clear();  // Clear any existing schedules

    while (getline(inFile, line)) {
        stringstream ss(line);
        getline(ss, deviceName, '|');
        if (deviceName == name) {  // Check if the line belongs to this device
            ss >> hour;
            ss.ignore();
            ss >> minute;
            ss.ignore();
            getline(ss, state);
            schedules.push_back({ hour, minute, state });  // Add the schedule
        }
    }
    inFile.close();
}

// Returns a quick overview of the device's status (On/Off).
string RadiatorValve::getQuickView() const {
    return name + ": " + (isOn ? "Heating On" : "Heating Off");
}

// Toggles the On/Off state of the RadiatorValve.
void RadiatorValve::oneClickAction() {
    isOn = !isOn;
    cout << name << " is now " << (isOn ? "ON" : "OFF") << ".\n";
}

// Returns the type of the device as a string.
string RadiatorValve::getDeviceType() const {
    return "Radiator Valve";
}

// Serializes the RadiatorValve's data into a string for storage.
string RadiatorValve::serialize() const {
    stringstream ss;
    ss << "RADIATOR|" << name << "|" << isOn;
    return ss.str();
}

// Deserializes a string to restore the RadiatorValve's data.
void RadiatorValve::deserialize(const string& data) {
    stringstream ss(data);
    string tmp;
    getline(ss, tmp, '|');  // type
    getline(ss, name, '|');
    getline(ss, tmp, '|');
    isOn = (tmp == "1");
}
