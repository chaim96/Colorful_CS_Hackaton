#include "filterApply.h"

using namespace std;

void filterApply(Request_Change_Specific* arrayChange, int sizeOfArrayChange, Request_Darken* arrayShade, int sizeOfArrayShade)
{
    ifstream originalImage;
    ofstream filteredImage;
    originalImage.open(NAME_OF_ORIGINAL_IMAGE);
    filteredImage.open(NAME_OF_IMAGE_OUTPUT);

    // copying ppm header info to the filtered image
    string type, width, height, RGBmax;
    originalImage >> type;
    originalImage >> width;
    originalImage >> height;
    originalImage >> RGBmax;

    filteredImage << type << endl;
    filteredImage << width << " " << height << endl;
    filteredImage << RGBmax << endl;


    // filtering RGB to filtered image
    string red_str, green_str, blue_str;
    while (!originalImage.eof())
    {
        // reading RGB from image and converting to int
        originalImage >> red_str;
        originalImage >> green_str;
        originalImage >> blue_str;
        Color2 currentColor;
        currentColor.putRGB(red_str, green_str, blue_str);

        // converting RGB to HSV
        currentColor.RGBtoHSV();

        //color changing
        colorChange(currentColor, arrayChange, sizeOfArrayChange);

        //color darkening or lightening
        colorShade(currentColor, arrayShade, sizeOfArrayShade);

        // converting back to RGB and printing to output
        currentColor.HSVtoRGB();
        filteredImage << currentColor.getRed() << " " << currentColor.getGreen() << " " << currentColor.getBlue() << " " << endl;
    }

    originalImage.close();
    filteredImage.close();
}


void Color2::putRGB(string red_str, string green_str, string blue_str)
{
    red = stringToInt(red_str);
    green = stringToInt(green_str);
    blue = stringToInt(blue_str);
}


int stringToInt(string color_str)
{
    int color_int;
    stringstream colorStream(color_str);
    colorStream >> color_int;
    return color_int;
}


void Color2::RGBtoHSV()
	{
		double r = red;
        double g = green;
        double b = blue;

        // R, G, B values are divided by 255
		// to change the range from 0..255 to 0..1
		r = r / 255.0;
		g = g / 255.0;
		b = b / 255.0;

		// h, s, v = hue, saturation, value
		double cmax = max(r, max(g, b)); // maximum of r, g, b
		double cmin = min(r, min(g, b)); // minimum of r, g, b
		double diff = cmax - cmin; // diff of cmax and cmin.
		double h = -1, s = -1;

		// if cmax and cmax are equal then h = 0
		if (cmax == cmin)
			h = 0;

		// if cmax equal r then compute h
		else if (cmax == r)
			h = (360 + 60 * ((g - b) / diff));

		// if cmax equal g then compute h
		else if (cmax == g)
			h = (60 * ((b - r) / diff) + 120);

		// if cmax equal b then compute h
		else if (cmax == b)
			h = (60 * ((r - g) / diff) + 240);

        while(h - 360 > 0)
        {
            h = h - 360;
        }

		// if cmax equal zero
		if (cmax == 0)
			s = 0;
		else
			s = (diff / cmax) * 100;

		// compute v
		double v = cmax * 100;

        hue = h;
        saturation = s;
        value = v;
	}



