//***************************************************************************//
//* 版权所有  www.mediapro.cc
//*
//* 内容摘要：windows音频采集、播放、3A一体类 DLL封装
//*	
//* 当前版本：V1.0		
//* 作    者：mediapro
//* 完成日期：2017-11-05
//**************************************************************************//

#ifndef _SD_AUDIO_3A_CAP_RENDER_PROCESS_SDK_H_
#define _SD_AUDIO_3A_CAP_RENDER_PROCESS_SDK_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__
#ifdef DLL_EXPORTS
#ifdef __GNUC__
#define DLLIMPORT_CAP_RENDER_3A __attribute__ ((dllexport))
#else
#define DLLIMPORT_CAP_RENDER_3A __declspec(dllexport) 
#endif
#else
#ifdef __GNUC__
#define DLLIMPORT_CAP_RENDER_3A 
#else
#define DLLIMPORT_CAP_RENDER_3A 
#endif
#endif
#else
#if __GNUC__ >= 4
#define DLLIMPORT_CAP_RENDER_3A __attribute__ ((visibility ("default")))
#else
#define DLLIMPORT_CAP_RENDER_3A
#endif
#endif

#ifdef WIN32
	typedef signed __int64       int64_t;
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


//枚举音频采集或渲染设备的最大数量
#define SD_MAX_SUPPORT_AUDIO_DEVICES_NUM			8
//枚举音频采集或渲染设备名称的最大长度																															
#define SD_MAX_SUPPORT_AUDIO_DEVICES_NAME_LEN		512

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
typedef void (*Output3AProcessedCaptureData)(unsigned char *pucData, int nLen, BOOL bInVoiceStatus, void *pObject);



////////////////////////////////////////////基础接口////////////////////////////////////////////////////
// 音频3A处理-采集-渲染封装接口

/***
* 枚举当前系统可用的音频采集设备(获取设备ID和设备名称)
* @param: pnDeviceIds 设备ID存放列表，系统默认音频采集设备其ID为-1
* @param: pDeviceNames 设备名称存放列表
* @param: nMaxDeviceNum 最大枚举的设备数量，需小于等于SD_MAX_SUPPORT_AUDIO_DEVICES_NUM
* @param: nMaxDeviceNameLen 最大设备名长度，必须等于SD_MAX_SUPPORT_AUDIO_DEVICES_NAME_LEN
* @return: 返回设备数量
*/
DLLIMPORT_CAP_RENDER_3A int SD3ACapRender_GetAudioInputDeviceList(int *pnDeviceIds, char(*pDeviceNames)[SD_MAX_SUPPORT_AUDIO_DEVICES_NAME_LEN], int nMaxDeviceNum, int nMaxDeviceNameLen);

/***
* 枚举当前系统可用的音频渲染设备(获取设备ID和设备名称)
* @param: pnDeviceIds 设备ID存放列表，系统默认音频渲染设备其ID为-1
* @param: pDeviceNames 设备名称存放列表
* @param: nMaxDeviceNum 最大枚举的设备数量，需小于等于SD_MAX_SUPPORT_AUDIO_DEVICES_NUM
* @param: nMaxDeviceNameLen 最大设备名长度，必须等于SD_MAX_SUPPORT_AUDIO_DEVICES_NAME_LEN
* @return: 返回设备数量
*/
DLLIMPORT_CAP_RENDER_3A int SD3ACapRender_GetAudioOutputDeviceList(int *pnDeviceIds, char(*pDeviceNames)[SD_MAX_SUPPORT_AUDIO_DEVICES_NAME_LEN], int nMaxDeviceNum, int nMaxDeviceNameLen);


/***
* 环境初始化，系统只需调用一次，主要用于日志模块的初始化
* @param: outputPath表示日志存放路径，支持相对路径和绝对路径，若目录不存在将自动创建
* @param: outputLevel表示日志输出的级别，只有等于或者高于该级别的日志输出到文件，取值范围参考3A_CAP_RENDER_LOG_OUTPUT_LEVEL
* @return:
*/
DLLIMPORT_CAP_RENDER_3A void  SD3ACapRender_Enviroment_Init(const char* outputPath, int outputLevel);

DLLIMPORT_CAP_RENDER_3A void  SD3ACapRender_Enviroment_Free();


/***
* 创建SD3ACapRenderProcess对象
* @return: 返回模块指针，为NULL则失败
*/
DLLIMPORT_CAP_RENDER_3A void*  SD3ACapRender_New();


/***
* 销毁SD3ACapRenderProcess，使用者应该做好与其他API之间的互斥保护
* @param pp3AProcess: 模块指针指针
* @return:
*/
DLLIMPORT_CAP_RENDER_3A void  SD3ACapRender_Delete(void** pp3AProcess);



