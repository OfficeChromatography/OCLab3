class ExtrudeEndstop{
  private:
    int pin;
    bool state;
  public:
    ExtrudeEndstop();
    void checkEndstop();
};