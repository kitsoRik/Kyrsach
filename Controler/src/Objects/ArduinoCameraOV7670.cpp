#include "ArduinoCameraOV7670.h"

unsigned char ArduinoCameraOV7670Object::headerSize = BMP::headerSize;
unsigned char ArduinoCameraOV7670Object::bmpHeader[BMP::headerSize];

ArduinoCameraOV7670Object::ArduinoCameraOV7670Object(const int SIOD,
     const int SIOC, 
     const int VSYNC, 
     const int HREF, 
     const int XCLK, 
     const int PCLK, 
     const int D0, 
     const int D1, 
     const int D2, 
     const int D3, 
     const int D4, 
     const int D5, 
     const int D6, 
     const int D7) 
     : ArduinoObject(2)
{
    OV7670::Mode mode = OV7670::Mode::QQQVGA_RGB565;
  camera = new OV7670(mode, SIOD, SIOC, VSYNC, HREF, XCLK, PCLK, D0, D1, D2, D3, D4, D5, D6, D7);


  BMP::construct16BitHeader(bmpHeader, width(), height());
  setType(ArduinoObject::Camera);
}