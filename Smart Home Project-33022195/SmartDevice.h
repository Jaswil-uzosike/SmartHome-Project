#pragma once
#include <string>
#include <thread>
#include <atomic>
#include <chrono>

using namespace std;

class SmartDevice {
protected:
    string name;
    bool isOn;

    // Timer members
    atomic<int> timer;         // Timer countdown in seconds
    atomic<bool> timerRunning; // Timer running flag
    thread timerThread;        // Background timer thread

public:
    SmartDevice(const string& name);
    virtual ~SmartDevice();

    virtual string getQuickView() const = 0;
    virtual void oneClickAction() = 0;
    virtual void showMenu() const = 0;
    virtual void handleMenuChoice(int choice) = 0;
    virtual string getDeviceType() const = 0;
    virtual string serialize() const = 0;
    virtual void deserialize(const string& data) = 0;

    // Timer control
    virtual void startTimer(int seconds);
    virtual void stopTimer();
    virtual bool isTimerRunning() const;

    string getName() const;
    void setName(const string& newName);
    void editName();
    bool getIsOn() const;
};
