
class PumpControl{
  private:
    int ABSOLUTE_MAX_PRESSURE = 80;
    int ABSOLUTE_MIN_PRESSURE = 2;
    float pressure_read = force.readPressure();
    float pos=current_position.z;
    float pressure_set;
    float min_pressure;
    float max_pressure;


    int errorFunction(float set, float real){
      int error = abs(set - real);
      return (error)<1?(1):(error);
    }
    
    void calculateNewPosition(){
      if(this->pressure_read< this->min_pressure){
        int error = this->errorFunction(this->min_pressure, this->pressure_read);
        this->pos+=(0.03*error);
      }
      if(this->pressure_read> this->max_pressure){
        int error = this->errorFunction(this->max_pressure, this->pressure_read);
        this->pos-=(0.03*error);
      }
    }

    void move(){
      endstops.enable(true);
      do_blocking_move_to_z(this->pos, 10);
      this->pressure_read = force.readPressure();
      SERIAL_ECHOPAIR("Ps: ", this->pressure_set);
      SERIAL_ECHOLNPAIR("\tPr: ", this->pressure_read);
    }

  public:
    PumpControl(float pressure_set){
      boolean toHigh = pressure_set > this->ABSOLUTE_MAX_PRESSURE;
      boolean toLow = pressure_set < this->ABSOLUTE_MIN_PRESSURE;

      if(toHigh || toLow){
        (toHigh)?(SERIAL_ECHOLN("Pressure settled too high!!")):
        (SERIAL_ECHOLN("Pressure settled too low, must be greater than 2!!"));
      }
      else{
        this->pressure_set = pressure_set;
        this->min_pressure = pressure_set-1;
        this->max_pressure = pressure_set-1;
      }
    }

    void compute(){
      while(pressure_read<min_pressure || pressure_read>=max_pressure){
        this->calculateNewPosition();
        if(this->pressure_read < this->ABSOLUTE_MAX_PRESSURE){
          this->move();
        }else{
          SERIAL_ECHOLN("Pressure is too high!!");
          break;
        }
      }
    }
};
