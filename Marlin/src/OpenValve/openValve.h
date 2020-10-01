#include "../core/millis_t.h"
class ValveOpen{
  private:
    int pin;
    bool open;
    millis_t closingTime;
    int MaxOpenPeriod;
    void setClosingTime(millis_t ms);
    void delayFromFrequency(int frequency);
  public:
    ValveOpen();
    void toggleValve();
    void frequencyToggleValve(int frequency);
    void openValve();
    void closeValve();
    bool isOpen();
    void securityCheck();
    millis_t getClosingTime();
};