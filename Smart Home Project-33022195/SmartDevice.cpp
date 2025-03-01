#include "SmartDevice.h"
#include <iostream>

using namespace std;

// Constructor: Initializes the SmartDevice object with the provided name.
// Sets the device's initial state to OFF, with no active timer or timer thread running.
SmartDevice::SmartDevice(const string& name)
    : name(name), isOn(false), timer(0), timerRunning(false) {}

// Destructor: Ensures proper cleanup of resources.
// Stops the timer if it is running and joins the timer thread to prevent dangling threads.
SmartDevice::~SmartDevice() {
    stopTimer();  // Ensure the timer stops before destruction
    if (timerThread.joinable()) {
        timerThread.join();  // Safely join the thread
    }
}

// Starts a countdown timer for the SmartDevice.
// The timer runs on a separate thread and decrements the timer every second while the device is ON.
// Automatically turns OFF the device when the timer reaches zero.
void SmartDevice::startTimer(int seconds) {
    if (!isOn) {
        cout << "Cannot start timer: " << name << " is currently OFF.\n";
        return;
    }

    timer = seconds;         // Set the timer duration
    timerRunning = true;     // Mark the timer as running

    // Start a new thread to handle the countdown
    timerThread = thread([this]() {
        while (timer > 0 && timerRunning && isOn) {
            this_thread::sleep_for(chrono::seconds(1));  // Pause for one second
            --timer;  // Decrement the timer

            if (timerRunning && isOn) {
                // Display the remaining time on the same line
                cout << "\rTimer for " << name << ": " << timer << " seconds remaining." << flush;
            }
        }
        // Handle timer completion or interruption
        if (timer == 0 && timerRunning && isOn) {
            cout << "\nTimer for " << name << " has finished. Turning off the device.\n";
            isOn = false;  
            timerRunning = false;
        }
        else if (!isOn) {
            cout << "\nTimer for " << name << " stopped as the device was turned OFF.\n";
            timerRunning = false;
        }
        });

    cout << "Timer started for " << name << "!\n";
}

// Stops the active timer for the SmartDevice.
void SmartDevice::stopTimer() {
    timerRunning = false;  // Stop the timer
}

// Checks if the timer is currently running for the device.
bool SmartDevice::isTimerRunning() const {
    return timerRunning;
}

// Returns the name of the SmartDevice.
string SmartDevice::getName() const {
    return name;
}

// Updates the name of the SmartDevice to a new name provided by the caller.
void SmartDevice::setName(const string& newName) {
    name = newName;  // Update the device name
}

// Allows the user to manually edit the name of the SmartDevice.
void SmartDevice::editName() {
    string newName;
    cout << "Enter new name for the device: ";
    getline(cin, newName);  // Get the new name from user input
    setName(newName);       // Update the device's name
    cout << "Device name updated to: " << name << "\n";
}

// Returns the current ON/OFF state of the SmartDevice.
bool SmartDevice::getIsOn() const {
    return isOn;
}
