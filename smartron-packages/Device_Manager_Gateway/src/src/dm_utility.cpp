#include "dm_utility.hpp"


int ConvertStringToInt(std::string letter) {
	int AsciitoInt = 0;
	for (size_t iplace = 0; iplace < letter.length(); iplace++) {
		char charascii = letter.at(iplace);
		AsciitoInt += int(charascii);
	}
	return AsciitoInt;
}

static int HexadecimalToDecimal(std::string hex) {
	int hexLength = hex.length();
	double dec = 0;

	for (int i = 0; i < hexLength; ++i) {
		char b = hex[i];

		if (b >= 48 && b <= 57)
			b -= 48;
		else if (b >= 65 && b <= 70)
			b -= 55;
		else if (b >= 97 && b <= 102)
			b -= 87;  //For lower case hex values

		dec += b * pow(16, ((hexLength - i) - 1));
	}
	return (int) dec;
}

RGBVALUE convertHEXAtoRGB(std::string hex) {
	RGBVALUE RGBValueObj;
	if (hex[0] == '#')
		hex = hex.erase(0, 1);

	RGBValueObj.Red = HexadecimalToDecimal(hex.substr(0, 2));
	RGBValueObj.Green = HexadecimalToDecimal(hex.substr(2, 2));
	RGBValueObj.Blue = HexadecimalToDecimal(hex.substr(4, 2));

	return RGBValueObj;
}

std::string DecimalToHexadecimal(int dec) {
	if (dec < 1)
		return "00";
	int hex = dec;
	std::string hexStr = "";
	while (dec > 0) {
		hex = dec % 16;
		if (hex < 10)
			hexStr = hexStr.insert(0, std::string(1, (hex + 48)));
		else
			hexStr = hexStr.insert(0, std::string(1, (hex + 55)));
		dec /= 16;
	}
	if (hexStr.size() < 2)
		return std::string("0") + hexStr;
	else
		return hexStr;
}

long long CurrentZWReqTime() {
	struct timeval te;
	gettimeofday(&te, NULL);
	long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000; // calculate milliseconds
	return milliseconds;
}

std::string convertRGBtoHEXA(int Red_value, int Green_Value, int Blue_Value) {

	syslog(LOG_INFO,
			"[DM]: CONVERTER CONVERT RGB_VALUE TO HEXA_VALUE START");

	syslog(LOG_INFO,
			"[DM]: CONVERTER CONVERT RGB_VALUE TO HEXA_VALUE RED: [%d] GREEN: [%d] BLUE: [%d]",
			Red_value, Green_Value, Blue_Value);

	std::string RED = DecimalToHexadecimal(Red_value);
	std::string GREEN = DecimalToHexadecimal(Green_Value);
	std::string BLUE = DecimalToHexadecimal(Blue_Value);

	syslog(LOG_INFO,
			"[DM]: CONVERTER CONVERT RGB_VALUE TO HEXA_VALUE: [%s]",
			(std::string("#") + RED + GREEN + BLUE).c_str());

	return std::string("#") + RED + GREEN + BLUE;
}

XYVALUE convertRGBtoXY(uint16_t Red_Value, uint16_t Green_Value,
		uint16_t Blue_Value) {

	syslog(LOG_INFO,
			"[DM]: CONVERTER CONVERT RGB_VALUE TO XY_VALUE START");

	syslog(LOG_INFO,
			"[DM]: CONVERTER CONVERT RGB_VALUE TO XY_VALUE RED: [%d] GREEN: [%d] BLUE: [%d]",
			Red_Value, Green_Value, Blue_Value);

	float Z = 0.0, X = 0.0, Y = 0.0;
	float x = 0.0, y = 0.0;
	float R = 0, G = 0, B = 0;

	R = (Red_Value / 255.0f); //R from 0 to 255
	G = (Green_Value / 255.0f); //G from 0 to 255
	B = (Blue_Value / 255.0f); //B from 0 to 255

	if (R > 0.04045f)
		R = powf(((R + 0.055f) / 1.055f), 2.4f);
	else
		R = R / 12.92f;

	if (G > 0.04045)
		G = powf(((G + 0.055f) / 1.055f), 2.4f);
	else
		G = G / 12.92f;

	if (B > 0.04045f)
		B = powf(((B + 0.055f) / 1.055f), 2.4f);
	else
		B = B / 12.92f;

	X = R * 0.4124f + G * 0.3576f + B * 0.1805f;
	Y = R * 0.2126f + G * 0.7152f + B * 0.0722f;
	Z = R * 0.0193f + G * 0.1192f + B * 0.9505f;

	x = X / (X + Y + Z);
	y = Y / (X + Y + Z);

	XYVALUE XYValueObj;
	XYValueObj.X = x * 65536;
	XYValueObj.Y = y * 65536;

	syslog(LOG_INFO,
			"[DM]: CONVERTER CONVERT RGB_VALUE TO X_VALUE: [%d] Y_VALUE: [%d]",
			XYValueObj.X, XYValueObj.Y);

	return XYValueObj;
}

float fun(float x) {
	if (x < 0)
		return 0;
	else if (x > 1)
		return 1;
	else
		return x;
}

RGBVALUE convertXYtoRGB(float x_Value, float y_Value) {
	RGBVALUE RGBValueObj;

	syslog(LOG_INFO,
			"[DM]: CONVERTER CONVERT XY_VALUE TO RGB_VALUE START");

	syslog(LOG_INFO,
			"[DM]: CONVERTER CONVERT XY_VALUE TO RGB_VALUE X_VALUE: [%f] Y_VALUE: [%f]",
			x_Value, y_Value);

	float Z_Value = 0.0, X_Value = 0.0, Y_Value = 0.0, r_Value = 0.0, g_Value =
			0.0, b_Value = 0.0;

	X_Value = x_Value / 65279;
	Y_Value = y_Value / 65279;
	Z_Value = 1 - (X_Value + Y_Value);

	r_Value = 3.24103 * X_Value - 1.53741 * Y_Value - 0.49862 * Z_Value;
	r_Value = fun(r_Value);
	g_Value = -0.969242 * X_Value + 1.87596 * Y_Value + 0.041555 * Z_Value;
	g_Value = fun(g_Value);
	b_Value = 0.055632 * X_Value - 0.203979 * Y_Value + 1.05698 * Z_Value;
	b_Value = fun(b_Value);

	RGBValueObj.Red = r_Value * 255;
	RGBValueObj.Green = g_Value * 255;
	RGBValueObj.Blue = b_Value * 255;

	syslog(LOG_INFO,
			"[DM]: CONVERTER CONVERT XY_VALUE TO RGB RED_VALUE: [%d] GREEN_VALUE: [%d] BLUE_VALUE: [%d]",
			RGBValueObj.Red, RGBValueObj.Green, RGBValueObj.Blue);

	return RGBValueObj;
}

