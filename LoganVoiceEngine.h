#ifndef __LOGAN_VOICE_ENGINE_H__
#define __LOGAN_VOICE_ENGINE_H__

#define LOGAN_API_EXPORTS

#ifdef LOGAN_API_EXPORTS
#define LOGAN_API  __declspec(dllexport)
#else
#define LOGAN_API __declspec(dllimport)
#endif


// Each codec supported can be described by this structure.
struct LoganCodec
{
    int pltype;
    char plname[32];
    int plfreq;
    int pacsize;
    int channels;
    int rate;
};

//�������ȡ����
typedef int  (__stdcall *LoganReadStreamCallBack)(void *buf,int len);
//�ɼ�������д������
typedef bool (__stdcall *LoganWriteStreamCallBack)(const void *buf,int len);

class LOGAN_API LoganVoiceEngine
{
protected:
    ~LoganVoiceEngine(){}

public:

    //���к���֮ǰʹ��
    static LoganVoiceEngine *Create();

    //���к���������ʹ��
    static void Destroy(LoganVoiceEngine* lve);

    //����һ��ͨ��ID��������������(Ŀǰֻ��һ��),���������������й�
    virtual int LoganVoe_CreateChannel() = 0;

    //������ɾ�����ͨ��������
    virtual int LoganVoe_DeleteChannel(int channel) = 0;

    //������������(��ʱδ��)
    virtual int LoganVoe_SetSendDestination(int channelId, char* ip, int port) = 0;
    virtual int LoganVoe_SetLocalReceiver(int channelId, int port) = 0;

    /************************************************************************/
    /* ���ö�ȡ�������ݺʹ���ɼ����ݵĻص�����                     */
    /************************************************************************/
    virtual int LoganVoe_SetCallBackFuncs(LoganReadStreamCallBack lrscb,
        LoganWriteStreamCallBack lwscb) = 0;

    /************************************************************************/
    /* ������(��ʱδ��)                                                */
    /************************************************************************/
    //��ȡ�����������Ŀ
    virtual int LoganVoe_GetNumsOfCodec() = 0;
    //��ȡָ�������ı������
    virtual int LoganVoe_GetSpecCodec(int index, LoganCodec &lc) = 0;
    //����ָ��ͨ���ı�����
    virtual int LoganVoe_SetSendCodec(int channelid, LoganCodec lc) = 0;


    /************************************************************************/
    /* �豸����                                                           */
    /************************************************************************/
    //��ȡ¼���豸����Ŀ
    virtual int LoganVoe_GetNumOfRecordingDevices() = 0;
    //��ȡ�����豸����Ŀ
    virtual int LoganVoe_GetNumOfPlayoutDevices() = 0;
    //��ȡָ�������Ĳ����豸������
    virtual int LoganVoe_GetPlayoutDeviceName(int index, 
        char nameUTF8[128], char guidUTF8[128]) = 0;
    //��ȡָ��������¼���豸������
    virtual int LoganVoe_GetRecordingDeviceName(int index, 
       char nameUTF8[128], char guidUTF8[128]) = 0;
    //���ò����豸
    virtual int LoganVoe_SetPlayoutDevice(int idx) = 0;
    //����¼���豸
    virtual int LoganVoe_SetRecordingDevice(int idx) = 0;


    /************************************************************************/
    /*������������                                                                  */
    /************************************************************************/
    /*
    **vad: mode 
    **0: low
    **1:
    **2:
    **3: most high
    */
    virtual int LoganVoe_SetVADStatus(int channelsid, bool b, int mode = 1) = 0;
    /*
    **agc: mode 
    **0: previously set mode
    **1: platform default
    **2: adaptive mode for use when analog volume control exists (e.g. for PC softphone)
    **3: scaling takes place in the digital domain (e.g. for conference servers and embedded devices)
    **4: can be used on embedded devices where the capture signal level is predictable
    */
    virtual int LoganVoe_SetAgcStatus(bool b, int mode = 1) = 0;

    /*
    **EC mode
    **0: previously set mode
    **1: platform default
    **2: conferencing default (aggressive AEC)
    **3: Acoustic Echo Cancellation
    **4: AEC mobile
    */
    virtual int LoganVoe_SetEcStatus(bool b, int mode = 3) = 0;

    /*
    **NS mode
    **0: previously set mode
    **1: platform default
    **2: conferencing default
    **3: lowest suppression
    **4: Moderate Suppression
    **5: High Suppression
    **6: highest suppression
    */
    virtual int LoganVoe_SetNsStatus(bool b, int mode = 4) = 0;

    virtual int LoganVoe_GetVADStatus(int channelsid, bool &b, int &mode) = 0;
    virtual int LoganVoe_GetAgcStatus(bool &b, int &mode) = 0;
    virtual int LoganVoe_GetEcStatus(bool &b, int &mode) = 0;
    virtual int LoganVoe_GetNsStatus(bool &b, int &mode) = 0;

    /*
    ** Play And Record control
    */
    virtual int LoganVoe_StartReceive(int channelid) = 0;
    virtual int LoganVoe_StartPlayout(int channelid) = 0;
    virtual int LoganVoe_StartSend(int channelid) = 0;
    virtual int LoganVoe_StopReceive(int channelid) = 0;
    virtual int LoganVoe_StopPlayout(int channelid) = 0;
    virtual int LoganVoe_StopSend(int channelid) = 0;
    virtual int LoganVoe_StartRecordingMicrophone() = 0;
    virtual int LoganVoe_StartPlayingFileLocally(int channelid) = 0;
    virtual int LoganVoe_StopPlayingFileLocally(int channelid) = 0;
    virtual int LoganVoe_StopRecordingMicrophone() = 0;
   
    /*
    ** Volume control
    */
    //Valid range is [0,255]
    virtual unsigned int LoganVoe_GetMicVolume() = 0;
    virtual unsigned int LoganVoe_GetSpeakerVolume() = 0;
    virtual int LoganVoe_SetMicVolume(unsigned int vol) = 0;
    virtual int LoganVoe_SetSpeakerVolume(unsigned int vol) = 0;
    virtual int LoganVoe_SetInputMute(int channel, bool enable) = 0;
    virtual int LoganVoe_GetInputMute(int channel, bool& enabled) = 0;
    // Mutes the speaker device completely in the operating system.
    virtual int LoganVoe_SetSystemOutputMute(bool enable) = 0;
    // Gets the output device mute state in the operating system.
    virtual int LoganVoe_GetSystemOutputMute(bool &enabled) = 0;
    // Mutes the microphone device completely in the operating system.
    virtual int LoganVoe_SetSystemInputMute(bool enable) = 0;
    // Gets the mute state of the input device in the operating system.
    virtual int LoganVoe_GetSystemInputMute(bool& enabled) = 0;


};
#endif //__LOGAN_VOICE_ENGINE_H__