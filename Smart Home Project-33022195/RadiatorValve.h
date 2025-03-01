#pragma once
#include "SmartDevice.h"
#include <vector>

using namespace std;

class RadiatorValve : public SmartDevice {
private:
    struct Schedule {
        int hour;           // Hour in 24-hour format
        int minute;         // Minute
        string state;       // "ON" or "OFF"
    };

    vector<Schedule> schedules;  // List of schedules

    void saveScheduleToFile() const;  // Save to file
    void loadScheduleFromFile();      // Load from file

public:
    RadiatorValve(const string& name);
    ~RadiatorValve();

    void showMenu() const override;
    void handleMenuChoice(int choice) override;
    void manageSchedule();  // Schedule management menu
    void viewSchedule() const;
    void deleteSchedule();
    string getQuickView() const override;
    void oneClickAction() override;
    string getDeviceType() const override;
    string serialize() const override;
    void deserialize(const string& data) override;
};
