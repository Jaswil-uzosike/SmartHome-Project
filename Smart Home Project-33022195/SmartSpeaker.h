#pragma once
#include "SmartDevice.h"

using namespace std;

class SmartSpeaker : public SmartDevice {
private:
    int* volume;       // Pointer for volume
    bool* isPlaying;   // Pointer for isPlaying

public:
    SmartSpeaker(const string& name);
    ~SmartSpeaker();  // Destructor to clean up allocated memory

    string getQuickView() const override;
    void oneClickAction() override;
    void showMenu() const override;
    void handleMenuChoice(int choice) override;
    string getDeviceType() const override;
    string serialize() const override;
    void deserialize(const string& data) override;
};
