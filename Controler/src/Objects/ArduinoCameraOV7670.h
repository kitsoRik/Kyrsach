#ifndef ARDUINOCAMERAOV7670OBJECT_H
#define ARDUINOCAMERAOV7670OBJECT_H

#include "ArduinoOutputObject.h"
#include "libs/BMP.h"
#include "libs/OV7670.h"

class ArduinoCameraOV7670Object : public ArduinoObject
{
public:
    enum Mode
    {
        QQQVGA_RGB565,
        QQVGA_RGB565
    };

    ArduinoCameraOV7670Object(const int SIOD,
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
     const int D7);

    int width() const
    {
        return camera->xres;
    }

    int height() const
    {
        return camera->yres;
    }

    unsigned char* frame()
    {
        return camera->frame;
    }

    void oneFrame()
    {
        camera->oneFrame();
    }

    static unsigned char headerSize;
    static unsigned char bmpHeader[BMP::headerSize];

private:
    OV7670 *camera;
};

#endif