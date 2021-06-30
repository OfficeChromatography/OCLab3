extern ForceSensor force ;
class PumpControl{
  private:
    int ABSOLUTE_MAX_PRESSURE = 80;
    int ABSOLUTE_MIN_PRESSURE = 2;
    float pressure_read = force.readPressure();
    float pos=current_position.z;
    float pressure_set;
    float min_pressure;
    float max_pressure;

    int errorFunction(float set, float real);
    void calculateNewPosition();
    void move();

  public:
    PumpControl(float pressure_set);
    void compute();
};