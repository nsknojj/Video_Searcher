// *****************************************************************
//  EV2641CDVSLib.h  视觉搜索SDK   Version:  1.0    Date: 2015.01.15
//  -------------------------------------------------------------
// *****************************************************************
//	版本历史:
//		2015.01.15: v1.0, 视觉搜索SDK的接口
// *****************************************************************
// 调用流程
// 【一、远程在线搜索】
//	1. 初始化EV2641_InitCDVSSDK
//	2.    从本地摄像头或文件夹获取图像，转成EV2641Image格式
//	3.    调用EV2641_GetFeature获取图像特征
//	4.    调用EV2641_Retrieval_Remote从服务端检索得到检索结果
//	5.    调用EV2641_FreeFeature释放提取的特征资源
//	6. 调用EV2641_UninitCDVSSDK退出程序释放内存资源

// 【二、本地离线搜索】
//	1. 初始化EV2641_InitCDVSSDK
//	2. 根据需要调用EV2641_SyncIndex从网络同步存储为本地索引，需要依赖网络
//	3. 调用EV2641_LoadIndex加载索引到内存
//	4.    从本地摄像头或文件夹获取图像，转成EV2641Image格式
//	5.    调用EV2641_GetFeature获取图像特征
//	6.    调用EV2641_Retrieval从本地检索得到检索结果
//	7.    调用EV2641_FreeFeature释放提取的特征资源
//	8. 调用EV2641_FreeIndex释放索引库内存资源
//	9. 调用EV2641_UninitCDVSSDK退出程序释放内存资源

#ifndef _CDVS_LOCAL_SDK_H_
#define _CDVS_LOCAL_SDK_H_
#include<stdlib.h>

#define CDVS_SI_EXPORTS
//#define CDVS_SI_API

