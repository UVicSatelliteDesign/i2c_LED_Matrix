// AS1115 example
// by omzn

#include <Wire.h>

byte i2c_addr = 0x00;
/*byte mat[8][8]={{1,0,0,0,0,0,0,0},
                {1,0,0,0,0,0,0,0},
                {1,0,0,0,0,0,0,0},
                {1,0,0,0,0,0,0,0},
                {1,0,0,0,0,0,0,0},
                {1,0,0,0,0,0,0,0},
                {1,0,0,0,0,0,0,0},
                {1,0,0,0,0,0,0,0}};*/

byte mat[8][8]={{0,0,1,1,1,1,0,0},
                {0,1,0,0,0,0,1,0},
                {1,0,1,0,0,1,0,1},
                {1,0,0,0,0,0,0,1},
                {1,0,1,0,0,1,0,1},
                {1,0,0,1,1,0,0,1},
                {0,1,0,0,0,0,1,0},
                {0,0,1,1,1,1,0,0}};
            
/*byte mat[8][8]={{1,1,1,1,1,1,1,1},
                {1,1,1,1,1,1,1,1},
                {1,1,1,1,1,1,1,1},
                {1,1,1,1,1,1,1,1},
                {1,1,1,1,1,1,1,1},
                {1,1,1,1,1,1,1,1},
                {1,1,1,1,1,1,1,1},
                {1,1,1,1,1,1,1,1}};*/


void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)

  // Wakeup from shutdown mode
  Wire.beginTransmission(i2c_addr);
  Wire.write(0x0c); 
  Wire.write(0x81); 
  Wire.endTransmission();
  delay(1);

  // This device can change slave address dynamically.
  // see p.15 more detail

  // Change device address (p.15)
  Wire.beginTransmission(i2c_addr);
  Wire.write(0x2d); // register
  Wire.write(0x01); // user-defined (0x03 if not connected any pins)
  Wire.endTransmission();  
  delay(1);

  i2c_addr = 0x03; // address changed from here

  // Intensity control (0x0A)
  Wire.beginTransmission(i2c_addr);
  Wire.write(0x0a); // register
  Wire.write(0x0f); // 15/16 (max)
  //Wire.write(0x07); // 8/16 (median)
  //Wire.write(0x00); // 1/16 (min)
  Wire.endTransmission();  
  delay(1);

  // Scan-Limit Register (0x0B)
  Wire.beginTransmission(i2c_addr);
  Wire.write(0x0b); 
  Wire.write(0x07); // display all digits 0:7
  Wire.endTransmission();  
  delay(1);

  // Non-decode mode (0x09) for 8x8 dot matrix
  Wire.beginTransmission(i2c_addr);
  Wire.write(0x09); 
  Wire.write(0x00); // non-decode mode
  Wire.endTransmission();  
  delay(1);
  flip();
}

void loop()
{
  
  for (int x=0;x<8;x++){
    byte tmp = mat[x][0];
    for(int y=0;y<7;y++){
      mat[x][y]=mat[x][y+1];
    }
    mat[x][7]=tmp;
  }

  show(mat);
  delay(100);
  //clear();
}

void flip(){
  byte row [] = {0,0,0,0,0,0,0,0};
  for(int x=0;x<4;x++){
    for(int y=0;y<8;y++){
      row[y]=mat[x][y];
    }
    for(int y=0;y<8;y++){
      mat[x][y]=mat[7-x][y];
    }
    for(int y=0;y<8;y++){
      mat[7-x][y]=row[y];
    }
  }
}

void clear() {
  byte data[][8] = {{0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0}};
  show(data);
}

void show(byte data[8][8]) {

  for (int x=0; x < 8; x++) {
    byte line = 0;
    for (int y=0;y < 8; y++) {
        line = line << 1;
        line = line | data[x][y];
    }
    Wire.beginTransmission(i2c_addr);
    Wire.write(x+1);  // select segment
    Wire.write(line); // select digit
    Wire.endTransmission();  
  }
}
