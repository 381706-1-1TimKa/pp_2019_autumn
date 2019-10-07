// Copyright 2019 Karin Timofey
#ifndef RECTANGLES_INTEGRATION_H_
#define RECTANGLES_INTEGRATION_H_

double QuadrFunInt(double a, double b, double c, double k1, double k2);
double Integration(double(*f)(double), double a, double b, int k);
double ParIntegration(double(*f)(double), double a, double b, int k);

#endif



