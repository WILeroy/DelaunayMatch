#ifndef _SC_CALCULATOR_H
#define _SC_CALCULATOR_H

#include <iostream>
#include "_c_triangle.h"
#include "_c_triangle_net.h"

double _sf_similarityValue(_c_triangle t1, _c_triangle t2);
void _sf_similarityMatrix(const _c_triangleNet net1, const _c_triangleNet net2, double *similarityMatrix);
double _sf_checkConstraint(_c_triangleNet net1, _c_triangleNet net2, const int triangle1, const int triangle2);

#endif // _SC_CALCULATOR_H
