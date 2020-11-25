# audio capture render and 3A (AGC\AEC\ANS\VAD) process with simple api and single lib


## Audio3ACapRenderSdk简介
极简的音频采集、渲染、3A(AGC AEC ANS)一体处理封装库：
音频3A处理（AEC、AGC、ANS）是实时互动领域无法绕开的问题。其中AEC处理与音频的采集、渲染紧密相关，三者的合理耦合对AEC效果影响很大，这也对开发者提出了更高的要求。为了简化上层业务的工作量，我们将音频采集、渲染、3A处理封装成一体库，对外提供非常易用的接口。该库具备以下特点：<br>
* 1、AEC尽量往底层靠，不受网络传输过程中的抖动、丢包等因素影响。
* 2、基于Webrtc AEC（非AECM、非AEC3）
* 3、支持8KHZ、16KHZ、32KHZ、44.1KHZ、48KHZ，支持单声道、双声道。
* 4、支持AEC自动延时估计。
* 5、仅5个API，仅一个DLL组成，占用空间小，无第三方依赖，集成简易。
* 6、C++开发，支持C、C++、C#，性能强劲

####单独3A处理库（不集成采集、渲染）的见：https://github.com/waterfoxfox/Audio3AProcess

## Audio3ACapRenderSdk C API

### 
* 创建SD3ACapRenderProcess对象<br>
@param: outputPath：日志文件输出的目录，若目录不存在将自动创建<br>
@param: outputLevel：日志输出的级别，只有等于或者高于该级别的日志输出到文件<br>
@return: 返回模块指针，为NULL则失败<br>
void*  `SD3ACapRenderProcess_New`(const char* outputPath, int outputLevel);;


### 
* 销毁SD3ACapRenderProcess对象<br>
@param pp3AProcess: 模块指针指针<br>
@return:<br>
void  `SD3ACapRenderProcess_Delete`(void** pp3AProcess);


### 
* 开始启动SD3ACapRenderProcess处理，即开始采集和3A处理，并支持渲染播放<br>
@param p3AProcess: 模块指针<br>
@param nCapDeviceID: 采集音频设备ID，-1为默认设备<br>
@param nSampleRate: 输入待处理数据采样率，比如32000、44100<br>
@param nChannelNum: 输入待处理数据声道数，比如2、1<br>
@param bEnableAec: 是否使能AEC<br>
@param bEnableAgc: 是否使能AGC<br>
@param bEnableAns: 是否使能ANS<br>
@param pfOutput3ACallback: 经过3A处理后的音频采集数据输出回调接口<br>
@param pObject: 上述输出回调接口的透传指针<br>
@return: TURE成功，FALSE失败<br>
BOOL  `SD3ACapRenderProcess_Start`(void* p3AProcess, int nCapDeviceID, int nSampleRate, int nChannelNum, BOOL bEnableAec, BOOL bEnableAgc, BOOL bEnableAns, Output3AProcessedCaptureData pfOutput3ACallback, void* pObject);

### 
* 停止SD3ACapRenderProcess处理<br>
@param p3AProcess: 模块指针<br>
@return: <br>
void  `SD3ACapRenderProcess_Stop`(void* p3AProcess);


### 
* 播放数据，要求采样率、声道数与创建时一致<br>
@param p3AProcess: 模块指针<br>
@param pucData: 待播放数据<br>
@param nLen: 待播放数据大小<br>
@return: 实际播放数据大小<br>
int  `SD3ACapRenderProcess_Play`(void* p3AProcess, unsigned char *pucData, int nLen);



### 本库仅做演示用途，若需要商业用途与技术支持请联系 www.mediapro.cc
