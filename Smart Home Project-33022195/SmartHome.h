#pragma once
#include "SmartDevice.h"
#include <vector>
#include <memory>

using namespace std;

class SmartHome {
private:
    vector<unique_ptr<SmartDevice>> devices;

public:
    SmartHome();
    ~SmartHome();

    static SmartHome& getInstance();
    void loadDevices();
    void saveDevices();
    void listDevices() const;
    void sortByName();
    void sortByType();
    void removeDevice(const string& deviceName);
    void addDevice();
    void handleOneClickAction(const string& name);
    void interactWithDevice(const string& name);
    void run();
};

