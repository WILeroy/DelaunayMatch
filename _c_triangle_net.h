#ifndef _C_TRIANGLENET_H
#define _C_TRIANGLENET_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class _c_triangleNet
{
public:
    // -- Init triangle net by point set
    // like:
    //      Point2f set
    //      KeyPoint set
    _c_triangleNet(const std::vector< cv::Point2f >::iterator begin,
                   const std::vector< cv::Point2f >::iterator end,
                   int width, int heigh);
    _c_triangleNet(const std::vector< cv::KeyPoint >::iterator begin,
                   const std::vector< cv::KeyPoint >::iterator end,
                   int width, int heigh);

    void _f_drawDelaunay(cv::Mat& img, cv::Scalar delaunaColor);

    // -- delaunay triangle by OpenCV3.2
    cv::Subdiv2D subdiv;
};

#endif // _C_TRIANGLENET_H
