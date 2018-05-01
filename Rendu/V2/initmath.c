#include <math.h> /* A inclure dans le fichier SG_math.c */

/* A recopier dans le fichier SG_math.c */
double f1 (int i, int j){
	return sin ((double)i*j + (double)j);
}

double f2 (int i, int j){
	double z = f1 (i, j);
	return z * z;
}