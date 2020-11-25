//***************************************************************************//
//* 版权所有  www.mediapro.cc
//*
//* 内容摘要：windows音频采集、播放、3A一体类 DLL封装
//*	
//* 当前版本：V1.0		
//* 作    者：www.mediapro.cc
//* 完成日期：2017-11-05
//**************************************************************************//

#ifndef _SD_AUDIO_3A_CAP_RENDER_PROCESS_SDK_H_
#define _SD_AUDIO_3A_CAP_RENDER_PROCESS_SDK_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#ifndef BUILDING_DLL
#define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
#define DLLIMPORT __declspec (dllimport)
#endif /* Not BUILDING_DLL */
#else
#define DLLIMPORT
#endif

#ifdef __APPLE__
#ifndef OBJC_BOOL_DEFINED
typedef int BOOL;
#endif 
#else
#ifndef BOOL
typedef int BOOL;
#endif
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

//日志输出的级别
typedef enum CAP_RENDER_3A_LOG_OUTPUT_LEVEL
{
	CAP_RENDER_3A_LOG_LEVEL_DEBUG = 1,
	CAP_RENDER_3A_LOG_LEVEL_INFO,
	CAP_RENDER_3A_LOG_LEVEL_WARNING,
	CAP_RENDER_3A_LOG_LEVEL_ERROR,
	CAP_RENDER_3A_LOG_LEVEL_ALARM,
	CAP_RENDER_3A_LOG_LEVEL_FATAL,
	CAP_RENDER_3A_LOG_LEVEL_NONE
} CAP_RENDER_3A_LOG_OUTPUT_LEVEL;

//3A处理后的采集数据回调
typedef void (*Output3AProcessedCaptureData)(unsigned char *pucData, int nLen, void *pObject);



//////////////////////////////////////////////////////////////////////////
// 音频3A处理-采集-渲染封装接口

/***
* 创建SD3ACapRenderProcess对象
* @param: outputPath表示日志存放路径，支持相对路径和绝对路径，若目录不存在将自动创建
* @param: outputLevel表示日志输出的级别，只有等于或者高于该级别的日志输出到文件，取值范围参考3A_CAP_RENDER_LOG_OUTPUT_LEVEL
* @return: 返回模块指针，为NULL则失败
*/
DLLIMPORT void*  SD3ACapRenderProcess_New(const char* outputPath, int outputLevel);


/***
* 销毁SD3ACapRenderProcess，使用者应该做好与其他API之间的互斥保护
* @param pp3AProcess: 模块指针指针
* @return:
*/
DLLIMPORT void  SD3ACapRenderProcess_Delete(void** pp3AProcess);



/***
* 开始启动SD3ACapRenderProcess
* @param p3AProcess: 模块指针
* @param nCapDeviceID: 采集音频设备ID，-1为默认设备
* @param nSampleRate: 输入待处理数据采样率
* @param nChannelNum: 输入待处理数据声道数
* @param bEnableAec: 是否使能AEC
* @param bEnableAgc: 是否使能AGC
* @param bEnableAns: 是否使能ANS
* @param pfOutput3ACallback: 经过3A处理后的音频采集数据输出回调接口
* @param pObject: 上述输出回调接口的透传指针，将通过回调函数形参方式透传外层
* @return: TRUE成功，FALSE则失败
*/
DLLIMPORT BOOL  SD3ACapRenderProcess_Start(void* p3AProcess, int nCapDeviceID, int nSampleRate, int nChannelNum, BOOL bEnableAec, BOOL bEnableAgc, BOOL bEnableAns,
                                            Output3AProcessedCaptureData pfOutput3ACallback, void* pObject);



/***
* 停止SD3ACapRenderProcess
* @param p3AProcess: 模块指针
* @return:
*/
DLLIMPORT void  SD3ACapRenderProcess_Stop(void* p3AProcess);


/***
* 播放数据，要求采样率、声道数与创建时一致
* @param p3AProcess: 模块指针
* @param pucData: 待播放数据
* @param nLen: 数据大小
* @return: 
*/
DLLIMPORT int  SD3ACapRenderProcess_Play(void* p3AProcess, unsigned char *pucData, int nLen);



/***
* 启用AEC调试模式，此时将生成AEC处理前的Ref信号和Mic信号到指定的路径，便于观察二者延时差
* @param p3AProcess: 模块指针
* @param pcTempFileSaveDir: 调试文件存放路径
* @return: TRUE-使能成功， FALSE-使能失败
*/
DLLIMPORT BOOL  SD3ACapRenderProcess_EnableDebugMode(void* p3AProcess, const char *pcTempFileSaveDir);




#ifdef __cplusplus
}
#endif

#endif // _SD_AUDIO_3A_CAP_RENDER_PROCESS_SDK_H_
