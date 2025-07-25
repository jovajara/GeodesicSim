#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif
 
typedef struct {
    char type;        
    double M;         
    double a;         
} Metric;

void metricInit(double* g, double* r, const Metric* metric);
void qVelNorm(double* v, double* g);
void symbolUpdate(double* chrstff, double* r, const Metric* metric);