void colorChange(Color2& color2, Request_Change_Specific* arrayChange, int sizeOfArrayChange)
{
    int  flag = 0;
    double hue, saturation, value, newHue, newSaturation, newValue;
    Filter origin;
    Filter target;
    for (int i = 0; i < sizeOfArrayChange; i++)
    {
        origin.hueMin = arrayChange[i].gets_source_hue_low();
        origin.hueMax = arrayChange[i].get_source_hue_high();
        origin.valueMin = arrayChange[i].get_source_value_low();
        origin.valueMax = arrayChange[i].get_source_value_high();
        origin.saturationMin = arrayChange[i].get_source_saturation_low();
        origin.saturationMax = arrayChange[i].get_source_saturation_high();
        target.hueMin = arrayChange[i].get_target_hue_low();
        target.hueMax = arrayChange[i].get_target_hue_high();
        target.valueMin = arrayChange[i].get_target_value_low();
        target.valueMax = arrayChange[i].get_target_value_high();
        target.saturationMin = arrayChange[i].get_target_saturation_low();
        target.saturationMax = arrayChange[i].get_target_saturation_high();
        hue = color2.getHue();
        saturation = color2.getSaturation();
        value = color2.getValue();
        if( (hue < origin.hueMax && hue > origin.hueMin)  &&  (saturation < origin.saturationMax && saturation > origin.saturationMin)
            &&  (value < origin.valueMax && value > origin.valueMin) && (flag == 0) )
        {
            newHue =  target.hueMin + ((target.hueMax - target.hueMin) / (origin.hueMax - origin.hueMin)) * (hue - origin.hueMin);
            newSaturation =  target.saturationMin + ((target.saturationMax - target.saturationMin) / (origin.saturationMax - origin.saturationMin)) * (saturation - origin.saturationMin);
            newValue =  target.valueMin + ((target.valueMax - target.valueMin) / (origin.valueMax - origin.valueMin)) * (value - origin.valueMin);
            flag = 1;
            if (newHue > target.hueMax)
            {
                newHue = target.hueMax;
            }
            if (newHue < target.hueMin)
            {
                newHue = target.hueMin;
            }
            if (newValue > target.valueMax)
            {
                newValue = target.valueMax;
            }
            if (newValue < target.valueMin)
            {
                newValue = target.valueMin;
            }
            if (newSaturation > target.saturationMax)
            {
                newSaturation = target.saturationMax;
            }
            if (newSaturation < target.saturationMin)
            {
                newSaturation = target.saturationMin;
            }
            color2.setHue(newHue);
            color2.setSaturation(newSaturation);
            color2.setValue(newValue);
        }
    }
}

void colorShade(Color2& color2, Request_Darken* arrayShade, int sizeOfArrayShade)
{
    double hue, saturation, value, newValue;
    int flag = 0;
    Filter origin;
    for (int i = 0; i < sizeOfArrayShade; i++)
    {
        hue = color2.getHue();
        saturation = color2.getSaturation();
        value = color2.getValue();
        origin.hueMin = arrayShade[i].get_hue_low();
        origin.hueMax = arrayShade[i].get_hue_high();
        origin.valueMin = arrayShade[i].get_value_low();
        origin.valueMax = arrayShade[i].get_value_high();
        origin.saturationMin = arrayShade[i].get_saturation_low();
        origin.saturationMax = arrayShade[i].get_saturation_high();
        if( (hue < origin.hueMax && hue > origin.hueMin)  &&  (saturation < origin.saturationMax && saturation > origin.saturationMin)
               &&  (value < origin.valueMax && value > origin.valueMin) && (flag == 0) )
        {
            newValue = value + arrayShade[i].get_color_percentage();
            if (newValue > 100)
            {
                newValue = 100;
            }
            if (newValue < 0)
            {
                newValue = 0;
            }
            color2.setValue(newValue);
            flag = 1;
        }
    }

}







void Color2::HSVtoRGB()
{
    double H = hue;
    double S = saturation;
    double V = value;

    if(H>360 || H<0 || S>100 || S<0 || V>100 || V<0)
    {
        cout<<"The given HSV values are not in valid range"<<endl;
        return;
    }
    double s = S/100;
    double v = V/100;
    double C = s*v;
    double X = C*(1-fabs(fmod(H/60.0, 2)-1));
    double m = v-C;
    double r,g,b;
    if(H >= 0 && H < 60){
        r = C,g = X,b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X,g = C,b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0,g = C,b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0,g = X,b = C;
    }
    else if(H >= 240 && H < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }

    red = (r+m)*255;
    green = (g+m)*255;
    blue = (b+m)*255;
}