/***
* 开始启动SD3ACapRenderProcess
* @param p3AProcess: 模块指针
* @param nCapDeviceID: 采集音频设备ID，-1为默认设备
* @param nRenderDeviceID: 音频渲染设备ID，-1为默认设备
* @param nSampleRate: 输入待处理数据采样率
* @param nChannelNum: 输入待处理数据声道数
* @param bEnableAec: 是否使能AEC
* @param bEnableAgc: 是否使能AGC，多方互动时建议关闭AGC获得更好的AEC效果
* @param bEnableAns: 是否使能ANS
* @param bEnableVad: 是否使能VAD
* @param pfOutput3ACallback: 经过3A处理后的音频采集数据输出回调接口
* @param pObject: 上述输出回调接口的透传指针，将通过回调函数形参方式透传外层
* @return: TRUE成功，FALSE失败
*/
DLLIMPORT_CAP_RENDER_3A BOOL  SD3ACapRender_Start(void* p3AProcess, int nCapDeviceID, int nRenderDeviceID, int nSampleRate, int nChannelNum, BOOL bEnableAec, BOOL bEnableAgc, BOOL bEnableAns, BOOL bEnableVad,
                                            Output3AProcessedCaptureData pfOutput3ACallback, void* pObject);



/***
* 停止SD3ACapRenderProcess
* @param p3AProcess: 模块指针
* @return:
*/
DLLIMPORT_CAP_RENDER_3A void  SD3ACapRender_Stop(void* p3AProcess);


/***
* 播放数据，要求采样率、声道数与创建时一致
* @param p3AProcess: 模块指针
* @param pucData: 待播放数据
* @param nLen: 数据大小
* @return: 实际输出数据大小
*/
DLLIMPORT_CAP_RENDER_3A int  SD3ACapRender_Play(void* p3AProcess, unsigned char *pucData, int nLen);



////////////////////////////////////////////高级接口////////////////////////////////////////////////////

/***
* 切换音频采集设备
* @param p3AProcess: 模块指针
* @param nCapDeviceId: 需要切换到采集设备ID
* @return: TRUE成功，FALSE失败
*/
DLLIMPORT_CAP_RENDER_3A BOOL  SD3ACapRender_ChangeCapDev(void* p3AProcess, int nCapDeviceId);

/***
* 切换音频渲染设备
* @param p3AProcess: 模块指针
* @param nCapDeviceId: 需要切换到渲染设备ID
* @return: TRUE成功，FALSE失败
*/
DLLIMPORT_CAP_RENDER_3A BOOL  SD3ACapRender_ChangeRenderDev(void* p3AProcess, int nRenderDeviceId);


/***
* 启用AEC调试模式，此时将生成AEC处理前的Ref信号和Mic信号到指定的路径，便于观察二者延时差
* @param p3AProcess: 模块指针
* @param pcTempFileSaveDir: 调试文件存放路径
* @return: TRUE成功，FALSE失败
*/
DLLIMPORT_CAP_RENDER_3A BOOL  SD3ACapRender_EnableDebugMode(void* p3AProcess, const char *pcTempFileSaveDir);


/***
* 设置AGC参数，若需要调用本API，请于Start接口之前调用。未调用本API时将使用WEBRTC默认值
* @param p3AProcess: 模块指针
* @param nCompressionGaindB: 见WEBRTC定义，默认值9
* @param nTargetLevelDbfs: 见WEBRTC定义，默认值3
* @return:
*/
DLLIMPORT_CAP_RENDER_3A void  SD3ACapRender_ConfigAgc(void* p3AProcess, short nCompressionGaindB, short nTargetLevelDbfs);



/***
* 设置ANS参数，若需要调用本API，请于Start接口之前调用。未调用本API时将使用WEBRTC默认值
* @param p3AProcess: 模块指针
* @param nMode: 噪声消除强度， 0: Mild, 1: Medium , 2: Aggressive，默认值1
* @return:
*/
DLLIMPORT_CAP_RENDER_3A void  SD3ACapRender_ConfigAns(void* p3AProcess, short nMode);



/***
* 设置VAD参数，若需要调用本API，请于Start接口之前调用。未调用本API时将使用WEBRTC默认值
* @param p3AProcess: 模块指针
* @param nMode: 四种模式，用数字0~3来区分，数字越大越不敏感，默认值3
* @return:
*/
DLLIMPORT_CAP_RENDER_3A void  SD3ACapRender_ConfigVad(void* p3AProcess, short nMode);



#ifdef __cplusplus
}
#endif

#endif // _SD_AUDIO_3A_CAP_RENDER_PROCESS_SDK_H_
