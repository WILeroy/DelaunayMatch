#include "_sc_calculator.h"
#include <math.h>

void _sf_similarityMatrix(const _c_triangleNet net1, const _c_triangleNet net2, double *similarityMatrix)
{
    std::vector<cv::Vec6f> triangleList1;
    std::vector<cv::Vec6f> triangleList2;
    net1.subdiv.getTriangleList(triangleList1);
    net2.subdiv.getTriangleList(triangleList2);

    for (int i=0; i<triangleList1.size(); i++)
    {
        cv::Vec6f t1 = triangleList1[i];
        _c_triangle triangle1(t1[0], t1[1], t1[2], t1[3], t1[4], t1[5]);

        for (int j=0; j<triangleList2.size(); j++)
        {
            cv::Vec6f t2 = triangleList2[j];
            _c_triangle triangle2(t2[0], t2[1], t2[2], t2[3], t2[4], t2[5]);
            double a = _sf_similarityValue(triangle1, triangle2);
            similarityMatrix[i * 1000 + j] = (a < 0.75) ? 0 : a;
        }
    }
}

// *****************************************************
const double PI = 3.1415926;
const double Pa = 0.5;
const double Ks = PI/2;
const double Ns = 3;

double _sf_similarityValue(_c_triangle t1, _c_triangle t2)
{
    double q[3];
    double k[3];
    double dx[3];
    double u[3];
    for (int i=0; i<3; i++)
    {
        q[i] = (t1.angle[i] * Pa) / 3;
        k[i] = 1 / (2 * q[i] * q[i]);
        dx[i] = exp(-1 * k[i] * (t2.angle[i] - t1.angle[i]) * (t2.angle[i] - t1.angle[i]));
        u[i] = pow(cos(Ks * (1 - dx[i])), Ns);
    }

    return (u[0] + u[1] + u[2]) / 3;
}

