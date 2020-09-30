class ValveOpen{
  private:
    int pin;
    bool open;
  public:
    ValveOpen();
    void toggleValve();
    void openValve();
    void closeValve();
    bool getOpen();
};