#if defined(_WIN32) || defined(_WIN64)
#ifdef CDVS_SI_EXPORTS
#define CDVS_SI_API __declspec(dllexport)
#else
#define CDVS_SI_API __declspec(dllimport)
#endif
#else
#define CDVS_SI_API
typedef long long __int64;
#endif//*/

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#ifdef __cplusplus
extern "C"{
#endif

// 错误码
#define		EV2641_ERR_SUCCESS							0		//操作成功
#define		EV2641_ERR_GENERIC							1		//一般错误，没有专门的意思
#define		EV2641_ERR_PARA								-1		//参数文件加载失败
#define		EV2641_ERR_IMAGE							-2		//图像EV2641Image未加载
#define		EV2641_ERR_MODE								-3		//不支持的模式，仅支持7-13模式
#define		EV2641_ERR_FEATURE							-4		//CDVS特征未提取
#define		EV2641_ERR_CONNECT							-5		//网络连接失败
#define		EV2641_ERR_SEND								-6		//网络发送错误
#define		EV2641_ERR_RECV								-7		//网络接收错误
#define		EV2641_ERR_AUTOR							-8		//授权使用失败
#define		EV2641_ERR_INDEX							-9		//未加载索引
#define		EV2641_ERR_UNSUPPORT						-10		//函数功能不支持
#define		EV2641_ERR_DB_MODE							-11		//不是DB模式
#define		EV2641_ERR_FEATURE2							-12		//未检测到特征

//图像类型
#define		EV2641_IMGTYPE_GRAY		1				//灰度格式
#define		EV2641_IMGTYPE_RGB		2				//RGB格式
#define		EV2641_IMGTYPE_YUV		3				//YUV格式
#define		EV2641_IMGTYPE_BITMAP	4				//Android的Bitmap格式
#define		EV2641_IMGTYPE_UIIMAGE	5				//iOS的UImage格式

//矩形定义
typedef struct {
	int x;
	int y;
	int w;
	int h;
}EV2641Rect;

//缓冲结构体
typedef struct {
	char* data;
	size_t size;
}EV2641Buffer;

//图像结构体
typedef struct
{
	unsigned char * imagedata;			//图像矩阵指针
	int width;							//图像宽度
	int height;							//图像高度
	int widthStep;						//每行像素的字节数
	int type;							//图像类型
	char useROI;						//感兴趣区域表示
	EV2641Rect mROI;					//感兴趣区域
}EV2641Image;

//检索、匹配返回结构体
typedef struct
{
	__int64 id;							//图像关联数据库的ID
	float similarity;					//与查询图像的相似度
	float similarity2;					//与查询图像的相似度2
	EV2641Buffer record;
}EV2641Result;

//网络地址（IP+端口）存储结构体
typedef struct {
	int _reserved[7];
}EV2641NetAddr;
//----------------全局初始化、释放----------------

/************************************************************************/
/* EV2641_InitCDVSSDK: 初始化视觉搜索SDK，在使用该SDK的功能前			*/
/*      需要在调用其他任何SDK函数前调用，必需且仅需调用一次该函数		*/
/*		参数:															*/
/*			nThreadNum[in]: 多线程参数，默认为1，						*/
/*			                当该值大于1时表示使用的线程数目				*/
/*			                注意：设置的线程数目必须大于等于实际线程数	*/
/*			param_path[in]: 本地参数文件路径,为NULL则采用默认参数		*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_InitCDVSSDK(int nThreadNum = 1, const char * param_path = NULL);

/************************************************************************/
/* EV2641_UninitCDVSSDK: 释放视觉搜索SDK，在使用该SDK的功能后			*/
/*      必需且仅需调用一次该函数，以便释放内存							*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_UninitCDVSSDK();

/************************************************************************/
/* EV2641_FreeBuffer: 释放分配的内存									*/
/*		参数:															*/
/*			pBuf[in]:			缓存空间								*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API void EV2641_FreeBuffer(EV2641Buffer* pBuf);


//------------------网络地址转换------------------

CDVS_SI_API int EV2641_GetNetAddr(const char* url_or_ip, int port, EV2641NetAddr* pAddr);



//----------------特征提取----------------

/************************************************************************/
/* EV2641_GetFeature: 从给定的图像提取压缩特征							*/
/*		参数:															*/
/*			image[in]:			输入图像								*/
/*			pBufCDVS[out]:		输出压缩特征							*/
/*			mode[in]:			特征模式，取值1,2,7-13					*/
/*								其中13表示服务端特征,1表示未压缩特征	*/
/*								2表示定长特征							*/
/*			feature_num[out]:	提取到的特征点的数目					*/
/*			pBufCDVS2[out]:		当mode为13时候，额外提取mode为10特征	*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_GetFeature(const EV2641Image * image, EV2641Buffer* pBufCDVS, int mode, int * feature_num = NULL, EV2641Buffer* pBufCDVS2 = NULL, float * feature_time = NULL);

/************************************************************************/
/* EV2641_GetFeature0: 从给定的未压缩提取压缩特征						*/
/*		参数:															*/
/*			pBufOri[in]:		输入未压缩特征							*/
/*			pBufCDVS[out]:		输出未压缩的特征						*/
/*			mode[in]:			特征模式，取值7-13，其中13表示服务端特征*/
/*			pBufCDVS2[out]:		当mode为13时候，额外提取mode为10特征	*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_GetFeature0(const EV2641Buffer* pBuf, EV2641Buffer* pBufCDVS, int mode, EV2641Buffer* pBufCDVS2 = NULL);

/************************************************************************/
/* EV2641_GetFeatureByPath: 从给定的图像路径提取特征					*/
/*		参数:															*/
/*			image_path[in]:		输入图像的全局路径，只支持JPG格式		*/
/*			pBufCDVS[out]:		输出特征								*/
/*			mode[in]:			特征模式，取值1,7-13					*/
/*								其中13表示服务端特征,1表示未压缩特征	*/
/*		返回值: int(errorcode)											*/
/*      注意：仅支持.jpg格式的图像										*/
/************************************************************************/
CDVS_SI_API int EV2641_GetFeatureByPath(const char * image_path, EV2641Buffer* pBufCDVS, int mode);


//----------------索引库----------------

/************************************************************************/
/* EV2641_LoadIndex: 从本地索引文件中加载索引到内存						*/
/*		参数:															*/
/*			index_file[in]:		本地索引文件路径						*/
/*		返回值: 成功返回索引库的句柄。失败返回NULL						*/
/************************************************************************/
CDVS_SI_API void * EV2641_LoadIndex(const char * index_file);

/************************************************************************/
/* EV2641_AddToIndex: 将特征添加到索引中								*/
/*		参数:															*/
/*			hIndex[in]:			索引库句柄								*/
/*			pBufCDVS[in]:		特征									*/
/*			param[in]:			附加对特征的描述						*/
/*			param[inout]:		附加参数								*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_AddToIndex(void * hIndex, const EV2641Buffer* pBufCDVS , const char * param, int * param2 = NULL);

/************************************************************************/
/* EV2641_MergeIndex: 将第二个索引库合并到第一个						*/
/*		参数:															*/
/*			hIndex[in]:			索引库句柄								*/
/*			hIndex_tb_merged[in]:需要被合并的索引库句柄					*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_MergeIndex(void * hIndex, const void* hIndex_tb_merged);

/************************************************************************/
/* EV2641_CreateIndex: 创建索引											*/
/*		参数:															*/
/*			无															*/
/*		返回值: 成功返回索引库的句柄。失败返回NULL						*/
/************************************************************************/
CDVS_SI_API void * EV2641_CreateIndex();

/************************************************************************/
/* EV2641_SaveIndex: 将索引保存到指定文件								*/
/*		参数:															*/
/*			hIndex[in]:			索引库句柄								*/
/*			index_file[in]:		索引文件存储路							*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_SaveIndex(void * hIndex, char * index_file);

/************************************************************************/
/* EV2641_GetIndexImgNum: 获取索引库中图片总数							*/
/*		参数:															*/
/*			hIndex[in]:			索引库句柄								*/
/*		返回值: int索引库中图片总数,返回值小于0为错误码					*/
/************************************************************************/
CDVS_SI_API int EV2641_GetIndexImgNum(void * hIndex);

/************************************************************************/
/* EV2641_FreeIndex: 关闭索引库库，释放资								*/
/*		每个打开的索引库最后都调用该函									*/
/*		参数:															*/
/*			hIndex[in]:			索引库句柄								*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_FreeIndex(void * hIndex);



//----------------匹配与检索----------------

/************************************************************************/
/* EV2641_Match: 用EV2641_GetFeature得到的两个特征之间进行匹配			*/
/*		参数:															*/
/*			pBufCDVS1[in]:		特征1									*/
/*			pBufCDVS2[in]:		特征2									*/
/*			result[out]:		返回的匹配结果							*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_Match(const EV2641Buffer* pBufCDVS1, const EV2641Buffer* pBufCDVS2, EV2641Result* result);

/************************************************************************/
/* EV2641_Match2: 用EV2641_GetFeature得到的两个特征之间进行匹配			*/
/*				  根据第二幅图像中的物体在第一幅图像求对应边界			*/
/*		参数:															*/
/*			pBufCDVS1[in]:		特征1									*/
/*			pBufCDVS2[in]:		特征2									*/
/*			box1[out]:			第一幅图像的物体边界					*/
/*			box2[in]:			第二幅图像的物体边界					*/
/*			threshold[in]:		图像匹配的阈值，默认为15				*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_Match2(const EV2641Buffer* pBufCDVS1, const EV2641Buffer* pBufCDVS2, float * box1, float * box2, int threshold = 15);


/************************************************************************/
/* EV2641_MatchScore: EV2641_GetFeature得到的两个mode为2特征之间进行匹配*/
/*		参数:															*/
/*			pBufCDVS1[in]:		特征1									*/
/*			pBufCDVS2[in]:		特征2									*/
/*			type[in]:			匹配类型：1表示全局，2表示局部			*/
/*			score[out]:			返回特征相似度							*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_MatchScore(const EV2641Buffer* pBufCDVS1, const EV2641Buffer* pBufCDVS2, char type, float &score);

/************************************************************************/
/* EV2641_MatchEx: 用EV2641_GetFeature得到的两个特征之间进行匹配		*/
/*		参数:															*/
/*			pBufCDVS1[in]:		特征1									*/
/*			pBufCDVS2[in]:		特征2									*/
/*			result[out]:		返回的匹配结果							*/
/*			nMatched[out]:		匹配的特征对数							*/
/*			x1[out]:			图像1的坐标x							*/
/*			y1[out]:			图像1的坐标y							*/
/*			x2[out]:			图像2的坐标x							*/
/*			y2[out]:			图像2的坐标y							*/
/*			threshold[in]:		图像匹配的阈值，默认为15				*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_MatchEx(const EV2641Buffer* pBufCDVS1, const EV2641Buffer* pBufCDVS2, EV2641Result* result, int & nMatched, int * x1, int * y1, int * x2, int * y2, int threadhold = 15);

/************************************************************************/
/* EV2641_Retrieval: 用EV2641_GetFeature得到的特征在索引库中检索		*/
/*		参数:															*/
/*			pBufCDVS[in]:		特征流									*/
/*			hIndex[in]:			索引库句柄								*/
/*			resultArr[out]:		返回的检索结果							*/
/*			max_ret_num[inout]: 输入最多返回结果数，输出返回的检实际个数*/
/*			isReg[in]:			是否做识别，默认为1表示是				*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_Retrieval(const EV2641Buffer* pBufCDVS,		
							 void * hIndex,	
							 EV2641Result * resultArr, int * max_ret_num,
							 int isReg = 1);

/************************************************************************/
/* EV2641_RetrievalEx: 用EV2641_GetFeature得到的特征在索引库中检索		*/
/*		参数:															*/
/*			pBufCDVS[in]:		特征流									*/
/*			hIndex[in]:			索引库句柄								*/
/*			resultArr[out]:		返回的检索结果							*/
/*			max_ret_num[inout]: 输入最多返回结果数，输出返回的检实际个数*/
/*			isReg[in]:			是否做识别，默认为1表示是				*/
/*			retrieval_time[out]:检索时间								*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_RetrievalEx(const EV2641Buffer* pBufCDVS,		
									void * hIndex,	
									EV2641Result * resultArr, int * max_ret_num,
									int isReg = 1,
									float * retrieval_time = NULL);


//----------------远程操作----------------

/************************************************************************/
/* EV2641_Retrieval_Remote: 用EV2641_GetFeature得到的特征从远程服务端	*/
/*		参数:															*/
/*			ip[in]:				远程服务端ip							*/
/*			port[in]:			远程服务端端							*/
/*			db_uid[in]:			服务端提供的应用唯一ID					*/
/*			db_seckey[in]:		服务端提供的应用授权码					*/
/*			EV2641Buffer[in]:	特征流									*/
/*			hIndex[in]:			索引库句柄								*/
/*			resultArr[out]:		返回的检索结果							*/
/*			max_ret_num[inout]: 输入最多返回结果数，输出返回的检实际个数*/
/*			req_param[in]:		检索输入参数							*/
/*			whereStmt[in]:		查询条件								*/
/*			isReg[in]:			是否做识别								*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_RetrievalRemote(const EV2641NetAddr* pAddr, 
									const char* db_uid, const char* db_seckey, 
									const EV2641Buffer* pBufCDVS,
									const char* req_param,const char* whereStmt,
									EV2641Result* resultArr, int* max_ret_num, 
									int isReg = 1);

/************************************************************************/
/* EV2641_SyncIndex: 从远程服务端同步索引文件							*/
/*		参数:															*/
/*			ip[in]:				远程服务端ip							*/
/*			port[in]:			远程服务端端							*/
/*			db_uid[in]:			服务端提供的应用唯一ID					*/
/*			db_seckey[in]:		服务端提供的应用授权码					*/
/*			sync_parameter[in]:	同步参数								*/
/*			index_file[in]:		索引文件同步保存全局路径				*/
/*		返回值: int(errorcode)											*/
/************************************************************************/
CDVS_SI_API int EV2641_SyncIndex(const EV2641NetAddr* pAddr,
							 const char* db_uid, const char* db_seckey, 
							 const char* sync_parameter, const char* index_file);


#ifdef __cplusplus
}
#endif

#endif	//_CDVS_LOCAL_SDK_H_