double _sf_checkConstraint(_c_triangleNet net1, _c_triangleNet net2,
                           const int t1Index, const int t2Index)
{
    std::vector<cv::Vec6f> triangleList1;
    std::vector<cv::Vec6f> triangleList2;
    net1.subdiv.getTriangleList(triangleList1);
    net2.subdiv.getTriangleList(triangleList2);

    cv::Vec6f t1 = triangleList1[t1Index];
    cv::Vec6f t2 = triangleList2[t2Index];

    //int vertex[6];
    // would not to be used
    //int edge[6];
    cv::Point2f p0(t1[0], t1[1]);
    cv::Point2f p1(t1[2], t1[3]);
    cv::Point2f p2(t1[4], t1[5]);

    cv::Point2f p3(t2[0], t2[1]);
    cv::Point2f p4(t2[2], t2[3]);
    cv::Point2f p5(t2[4], t2[5]);

    // locate vertex of two triangles
    //net1.subdiv.locate(p0, edge[0], vertex[0]);
    //net1.subdiv.locate(p1, edge[1], vertex[1]);
    //net1.subdiv.locate(p2, edge[2], vertex[2]);
    //net2.subdiv.locate(p3, edge[3], vertex[3]);
    //net2.subdiv.locate(p4, edge[4], vertex[4]);
    //net2.subdiv.locate(p5, edge[5], vertex[5]);

    std::vector< cv::Vec4f > edgeList1;
    std::vector< cv::Vec4f > edgeList2;

    net1.subdiv.getEdgeList(edgeList1);
    net2.subdiv.getEdgeList(edgeList2);

    // locate edge of two triangles, and the next edge
    std::vector< int > p0EdgeIndex;
    std::vector< int > p1EdgeIndex;
    std::vector< int > p2EdgeIndex;
    std::vector< int > p3EdgeIndex;
    std::vector< int > p4EdgeIndex;
    std::vector< int > p5EdgeIndex;

    for (int i=0; i<edgeList1.size(); i++)
    {
        cv::Vec4f e = edgeList1[i];
        if ((e[0]==p0.x && e[1]==p0.y) || (e[2]==p0.x && e[3]==p0.y))
        {
            p0EdgeIndex.push_back(i);
        }
        if ((e[0]==p1.x && e[1]==p1.y) || (e[2]==p1.x && e[3]==p1.y))
        {
            p1EdgeIndex.push_back(i);
        }
        if ((e[0]==p2.x && e[1]==p2.y) || (e[2]==p2.x && e[3]==p2.y))
        {
            p2EdgeIndex.push_back(i);
        }
    }

    for (int i=0; i<edgeList2.size(); i++)
    {
        cv::Vec4f e = edgeList2[i];
        if ((e[0]==p3.x && e[1]==p3.y) || (e[2]==p3.x && e[3]==p3.y))
        {
            p3EdgeIndex.push_back(i);
        }
        if ((e[0]==p4.x && e[1]==p4.y) || (e[2]==p4.x && e[3]==p4.y))
        {
            p4EdgeIndex.push_back(i);
        }
        if ((e[0]==p5.x && e[1]==p5.y) || (e[2]==p5.x && e[3]==p5.y))
        {
            p5EdgeIndex.push_back(i);
        }
    }

    std::vector< cv::Point2f > outP(6);

    for (int i=0; i<p0EdgeIndex.size(); i++)
    {
        for (int j=0; j<p1EdgeIndex.size(); j++)
        {
            cv::Vec4f e1 = edgeList1[i];
            cv::Vec4f e2 = edgeList1[j];
            if (e1[0]==e2[0] && e1[1]==e2[1])
            {
                outP[0].x = e1[0];
                outP[0].y = e1[1];
            }
            if (e1[0]==e2[2] && e1[1]==e2[3])
            {
                outP[0].x = e1[0];
                outP[0].y = e1[1];
            }
            if (e1[2]==e2[0] && e1[3]==e2[1])
            {
                outP[0].x = e1[2];
                outP[0].y = e1[3];
            }
            if (e1[2]==e2[3] && e1[2]==e2[3])
            {
                outP[0].x = e1[2];
                outP[0].y = e1[3];
            }
        }
    }

    for (int i=0; i<p0EdgeIndex.size(); i++)
    {
        for (int j=0; j<p2EdgeIndex.size(); j++)
        {
            cv::Vec4f e1 = edgeList1[i];
            cv::Vec4f e2 = edgeList1[j];
            if (e1[0]==e2[0] && e1[1]==e2[1])
            {
                outP[1].x = e1[0];
                outP[1].y = e1[1];
            }
            if (e1[0]==e2[2] && e1[1]==e2[3])
            {
                outP[1].x = e1[0];
                outP[1].y = e1[1];
            }
            if (e1[2]==e2[0] && e1[3]==e2[1])
            {
                outP[1].x = e1[2];
                outP[1].y = e1[3];
            }
            if (e1[2]==e2[3] && e1[2]==e2[3])
            {
                outP[1].x = e1[2];
                outP[1].y = e1[3];
            }
        }
    }

    for (int i=0; i<p1EdgeIndex.size(); i++)
    {
        for (int j=0; j<p2EdgeIndex.size(); j++)
        {
            cv::Vec4f e1 = edgeList1[i];
            cv::Vec4f e2 = edgeList1[j];
            if (e1[0]==e2[0] && e1[1]==e2[1])
            {
                outP[2].x = e1[0];
                outP[2].y = e1[1];
            }
            if (e1[0]==e2[2] && e1[1]==e2[3])
            {
                outP[2].x = e1[0];
                outP[2].y = e1[1];
            }
            if (e1[2]==e2[0] && e1[3]==e2[1])
            {
                outP[2].x = e1[2];
                outP[2].y = e1[3];
            }
            if (e1[2]==e2[3] && e1[2]==e2[3])
            {
                outP[2].x = e1[2];
                outP[2].y = e1[3];
            }
        }
    }

    for (int i=0; i<p3EdgeIndex.size(); i++)
    {
        for (int j=0; j<p4EdgeIndex.size(); j++)
        {
            cv::Vec4f e1 = edgeList2[i];
            cv::Vec4f e2 = edgeList2[j];
            if (e1[0]==e2[0] && e1[1]==e2[1])
            {
                outP[3].x = e1[0];
                outP[3].y = e1[1];
            }
            if (e1[0]==e2[2] && e1[1]==e2[3])
            {
                outP[3].x = e1[0];
                outP[3].y = e1[1];
            }
            if (e1[2]==e2[0] && e1[3]==e2[1])
            {
                outP[3].x = e1[2];
                outP[3].y = e1[3];
            }
            if (e1[2]==e2[3] && e1[2]==e2[3])
            {
                outP[3].x = e1[2];
                outP[3].y = e1[3];
            }
        }
    }

    for (int i=0; i<p3EdgeIndex.size(); i++)
    {
        for (int j=0; j<p5EdgeIndex.size(); j++)
        {
            cv::Vec4f e1 = edgeList2[i];
            cv::Vec4f e2 = edgeList2[j];
            if (e1[0]==e2[0] && e1[1]==e2[1])
            {
                outP[4].x = e1[0];
                outP[4].y = e1[1];
            }
            if (e1[0]==e2[2] && e1[1]==e2[3])
            {
                outP[4].x = e1[0];
                outP[4].y = e1[1];
            }
            if (e1[2]==e2[0] && e1[3]==e2[1])
            {
                outP[4].x = e1[2];
                outP[4].y = e1[3];
            }
            if (e1[2]==e2[3] && e1[2]==e2[3])
            {
                outP[4].x = e1[2];
                outP[4].y = e1[3];
            }
        }
    }

    for (int i=0; i<p4EdgeIndex.size(); i++)
    {
        for (int j=0; j<p5EdgeIndex.size(); j++)
        {
            cv::Vec4f e1 = edgeList2[i];
            cv::Vec4f e2 = edgeList2[j];
            if (e1[0]==e2[0] && e1[1]==e2[1])
            {
                outP[5].x = e1[0];
                outP[5].y = e1[1];
            }
            if (e1[0]==e2[2] && e1[1]==e2[3])
            {
                outP[5].x = e1[0];
                outP[5].y = e1[1];
            }
            if (e1[2]==e2[0] && e1[3]==e2[1])
            {
                outP[5].x = e1[2];
                outP[5].y = e1[3];
            }
            if (e1[2]==e2[3] && e1[2]==e2[3])
            {
                outP[5].x = e1[2];
                outP[5].y = e1[3];
            }
        }
    }

    // use out point and three points of triangle to build new triangle
    _c_triangle outT1(p0, p1, outP[0]);
    _c_triangle _outT1(p3, p4, outP[3]);

    _c_triangle outT2(p0, p2, outP[1]);
    _c_triangle _outT2(p3, p5, outP[4]);

    _c_triangle outT3(p1, p2, outP[2]);
    _c_triangle _outT3(p4, p5, outP[5]);

    double similarityT1 = _sf_similarityValue(outT1, _outT1);
    double similarityT2 = _sf_similarityValue(outT2, _outT2);
    double similarityT3 = _sf_similarityValue(outT3, _outT3);

    if (similarityT1<0.75 || similarityT2<0.75 || similarityT3<0.75)
    {
        return 0;
    }
    return 1;
}
