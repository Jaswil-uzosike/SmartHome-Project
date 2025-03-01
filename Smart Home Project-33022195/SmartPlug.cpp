#include "SmartPlug.h"
#include "SmartHome.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>

using namespace std;

// Constructor: Initializes the SmartPlug with the provided name.
// Allocates memory for the sleep timer and historic usage data.
// Loads the existing schedule data from the file if available.
SmartPlug::SmartPlug(const string& name)
    : SmartDevice(name) {
    sleepTimer = new int(0);
    historicUsage = new vector<PowerReading>();
    totalEnergy = 0.0;
    lastUpdateTime = time(nullptr);
    loadScheduleFromFile(); // Load existing schedules from file
}

// Destructor: Cleans up dynamically allocated resources (sleepTimer and historicUsage).
// Saves the schedule data to the file before exiting.
SmartPlug::~SmartPlug() {
    delete sleepTimer;
    delete historicUsage;
    saveScheduleToFile(); // Save schedules before exiting
}

// Saves the schedule data to a file in append mode.
// The schedule entries are saved in the format: deviceName|hour|minute|state.
void SmartPlug::saveScheduleToFile() const {
    ofstream outFile("smart_home.txt", ios::app);
    if (outFile) {
        for (const auto& schedule : schedules) {
            outFile << name << "|" << schedule.hour << "|" << schedule.minute
                << "|" << schedule.state << "\n";
        }
        outFile.close();
    }
}

// Loads the schedule data for the SmartPlug from the file.
// Reads the file line by line and populates the schedules vector for matching device names.
void SmartPlug::loadScheduleFromFile() {
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

// Updates the historic power usage data based on the time elapsed since the last update.
// If the plug is ON, energy usage is calculated (500 watts = 0.5 kWh per second)
// and appended to the historicUsage vector.
void SmartPlug::updateHistoricData() {
    time_t now = time(nullptr);
    double secondsElapsed = difftime(now, lastUpdateTime);

    if (isOn && secondsElapsed > 0) {
        float energyUsed = 0.5 * secondsElapsed; // 500 watts -> 0.5 kWh per second
        totalEnergy += energyUsed;

        PowerReading reading;
        reading.energyUsed = energyUsed;
        reading.timestamp = now;

        historicUsage->push_back(reading);
        lastUpdateTime = now; // Update the last recorded time
    }
}

// Provides a quick sumary of the SmartPlug's state, including its ON/OFF status
// and total energy usage. If the timer is running, its remaining time is included.
string SmartPlug::getQuickView() const {
    stringstream ss;
    ss << fixed << setprecision(2);
    ss << name << ": " << (isOn ? "On" : "Off") << " (" << totalEnergy << " kWh total usage)";
    if (isTimerRunning()) {
        ss << " [Timer: " << timer << " seconds remaining]";
    }
    return ss.str();
}

// Toggles the ON/OFF state of the SmartPlug. If turned OFF, the timer is stopped,
// and historic data is updated. When turned ON, the last update time is reset
void SmartPlug::oneClickAction() {
    isOn = !isOn;

    if (!isOn) {
        stopTimer();
        updateHistoricData();
        cout << name << " turned OFF. Timer stopped.\n";
    }
    else {
        cout << name << " turned ON.\n";
        lastUpdateTime = time(nullptr);
    }
}

// Displays the control menu for the SmartPlug
void SmartPlug::showMenu() const {
    cout << "\nSmart Plug Controls for " << name << ":\n";
    cout << "1: Toggle On/Off (Currently " << (isOn ? "On" : "Off") << ")\n";
    cout << "2: Set Sleep Timer\n";
    cout << "3: View Total Energy Usage (" << totalEnergy << " kWh)\n";
    cout << "4: View Historic Power Usage\n";
    cout << "5: Edit Device Name\n";
    cout << "6: View Schedule\n";
    cout << "7: Delete Schedule\n";
    cout << "8: Manage Schedule\n";
    cout << "0: Delete Device\n";
    cout << "9: Back to Main Menu\n";
}

// Handles the user's choice from the SmartPlug menu.
void SmartPlug::handleMenuChoice(int choice) {
    switch (choice) {
    case 1:
        oneClickAction();
        break;
    case 2:
        if (!isOn) {
            cout << "Cannot set timer because " << name << " is OFF. Turn it ON first.\n";
        }
        else {
            int seconds;
            cout << "Enter sleep timer duration in seconds: ";
            cin >> seconds;
            *sleepTimer = seconds;
            startTimer(seconds);
        }
        break;
    case 3:
        updateHistoricData();
        cout << "Total Energy Usage: " << fixed << setprecision(2) << totalEnergy << " kWh\n";
        break;
    case 4:
        cout << "Historic Power Usage:\n";
        for (const auto& reading : *historicUsage) {
            cout << "Energy Used: " << fixed << setprecision(2) << reading.energyUsed
                << " kWh, Timestamp: " << reading.timestamp << "\n";
        }
        break;
    case 5:
        editName();
        break;
    case 6:
        viewSchedule();
        break;
    case 7:
        deleteSchedule();
        break;
    case 8:
        manageSchedule();
        break;
    case 0:  // Delete device
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
        break;
    }
}

// Adds a schedule entry (ON/OFF) based on user input and saves it to the file
void SmartPlug::manageSchedule() {
    int choice;
    cout << "\nManage Schedule:\n1: Schedule ON\n2: Schedule OFF\n3: Back to Menu\nEnter choice: ";
    cin >> choice;

    if (choice == 1 || choice == 2) {
        int hour, minute;
        cout << "Enter time in 24-hour format (HH MM): ";
        cin >> hour >> minute;

        if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60) {
            schedules.push_back({ hour, minute, (choice == 1) ? "ON" : "OFF" });
            cout << "Schedule added.\n";
            saveScheduleToFile();
        }
        else {
            cout << "Invalid time.\n";
        }
    }
}

// Displays all schedule entries for the SmartPlug in a readable format
// If no schedules are set, it notifies the user
void SmartPlug::viewSchedule() const {
    if (schedules.empty()) {
        cout << "No schedules set.\n";
        return;
    }
    cout << "Schedules:\n";
    for (const auto& s : schedules) {
        cout << setw(2) << setfill('0') << s.hour << ":" << setw(2) << s.minute
            << " -> " << s.state << "\n";
    }
}

// Deletes a specific schedule entry based on user selection
void SmartPlug::deleteSchedule() {
    viewSchedule();
    int index;
    cout << "Enter the schedule number to delete: ";
    cin >> index;

    if (index > 0 && index <= static_cast<int>(schedules.size())) {
        schedules.erase(schedules.begin() + index - 1);
        cout << "Schedule deleted.\n";
        saveScheduleToFile();
    }
    else {
        cout << "Invalid number.\n";
    }
}

// Returns the type of the device as a string ("Smart Plug")
string SmartPlug::getDeviceType() const { return "Smart Plug"; }

// Serializes the state of the SmartPlug into a string, including its name, state, and total energy usage
string SmartPlug::serialize() const {
    stringstream ss;
    ss << "PLUG|" << name << "|" << isOn << "|" << totalEnergy;
    return ss.str();
}

// Deserializes the state of the SmartPlug from a string, restoring its name, state, and energy usage
void SmartPlug::deserialize(const string& data) {
    stringstream ss(data);
    string tmp;
    getline(ss, tmp, '|');
    getline(ss, name, '|');
    getline(ss, tmp, '|');
    isOn = (tmp == "1");
    getline(ss, tmp, '|');
    totalEnergy = stof(tmp);
}
