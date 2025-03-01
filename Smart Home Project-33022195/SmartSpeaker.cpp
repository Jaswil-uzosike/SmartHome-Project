#include "SmartSpeaker.h"
#include "SmartHome.h"
#include <iostream>
#include <sstream>

using namespace std;

// Constructor: Initializes the SmartSpeaker with the provided name.
// Dynamically allocates memory for the volume (default: 50%) and isPlaying status (default: false).
SmartSpeaker::SmartSpeaker(const string& name)
    : SmartDevice(name) {
    volume = new int(50);        // Allocate memory for volume
    isPlaying = new bool(false); // Allocate memory for isPlaying
}

// Destructor: Frees dynamically allocated memory for volume and isPlaying.
// Ensures proper cleanup when the SmartSpeaker is deleted.
SmartSpeaker::~SmartSpeaker() {
    delete volume;       // Free memory for volume
    delete isPlaying;    // Free memory for isPlaying
}

// Provides a quick summary of the SmartSpeaker's current status.
// Displays the name, whether it's playing or stopped, the current volume level,
// and a suggested next action (play or stop).
string SmartSpeaker::getQuickView() const {
    stringstream ss;
    ss << name << ": " << (*isPlaying ? "Playing" : "Stopped")
        << " (Vol: " << *volume << "%) [" << (*isPlaying ? "stop" : "play") << "]";
    return ss.str();
}

// Toggles the play/stop state of the SmartSpeaker.
// Changes the isPlaying status by dereferencing the pointer and flipping its value.
void SmartSpeaker::oneClickAction() {
    *isPlaying = !(*isPlaying);  // Dereference pointer to toggle value
}

// Displays the control menu for the SmartSpeaker.
// Includes options for play/stop, adjusting volume, deleting the device, and editing the device name.
void SmartSpeaker::showMenu() const {
    cout << "\nSpeaker Controls for " << name << ":\n";
    cout << "1: Play/Stop (Currently " << (*isPlaying ? "Playing" : "Stopped") << ")\n";
    cout << "2: Adjust Volume (Currently " << *volume << "%)\n";
    cout << "3: Delete Device\n";
    cout << "5: Edit Device Name\n";
    cout << "9: Back to Main Menu\n";
}

// Handles user input from the SmartSpeaker menu.
// Implements actions for toggling play/stop, adjusting volume, deleting the device, and editing its name.
void SmartSpeaker::handleMenuChoice(int choice) {
    switch (choice) {
    case 1:
        oneClickAction();  // Toggle play/stop
        break;
    case 2:
        cout << "Enter volume (0-100): ";
        cin >> *volume;
        *volume = max(0, min(100, *volume));  // Ensure volume stays within bounds
        break;
    case 3:  // Delete device
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
    case 5:
        editName();  // Edit the device name
        break;
    default:
        cout << "Invalid choice.\n";
        break;
    }
}

// Returns the type of the device as a string ("Speaker").
// Used for identifying the type of SmartDevice.
string SmartSpeaker::getDeviceType() const {
    return "Speaker";
}

// Serializes the state of the SmartSpeaker into a string format for storage.
// Includes the name, isOn status, volume, and isPlaying status.
string SmartSpeaker::serialize() const {
    stringstream ss;
    ss << "SPEAKER|" << name << "|" << isOn << "|" << *volume << "|" << *isPlaying;
    return ss.str();
}

// Deserializes the SmartSpeaker's state from a string.
// Restores the name, isOn status, volume, and isPlaying status from the serialized data.
void SmartSpeaker::deserialize(const string& data) {
    stringstream ss(data);
    string tmp;
    getline(ss, tmp, '|'); // type
    getline(ss, name, '|');
    getline(ss, tmp, '|'); isOn = (tmp == "1");
    getline(ss, tmp, '|'); *volume = stoi(tmp);
    getline(ss, tmp, '|'); *isPlaying = (tmp == "1");
}
