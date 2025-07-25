#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "geodesic.h"

double *chrstff = NULL, *g = NULL, *r = NULL, *v = NULL, *a = NULL;
FILE *out = NULL;

void cleanup() {
  // Free allocated memory and close files
  if (chrstff) free(chrstff);
  if (g)       free(g);
  if (r)       free(r);
  if (v)       free(v);
  if (a)       free(a);
  if (out)     fclose(out);
}

int main() {

  // Register cleanup.
  atexit(cleanup);

  Metric schwarzschild = {.type = 'S', .M = 1.0, .a = 0.0};

  chrstff = malloc(4 * 4 * 4 * sizeof(double));
  g = malloc(4 * 4 * sizeof(double));
  r = malloc(4 * sizeof(double));
  v = malloc(4 * sizeof(double));
  a = malloc(4 * sizeof(double));
  out = fopen("geodesic.txt", "w");

  if (!chrstff || !g || !r || !v || !a || !out) {
    fprintf(stderr, "Memory allocation failed\n");
    return 1;
  }
  
  // Initialize the four-position
  r[0] = 0.0;
  r[1] = 20 * schwarzschild.M; 
  r[2] = M_PI / 4;
  r[3] = 0.0;

  // Initialize the metric
  metricInit(g, r, &schwarzschild);
  
  // Initialize the four-velocity
  v[0] = 0.0;
  v[1] = 0.0;
  v[2] = 0.0;
  v[3] = 0.0; 

  // Normalize the four-velocity
  qVelNorm(v, g);
  
  for (int i=0; i<4; i++){
    a[i] = 0.0;
  }
  
  // Initialize Christoffel symbols
  symbolUpdate(chrstff, r, &schwarzschild);

  double dtau = 1e-5;
  double tauf = 500.0;
  int pstep = 50000;  // Print every n steps
  int step = 0;

  // Simulation loop using the leapfrog method
  for (double tau = 0.0; tau < tauf; tau += dtau, step++) {

    // Print the results at specified intervals
    if (step % pstep == 0) {
      fprintf(out, "%lf %lf %lf %lf \n", tau, r[1]*sin(r[2])*cos(r[3]), r[1]*sin(r[2])*sin(r[3]), r[1]*cos(r[2]));
    }

    // check if the particle has crossed the event horizon
    if (r[1] < 2 * schwarzschild.M) {
      printf("Particle crossed the event horizon at tau = %lf\n", tau);
      break;
    }

    // Synchronized leapfrog method
    // Update positions by full-step and velocities by half-step
    for (int i = 0; i < 4; i++) {
      v[i] += 0.5 * a[i] * dtau;
      r[i] += v[i] * dtau + 0.5 * a[i] * dtau * dtau;
    }

    // Update the Christoffel symbols based on the current position
    symbolUpdate(chrstff, r, &schwarzschild);

    for (int i = 0; i < 4; i++) {
      a[i] = 0.0; // Reset accelerations before recalculating
    }

    // Calculate accelerations 
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        for (int k = 0; k < 4; k++) {
          a[i] -= chrstff[4 * 4 * i + 4 * j + k] * v[j] * v[k];
        }
      }
    }

    // Update velocities by half-step again
    for (int i = 0; i < 4; i++) {
      v[i] += 0.5 * a[i] * dtau;
    }
  }

  return 0;
}
