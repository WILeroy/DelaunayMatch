#include "_c_triangle_net.h"
#include "_c_triangle.h"

using namespace cv;
using namespace std;

_c_triangleNet::_c_triangleNet(const vector< Point2f >::iterator begin,
                               const vector< Point2f >::iterator end,
                               int width, int heigh)
{
    Rect rect(0, 0, width, heigh);

    Subdiv2D temp(rect);

    for (vector< Point2f >::iterator i=begin; i!=end; i++)
    {
        temp.insert(*i);
    }

    subdiv = temp;
}

_c_triangleNet::_c_triangleNet(const vector< KeyPoint >::iterator begin,
                               const vector< KeyPoint >::iterator end,
                               int width, int heigh)
{
    Rect rect(0, 0, width, heigh);

    Subdiv2D temp(rect);

    for (vector< KeyPoint >::iterator i=begin; i!=end; i++)
    {
        Point2f p;
        p.x = (*i).pt.x;
        p.y = (*i).pt.y;
        temp.insert(p);
    }

    subdiv = temp;
}

void _c_triangleNet::_f_drawDelaunay(Mat& img, Scalar delaunayColor)
{
    std::vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    std::vector<Point> pt(3);
    Size size = img.size();
    Rect rect(0, 0, size.width, size.height);

    for (size_t i = 0; i < triangleList.size(); i++)
    {
        Vec6f t = triangleList[i];
        pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = Point(cvRound(t[4]), cvRound(t[5]));

        // Draw rectangles completely inside the image.
        if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2]))
        {
            line(img, pt[0], pt[1], delaunayColor, 1, CV_AA, 0);
            line(img, pt[1], pt[2], delaunayColor, 1, CV_AA, 0);
            line(img, pt[2], pt[0], delaunayColor, 1, CV_AA, 0);
        }
    }
}
