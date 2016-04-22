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
int recursive_mkdir(char *dir);//�����༶Ŀ¼  
int ImageToVideo(char* outDir, char* videoName, char* inputDir, int startFrame, int endFrame, int imgW,
	int imgH, char* imgExt, double fps, int isColor, int fourcc);//ͼƬ����ת��Ϊ��Ƶ  
int  VideoToImage(char* videoName, char* outDir, char* imgExt, int maxFrameCount);//��Ƶת��ΪͼƬ����

void * hIndex = NULL;

//��ͼƬ����ת��Ϊ��Ƶ,������Ƶ֡��  
int ImageToVideo(char* outDir, char* videoName, char* inputDir, int startFrame, int endFrame, int imgW, int imgH, char* imgExt, double fps = 24, int isColor = 1, int fourcc = CV_FOURCC('X', 'V', 'I', 'D'))
{
	//�ж������ļ����Ƿ����  
	if (_access(inputDir, 0) == -1)
	{
		std::cout << "the input directory does not exist!" << std::endl;
		return 0;
	}
	//�ж�����ļ����Ƿ񴴽� ��û���򴴽�����ΪNULL��Ĭ�ϵ�ǰ����Ŀ¼  
	char fullVideoName[255];//�����Ƶ�������ļ�����·��+�ļ���  
	strcpy_s(fullVideoName, "");
	if (outDir == NULL)
	{
		sprintf_s(fullVideoName, "%s", videoName);//��videoName��ӡ��һ���ַ���������fullVideoName ��   
	}
	else
	{
		if (_access(outDir, 0) == -1)
		{
			recursive_mkdir(outDir);
		}
		sprintf_s(fullVideoName, "%s%s", outDir, videoName);//���ַ���outDir��videoName������������ӡ��������fullVideoName��  
	}
	int frameCount = 0;
	CvVideoWriter *pWriter = NULL;
	CvSize size = cvSize(imgW, imgH);
	pWriter = cvCreateVideoWriter(videoName, fourcc, fps, size, isColor);//CREATE WRITER  

	IplImage *pImg = NULL;
	char cur_fn[255];//��ʾĳ��ͼƬ��·��  
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
	rename(videoName, fullVideoName);//�ƶ��ļ���ָ���ļ���  
	return  frameCount;
}

//����Ƶת��ΪͼƬ���� ��������Ƶ�ֽ�õ���ͼƬ��֡�� ĿǰOpenCVֻ֧��AVI��ʽ ���ʹ��֮ǰ��Ҫ  
//����Ƶת����AVI��ʽ  

