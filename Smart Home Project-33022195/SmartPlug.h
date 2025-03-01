#pragma once
#include "SmartDevice.h"
#include <vector>

using namespace std;

class SmartPlug : public SmartDevice {
private:
    int* sleepTimer;             // Pointer for sleep timer
    struct PowerReading {
        float energyUsed;         // Energy used in kWh
        time_t timestamp;         // Timestamp of the reading
    };
    vector<PowerReading>* historicUsage; // Pointer for historic data

    struct Schedule {
        int hour;
        int minute;
        string state; // "ON" or "OFF"
    };
    vector<Schedule> schedules;  // List of ON/OFF schedules

    float totalEnergy;            // Accumulated total energy in kWh
    time_t lastUpdateTime;        // Last update time for energy calculations

    void saveScheduleToFile() const;  // Save schedules to file
    void loadScheduleFromFile();      // Load schedules from file

public:
    SmartPlug(const string& name);
    ~SmartPlug();

    void updateHistoricData();
    string getQuickView() const override;
    void oneClickAction() override;
    void showMenu() const override;
    void handleMenuChoice(int choice) override;
    string getDeviceType() const override;
    string serialize() const override;
    void deserialize(const string& data) override;

    void manageSchedule();  // Schedule management
    void viewSchedule() const;
    void deleteSchedule();
};
