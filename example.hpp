#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include <stdio.h>
#include <string>
#include "EV2641CDVSLib.h"
#include <opencv2\opencv.hpp>
#include "highgui.h"

#include <cv.h>  
//#include <highgui.h>  
#include <iostream>  
#include <direct.h>//for mk_dir  
#include <io.h>//for _acess()  
//#include <string>

using namespace cv;

using namespace std;

int dexNum = 0;
int recursive_mkdir(char *dir);//创建多级目录  
int ImageToVideo(char* outDir, char* videoName, char* inputDir, int startFrame, int endFrame, int imgW,
	int imgH, char* imgExt, double fps, int isColor, int fourcc);//图片序列转化为视频  
int  VideoToImage(char* videoName, char* outDir, char* imgExt, int maxFrameCount);//视频转化为图片序列

void * hIndex = NULL;

//将图片序列转换为视频,返回视频帧数  
int ImageToVideo(char* outDir, char* videoName, char* inputDir, int startFrame, int endFrame, int imgW, int imgH, char* imgExt, double fps = 24, int isColor = 1, int fourcc = CV_FOURCC('X', 'V', 'I', 'D'))
{
	//判断输入文件夹是否存在  
	if (_access(inputDir, 0) == -1)
	{
		std::cout << "the input directory does not exist!" << std::endl;
		return 0;
	}
	//判断输出文件夹是否创建 若没有则创建；若为NULL则默认当前工作目录  
	char fullVideoName[255];//输出视频的完整文件名：路径+文件名  
	strcpy_s(fullVideoName, "");
	if (outDir == NULL)
	{
		sprintf_s(fullVideoName, "%s", videoName);//把videoName打印成一个字符串保存在fullVideoName 中   
	}
	else
	{
		if (_access(outDir, 0) == -1)
		{
			recursive_mkdir(outDir);
		}
		sprintf_s(fullVideoName, "%s%s", outDir, videoName);//将字符串outDir和videoName连接起来，打印，保存在fullVideoName中  
	}
	int frameCount = 0;
	CvVideoWriter *pWriter = NULL;
	CvSize size = cvSize(imgW, imgH);
	pWriter = cvCreateVideoWriter(videoName, fourcc, fps, size, isColor);//CREATE WRITER  

	IplImage *pImg = NULL;
	char cur_fn[255];//表示某张图片的路径  
	while (startFrame <= endFrame)
	{
		strcpy_s(cur_fn, "");
		sprintf_s(cur_fn, "%s%d%s", inputDir, startFrame, imgExt);//need to change    
		pImg = cvLoadImage(cur_fn, isColor);
		if (!pImg)
		{
			std::cout << "can't open an image file" << std::endl;
			return frameCount;
		}
		cvWriteFrame(pWriter, pImg);
		cvWaitKey(1);
		std::cout << "Write frame " << startFrame << std::endl;
		startFrame++;
		cvReleaseImage(&pImg);
		frameCount++;
	}
	cvReleaseVideoWriter(&pWriter);
	rename(videoName, fullVideoName);//移动文件到指定文件夹  
	return  frameCount;
}

//将视频转换为图片序列 返回由视频分解得到的图片总帧数 目前OpenCV只支持AVI格式 因此使用之前需要  
//将视频转化问AVI格式  

