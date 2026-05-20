#ifndef __NEWTON_H__
#define __NEWTON_H__

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

// monta a matriz das jacobianas [F'(X(i))], vai ser uma matriz 3-diagonal acho, recebe o vetor X(i) e o tamanho do vetor
double **montaJacobiana(double *x, long n);

// acha os valores de X(i + 1) a partir da matriz jacobiana, do valor de X(i) e do tamanho do vetor X
double *achaProxX(double **jacobianas, double *x, double *fx, long n);

// resolve o sistema nao-linear atraves do metodo de Newton
double *newton(double *x0, double epsilon, long max, long n);

#endif __NEWTON_H__