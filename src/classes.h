#ifndef CLASSES_H
#define CLASSES_H

#include <Arduino.h>

class Listener
{
#define MSGSIZE 20
public:
    Listener()
    {
        _hasNewMsg = LOW;
    }
    void receiveMsg()
    {
        int index = 0;
        // Clear msg variable
        for (int i = 0; i < (MSGSIZE - 1); i++)
            _msg[i] = '\0';
        // Check if first character is '<'
        if (Serial.read() != '<')
            return;
        delay(2);
        _hasNewMsg = HIGH;
        while (Serial.available())
        {
            _msg[index] = Serial.read();
            // Check if message is finished, if so replace '>' by '\0'
            if (_msg[index] == '>')
                _msg[index] = '\0';
            index++;
            delay(2);
        }
        // Clear message if longer than msgSize
        if (index >= MSGSIZE)
            for (int i = 0; i < (MSGSIZE - 1); i++)
                _msg[i] = '\0';
    }

    bool isMsgValid(){
        bool isValid = LOW;
        if(_msg[0] == 'o' && _msg[1] == 'n'){ 
            isValid = HIGH;
        }
        if(_msg[0] == 'o' && _msg[1] == 'f'){ 
            isValid = HIGH;
        }
        if(_msg[0] == 'b' && _msg[1] == 'l'){ 
            isValid = HIGH;
        }
        return isValid;
    }

    char *getMsg()
    {
        return _msg;
    }

    bool hasNewMsg()
    {
        if(_hasNewMsg){
            if(isMsgValid()){
                return HIGH;
            } else {
                clearMsg();
                return LOW;
            }
        }
        return _hasNewMsg;
    }

    void clearMsg()
    {
        for (int i = 0; i < (MSGSIZE - 1); i++)
            _msg[i] = '\0';
        _hasNewMsg = LOW;
    }

private:
    char _msg[MSGSIZE];
    bool _hasNewMsg;
};

class Timer
{
public:
    Timer()
    {
        _done = LOW;
        _running = LOW;
    }

    void start(unsigned long delayTime)
    {
        _delay = delayTime;
        _start = millis();
        _end = _start + _delay;
        _done = LOW;
        _running = HIGH;
    }

    bool isTimerDone()
    {
        return _done;
    }

    void update()
    {
        if (_running)
        {
            if (millis() > _end)
            {
                _done = HIGH;
            }
        }
    }

    unsigned long getElapsed(){
        return (millis() - _start);
    }

private:
    unsigned long _delay;
    unsigned long _start;
    unsigned long _end;
    bool _done;
    bool _running;
};

#endif //CLASSES_H