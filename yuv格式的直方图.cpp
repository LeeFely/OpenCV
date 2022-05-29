//c++语言利用opencv画出yuv格式的直方图
#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define w 1920
#define h 1080


Mat cal_zhifangtu_8(Mat SrcImage)
{
	vector<Mat>rgb_channel;
	split(SrcImage,rgb_channel);
	int bins = 256;
	int histsize[] = { bins };
	float range[] = { 0,256 };
	const float* histRange = {range};
    Mat  b_Hist, g_Hist, r_Hist;
    //-------------计算各个通道的直方图--------------------------------------
    calcHist(&rgb_channel[0], 1, 0, Mat(), b_Hist, 1, histsize, &histRange, true, false); //B-通道
    calcHist(&rgb_channel[1], 1, 0, Mat(), g_Hist, 1, histsize, &histRange, true, false); //G-通道
    calcHist(&rgb_channel[2], 1, 0, Mat(), r_Hist, 1, histsize, &histRange, true, false); //R-通道
    //-------------设置直方图绘图参数----------------------------------------------------
    int hist_h = 360;
    int hist_w = bins * 3;
    int bin_w = cvRound((double)hist_w / bins);
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    //-------------将直方图归一化到[0,histImage.rows]  ------------------------------------------------------------------------------

    normalize(b_Hist, b_Hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());  //B-通道
    normalize(g_Hist, g_Hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());  //G-通道
    normalize(r_Hist, r_Hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());  //R-通道
    //--------------绘制直方图  ----------------------------------------------------------------
    for (int i = 1; i < bins; i++)
    {
        
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_Hist.at<float>(i - 1))), Point(bin_w * (i), hist_h - cvRound(b_Hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
       
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_Hist.at<float>(i - 1))), Point(bin_w * (i), hist_h - cvRound(g_Hist.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);
        
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_Hist.at<float>(i - 1))), Point(bin_w * (i), hist_h - cvRound(r_Hist.at<float>(i))), Scalar(0, 0, 255), 2, 8, 0);

    }
    return histImage;

}


Mat cal_zhifangtu_16(Mat m)
{
    //16->8
    Mat tmp;
    Mat dst8 = Mat::zeros(m.size(), CV_8U);
    normalize(m, tmp, 0, 255, NORM_MINMAX);
    convertScaleAbs(tmp, dst8);

    Mat SrcImage = tmp;
    vector<Mat>rgb_channel;
    split(SrcImage,rgb_channel);
    int bins = 256;
    int histsize[] = { bins };
    float range[] = { 0, 256 };
    const float* histRange = { range };
    Mat  b_Hist, g_Hist, r_Hist;
    //-------------【3】计算各个通道的直方图--------------------------------------
    calcHist(&rgb_channel[0], 1, 0, Mat(), b_Hist, 1, histsize, &histRange, true, false); //B-通道
    calcHist(&rgb_channel[1], 1, 0, Mat(), g_Hist, 1, histsize, &histRange, true, false); //G-通道
    calcHist(&rgb_channel[2], 1, 0, Mat(), r_Hist, 1, histsize, &histRange, true, false); //R-通道
    //-------------【4】设置直方图绘图参数----------------------------------------------------
    int hist_h = 360;
    int hist_w = bins * 3;
    int bin_w = cvRound((double)hist_w / bins);
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));
    //-------------【5】将直方图归一化到[0,histImage.rows]  ------------------------------------------------------------------------------

    normalize(b_Hist, b_Hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());  //B-通道
    normalize(g_Hist, g_Hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());  //G-通道
    normalize(r_Hist, r_Hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());  //R-通道
    //--------------【6】绘制直方图  ----------------------------------------------------------------
    for (int i = 1; i < bins; i++)
    {
        
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_Hist.at<float>(i - 1))), Point(bin_w * (i), hist_h - cvRound(b_Hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
        
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_Hist.at<float>(i - 1))), Point(bin_w * (i), hist_h - cvRound(g_Hist.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);
        
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_Hist.at<float>(i - 1))), Point(bin_w * (i), hist_h - cvRound(r_Hist.at<float>(i))), Scalar(0, 0, 255), 2, 8, 0);

    }
    return histImage;




}

int main()
{
  
    string yuv_video_path = "ParkScene_1920x1080_24_16bit_444.yuv";
  
    FILE* pFile = fopen(yuv_video_path.c_str(), "rb+");

    int bufLen = w * h * 6;
    
    unsigned char* YuvBuf = new unsigned char[bufLen];

 
    Mat my_yuv = Mat::zeros(h, w, CV_16UC3);

   
    vector<Mat> yuvChannels;

    
    split(my_yuv, yuvChannels);

   
    while (fread(YuvBuf, bufLen * sizeof(unsigned char), 1, pFile))
    {
        static int i = 1;
       
        yuvChannels.at(0).data = (unsigned char*)YuvBuf;//Y
        yuvChannels.at(1).data = (unsigned char*)YuvBuf + w * h * 2;//U
        yuvChannels.at(2).data = (unsigned char*)YuvBuf + w * h * 4;//V

        
        merge(yuvChannels, my_yuv);

        Mat my_bgr_16;

       
        cvtColor(my_yuv, my_bgr_16, COLOR_YUV2BGR);
       
        Mat my_bgr_8(my_bgr_16.cols, my_bgr_16.rows, CV_8UC3);//8位3通道 unsigned

        my_bgr_16.convertTo(my_bgr_8, CV_8UC3, 1 / 255.0);

        imshow("16", my_bgr_16);
        imshow("8", my_bgr_8);

        cout << "正在处理第" << i << "帧" << endl;
        i++;

        cout << "16的通道数：" << my_bgr_16.type() << endl;//18
        cout << "8的通道数：" << my_bgr_8.type() << endl;//16

        Mat zhifangtu_16 = cal_zhifangtu_16(my_bgr_16);
        Mat zhifangtu_8 = cal_zhifangtu_8(my_bgr_8);

        imshow("zhifangtu_16", zhifangtu_16);
        imshow("zhifangtu_8", zhifangtu_8);


        cv::waitKey(1000);
    }
    cin.get();
    delete[] YuvBuf;
    fclose(pFile);
}