int  VideoToImage(char* videoName, char* outDir, char* imgExt, int maxFrameCount)
{
	CvCapture *cap = cvCaptureFromFile(videoName);
	if (cap == NULL)
	{
		return 0;
	}
	//保存图片的文件夹路径一定要有，因为OpenCV不会自动创建文件夹  
	if (_access(outDir, 0) == -1)
	{
		recursive_mkdir(outDir);
		std::cout << "the ouput directory does not exist, and the have been created autonomously!" << std::endl;
	}

	/*
	char cur_fn[255];//保存当前帧所得图片的文件名

	IplImage* pImg = NULL;
	int frame = 0;
	cv::Mat matSrc, matSrc0, matSrc1, matSrc2;
	while ((pImg = cvQueryFrame(cap)) != NULL && (frame < maxFrameCount))
	{
	if (frame == 0)
	{
	matSrc = cv::cvarrToMat(pImg);
	CV_Assert(matSrc.channels() == 3);
	frame++;
	dexNum++;
	cout << "frame = " << frame << " , image = " << dexNum << endl;
	strcpy_s(cur_fn, "");
	sprintf_s(cur_fn, "%s%d%s%d%s", outDir, dexNum, "-", frame, imgExt);//这里的设置适合形如 123.jpg 124.jpg的图片序列
	cvSaveImage(cur_fn, pImg, NULL);
	EV2641Buffer tmp;
	char sframe[10];
	sprintf_s(sframe, "%d", frame);
	EV2641_GetFeatureByPath(cur_fn, &tmp, 13);
	EV2641_AddToIndex(hIndex, &tmp, sframe);
	EV2641_FreeBuffer(&tmp);
	continue;
	}
	else
	{
	matSrc0 = cv::cvarrToMat(pImg);
	}


	cv::resize(matSrc, matSrc1, cv::Size(357, 419), 0, 0, cv::INTER_NEAREST);
	//cv::flip(matSrc1, matSrc1, 1);
	cv::resize(matSrc0, matSrc2, cv::Size(2177, 3233), 0, 0, cv::INTER_LANCZOS4);

	cv::Mat matDst1, matDst2;

	cv::resize(matSrc1, matDst1, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);
	cv::resize(matSrc2, matDst2, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);

	cv::cvtColor(matDst1, matDst1, CV_BGR2GRAY);
	cv::cvtColor(matDst2, matDst2, CV_BGR2GRAY);

	int iAvg1 = 0, iAvg2 = 0;
	int arr1[64], arr2[64];

	for (int i = 0; i < 8; i++)
	{
	uchar* data1 = matDst1.ptr<uchar>(i);
	uchar* data2 = matDst2.ptr<uchar>(i);

	int tmp = i * 8;

	for (int j = 0; j < 8; j++)
	{
	int tmp1 = tmp + j;

	arr1[tmp1] = data1[j] / 4 * 4;
	arr2[tmp1] = data2[j] / 4 * 4;

	iAvg1 += arr1[tmp1];
	iAvg2 += arr2[tmp1];
	}
	}

	iAvg1 /= 64;
	iAvg2 /= 64;

	for (int i = 0; i < 64; i++)
	{
	arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
	arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
	}

	int iDiffNum = 0;

	for (int i = 0; i < 64; i++)
	if (arr1[i] != arr2[i])
	++iDiffNum;

	//cout << "iDiffNum = " << iDiffNum << endl;
	frame++;
	if (iDiffNum > 5)
	{
	matSrc = matSrc0;
	//int imgWithStep = pImg->widthStep;
	dexNum++;
	cout << "frame = " << frame << " , image = " << dexNum << endl;
	strcpy_s(cur_fn, "");
	sprintf_s(cur_fn, "%s%d%s%d%s", outDir, dexNum, "-", frame, imgExt);//这里的设置适合形如 123.jpg 124.jpg的图片序列
	cvSaveImage(cur_fn, pImg, NULL);
	EV2641Buffer tmp;
	char sframe[10];
	sprintf_s(sframe, "%d", frame);
	EV2641_GetFeatureByPath(cur_fn, &tmp, 13);
	EV2641_AddToIndex(hIndex, &tmp, sframe);
	EV2641_FreeBuffer(&tmp);

	}
	}
	pImg = NULL;
	cvReleaseCapture(&cap);
	return dexNum;
	*/

	IplImage* tempFrame = NULL;
	IplImage* currentFrame = NULL;
	IplImage* previousFrame = NULL;

	CvMat* tempFrameMat = NULL;
	CvMat* currentFrameMat = NULL;
	CvMat* previousFrameMat = NULL;

	int frameNum = 0;
	int n = 0;
	double sum = 0;
	char file[200];
	CvScalar count;
	while ((tempFrame = cvQueryFrame(cap)) != NULL && (frameNum < maxFrameCount))
	{
		if (!((tempFrame = cvQueryFrame(cap)) != NULL && (frameNum < maxFrameCount)))
			break;
		sum = 0;
		frameNum++;
		frameNum++;
		if (frameNum == 2)
		{
			previousFrame = cvCreateImage(cvSize(tempFrame->width, tempFrame->height), IPL_DEPTH_8U, 1);
			currentFrame = cvCreateImage(cvSize(tempFrame->width, tempFrame->height), IPL_DEPTH_8U, 1);
			currentFrameMat = cvCreateMat(tempFrame->height, tempFrame->width, CV_32FC1);
			previousFrameMat = cvCreateMat(tempFrame->height, tempFrame->width, CV_32FC1);
			tempFrameMat = cvCreateMat(tempFrame->height, tempFrame->width, CV_32FC1);
			cvCvtColor(tempFrame, previousFrame, CV_BGR2GRAY);
			n++;
			cout << "frame = " << frameNum << ", image = " << n << endl;
			strcpy_s(file, "");
			sprintf_s(file, "%s%d%s%d%s", outDir, n, "-", frameNum, imgExt);//这里的设置适合形如 123.jpg 124.jpg的图片序列
            cvSaveImage(file, tempFrame, NULL);//保存图像到指定文件
            EV2641Buffer tmp;
            char sframe[10];
            sprintf_s(sframe, "%d", frameNum);
            EV2641_GetFeatureByPath(file, &tmp, 13);
            EV2641_AddToIndex(hIndex, &tmp, sframe);
            EV2641_FreeBuffer(&tmp);
		}
		if (frameNum > 2)
		{
			cvCvtColor(tempFrame, currentFrame, CV_BGR2GRAY);
			cvConvert(currentFrame, currentFrameMat);
			cvConvert(previousFrame, previousFrameMat);
			cvAbsDiff(currentFrameMat, previousFrameMat, tempFrameMat);

            int div = 2;
            for (int i = 0; i < tempFrame->height / div; i++)
            {
                for (int j = 0; j < tempFrame->width / div; j++)
                {
                    count = cvGet2D(tempFrameMat, i*2, j*2);
                    sum = sum + count.val[0];
                }
            }
        sum = sum / (tempFrame->height / div * tempFrame->width / div);
		//cout << sum << endl;
		if (sum > 30)
		{
			n++;
			cout << "frame = " << frameNum << ", image = " << n << endl;
			strcpy_s(file, "");
			sprintf_s(file, "%s%d%s%d%s", outDir, n, "-", frameNum, imgExt);//这里的设置适合形如 123.jpg 124.jpg的图片序列
            cvSaveImage(file, tempFrame, NULL);//保存图像到指定文件
			cvCvtColor(tempFrame, previousFrame, CV_BGR2GRAY);//颜色空间转换函数，可以实现RGB颜色向HSV,HSI等颜色空间的转换，也可以转换为灰度图像
            EV2641Buffer tmp;
            char sframe[10];
            sprintf_s(sframe, "%d", frameNum);
            EV2641_GetFeatureByPath(file, &tmp, 13);
            EV2641_AddToIndex(hIndex, &tmp, sframe);
            EV2641_FreeBuffer(&tmp);
		}
		//(currentFrame, currentFrame);
		//cvErode(currentFrame, currentFrame);
	}
	//cvShowImage("main", tempFrame);//显示图像
	//cvCvtColor(tempFrame, previousFrame, CV_BGR2GRAY);//颜色空间转换函数，可以实现RGB颜色向HSV,HSI等颜色空间的转换，也可以转换为灰度图像
	//cvWaitKey(25); // 等待按键事件
	}
	//释放图像
	tempFrame = NULL;
	currentFrame = NULL;
	previousFrame = NULL;

	tempFrameMat = NULL;
	currentFrameMat = NULL;
	previousFrameMat = NULL;
	//cvReleaseImage(&tempFrame);
	//cvReleaseImage(&previousFrame);
	//cvReleaseMat(&previousFrameMat);
	cvReleaseCapture(&cap);
	return n;
	/*
	IplImage    *img = NULL;
	IplImage    *tpl = NULL;
	//IplImage    *res = NULL;
	IplImage    *temp_img = NULL, *temp_tpl = NULL, *temp_res = NULL;
	CvPoint     temp_minloc, temp_maxloc;
	double      temp_minval = 0;
	double      temp_maxval = 0;

	double      compare_minval = 0;
	double      compare_maxval = 0;

	CvPoint     minloc, maxloc;
	double      minval, maxval;
	int         img_width, img_height;
	int         tpl_width, tpl_height;
	int         res_width, res_height;
	double      templateScale[20] = {
		0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0
	};//缩放模板的比例

	//img = cvLoadImage(filename1, CV_LOAD_IMAGE_COLOR);
	//tpl = cvLoadImage(filename2, CV_LOAD_IMAGE_COLOR);
	int frameNum = 0;
	int n = 0;
	char file[50];
	while ((img = cvQueryFrame(cap)) != NULL && (frameNum < maxFrameCount))
	{
		temp_minval = 0;
		temp_maxval = 0;

		compare_minval = 0;
		compare_maxval = 0;
		frameNum++;
		if (frameNum == 1)
		{
			n++;
			cout << "frame = " << frameNum << ", image = " << n << endl;
			strcpy_s(file, "");
			sprintf_s(file, "%s%d%s%d%s", outDir, n, "-", frameNum, imgExt);//这里的设置适合形如 123.jpg 124.jpg的图片序列 
			cvSaveImage(file, img, NULL);//保存图像到指定文件
			EV2641Buffer tmp;
			char sframe[10];
			sprintf_s(sframe, "%d", frameNum);
			EV2641_GetFeatureByPath(file, &tmp, 13);
			EV2641_AddToIndex(hIndex, &tmp, sframe);
			EV2641_FreeBuffer(&tmp);
			tpl = img;
			continue;
		}


		// cvNamedWindow( "src", CV_WINDOW_AUTOSIZE ); 
		// cvShowImage( "src", img ); 
		//cvNamedWindow("template", CV_WINDOW_AUTOSIZE);
		//cvShowImage("template", tpl);


		for (int i = 0; i < 20; i++){

			CvSize dst_size;
			dst_size.height = (int)((tpl->height)* templateScale[i]);//模板图的原始大小
			dst_size.width = (int)((tpl->width)* templateScale[i]);
			IplImage* dst = cvCreateImage(dst_size, tpl->depth, tpl->nChannels);
			cvResize(tpl, dst, CV_INTER_LINEAR);

			//get image's properties 
			img_width = img->width;
			img_height = img->height;
			tpl_width = dst->width;
			tpl_height = dst->height; //模板图的原始大小
			res_width = abs(img_width - tpl_width) + 1;
			res_height = abs(img_height - tpl_height) + 1;

			//create new image for template matching computation 
			IplImage *res = cvCreateImage(cvSize(res_width, res_height), IPL_DEPTH_32F, 1);


			//choose template matching method to be used 
			cvMatchTemplate(img, dst, res, CV_TM_CCORR_NORMED);
			//cvMatchTemplate( img, tpl, res, CV_TM_SQDIFF_NORMED );
			//cvMatchTemplate( img, tpl, res, CV_TM_CCORR );
			//cvMatchTemplate( img, tpl, res, CV_TM_SQDIFF );
			//cvMatchTemplate( img, tpl, res, CV_TM_CCOEFF );
			//cvMatchTemplate( img, tpl, res, CV_TM_CCOEFF_NORMED );

			//cvMinMaxLoc( res, &minval, &maxval, &minloc, &maxloc, 0 );
			cvMinMaxLoc(res, &temp_minval, &temp_maxval, &temp_minloc, &temp_maxloc, 0);
			//cout << "第" << i << "次匹配，匹配比例为" << templateScale[i] << "模板大小为" << tpl_width << "+" << tpl_height << "匹配程度为 :" << temp_maxval << endl;

			if (temp_maxval >= compare_maxval){

				compare_maxval = temp_maxval;
				maxval = temp_maxval;
				maxloc = temp_maxloc;

			}


		}// end of for loop

		//cvNamedWindow("res", CV_WINDOW_AUTOSIZE);
		//cvShowImage("res", res);
		//////////////////////////  maxval 是相匹配指数，经测试，小于0.92一般为不匹配

		if (maxval > 0.92){

			cout << "匹配！" << endl;
			CvPoint pt1;
			CvPoint pt2;
			CvRect rect;
			rect = cvRect(maxloc.x, maxloc.y, tpl->width, tpl->height);//最佳的匹配区域
			pt1 = cvPoint(rect.x, rect.y);
			pt2 = cvPoint(rect.x + rect.width, rect.y + rect.height);
			cvRectangle(img, pt1, pt2, cvScalar(0, 0, 255), 1, 8, 0);

			//display images 
			//cvNamedWindow("reference", CV_WINDOW_AUTOSIZE);
			//cvShowImage("reference", img);


			//wait until user press a key to exit
			//cvWaitKey(0);

			//free memory 
			
			cvDestroyWindow("reference");
			cvDestroyWindow("template");
			cvReleaseImage(&img);
			cvReleaseImage(&tpl);
			cvReleaseImage(&res);
		

		}


		else{

			n++;
			cout << "frame = " << frameNum << ", image = " << n << endl;
			strcpy_s(file, "");
			sprintf_s(file, "%s%d%s%d%s", outDir, n, "-", frameNum, imgExt);//这里的设置适合形如 123.jpg 124.jpg的图片序列 
			cvSaveImage(file, img, NULL);//保存图像到指定文件
			EV2641Buffer tmp;
			tpl = img;
			char sframe[10];
			sprintf_s(sframe, "%d", frameNum);
			EV2641_GetFeatureByPath(file, &tmp, 13);
			EV2641_AddToIndex(hIndex, &tmp, sframe);
			EV2641_FreeBuffer(&tmp);

		}
	}
	cvReleaseCapture(&cap);
	return n;*/
}


