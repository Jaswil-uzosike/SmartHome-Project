#include "SmartLight.h"
#include "SmartHome.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// Constructor: Initializes a SmartLight object with the given name and sets default brightness to 100%.
SmartLight::SmartLight(const string& name) : SmartDevice(name) {
    brightness = new int(100);  // Dynamically allocate memory for brightness
}

// Destructor: Cleans up the SmartLight object by releasing allocated memory for brightness.
SmartLight::~SmartLight() {
    delete brightness;
}

// Returns a quick overview of the device's status.
// Displays the name, brightness level if ON, or "off" if the light is OFF.
string SmartLight::getQuickView() const {
    stringstream ss;
    ss << name << ": " << (isOn ? to_string(*brightness) + "% Brightness [switch off]" : "off [switch on]");
    return ss.str();
}

// Toggles the On/Off state of the SmartLight.
// Since it's a sleep timer, if the device is turned off, any active timer is also stopped.
// This same principle applies to all of the classes with the timer functionality.
void SmartLight::oneClickAction() {
    isOn = !isOn;
    cout << (isOn ? name + " is now ON." : name + " is now OFF.") << endl;

    if (!isOn) {
        stopTimer(); // Stop the timer if the device is turned off
    }
}

// Displays the control menu for the SmartLight.
void SmartLight::showMenu() const {
    cout << "\nLight Controls for " << name << ":\n";
    cout << "1: Toggle On/Off (Currently " << (isOn ? "On" : "Off") << ")\n";
    cout << "2: Adjust Brightness (Currently " << *brightness << "%)\n";
    cout << "3: Set Sleep Timer (Countdown Timer)\n";
    cout << "5: Edit Device Name\n";
    cout << "6: Delete Device\n";
    cout << "9: Back to Main Menu\n";
}

// Handles user input for the menu options.
void SmartLight::handleMenuChoice(int choice) {
    switch (choice) {
    case 1:
        oneClickAction();
        break;
    case 2:
        cout << "Enter brightness (0-100): ";
        cin >> *brightness;
        *brightness = max(0, min(100, *brightness)); // Clamp brightness between 0 and 100
        break;
    case 3:
        if (!isOn) {
            cout << "Cannot set a timer because " << name << " is OFF. Turn it ON first.\n";
        }
        else {
            int seconds;
            cout << "Enter timer duration in seconds: ";
            cin >> seconds;
            startTimer(seconds); // Start a countdown timer
        }
        break;
    case 5:
        editName();
        break;
    case 6:
        cout << "\nAre you sure you want to delete this device?\n";
        cout << "1: Yes, delete\n";
        cout << "2: No, cancel\n";
        cout << "Enter your choice: ";

        int confirmChoice;
        cin >> confirmChoice;

        if (confirmChoice == 1) {
            SmartHome::getInstance().removeDevice(name);  // Pass device name for deletion
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

// Returns the type of the device as a string.
string SmartLight::getDeviceType() const {
    return "Smart Light";
}

// Serializes the SmartLight's data into a single string.
// Includes the device type, name, On/Off state, and brightness level.
string SmartLight::serialize() const {
    stringstream ss;
    ss << "LIGHT|" << name << "|" << isOn << "|" << *brightness;
    return ss.str();
}

// Deserializes a string to restore the SmartLight's data.
void SmartLight::deserialize(const string& data) {
    stringstream ss(data);
    string tmp;
    getline(ss, tmp, '|'); // type
    getline(ss, name, '|');
    getline(ss, tmp, '|'); isOn = (tmp == "1");
    getline(ss, tmp, '|'); *brightness = stoi(tmp);
}
