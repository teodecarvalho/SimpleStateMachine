#ifndef DEVICE_H
#define DEVICE_H
#include "classes.h"

class State;
class Device;
class StateLedOn;
class StateLedoff;

class Device
{
public:
    State *state_;
    Device(State *state);
    ~Device();
    void TransitionTo(State *state);
    void update();
};

class State
{
public:
    State();
    Listener listener;
    Device *device_;
    virtual ~State();
    void set_device(Device *device);
    virtual void update();
    void makeTransition(State *nextState);
    Timer _timer;
};

class StateLed: public State
{
    public:
        StateLed();
        void update();
        virtual void makeTransitionLed();
        void handleMsg();
};

class StateLedOff : public StateLed
{
public:
    StateLedOff();
    void turnLedOff();
    void makeTransitionLed();
};

class StateLedOn : public StateLed
{
public:
    StateLedOn();
    void turnLedOn();
    void makeTransitionLed();
};

class StateLedBlink : public StateLed
{
public:
    StateLedBlink();
    void turnLedOn();
    void turnLedOff();
    void update();
    void makeTransitionLed();
};

#endif // DEVICE_H