//该函数借鉴了网上资料，自动创建多级目录  
int recursive_mkdir(char *dir)
{
	//分解路径名E:\\AA\\BB\\CC\\  
	//  
	std::string str = dir;
	int index = 0;
	int i = 0;
	while (1)
	{
		std::string::size_type pos = str.find("\\", index);
		std::string str1;
		str1 = str.substr(0, pos);
		if (pos != -1 && i > 0)
		{
			if (_access(str1.c_str(), 0) == -1)
			{
				_mkdir(str1.c_str());
			}
		}
		if (pos == -1)
		{
			break;
		}
		i++;
		index = (int)pos + 1;
	}
	return 0;
}

double Test(const char *videoPath, const char *imagePath)
{
	//视频转图片  
	/*char videoName1[100];
	cout << "Please input the videoo name you want to query(suffix with .avi):";
	cin >> videoName1;*/


	/*Mat picture = imread("1.jpg");//图片必须添加到工程目录下
	//也就是和example.cpp文件放在一个文件夹下！！！
	imshow("测试程序", picture);
	waitKey(20150901);*/

	/*cvNamedWindow("example");
	CvCapture * capture = cvCreateFileCapture("3.avi");
	IplImage * frame;
	while (1)
	{
		frame = cvQueryFrame(capture);
		if (!frame)break;
		cvShowImage("example", frame);
		char c = cvWaitKey(33);
		if (c == 27) break;
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("example");*/


	/*
	
	//打开视频文件：其实就是建立一个VideoCapture结构
	VideoCapture capture("altare.avi");                                                    //
	//检测是否正常打开:成功打开时，isOpened返回ture
	if (!capture.isOpened())
		cout << "fail to open!" << endl;
	//获取整个帧数
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "整个视频共" << totalFrameNumber << "帧" << endl;

	//设置开始帧()
	long frameToStart = 0;
	capture.set(CV_CAP_PROP_POS_FRAMES, frameToStart);
	cout << "从第" << frameToStart << "帧开始读" << endl;

	//设置结束帧
	int frameToStop = totalFrameNumber;

	if (frameToStop < frameToStart)
	{
		cout << "结束帧小于开始帧，程序错误，即将退出！" << endl;
		exit(0);
	}
	else
	{
		cout << "结束帧为：第" << frameToStop << "帧" << endl;
	}

	//获取帧率
	double rate = capture.get(CV_CAP_PROP_FPS);
	cout << "帧率为:" << rate << endl;

	//定义一个用来控制读取视频循环结束的变量
	bool stop = false;
	//承载每一帧的图像
	Mat frame;
	//显示每一帧的窗口
	namedWindow("Extracted frame");
	//两帧间的间隔时间:
	int delay = 1000 / rate;

	//利用while循环读取帧
	//currentFrame是在循环体中控制读取到指定的帧后循环结束的变量
	long currentFrame = frameToStart;

	//滤波器的核
	//int kernel_size = 3;
	//Mat kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size*kernel_size);

	while (!stop)
	{
		//读取下一帧
		if (!capture.read(frame))
		{
			cout << "读取视频失败" << endl;
			break;
		}

		//这里加滤波程序
		imshow("Extracted frame", frame);
		//filter2D(frame, frame, -1, kernel);

		//imshow("after filter", frame);
		cout << "正在读取第" << currentFrame << "帧" << endl;
		//waitKey(int delay=0)当delay ≤ 0时会永远等待；当delay>0时会等待delay毫秒
		//当时间结束前没有按键按下时，返回值为-1；否则返回按键

		int c = waitKey(delay);
		//按下ESC或者到达指定的结束帧后退出读取视频
		if ((char)c == 27 || currentFrame > frameToStop)
		{
			stop = true;
		}
		//按下按键后会停留在当前帧，等待下一次按键
		if (c >= 0)
		{
			waitKey(0);
		}
		currentFrame++;

	}
	//关闭视频文件
	capture.release();
	destroyWindow("Extracted frame");
	*/

	//waitKey(0);
	
	EV2641_InitCDVSSDK();
	hIndex = EV2641_CreateIndex();

    int len = strlen(videoPath) + 10;
    char *videoName1 = new char[len];
    strcpy(videoName1, videoPath);
//    char* videoName1 = "E:\\QTproject\\HelloWorld\\video\\altare.avi";                                              //
    char* outDir1 = new char[100];
    string tmpStr("E:\\QTproject\\HelloWorld\\pic\\");
    strcpy(outDir1, tmpStr.c_str());
    char* jpg = new char[10];
    tmpStr = ".jpg";
    strcpy(jpg, tmpStr.c_str());
    int images = VideoToImage(videoName1, outDir1, jpg, 1000);       //

	cout << "total " << images << " frames have been extracted from video." << endl;

	EV2641Buffer pCDVS1;
	/*cout << "Please input the picture name you want to query(suffix with .jpg):";
	char picName1[100];
	cin >> picName1;
	EV2641_GetFeatureByPath(picName1, &pCDVS1, 10);*/
    EV2641_GetFeatureByPath(imagePath, &pCDVS1, 13);                                                          //
	/*
	EV2641Buffer pCDVS2;
	EV2641_GetFeatureByPath("pic\\521.jpg", &pCDVS2, 10);

	//测试匹配
	EV2641Result result;
	int nMatched = 1000;
	int x1[1000];
	int y1[1000];
	int x2[1000];
	int y2[1000];
	EV2641_MatchEx(&pCDVS1, &pCDVS2, &result, nMatched, x1, y1, x2, y2);
	printf("Matched pair number: %d\n", nMatched);
	printf("Match Result: %d, %.4f\n", result.id, result.similarity);*/

	//建立索引
	//void * hIndex = NULL;
	/*
	if(hIndex)
	{
		EV2641Buffer tmp;
		while (dex < images)
		{
			sprintf_s(dexto, "%d", dex);
			strcpy_s(dextosuffix,dexto);
			strcat_s(dextosuffix,".jpg");
			char dextosuffix1[20] = "pic\\";
			strcat_s(dextosuffix1, dextosuffix);
			EV2641_GetFeatureByPath(dextosuffix1, &tmp, 13);
			EV2641_AddToIndex(hIndex, &tmp, dexto);
			EV2641_FreeBuffer(&tmp);

			dex += 20;
		}
		printf("%d image added to index\n", EV2641_GetIndexImgNum(hIndex));
	}
	*/
	printf("%d image added to index\n", EV2641_GetIndexImgNum(hIndex));
	//检索
	int max_num = 10;
	EV2641Result res[10];

	//EV2641Buffer pCDVS3;
	//EV2641_GetFeatureByPath("1.jpg", &pCDVS3, 13);

    EV2641_Retrieval(&pCDVS1, hIndex, res, &max_num, 0);
	printf("return num: %d\n", max_num);
	string s;
	for(int i = 0; i < max_num; i++)
	{
		printf("%.3f: %s\n", res[i].similarity, res[i].record.data);
		if (i == 0)
			s = res[i].record.data;
		EV2641_FreeBuffer(&res[i].record);
	}
	cout << s;
	EV2641_FreeBuffer(&pCDVS1);
	//EV2641_FreeBuffer(&pCDVS2);
	//EV2641_FreeBuffer(&pCDVS3);
	//EV2641_FreeBuffer(&result.record);

	EV2641_UninitCDVSSDK();


//	system("pause");

	//打开视频文件：其实就是建立一个VideoCapture结构
    CvCapture *capture1 = cvCreateFileCapture(videoName1);
	//检测是否正常打开:成功打开时，isOpened返回ture
//	if (!capture1.isOpened())
//		cout << "fail to open!" << endl;
	//获取整个帧数
    long totalFrameNumber1 = cvGetCaptureProperty(capture1, CV_CAP_PROP_FRAME_COUNT);
	cout << "整个视频共" << totalFrameNumber1 << "帧" << endl;

	//设置开始帧()
    long frameToStart1 = atoi(s.c_str());
//	capture1.set(CV_CAP_PROP_POS_FRAMES, frameToStart1);

    return 1.0 * frameToStart1 / totalFrameNumber1;       // return value

//	cout << "从第" << frameToStart1 << "帧开始读" << endl;

//	//设置结束帧
//	int frameToStop1 = frameToStart1 + 100;

//	if (frameToStop1 < frameToStart1)
//	{
//		cout << "结束帧小于开始帧，程序错误，即将退出！" << endl;
//		exit(0);
//	}
//	else
//	{
//		cout << "结束帧为：第" << frameToStop1 << "帧" << endl;
//	}

//	//获取帧率
//	double rate1 = capture1.get(CV_CAP_PROP_FPS);
//	cout << "帧率为:" << rate1 << endl;

//	//定义一个用来控制读取视频循环结束的变量
//	bool stop1 = false;
//	//承载每一帧的图像
//	Mat frame1;
//	//显示每一帧的窗口
//	namedWindow("Extracted frame1");
//	//两帧间的间隔时间:
//	int delay1 = (int)1000 / (int)rate1;

//	//利用while循环读取帧
//	//currentFrame是在循环体中控制读取到指定的帧后循环结束的变量
//	long currentFrame1 = frameToStart1;

//	//滤波器的核
//	//int kernel_size = 3;
//	//Mat kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size*kernel_size);

//	while (!stop1)
//	{
//		//读取下一帧
//		if (!capture1.read(frame1))
//		{
//			cout << "读取视频失败" << endl;
//			break;
//		}

//		//这里加滤波程序
//		imshow("Extracted frame1", frame1);
//		//filter2D(frame, frame, -1, kernel);

//		//imshow("after filter", frame);
//		cout << "正在读取第" << currentFrame1 << "帧" << endl;
//		//waitKey(int delay=0)当delay ≤ 0时会永远等待；当delay>0时会等待delay毫秒
//		//当时间结束前没有按键按下时，返回值为-1；否则返回按键

//		int c = waitKey(delay1);
//		//按下ESC或者到达指定的结束帧后退出读取视频
//		if ((char)c == 27 || currentFrame1 > frameToStop1)
//		{
//			stop1 = true;
//		}
//		//按下按键后会停留在当前帧，等待下一次按键
//		if (c >= 0)
//		{
//			waitKey(0);
//		}
//		currentFrame1++;

//	}
//	//关闭视频文件
//	capture1.release();
//	destroyWindow("Extracted frame1");
//	//waitKey(0);
}

//int main()
//{
//	Test();
//	system("pause");
//	return 0;
//}

#endif
