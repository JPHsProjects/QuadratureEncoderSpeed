#ifndef Encoder_h
#define Encoder_h

#include "Arduino.h"

#define MAX_NUM_ENCODERS 4

class Encoders{
  public:  
    Encoders(byte pinA, byte pinB);
    
    void encoderCount();    // To be called for memory allocation
    void encoderInit();     // To be called in setup function
    long getEncoderCount();
    void setEncoderCount(long);
    long getEncoderErrorCount();
    static Encoders *_instances[MAX_NUM_ENCODERS];
    int32_t getSpeed();
    int32_t getTestVal();
    
  private:
    static uint8_t _whichEncoder;
    uint8_t _currentEncoder;
    uint8_t _encoderPINA;
    uint8_t _encoderPINB;
    volatile long _encoderCount = 0;
    volatile int _lastEncoded = 0;
    volatile long _encoderErrors = 0;
    int64_t _last_isr_time = 0;
    int32_t _us_between_encodercounts = 0;
    int64_t _last_count_time = 0;
    int64_t _testval = 0;
};

#endif
