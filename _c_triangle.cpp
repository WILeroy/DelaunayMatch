#include "_c_triangle.h"
#include <math.h>

_c_triangle::_c_triangle(double sidea, double sideb, double sidec)
{
    side[0] = sidea;
    side[1] = sideb;
    side[2] = sidec;

    double cosA = (sideb * sideb + sidec * sidec - sidea * sidea) / (2 * sideb * sidec);
    double cosB = (sidea * sidea + sidec * sidec - sideb * sideb) / (2 * sidea * sidec);
    double cosC = (sidea * sidea + sideb * sideb - sidec * sidec) / (2 * sidea * sideb);

    angle[0] = acos(cosA);
    angle[1] = acos(cosB);
    angle[2] = acos(cosC);
}

_c_triangle::_c_triangle(double ax, double ay,
                            double bx, double by,
                                double cx, double cy)
{
    side[0] = sqrt( (bx - cx) * (bx - cx) + (by - cy) * (by - cy) );
    side[1] = sqrt( (ax - cx) * (ax - cx) + (ay - cy) * (ay - cy) );
    side[2] = sqrt( (ax - bx) * (ax - bx) + (ay - by) * (ay - by) );

    double cosA = (side[1] * side[1] + side[2] * side[2] - side[0] * side[0]) / (2 * side[1] * side[2]);
    double cosB = (side[0] * side[0] + side[2] * side[2] - side[1] * side[1]) / (2 * side[0] * side[2]);
    double cosC = (side[0] * side[0] + side[1] * side[1] - side[2] * side[2]) / (2 * side[0] * side[1]);

    angle[0] = acos(cosA);
    angle[1] = acos(cosB);
    angle[2] = acos(cosC);
}

_c_triangle::_c_triangle(cv::Point2f p1, cv::Point2f p2, cv::Point2f p3)
{
    side[0] = sqrt( (p2.x - p3.x) * (p2.x - p3.x) + (p2.y - p3.y) * (p2.y - p3.y) );
    side[1] = sqrt( (p1.x - p3.x) * (p1.x - p3.x) + (p1.y - p3.y) * (p1.y - p3.y) );
    side[2] = sqrt( (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) );

    double cosA = (side[1] * side[1] + side[2] * side[2] - side[0] * side[0]) / (2 * side[1] * side[2]);
    double cosB = (side[0] * side[0] + side[2] * side[2] - side[1] * side[1]) / (2 * side[0] * side[2]);
    double cosC = (side[0] * side[0] + side[1] * side[1] - side[2] * side[2]) / (2 * side[0] * side[1]);

    angle[0] = acos(cosA);
    angle[1] = acos(cosB);
    angle[2] = acos(cosC);
}
