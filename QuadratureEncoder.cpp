#include "QuadratureEncoder.h"


static const char *TAG = "QuadEncoder";



void IRAM_ATTR interruptEncoder1(){
  if(Encoders::_instances[0] != NULL)
  Encoders::_instances[0]->encoderCount();
}
void IRAM_ATTR interruptEncoder2(){
  if(Encoders::_instances[1] != NULL)
  Encoders::_instances[1]->encoderCount();
}
void IRAM_ATTR interruptEncoder3(){
  if(Encoders::_instances[2] != NULL)
  Encoders::_instances[2]->encoderCount();
}
void IRAM_ATTR interruptEncoder4(){
  if(Encoders::_instances[3] != NULL)
  Encoders::_instances[3]->encoderCount();
}



// initialize all instance of encoder to null.

Encoders *Encoders::_instances[MAX_NUM_ENCODERS] = {NULL, NULL,NULL, NULL};

uint8_t Encoders::_whichEncoder = 0;



Encoders::Encoders(byte pinA, byte pinB){
   _encoderPINA = pinA;
   _encoderPINB = pinB;
   _whichEncoder++;
   _currentEncoder = _whichEncoder;
   switch(_whichEncoder){
    case 1:
        _instances[0] = this;
        break;
     case 2:
        _instances[1] = this;
        break;
     case 3:
        _instances[2] = this; 
        break;
     case 4:
        _instances[3] = this;
        break;
   }
ESP_LOGI(TAG,"Initialized Encoder %d on PinA %d and PinB %d",_whichEncoder,pinA,pinB);
}


void Encoders::encoderInit(){
   pinMode(this->_encoderPINA, INPUT);  
   pinMode(this->_encoderPINB, INPUT);
   switch(this->_currentEncoder){
    case 1:
        attachInterrupt(this->_encoderPINB, interruptEncoder1, CHANGE);
        attachInterrupt(this->_encoderPINA,  interruptEncoder1, CHANGE);  
        break;
     case 2:
        attachInterrupt(this->_encoderPINB, interruptEncoder2, CHANGE);
        attachInterrupt(this->_encoderPINA,  interruptEncoder2, CHANGE);  
        break;
     case 3:
        attachInterrupt(this->_encoderPINB, interruptEncoder3, CHANGE);
        attachInterrupt(this->_encoderPINA,  interruptEncoder3, CHANGE); 
        break;
     case 4:
        attachInterrupt(this->_encoderPINB, interruptEncoder4, CHANGE);
        attachInterrupt(this->_encoderPINA,  interruptEncoder4, CHANGE);  
        break;
   }
   ESP_LOGI(TAG,"Enabled Encoder %d on PinA %d and PinB %d",this->_currentEncoder,this->_encoderPINA,this->_encoderPINB);
}


void IRAM_ATTR Encoders::encoderCount(){
  
  int EncoderPhaseA = digitalRead(this->_encoderPINA);  // MSB
  int EncoderPhaseB = digitalRead(this->_encoderPINB);  // LSB

  int currentEncoded = (EncoderPhaseA << 1) | EncoderPhaseB;
  int sum = (this->_lastEncoded << 2) | currentEncoded;
  switch(sum){
    case 0b0001:
    case 0b0111:
    case 0b1110:
    case 0b1000:
      this->_encoderCount--;
      break;
    case 0b0010:
    case 0b1011:
    case 0b1101:
    case 0b0100:
      this->_encoderCount++;
      break;
    default:
      this->_encoderErrors++;
      break;
  }
  
  this->_lastEncoded = currentEncoded;
}

long Encoders::getEncoderCount(){
  return _encoderCount;
}
void Encoders::setEncoderCount(long setEncoderVal){
  this->_encoderCount = setEncoderVal;
}

long Encoders::getEncoderErrorCount(){
  return _encoderErrors;
}
