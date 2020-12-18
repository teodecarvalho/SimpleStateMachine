#include <Arduino.h>
#include "classes.h"
#include "device.h"

State::State()
{
}

void State::set_device(Device *device)
{
    this->device_ = device;
}

State::~State()
{
}

void State::update()
{
}

void State::makeTransition(State *nextState)
{
    this->device_->TransitionTo(nextState);
}

Device::Device(State *state) : state_(nullptr)
{
    this->TransitionTo(state);
}

Device::~Device()
{
    delete state_;
}

void Device::TransitionTo(State *state)
{
    if (this->state_ != nullptr)
        delete this->state_;
    this->state_ = state;
    this->state_->set_device(this);
}

void Device::update()
{
    this->state_->update();
}

StateLed::StateLed()
{
    Timer _timer = Timer();
}

void StateLed::handleMsg()
{
    if(listener.getMsg()[0] == 'o' && listener.getMsg()[1] == 'n'){ 
        //Serial.println("Transition to on");
        listener.clearMsg();
        makeTransition(new StateLedOn);
    }
    if(listener.getMsg()[0] == 'o' && listener.getMsg()[1] == 'f') {
        //Serial.println("Transition to off");
        listener.clearMsg();
        makeTransition(new StateLedOff);
    }
    if(listener.getMsg()[0] == 'b' && listener.getMsg()[1] == 'l') {
        //Serial.println("Transition to blink");
        listener.clearMsg();
        makeTransition(new StateLedBlink);
    }
}

void StateLed::update(){
    Timer listenerTimer;
    #define LISTEN_INTERVAL 100
    listenerTimer.start(LISTEN_INTERVAL);
    while (!_timer.isTimerDone())
    {
        _timer.update();
        listenerTimer.update();
        if(listenerTimer.isTimerDone()){
            listener.receiveMsg();
            if(listener.hasNewMsg()){
                break;
            }
            listenerTimer.start(LISTEN_INTERVAL);
        }
    }
    if(listener.hasNewMsg()){
        handleMsg();
    } else {
        makeTransitionLed();
    }
}

void StateLed::makeTransitionLed()
{
}
#define STATE_DURATION 5000

StateLedOff::StateLedOff()
{
    _timer.start(STATE_DURATION);
    turnLedOff();
    //update();
}

void StateLedOff::turnLedOff()
{
    digitalWrite(13, LOW);
}

void StateLedOff::makeTransitionLed(){
   makeTransition(new StateLedBlink);
}

StateLedOn::StateLedOn()
{
    _timer.start(STATE_DURATION);
    turnLedOn();
    //update();
}

void StateLedOn::turnLedOn()
{
    digitalWrite(13, HIGH);
}

void StateLedOn::makeTransitionLed(){
    makeTransition(new StateLedOff);
}

StateLedBlink::StateLedBlink()
{
    _timer.start(STATE_DURATION);
    //update();
}

void StateLedBlink::turnLedOn()
{
    digitalWrite(13, HIGH);
}

void StateLedBlink::turnLedOff()
{
    digitalWrite(13, LOW);
}

void StateLedBlink::update()
{
    Timer listenerTimer;
    Timer blinkerTimer;
    #define BLINK_INTERVAL 100
    listenerTimer.start(LISTEN_INTERVAL);
    blinkerTimer.start(BLINK_INTERVAL);
    while (!_timer.isTimerDone())
    {
        _timer.update();
        listenerTimer.update();
        blinkerTimer.update();
        if(listenerTimer.isTimerDone()){
            listener.receiveMsg();
            if(listener.hasNewMsg()){
                break;
            }
            listenerTimer.start(LISTEN_INTERVAL);
        }
        if(blinkerTimer.isTimerDone()){
            turnLedOn();
            delay(BLINK_INTERVAL);
            turnLedOff();
            blinkerTimer.start(BLINK_INTERVAL);
        }
    }
    if(listener.hasNewMsg()){
        handleMsg();
    } else {
        makeTransitionLed();
    }
}

void StateLedBlink::makeTransitionLed(){
    makeTransition(new StateLedOn);
}