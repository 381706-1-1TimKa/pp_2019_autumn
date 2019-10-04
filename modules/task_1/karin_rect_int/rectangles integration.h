//#pragma once
#ifndef RECTANGLES_INTEGRATION_H_
#define RECTANGLES_INTEGRATION_H_

double fun(double x);
double Integration(double(*f)(double), double a, double b, int k);
double ParIntegration(double(*f)(double), double a, double b, int k);

#endif // !RECTANGLES_INTEGRATION.H



