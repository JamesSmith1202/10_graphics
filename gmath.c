#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  normalize(normal);
  normalize(view);
  color ambient = calculate_ambient(alight, areflect);
  color diffuse = calculate_diffuse(light, dreflect, normal);
  color specular = calculate_specular(light, sreflect, view, normal);
  i.red = ambient.red + diffuse.red + specular.red;
  i.green = ambient.green + diffuse.green + specular.green;
  i.blue = ambient.blue + diffuse.blue + specular.blue;
  limit_color(&i);
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * (*areflect);
  a.green = alight.green * (*areflect);
  a.blue = alight.blue * (*areflect);
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  double diffuse_constant = (*dreflect) * dot_product(normal, light[LOCATION]);
  d.red = light[COLOR][RED] * diffuse_constant;
  d.green = light[COLOR][GREEN] * diffuse_constant;
  d.blue = light[COLOR][BLUE] * diffuse_constant;
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  double b = 2*dot_product(normal, light[LOCATION]);//base of R=2(N*L)N-L
  normal[0] *= b;
  normal[1] *= b;
  normal[2] *= b;
  double * r = normal;
  r[0] -= light[LOCATION][0];
  r[1] -= light[LOCATION][1];
  r[2] -= light[LOCATION][2];
  double spec_const = (*sreflect) * pow(dot_product(view, r), SPECULAR_EXP);
  s.red = light[COLOR][RED] * spec_const;
  s.green = light[COLOR][GREEN] * spec_const;
  s.blue = light[COLOR][BLUE] * spec_const;
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  c->red = c->red > 255 ? 255 : c->red;
  c->green = c->green > 255 ? 255 : c->green;
  c->blue = c->blue > 255 ? 255 : c->blue;
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double length = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
  vector[0] = vector[0]/length;
  vector[1] = vector[1]/length;
  vector[2] = vector[2]/length;
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

double *calculate_normal(struct matrix *polygons, int i) {
  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
