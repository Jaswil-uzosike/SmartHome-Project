#pragma once
#include "SmartDevice.h"
#include <vector>

using namespace std;

class Thermostat : public SmartDevice {
private:
    struct Schedule {
        int hour;           // Hour in 24-hour format
        int minute;         // Minute
        string state;       // "ON" or "OFF"
    };

    vector<Schedule> schedules; // List of schedules

    void saveScheduleToFile() const;  // Save schedules to file
    void loadScheduleFromFile();      // Load schedules from file

public:
    Thermostat(const string& name);
    ~Thermostat();

    void showMenu() const override;
    void handleMenuChoice(int choice) override;
    void manageSchedule();  // Manage schedule menu
    void viewSchedule() const;
    void deleteSchedule();
    string getQuickView() const override;
    void oneClickAction() override;
    string getDeviceType() const override;
    string serialize() const override;
    void deserialize(const string& data) override;
};
