#ifndef _C_TRIANGLE_H
#define _C_TRIANGLE_H

#include <opencv2/core/core.hpp>

class _c_triangle
{
public:
    _c_triangle(double sidea, double sideb, double sidec);
    _c_triangle(double ax, double ay,
                    double bx, double by,
                        double cx, double cy);
    _c_triangle(cv::Point2f p1, cv::Point2f p2, cv::Point2f p3);
    double side[3];
    double angle[3];
};

#endif // _C_TRIANGLE_H
