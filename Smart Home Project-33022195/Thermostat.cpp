#include "Thermostat.h"
#include "SmartHome.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

// Constructor: Initializes the Thermostat object with the given name.
// Automatically loads the schedule data from the file when the object is created.
Thermostat::Thermostat(const string& name) : SmartDevice(name) {
    loadScheduleFromFile(); // Load schedule data when initializing
}

// Destructor: Ensures that the schedule data is saved to the file when the object is destroyed.
Thermostat::~Thermostat() {
    saveScheduleToFile(); // Save schedule when the object is destroyed
}

// Displays the control menu for the Thermostat.
void Thermostat::showMenu() const {
    cout << "\nThermostat Controls for " << name << ":\n";
    cout << "1: Toggle On/Off (Currently " << (isOn ? "On" : "Off") << ")\n";
    cout << "2: Manage Schedule\n";
    cout << "3: View Schedule\n";
    cout << "4: Delete Schedule\n";
    cout << "5: Edit Device Name\n";
    cout << "6: Delete Device\n";
    cout << "9: Back to Main Menu\n";
}

// Processes the user's choice from the menu and performs the corresponding action.
void Thermostat::handleMenuChoice(int choice) {
    switch (choice) {
    case 1:
        oneClickAction();
        break;
    case 2:
        manageSchedule();
        break;
    case 3:
        viewSchedule();
        break;
    case 4:
        deleteSchedule();
        break;
    case 5:
        editName();
        break;
    case 6:  // Delete device
        cout << "\nAre you sure you want to delete this device?\n";
        cout << "1: Yes, delete\n";
        cout << "2: No, cancel\n";
        cout << "Enter your choice: ";

        int confirmChoice;
        cin >> confirmChoice;

        if (confirmChoice == 1) {
            SmartHome::getInstance().removeDevice(name);  // Pass the device name for deletion
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

// Allows the user to add ON/OFF schedules for the Thermostat.
// Prompts the user for a time in 24-hour format and the desired state (ON/OFF).
// Valid schedules are added to the schedules vector and saved to the file.
void Thermostat::manageSchedule() {
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
            schedules.push_back(newSchedule);
            cout << "Schedule added: " << setw(2) << setfill('0') << hour << ":"
                << setw(2) << setfill('0') << minute << " -> " << newSchedule.state << "\n";
            saveScheduleToFile();
        }
        else {
            cout << "Invalid time. Please enter a valid time in 24-hour format.\n";
        }
    }
}

// Displays all scheduled ON/OFF times for the Thermostat in a readable format.
// If no schedules exist, informs the user.
void Thermostat::viewSchedule() const {
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

// Deletes a specific schedule based on the user's input.
// Prompts the user to select a schedule by its index and removes it from the schedules vector.
// Saves the updated schedules to the file after deletion.
void Thermostat::deleteSchedule() {
    if (schedules.empty()) {
        cout << "No schedules to delete.\n";
        return;
    }

    viewSchedule();
    int index;
    cout << "Enter the schedule number to delete (1-" << schedules.size() << "): ";
    cin >> index;

    if (index >= 1 && index <= static_cast<int>(schedules.size())) {
        schedules.erase(schedules.begin() + index - 1);
        cout << "Schedule deleted successfully.\n";
        saveScheduleToFile();
    }
    else {
        cout << "Invalid schedule number.\n";
    }
}

// Saves the schedules to the file ("smart_home.txt") in append mode.
// Each schedule is written as a line in the format: deviceName|hour|minute|state.
void Thermostat::saveScheduleToFile() const {
    ofstream outFile("smart_home.txt", ios::app);  // Append schedules to file
    if (outFile) {
        for (const auto& schedule : schedules) {
            outFile << name << "|" << schedule.hour << "|" << schedule.minute
                << "|" << schedule.state << "\n";
        }
        outFile.close();
    }
}

// Loads the schedules for the Thermostat from the file ("smart_home.txt").
// Reads each line, checks for matching device names, and populates the schedules vector.
void Thermostat::loadScheduleFromFile() {
    ifstream inFile("smart_home.txt");
    string line, deviceName, state;
    int hour, minute;

    schedules.clear();

    while (getline(inFile, line)) {
        stringstream ss(line);
        getline(ss, deviceName, '|');
        if (deviceName == name) {
            ss >> hour;
            ss.ignore();
            ss >> minute;
            ss.ignore();
            getline(ss, state);
            schedules.push_back({ hour, minute, state });
        }
    }
    inFile.close();
}

// Provides a brief summary of the Thermostat's current state.
// Displays the name and whether the heating is ON or OFF.
string Thermostat::getQuickView() const {
    return name + ": " + (isOn ? "Heating On" : "Heating Off");
}

// Toggles the Thermostat's ON/OFF state.
// Updates the user about the new state.
void Thermostat::oneClickAction() {
    isOn = !isOn;
    cout << name << " is now " << (isOn ? "ON" : "OFF") << ".\n";
}

// Returns the device type as "Thermostat".
// Useful for identifying the type of device in mixed environments.
string Thermostat::getDeviceType() const {
    return "Thermostat";
}

// Serializes the Thermostat's state into a string for storage.
// Includes the device name and ON/OFF status.
string Thermostat::serialize() const {
    stringstream ss;
    ss << "THERMOSTAT|" << name << "|" << isOn;
    return ss.str();
}

// Deserializes the Thermostat's state from a string.
// Restores the device name and ON/OFF status from the serialized data.
void Thermostat::deserialize(const string& data) {
    stringstream ss(data);
    string tmp;
    getline(ss, tmp, '|');  // type
    getline(ss, name, '|');
    getline(ss, tmp, '|');
    isOn = (tmp == "1");
}