int  VideoToImage(char* videoName, char* outDir, char* imgExt, int maxFrameCount)
{
	CvCapture *cap = cvCaptureFromFile(videoName);
	if (cap == NULL)
	{
		return 0;
	}
	//����ͼƬ���ļ���·��һ��Ҫ�У���ΪOpenCV�����Զ������ļ���  
	if (_access(outDir, 0) == -1)
	{
		recursive_mkdir(outDir);
		std::cout << "the ouput directory does not exist, and the have been created autonomously!" << std::endl;
	}

	/*
	char cur_fn[255];//���浱ǰ֡����ͼƬ���ļ���

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
	sprintf_s(cur_fn, "%s%d%s%d%s", outDir, dexNum, "-", frame, imgExt);//����������ʺ����� 123.jpg 124.jpg��ͼƬ����
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
	sprintf_s(cur_fn, "%s%d%s%d%s", outDir, dexNum, "-", frame, imgExt);//����������ʺ����� 123.jpg 124.jpg��ͼƬ����
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
			sprintf_s(file, "%s%d%s%d%s", outDir, n, "-", frameNum, imgExt);//����������ʺ����� 123.jpg 124.jpg��ͼƬ����
            cvSaveImage(file, tempFrame, NULL);//����ͼ��ָ���ļ�
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
			sprintf_s(file, "%s%d%s%d%s", outDir, n, "-", frameNum, imgExt);//����������ʺ����� 123.jpg 124.jpg��ͼƬ����
            cvSaveImage(file, tempFrame, NULL);//����ͼ��ָ���ļ�
			cvCvtColor(tempFrame, previousFrame, CV_BGR2GRAY);//��ɫ�ռ�ת������������ʵ��RGB��ɫ��HSV,HSI����ɫ�ռ��ת����Ҳ����ת��Ϊ�Ҷ�ͼ��
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
	//cvShowImage("main", tempFrame);//��ʾͼ��
	//cvCvtColor(tempFrame, previousFrame, CV_BGR2GRAY);//��ɫ�ռ�ת������������ʵ��RGB��ɫ��HSV,HSI����ɫ�ռ��ת����Ҳ����ת��Ϊ�Ҷ�ͼ��
	//cvWaitKey(25); // �ȴ������¼�
	}
	//�ͷ�ͼ��
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
	};//����ģ��ı���

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
			sprintf_s(file, "%s%d%s%d%s", outDir, n, "-", frameNum, imgExt);//����������ʺ����� 123.jpg 124.jpg��ͼƬ���� 
			cvSaveImage(file, img, NULL);//����ͼ��ָ���ļ�
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
			dst_size.height = (int)((tpl->height)* templateScale[i]);//ģ��ͼ��ԭʼ��С
			dst_size.width = (int)((tpl->width)* templateScale[i]);
			IplImage* dst = cvCreateImage(dst_size, tpl->depth, tpl->nChannels);
			cvResize(tpl, dst, CV_INTER_LINEAR);

			//get image's properties 
			img_width = img->width;
			img_height = img->height;
			tpl_width = dst->width;
			tpl_height = dst->height; //ģ��ͼ��ԭʼ��С
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
			//cout << "��" << i << "��ƥ�䣬ƥ�����Ϊ" << templateScale[i] << "ģ���СΪ" << tpl_width << "+" << tpl_height << "ƥ��̶�Ϊ :" << temp_maxval << endl;

			if (temp_maxval >= compare_maxval){

				compare_maxval = temp_maxval;
				maxval = temp_maxval;
				maxloc = temp_maxloc;

			}


		}// end of for loop

		//cvNamedWindow("res", CV_WINDOW_AUTOSIZE);
		//cvShowImage("res", res);
		//////////////////////////  maxval ����ƥ��ָ���������ԣ�С��0.92һ��Ϊ��ƥ��

		if (maxval > 0.92){

			cout << "ƥ�䣡" << endl;
			CvPoint pt1;
			CvPoint pt2;
			CvRect rect;
			rect = cvRect(maxloc.x, maxloc.y, tpl->width, tpl->height);//��ѵ�ƥ������
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
			sprintf_s(file, "%s%d%s%d%s", outDir, n, "-", frameNum, imgExt);//����������ʺ����� 123.jpg 124.jpg��ͼƬ���� 
			cvSaveImage(file, img, NULL);//����ͼ��ָ���ļ�
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


//�ú���������������ϣ��Զ������༶Ŀ¼  
int recursive_mkdir(char *dir)
{
	//�ֽ�·����E:\\AA\\BB\\CC\\  
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
	//��ƵתͼƬ  
	/*char videoName1[100];
	cout << "Please input the videoo name you want to query(suffix with .avi):";
	cin >> videoName1;*/


	/*Mat picture = imread("1.jpg");//ͼƬ������ӵ�����Ŀ¼��
	//Ҳ���Ǻ�example.cpp�ļ�����һ���ļ����£�����
	imshow("���Գ���", picture);
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
	
	//����Ƶ�ļ�����ʵ���ǽ���һ��VideoCapture�ṹ
	VideoCapture capture("altare.avi");                                                    //
	//����Ƿ�������:�ɹ���ʱ��isOpened����ture
	if (!capture.isOpened())
		cout << "fail to open!" << endl;
	//��ȡ����֡��
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "������Ƶ��" << totalFrameNumber << "֡" << endl;

	//���ÿ�ʼ֡()
	long frameToStart = 0;
	capture.set(CV_CAP_PROP_POS_FRAMES, frameToStart);
	cout << "�ӵ�" << frameToStart << "֡��ʼ��" << endl;

	//���ý���֡
	int frameToStop = totalFrameNumber;

	if (frameToStop < frameToStart)
	{
		cout << "����֡С�ڿ�ʼ֡��������󣬼����˳���" << endl;
		exit(0);
	}
	else
	{
		cout << "����֡Ϊ����" << frameToStop << "֡" << endl;
	}

	//��ȡ֡��
	double rate = capture.get(CV_CAP_PROP_FPS);
	cout << "֡��Ϊ:" << rate << endl;

	//����һ���������ƶ�ȡ��Ƶѭ�������ı���
	bool stop = false;
	//����ÿһ֡��ͼ��
	Mat frame;
	//��ʾÿһ֡�Ĵ���
	namedWindow("Extracted frame");
	//��֡��ļ��ʱ��:
	int delay = 1000 / rate;

	//����whileѭ����ȡ֡
	//currentFrame����ѭ�����п��ƶ�ȡ��ָ����֡��ѭ�������ı���
	long currentFrame = frameToStart;

	//�˲����ĺ�
	//int kernel_size = 3;
	//Mat kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size*kernel_size);

	while (!stop)
	{
		//��ȡ��һ֡
		if (!capture.read(frame))
		{
			cout << "��ȡ��Ƶʧ��" << endl;
			break;
		}

		//������˲�����
		imshow("Extracted frame", frame);
		//filter2D(frame, frame, -1, kernel);

		//imshow("after filter", frame);
		cout << "���ڶ�ȡ��" << currentFrame << "֡" << endl;
		//waitKey(int delay=0)��delay �� 0ʱ����Զ�ȴ�����delay>0ʱ��ȴ�delay����
		//��ʱ�����ǰû�а�������ʱ������ֵΪ-1�����򷵻ذ���

		int c = waitKey(delay);
		//����ESC���ߵ���ָ���Ľ���֡���˳���ȡ��Ƶ
		if ((char)c == 27 || currentFrame > frameToStop)
		{
			stop = true;
		}
		//���°������ͣ���ڵ�ǰ֡���ȴ���һ�ΰ���
		if (c >= 0)
		{
			waitKey(0);
		}
		currentFrame++;

	}
	//�ر���Ƶ�ļ�
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

	//����ƥ��
	EV2641Result result;
	int nMatched = 1000;
	int x1[1000];
	int y1[1000];
	int x2[1000];
	int y2[1000];
	EV2641_MatchEx(&pCDVS1, &pCDVS2, &result, nMatched, x1, y1, x2, y2);
	printf("Matched pair number: %d\n", nMatched);
	printf("Match Result: %d, %.4f\n", result.id, result.similarity);*/

	//��������
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
	//����
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

	//����Ƶ�ļ�����ʵ���ǽ���һ��VideoCapture�ṹ
    CvCapture *capture1 = cvCreateFileCapture(videoName1);
	//����Ƿ�������:�ɹ���ʱ��isOpened����ture
//	if (!capture1.isOpened())
//		cout << "fail to open!" << endl;
	//��ȡ����֡��
    long totalFrameNumber1 = cvGetCaptureProperty(capture1, CV_CAP_PROP_FRAME_COUNT);
	cout << "������Ƶ��" << totalFrameNumber1 << "֡" << endl;

	//���ÿ�ʼ֡()
    long frameToStart1 = atoi(s.c_str());
//	capture1.set(CV_CAP_PROP_POS_FRAMES, frameToStart1);

    return 1.0 * frameToStart1 / totalFrameNumber1;       // return value

//	cout << "�ӵ�" << frameToStart1 << "֡��ʼ��" << endl;

//	//���ý���֡
//	int frameToStop1 = frameToStart1 + 100;

//	if (frameToStop1 < frameToStart1)
//	{
//		cout << "����֡С�ڿ�ʼ֡��������󣬼����˳���" << endl;
//		exit(0);
//	}
//	else
//	{
//		cout << "����֡Ϊ����" << frameToStop1 << "֡" << endl;
//	}

//	//��ȡ֡��
//	double rate1 = capture1.get(CV_CAP_PROP_FPS);
//	cout << "֡��Ϊ:" << rate1 << endl;

//	//����һ���������ƶ�ȡ��Ƶѭ�������ı���
//	bool stop1 = false;
//	//����ÿһ֡��ͼ��
//	Mat frame1;
//	//��ʾÿһ֡�Ĵ���
//	namedWindow("Extracted frame1");
//	//��֡��ļ��ʱ��:
//	int delay1 = (int)1000 / (int)rate1;

//	//����whileѭ����ȡ֡
//	//currentFrame����ѭ�����п��ƶ�ȡ��ָ����֡��ѭ�������ı���
//	long currentFrame1 = frameToStart1;

//	//�˲����ĺ�
//	//int kernel_size = 3;
//	//Mat kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size*kernel_size);

//	while (!stop1)
//	{
//		//��ȡ��һ֡
//		if (!capture1.read(frame1))
//		{
//			cout << "��ȡ��Ƶʧ��" << endl;
//			break;
//		}

//		//������˲�����
//		imshow("Extracted frame1", frame1);
//		//filter2D(frame, frame, -1, kernel);

//		//imshow("after filter", frame);
//		cout << "���ڶ�ȡ��" << currentFrame1 << "֡" << endl;
//		//waitKey(int delay=0)��delay �� 0ʱ����Զ�ȴ�����delay>0ʱ��ȴ�delay����
//		//��ʱ�����ǰû�а�������ʱ������ֵΪ-1�����򷵻ذ���

//		int c = waitKey(delay1);
//		//����ESC���ߵ���ָ���Ľ���֡���˳���ȡ��Ƶ
//		if ((char)c == 27 || currentFrame1 > frameToStop1)
//		{
//			stop1 = true;
//		}
//		//���°������ͣ���ڵ�ǰ֡���ȴ���һ�ΰ���
//		if (c >= 0)
//		{
//			waitKey(0);
//		}
//		currentFrame1++;

//	}
//	//�ر���Ƶ�ļ�
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
