
#ifndef FILTERAPPLY_FILTERAPPLY_H
#define FILTERAPPLY_FILTERAPPLY_H

#endif //FILTERAPPLY_FILTERAPPLY_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include "Color_Parser.h"

using namespace std;


// class to store image's current color coordinates by RGB & HSV
// class also holds relevant methods for filtering
class Color2{
private:
    int red;
    int green;
    int blue;
    double hue;
    double saturation;
    double value;

public:
    // gets RGB from image
    void putRGB(string red_str, string green_str, string blue_str);

    // converts RGB to HSV
    void RGBtoHSV();

    // converts HSV back to RGB after applying filter changes
    void HSVtoRGB();

    int getRed() { return red; };
    int getGreen() { return green; };
    int getBlue() { return blue; };
    double getHue() { return hue; };
    double getSaturation() { return saturation; };
    double getValue() { return value; };
    void setHue(double newHue) { hue = newHue; };
    void setSaturation(double newSaturation) { saturation = newSaturation; };
    void setValue(double newValue) { value = newValue; };
};



// darkens or lightens a specific color
void colorShade(Color2& color2, Request_Darken* arrayShade, int sizeOfArrayShade);


// changes a color to another
void colorChange(Color2& color2, Request_Change_Specific* arrayChange, int sizeOfArrayChange);


// main function for applying the filter chosen
void filterApply(Request_Change_Specific* arrayChange, int sizeOfArrayChange, Request_Darken* arrayShade, int sizeOfArrayShade);


const char* const NAME_OF_ORIGINAL_IMAGE = "../ppms/TP22_Amazon_Original.ppm";
const char* const NAME_OF_IMAGE_OUTPUT = "../outs/output.ppm";

// aid to getRGB - converts the RGB text read from the image to int for calculations
int stringToInt(string color_str);

class Filter
{
    double hueMin;
    double hueMax;
    double valueMin;
    double valueMax;
    double saturationMin;
    double saturationMax;
public:
    friend void colorShade(Color2& color2, Request_Darken* arrayShade, int sizeOfArrayShade);
    friend void colorChange(Color2& color2, Request_Change_Specific* arrayChange, int sizeOfArrayChange);
};