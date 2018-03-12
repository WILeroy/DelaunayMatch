#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "_c_triangle_net.h"
#include "_sc_calculator.h"

using namespace std;
using namespace cv;

int main ()
{
    //-- 读取图像
    Mat img_1 = imread ( "../source/1.jpg", CV_LOAD_IMAGE_COLOR ); // 520 * 346
    Mat img_2 = imread ( "../source/2.jpg", CV_LOAD_IMAGE_COLOR ); // 540 * 360

    //-- 初始化
    std::vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1, descriptors_2;
    Ptr<FeatureDetector> detector = ORB::create();
    Ptr<DescriptorExtractor> descriptor = ORB::create();
    Ptr<DescriptorMatcher> matcher  = DescriptorMatcher::create ( "BruteForce-Hamming" );

    if (img_1.empty() || img_2.empty())
    {
        cout << "No Image" << endl;
    }

    //-- 第一步:检测 Oriented FAST 角点位置
    detector->detect ( img_1, keypoints_1 );
    detector->detect ( img_2, keypoints_2 );

    //-- 第二步:根据角点位置计算 BRIEF 描述子
    descriptor->compute ( img_1, keypoints_1, descriptors_1 );
    descriptor->compute ( img_2, keypoints_2, descriptors_2 );

        //-- 第三步:对两幅图像中的BRIEF描述子进行匹配，使用 Hamming 距离
    vector<DMatch> matches;
    matcher->match ( descriptors_1, descriptors_2, matches );

    //-- 第四步:匹配点对筛选
    double min_dist=10000, max_dist=0;

    //找出所有匹配之间的最小距离和最大距离, 即是最相似的和最不相似的两组点之间的距离
    for ( int i = 0; i < descriptors_1.rows; i++ )
    {
        double dist = matches[i].distance;
        if ( dist < min_dist ) min_dist = dist;
        if ( dist > max_dist ) max_dist = dist;
    }

    //当描述子之间的距离大于两倍的最小距离时,即认为匹配有误.但有时候最小距离会非常小,设置一个经验值作为下限.
    std::vector< DMatch > good_matches;
    for ( int i = 0; i < descriptors_1.rows; i++ )
    {
        if ( matches[i].distance <= max ( 2*min_dist, 37.0 ) )
        {
            good_matches.push_back ( matches[i] );
        }
    }

    //-- 提取得到的两幅图像的匹配关键点对
    std::vector< KeyPoint > goodKeyPoint1;
    std::vector< KeyPoint > goodKeyPoint2;

    for (int i=0; i<good_matches.size(); i++)
    {
        goodKeyPoint1.push_back( keypoints_1[good_matches[i].queryIdx] );
        goodKeyPoint2.push_back( keypoints_2[good_matches[i].trainIdx] );
    }

    //-- use trianglenet class to build delaunay triangle net and draw
    std::vector< KeyPoint >::iterator iterBegin = goodKeyPoint1.begin();
    std::vector< KeyPoint >::iterator iterEnd   = goodKeyPoint1.end();
    _c_triangleNet net1(iterBegin, iterEnd, img_1.size().width, img_1.size().height);

    iterBegin = goodKeyPoint2.begin();
    iterEnd   = goodKeyPoint2.end();
    _c_triangleNet net2(iterBegin, iterEnd, img_2.size().width, img_2.size().height);

    //net1._f_drawDelaunay(img_1, Scalar(255, 255, 255));
    //net2._f_drawDelaunay(img_2, Scalar(255, 255, 255));

    //imshow("IMG1_DELAUNAY", img_1);
    //imshow("IMG2_DELAUNAY", img_2);

    //-- use triangle class to calculate degree of similarity for delaunay net

    std::vector< Vec6f > t1;
    std::vector< Vec6f > t2;
    net1.subdiv.getTriangleList(t1);
    net2.subdiv.getTriangleList(t2);

    double matrix[1000][1000];// = new double[t1.size() * t2.size()];

    _sf_similarityMatrix(net1, net2, (double *)matrix);

    std::vector<Point2f> pf;
    std::vector<Point2f> _pf;

    for (int i=0; i<t1.size(); i++)
    {
        for (int j=0; j<t2.size(); j++)
        {

            if (matrix[i][j]!=0 && _sf_checkConstraint(net1, net2, i, j)==0)
                matrix[i][j] = 0;
            if (matrix[i][j] != 0)
            {
                Point2f p1, p2, p3;
                Vec6f e = t1[i];
                p1.x = e[0];
                p1.y = e[1];
                p2.x = e[2];
                p2.y = e[3];
                p3.x = e[4];
                p3.y = e[5];
                Rect rect(0, 0, img_1.size().width, img_1.size().height);
                if (rect.contains(p1) && rect.contains(p2) && rect.contains(p3))
                {
                    pf.push_back(p1);
                    pf.push_back(p2);
                    pf.push_back(p3);
                }
                cout << "( " << i << ", " << j << " )" << endl;

                Point2f p4, p5, p6;
                Vec6f _e = t2[j];
                p4.x = _e[0];
                p4.y = _e[1];
                p5.x = _e[2];
                p5.y = _e[3];
                p6.x = _e[4];
                p6.y = _e[5];
                Rect _rect(0, 0, img_2.size().width, img_2.size().height);
                if (_rect.contains(p4) && _rect.contains(p5) && _rect.contains(p6))
                {
                    _pf.push_back(p4);
                    _pf.push_back(p5);
                    _pf.push_back(p6);
                }
            }

            /*
            if (matrix[i][j] != 0)
            {

            }
            */
        }
    }

    _c_triangleNet net3(pf.begin(), pf.end(), img_1.size().width, img_1.size().height);
    _c_triangleNet net4(_pf.begin(), _pf.end(), img_2.size().width, img_2.size().height);

    net3._f_drawDelaunay(img_1, Scalar(255,255,255));
    net4._f_drawDelaunay(img_2, Scalar(255,255,255));

    imshow("IMG1_DELAUNAY", img_1);
    imshow("IMG2_DELAUNAY", img_2);

    waitKey(0);
    return 0;
}
