#pragma once
#include "SmartDevice.h"

class SmartLight : public SmartDevice {
private:
    int* brightness;

public:
    SmartLight(const string& name);
    ~SmartLight();

    string getQuickView() const override;
    void oneClickAction() override;
    void showMenu() const override;
    void handleMenuChoice(int choice) override;
    string getDeviceType() const override;
    string serialize() const override;
    void deserialize(const string& data) override;
};
