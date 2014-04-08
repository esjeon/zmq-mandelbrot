#pragma once

#define PORT_VENT "5555"
#define PORT_SINK "5556"

#define MAX_STEP 30
#define NUM_POW 2

#define IMG_WIDTH 5000
#define IMG_HEIGHT 5000

#include <complex>
#include <cmath>

typedef double Number;
typedef std::complex<Number> Complex;

struct InputData {
	int x;
	int y;
	Complex c;

	InputData(int x, int y, Complex c)
		: x(x), y(y), c(c)
	{}
};

struct OutputData {
	int x;
	int y;
	int d;

	OutputData(int x, int y, int d)
		: x(x), y(y), d(d)
	{}
};

