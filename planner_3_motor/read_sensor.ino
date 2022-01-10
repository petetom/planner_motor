


int read_5600() {

    // read date from the AS5600 sensor

     Wire.beginTransmission(address);
     Wire.write(0x0B);
     Wire.endTransmission();
     Wire.requestFrom(0x36,1);
     st = Wire.read();

     Wire.beginTransmission(address);
     Wire.write(0x1A);
     Wire.endTransmission();
     Wire.requestFrom(0x36,1);
     agc = Wire.read();

     Wire.beginTransmission(address); //read the raw angle and angle data drom the sensor
     Wire.write(0x0C);
     Wire.endTransmission();
     Wire.requestFrom(0x36,4);

     rhb = Wire.read();
     rlb = Wire.read();
     hb = Wire.read();
     lb = Wire.read();

     return(rhb*256+rlb);
}
