#include "TempHumiditySensor.h"
#include "SmartHome.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>
#include <chrono>

using namespace std;

// Constructor: Initializes the TempHumiditySensor with the provided name.
// Dynamically allocates memory for historicData and historicUsage.
// Initializes energy tracking variables and sets the last update time.
TempHumiditySensor::TempHumiditySensor(const string& name)
    : SmartDevice(name) {
    historicData = new vector<Reading>();          // Holds temperature and humidity readings
    historicUsage = new vector<EnergyReading>();   // Holds energy usage readings
    totalEnergy = 0.0;                             // Tracks total energy consumed
    lastUpdateTime = time(nullptr);                // Tracks the last energy update
}

// Destructor: Frees dynamically allocated memory for historicData and historicUsage.
// Ensures proper cleanup of resources.
TempHumiditySensor::~TempHumiditySensor() {
    delete historicData;    // Free memory for sensor data
    delete historicUsage;   // Free memory for energy usage data
}

// Updates the temperature and humidity readings of the sensor.
// Uses a random generator to simulate real-world readings.
// The readings are stored in the historicData vector along with a timestamp.
// Updates energy usage after adding a new reading.
void TempHumiditySensor::updateSensorReadings() {
    static random_device rd;                                  // Random device for seed
    static mt19937 gen(rd());                                // Mersenne Twister RNG
    static uniform_real_distribution<> tempDist(18.0, 30.0); // Temperature range
    static uniform_real_distribution<> humidityDist(30.0, 70.0); // Humidity range

    Reading reading;                                         // Struct to hold the reading
    reading.temperature = tempDist(gen);                    // Generate random temperature
    reading.humidity = humidityDist(gen);                   // Generate random humidity
    reading.timestamp = time(nullptr);                      // Current timestamp

    historicData->push_back(reading);                        // Store reading in vector

    cout << "Updated Sensor Reading:\n";
    cout << "Temperature: " << fixed << setprecision(1) << reading.temperature << "C\n";
    cout << "Humidity: " << fixed << setprecision(1) << reading.humidity << "%\n";

    updateEnergyUsage();  // Update energy usage whenever readings are updated
}

// Updates the energy usage of the sensor based on the time elapsed since the last update.
// Calculates energy in kilowatt-hours (0.5 kWh per second) and adds it to the total.
// Stores the usage in the historicUsage vector along with a timestamp.
void TempHumiditySensor::updateEnergyUsage() {
    time_t now = time(nullptr);
    double secondsElapsed = difftime(now, lastUpdateTime);   // Time since last update

    if (isOn && secondsElapsed >= 1) {
        float energyUsed = 0.5 * secondsElapsed;             // Energy calculation
        totalEnergy += energyUsed;                           // Add to total energy

        EnergyReading reading;                               // Struct to hold the energy reading
        reading.energyUsed = energyUsed;
        reading.timestamp = now;

        historicUsage->push_back(reading);                   // Store reading in vector
        lastUpdateTime = now;                                // Update the last update time
    }
}

// Adds an energy reading if the device is ON by calling updateEnergyUsage.
// Ensures energy tracking stays consistent.
void TempHumiditySensor::addEnergyReading() {
    if (isOn) {
        updateEnergyUsage();
    }
}

// Provides a quick summary of the sensor's current state.
// Displays ON/OFF status, total energy usage, and the sensor's name.
string TempHumiditySensor::getQuickView() const {
    stringstream ss;
    ss << fixed << setprecision(2);
    ss << name << ": " << (isOn ? "On" : "Off") << " | Total Energy: " << totalEnergy << " kWh";
    return ss.str();
}

// Toggles the ON/OFF state of the sensor.
// Updates energy usage when turning OFF and resets the energy tracking timer when turning ON.
void TempHumiditySensor::oneClickAction() {
    isOn = !isOn;
    cout << name << " is now " << (isOn ? "ON." : "OFF.") << "\n";

    if (!isOn) {
        updateEnergyUsage(); // Final energy update when turning off
    }
    else {
        lastUpdateTime = time(nullptr); // Reset energy tracking time
    }
}

// Displays the control menu for the sensor, including options for toggling ON/OFF,
// updating readings, viewing data, and deleting the device.
void TempHumiditySensor::showMenu() const {
    cout << "\nTemperature & Humidity Sensor Controls for " << name << ":\n";
    cout << "1: Toggle On/Off (Currently " << (isOn ? "On" : "Off") << ")\n";
    cout << "2: Update Sensor Readings\n";
    cout << "3: View Historic Temperature/Humidity Data\n";
    cout << "4: View Total Energy Usage\n";
    cout << "5: Edit Device Name\n";
    cout << "6: Delete Device\n";
    cout << "9: Back to Main Menu\n";
}

// Processes user input from the sensor's menu and executes the appropriate action.
void TempHumiditySensor::handleMenuChoice(int choice) {
    switch (choice) {
    case 1:
        oneClickAction();
        break;
    case 2:
        updateSensorReadings();
        break;
    case 3:
        viewHistoricData();
        break;
    case 4:
        viewEnergyUsage();
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

// Displays all historic temperature and humidity readings in a readable format.
// If no readings are available, informs the user.
void TempHumiditySensor::viewHistoricData() const {
    if (historicData->empty()) {
        cout << "No sensor readings recorded yet.\n";
        return;
    }

    cout << "\nHistoric Sensor Readings:\n";
    for (const auto& reading : *historicData) {
        cout << "Temperature: " << fixed << setprecision(1) << reading.temperature
            << "C, Humidity: " << reading.humidity
            << "%, Timestamp: " << reading.timestamp << "\n";
    }
}

// Displays total energy usage along with all historic energy readings.
// If no usage is recorded, informs the user.
void TempHumiditySensor::viewEnergyUsage() const {
    cout << "\nTotal Energy Usage: " << fixed << setprecision(2) << totalEnergy << " kWh\n";

    if (historicUsage->empty()) {
        cout << "No energy usage recorded yet.\n";
        return;
    }

    cout << "Historic Energy Usage:\n";
    for (const auto& reading : *historicUsage) {
        cout << "Energy Used: " << reading.energyUsed << " kWh, Timestamp: " << reading.timestamp << "\n";
    }
}

// Returns the type of the device as a string ("TempHumidity Sensor").
string TempHumiditySensor::getDeviceType() const {
    return "TempHumidity Sensor";
}

// Serializes the state of the sensor into a string for storage.
// Includes the name, ON/OFF status, and total energy usage.
string TempHumiditySensor::serialize() const {
    stringstream ss;
    ss << "TEMPHUMIDITY|" << name << "|" << isOn << "|" << totalEnergy;
    return ss.str();
}

// Deserializes the sensor's state from a string and restores its properties.
// Updates the name, ON/OFF status, and total energy usage.
void TempHumiditySensor::deserialize(const string& data) {
    stringstream ss(data);
    string tmp;

    getline(ss, tmp, '|'); // type
    getline(ss, name, '|');
    getline(ss, tmp, '|'); isOn = (tmp == "1");
    getline(ss, tmp, '|'); totalEnergy = stof(tmp);
}
