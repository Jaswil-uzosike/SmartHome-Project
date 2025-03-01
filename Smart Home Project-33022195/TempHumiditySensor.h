#pragma once
#include "SmartDevice.h"
#include <vector>

using namespace std;

class TempHumiditySensor : public SmartDevice {
private:
    struct Reading {
        float temperature;   // Temperature value
        float humidity;      // Humidity value
        time_t timestamp;    // Timestamp of the reading
    };

    struct EnergyReading {
        float energyUsed;    // Energy used in kWh
        time_t timestamp;    // Timestamp of the reading
    };

    vector<Reading>* historicData;        // Pointer to historic temperature/humidity data
    vector<EnergyReading>* historicUsage; // Pointer to historic energy usage
    float totalEnergy;                    // Total energy used in kWh
    time_t lastUpdateTime;                // Last update time for energy calculations

    void updateEnergyUsage();             // Updates total energy usage
    void addEnergyReading();              // Adds energy reading to history

public:
    TempHumiditySensor(const string& name);
    ~TempHumiditySensor();

    void updateSensorReadings();          // Simulates sensor data
    string getQuickView() const override;
    void oneClickAction() override;
    void showMenu() const override;
    void handleMenuChoice(int choice) override;
    string getDeviceType() const override;
    string serialize() const override;
    void deserialize(const string& data) override;

    void viewHistoricData() const;        // View temperature/humidity readings
    void viewEnergyUsage() const;         // View energy usage
};
