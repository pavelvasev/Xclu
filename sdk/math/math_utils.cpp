#include "math_utils.h"
#include <random>
#include <QRandomGenerator>

//-------------------------------------------------------
//x*x
float xsqrf(float x) {
	return x * x;
}

int xsqri(int x) {
	return x * x; 
}


//-------------------------------------------------------
//x*x*x
float xpow3f(float x) {
	return x * x*x;
}

//-------------------------------------------------------
//min and max
float xminf(float x, float y) {
    return (x < y) ? x : y;
}

float xmaxf(float x, float y) {
    return (x > y) ? x : y;
}

double xmind(double x, double y) {
    return (x < y) ? x : y;
}

double xmaxd(double x, double y) {
    return (x > y) ? x : y;
}

int xmini(int x, int y) {
    return (x < y) ? x : y;
}

int xmaxi(int x, int y) {
    return (x > y) ? x : y;
}

//-------------------------------------------------------
//clamp to range [a,b], including a and b
float xclampf(float x, float a, float b) {
    if (b<a) std::swap(a,b);
    return ((x<a)?a : ((x>b)?b:x));
}
double xclampd(double x, double a, double b) {
    if (b<a) std::swap(a,b);
    return ((x<a)?a : ((x>b)?b:x));
}
int clampi(int x, int a, int b) {
    if (b<a) std::swap(a,b);
    return ((x<a)?a : ((x>b)?b:x));
}

//-------------------------------------------------------
//Linear interpolation from [0,1]
float xlerpf(float A, float B, float x) { //x = 0..1
    return A*(1-x) + B*x;
}

double xlerpd(double A, double B, double x) {
    return A*(1-x) + B*x;
}

//-------------------------------------------------------
//Linear interpolation
float xmapf(float x, float a, float b, float A, float B) {
    if (a==b) return A;
    return (x - a) / (b - a) * (B - A) + A;
}

double xmapd(double x, double a, double b, double A, double B) {
    if (a==b) return A;
    return (x - a) / (b - a) * (B - A) + A;
}

int xmapi(int x, int a, int b, int A, int B) {
    //Need to use "long long..." to avoid overflow.
    //And so we obtain perfect int result of map !
    if (a==b) return A;
    return ((long long int)(B-A)) * (x - a) / (b - a) + A;
}

//-------------------------------------------------------
//clamped linear interpolation
float xmapf_clamped(float x, float a, float b, float A, float B) {
    return xclampf(xmapf(x,a,b,A,B),A,B);
}

double xmapd_clamped(double x, double a, double b, double A, double B) {
    return xclampd(xmapd(x,a,b,A,B),A,B);
}

int xmapi_clamped(int x, int a, int b, int A, int B) {
    return clampi(xmapi(x,a,b,A,B),A,B);
}

//-------------------------------------------------------
//https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
//std::random_device randomi_rd_;  //Will be used to obtain a seed for the random number engine

//https://doc.qt.io/qt-5/qrandomgenerator.html#details
int xrandomi(int a, int b) {
    std::uniform_real_distribution<> dist(a, b+1);
    return dist(*QRandomGenerator::global());

    //std::mt19937 gen(randomi_rd_()); //Standard mersenne_twister_engine seeded with rd()
    //std::uniform_int_distribution<> distrib(a, b);
    //return distrib(gen);
}

//-------------------------------------------------------
float xrandomf(float a, float b) {
    std::uniform_real_distribution<> dist(a, b);
    return dist(*QRandomGenerator::global());
}

//-------------------------------------------------------
