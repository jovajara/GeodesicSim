#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "geodesic.h"

void metricInit(double* g, double* r, const Metric* metric) {
  
  // Helper variables for the metric components
  double M = metric->M;
  double a = metric->a;
  double theta = r[2];
  double sin_theta = sin(theta);
  double cos_theta = cos(theta);
  double r2 = r[1] * r[1];
  double a2 = a * a;
  double sin2 = sin_theta * sin_theta;
  double cos2 = cos_theta * cos_theta;
  double Sigma = r2 + a2 * cos2;
  double Delta = r2 - 2*M*r[1] + a2;

  // Initialize the metric tensor to zero
  for (int i = 0; i < 16; i++) {
    g[i] = 0.0;
  }

  if (metric->type == 'S') { // Schwarzschild metric
    g[0] = -(1.0 - 2*M/r[1]); // g_tt
    g[5] = 1.0/(1.0 - 2*M/r[1]); // g_rr
    g[10] = r2; // g_thetatheta
    g[15] = r2*sin2; // g_phiphi
  } else if (metric->type == 'K') { // Kerr metric
    g[0] = -(1.0 - 2*M*r[1]/Sigma); // g_tt
    g[3] = g[12] = -2*M*a*r[1]*sin2 / Sigma; // g_tphi & g_phit
    g[5] = Sigma / Delta; // g_rr
    g[10] = Sigma; // g_thetatheta
    g[15] = (r2 + a2 + (2*M*a2*r[1]*sin2)/Sigma) * sin2; // g_phiphi
  } else {
    printf("Unknown metric type: %c\n", metric->type);
    exit(1);
  }
}

void qVelNorm(double* v, double* g) {
  
  // Normalize the four-velocity by calculating the corresponding time component
  // and then check if the local velocity is less than 1 (1*c).

  double sum_spatial = 0.0;

  for (int i = 1; i < 4; i++){
    for (int j = 1; j < 4; j++){
      sum_spatial += g[4*i + j] * v[i] * v[j];
    }
  }

  double v0_squared = (-1.0 - sum_spatial) / g[0];

  if (v0_squared < 0.0) {
    printf("Non-physical four-velocity (v_t is complex). Try choosing another values for the spatial velocity.\n");
    exit(1);
  } else {
    v[0] = sqrt(v0_squared);
  }
  
  double vlocal_sq = sum_spatial / (-g[0] * v[0] * v[0]);
  
  if (sqrt(vlocal_sq) >= 1.0) {
    printf("The local velocity is greater than c! v_local = %.6f * c. Try choosing another values for the spatial velocity.\n", sqrt(vlocal_sq));
    exit(1);
  }
}

void symbolUpdate(double* chrstff, double* r, const Metric* metric) {
  
  double M = metric->M;
  
  // Initialize the Christoffel symbols to zero
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        chrstff[4*4*i + 4*j + k] = 0.0;
      }
    }
  }
  if (metric->type == 'S'){
    double term = 1 - 2*M/r[1];

    for (int i=0; i<4; i++){
      for (int j=0; j<4; j++){
        for (int k=0; k<4; k++){

          int ind = 4*4*i + 4*j + k;

          if ((i == 0 && j == 2 && k == 3) || (i==0 && j==3 && k==2)){
            chrstff[ind] = M/(r[1]*r[1]*term);
          }
          else if (i==1 && j == k){
            if (j == 0) chrstff[ind] = (M * term)/(r[1]*r[1]);
            else if (j == 1) chrstff[ind] = - M/(r[1]*r[1]*term);
            else if (j == 2 || j == 3) chrstff[ind] = - r[1]*term;
          }
          else if ((i == 2 && j == 1 && k == 2) || (i==2 && j==2 && k==1) ||
                   (i==3 && j==1 && k==3) || (i==3 && j==3 && k==1)) {
            chrstff[ind] = 1.0/r[1];
          }
          else chrstff[ind] = 0.0;
        }
      }
    }
  } else if (metric->type == 'K') {
    printf("Christoffel symbols for Kerr metric are not implemented yet.\n");
    exit(1);  
  } else {
    printf("Unknown metric type: %c\n", metric->type);
    exit(1);
  }
}
