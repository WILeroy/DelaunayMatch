#include "_c_trianglenet.h"

using namespace cv;
using namespace std;

_c_triangleNet::_c_triangleNet(const vector< Point2f >::iterator begin,
                               const vector< Point2f >::iterator end,
                               const int width, const int heigh)
{
    Rect rect(0, 0, width, heigh);

    subdiv = new Subdiv2D(rect);

    for (vector< Point2f >::iterator i=begin; i!=end; i++)
    {
        subdiv->insert(*i);
    }
}

_c_triangleNet::_c_triangleNet(const vector< KeyPoint >::iterator begin,
                               const vector< KeyPoint >::iterator end,
                               const int width, const int heigh)
{
    Rect rect(0, 0, width, heigh);

    subdiv = new Subdiv2D(rect);

    for (vector< KeyPoint >::iterator i=begin; i!=end; i++)
    {
        Point2f p;
        p.x = (*i).pt.x;
        p.y = (*i).pt.y;
        subdiv->insert(p);
    }
}

_c_triangleNet::~_c_triangleNet()
{
    delete(subdiv);
}

void _c_triangleNet::_f_drawDelaunay(Mat& img, const Scalar delaunayColor)
{
    std::vector<Vec6f> triangleList;
    subdiv->getTriangleList(triangleList);
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
