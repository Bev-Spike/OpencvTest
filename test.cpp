#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;
Mat img,dst;
void splitTest(){
    Mat channal[3];
    split(img, channal);
    cout << "分离前通道数："<< img.channels() << endl;
    cout << "分离后的通道数：" <<channal[0].channels() << endl;
    /*
    imshow("b", channal[0]);
    imshow("g", channal[1]);
    imshow("r", channal[2]);
    */
    Mat result = channal[0] - channal[2];
    imshow("result", result);

}

void thresholdTest(){
    Mat gary;
    cvtColor(img,gary, CV_BGR2GRAY);
    cout << "gary type:" <<gary.type() << endl;
    imshow("gary", gary);
    threshold(gary,dst, 100, 255, THRESH_BINARY);
    imshow("BINARY", dst);
//    threshold(gary,dst, 100, 255, THRESH_BINARY_INV);
//    imshow("BINARY_INV", dst);



}
void dilateTest(){
    Mat element,gary;


//    element = getStructuringElement(MORPH_RECT, Size(3,3));//一个大小为3的矩形元素
//    dilate(img, dst, element);
//    imshow("dilate_rect", dst);

    element = getStructuringElement(MORPH_ELLIPSE, Size(3,3));//一个大小为3的椭圆元素
//    dilate(img, dst, element);
//    imshow("dilate_ellipse", dst);

    cvtColor(img,gary, CV_BGR2GRAY);
    threshold(gary,dst, 100, 255, THRESH_BINARY);
    imshow("dilate_before", dst);
    dilate(dst, dst, element);
    imshow("dilate_after", dst);
}

void findContoursTest(){
    thresholdTest();
    Mat imageContours = Mat::zeros(img.size(),CV_8UC3);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(dst,contours, hierarchy, RETR_CCOMP,CHAIN_APPROX_SIMPLE);
    for(size_t i = 0; i < contours.size();i++){
        drawContours(imageContours,contours,i,Scalar(255,255,255), 1 ,8);
    }
    imshow("dst_after",dst);
    imshow("contours", imageContours);
}

void rotatedRectTest(){
    img = imread("juxing.png",IMREAD_GRAYSCALE);
    if(img.empty()){
        cout << "open file failed" << endl;
        return;
    }
    imshow("tuoyuan", img);
    Mat imageContours = Mat::zeros(img.size(),CV_8UC3);
    cout << img.type() << endl;
    vector<Vec4i> hierarchy;
    vector<vector<Point> > contours;
    Canny(img,dst, 30, 70);
    imshow("canny", dst);
    findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    for(size_t i = 0; i < contours.size(); i++){
        drawContours(imageContours,contours, i, Scalar(255,255,255),  1, 8);

    }

    imshow("contouts", imageContours);

    Mat image = Mat::zeros(img.size(),CV_8UC3);
    RotatedRect rotatedRect = minAreaRect(contours[0]);
    cout <<"rotateRect angle:" <<rotatedRect.angle << endl;
    cout <<"size: :" <<rotatedRect.size << rotatedRect.size.width <<  "  "<<rotatedRect.size.height << endl;//可以看出，width为第一个元素，height为第二个
    Point2f* vertices = new cv::Point2f[4];
    rotatedRect.points(vertices);
    vector<Point> con;
    for(int i = 0; i < 4; i++){
        con.push_back(vertices[i]);
    }
    vector<std::vector<cv::Point> > cons;
    cons.push_back(con);
    drawContours(imageContours, cons, 0, Scalar(255,255,255), 1,8);


    RotatedRect ellipse1 = fitEllipse(contours[0]);
    vertices = new cv::Point2f[4];
    ellipse1.points(vertices);
    vector<Point> con2;
    for(int i = 0; i < 4; i++){
        con2.push_back(vertices[i]);
        }
     vector<std::vector<cv::Point>  > cons2;
     cons2.push_back(con2);
     drawContours(imageContours, cons2, 0, Scalar(0,255,255), 1,8);

    ellipse(imageContours, ellipse1, cv::Scalar(0, 255, 255), 1, 8);//拟合椭圆
    imshow("result", imageContours);

//    Rect rect = rotatedRect.boundingRect();
//    const Rect srcBound(Point(0, 0), img.size());
//    //rect &= srcBound;
//    Mat drawRec = Mat::zeros(img.size(),CV_8UC3);
//    rectangle(drawRec, rect,Scalar(0,0,255));
//    imshow("rec",drawRec);

}

int main()
{
    img = imread("amiya.png", IMREAD_COLOR);
    //imshow("wife", img);
    cout << "src type:" << img.type() << endl;
    //findContoursTest();
    rotatedRectTest();

    waitKey(0);
    return 0;
}



