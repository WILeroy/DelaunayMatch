#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "_c_trianglenet.h"

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

    //-- 第一步:检测 Oriented FAST 角点位置
    detector->detect ( img_1,keypoints_1 );
    detector->detect ( img_2,keypoints_2 );

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

    //-- usr trianglenet class to build delaunay triangle net and draw
    std::vector< KeyPoint >::iterator iterBegin = goodKeyPoint1.begin();
    std::vector< KeyPoint >::iterator iterEnd   = goodKeyPoint1.end();
    _c_triangleNet net1(iterBegin, iterEnd, img_1.size().width, img_1.size().height);

    iterBegin = goodKeyPoint2.begin();
    iterEnd   = goodKeyPoint2.end();
    _c_triangleNet net2(iterBegin, iterEnd, img_2.size().width, img_2.size().height);

    net1._f_drawDelaunay(img_1, Scalar(255, 255, 255));
    net2._f_drawDelaunay(img_2, Scalar(255, 255, 255));

    imshow("IMG1_DELAUNAY", img_1);
    imshow("IMG2_DELAUNAY", img_2);


    /*
    Mat imgStart = imread("../source/3.jpg");
    std::vector<KeyPoint> keypointsStart;
    detector->detect ( imgStart, keypointsStart );
    iterBegin = keypointsStart.begin();
    iterEnd = keypointsStart.end();
    _c_triangleNet netStart(iterBegin, iterEnd, imgStart.size().width, imgStart.size().height);
    netStart._f_drawDelaunay(imgStart, Scalar(255, 255, 255));
    imshow("Start", imgStart);
    */

    waitKey(0);
    return 0;
}
