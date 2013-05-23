//#ifdef _DEBUG

#include <iostream>


#include "webrtc_wrapper.h"

using namespace std;

LoganVoiceEngine *lve;
FILE *rfile = NULL;
FILE *wfile = NULL;

void PrintCodecs();//��ӡ��������Ϣ
void PrintDevices();//��ӡ�豸��Ϣ
void ChanTransTest();//��webrtc������
void RunMyNetwork();//�Լ��ӹܷ��ͽ��գ����߶�д�ļ�

int _stdcall read(void *buf,int len)
{
    /*���������Ļ���������Ǵӻ�������ȡ���ݣ�
    ÿ��len���ֽڣ����������û�����ݣ���ΪNULL��
    ���Ƿ���ֵ��Ҫreturn 0,����return len.
    */
    fread(buf,1, len, rfile);

    return len;
}
//�ɼ�������д��������ļ�
bool _stdcall write(const void *buf,int len)
{
    /*
    ��������磬����ֱ�ӷ��;Ϳ����ˣ�����Ļ����⴦��
    */
    fwrite(buf, 1, len, wfile);

    return true;
}

void PrintOptions()
{
    cout << "webrtc wrapper test" <<endl;
    cout << "1. ��ӡ��������Ϣ"<<endl;
    cout << "2. ��ӡ��Ƶ�豸��Ϣ"<<endl;
    cout << "3. �������"<<endl;
    cout << "4. �ļ�����(�����Լ�������)"<<endl;
    cout << "0. ����"<<endl;
    cout << "    ��ѡ�������Ŀ:"<<endl;
}

int main(int argc, char **argv)
{

    PrintOptions();
    int i = -1;
    cin >> i;
    while (i != 0)
    {
        switch(i)
        {
        case 1:PrintCodecs();break;
        case 2:PrintDevices();break;
        case 3:ChanTransTest();break;
        case 4:RunMyNetwork();break;
        default:break;
        }
        
        PrintOptions();
        cin >> i;
    }

    return 0;
}

void PrintCodecs()
{
    lve = LoganVoiceEngine::Create();
    LoganCodec cinst;
    for (int i = 0; i < lve->LoganVoe_GetNumsOfCodec(); i++) {
        lve->LoganVoe_GetSpecCodec(i, cinst);

        if (strncmp(cinst.plname, "ISAC", 4) == 0 && cinst.plfreq == 32000) {
            printf("%i. ISAC-swb pltype:%i plfreq:%i channels:%i\n", i, cinst.pltype,
                cinst.plfreq, cinst.channels);
        } else if (strncmp(cinst.plname, "ISAC", 4) == 0 && cinst.plfreq == 48000) {
            printf("%i. ISAC-fb pltype:%i plfreq:%i channels:%i\n", i, cinst.pltype,
                cinst.plfreq, cinst.channels);
        } else {
            printf("%i. %s pltype:%i plfreq:%i channels:%i\n", i, cinst.plname,
                cinst.pltype, cinst.plfreq, cinst.channels);
        }
    }

    cout << "���롰0������:";
    while(true)
    {
        int i;
        cin >>i;
        if (i == 0)
        {
            break;
        }
    }
    LoganVoiceEngine::Destroy(lve);
}

void ChanTransTest()
{
    lve = LoganVoiceEngine::Create();

    //PrintCodecs();
    int channelid = lve->LoganVoe_CreateChannel();

    //���ñ�����
    LoganCodec codec;
    lve->LoganVoe_GetSpecCodec(0, codec);
    lve->LoganVoe_SetSendCodec(channelid,codec);//���ñ�����
    cout << "���뱾�ض˿�:";
    int localport, remoteport;
    char strip[128] = {0};
    cin >>localport;
    cout <<"����Է�IP:";
    cin >> strip;
    cout <<"����Է��˿�:";
    cin>> remoteport;


    lve->LoganVoe_SetSendDestination(channelid, strip, remoteport);
    lve->LoganVoe_SetLocalReceiver(channelid, localport);

    lve->LoganVoe_SetEcStatus(true);//������������
    lve->LoganVoe_SetNsStatus(true);//������������

    lve->LoganVoe_StartPlayout(channelid);
    lve->LoganVoe_StartSend(channelid);
    lve->LoganVoe_StartReceive(channelid);

    cout << "���롰0������:";
    while(true)
    {
        int i;
        cin >>i;
        if (i == 0) //����0�ͽ�������
        {
            break;
        }
    }

    lve->LoganVoe_StopReceive(channelid);
    lve->LoganVoe_StopSend(channelid);
    lve->LoganVoe_StopPlayout(channelid);
    lve->LoganVoe_DeleteChannel(channelid);

    LoganVoiceEngine::Destroy(lve);
}

//һ����16000�����ʣ�16λ��������
void RunMyNetwork()
{   
    string strFileIn = "audio_short16.pcm";
    string strFileOut = "my_recorded.pcm";
    rfile = fopen((strFileIn).c_str(), "rb");
    wfile = fopen((strFileOut).c_str(),"wb");

    lve = LoganVoiceEngine::Create();
    int id = lve->LoganVoe_CreateChannel();
    lve->LoganVoe_StartPlayout(id);

    lve->LoganVoe_StartPlayingFileLocally(id,read);

    lve->LoganVoe_StartRecordingMicrophone(write);
    cout << "���롰0������:";
    while(true)
    {
        int i;
        cin >>i;
        if (i == 0) 
        {
            break;
        }
    }

    LoganVoiceEngine::Destroy(lve);
}

void PrintDevices()
{
    lve = LoganVoiceEngine::Create();
    char name[128] = {0};
    char guid[128] = {0};
    for (int i = 0; i < lve->LoganVoe_GetNumOfPlayoutDevices(); i++)
    {
        lve->LoganVoe_GetPlayoutDeviceName(i, name, guid);
        cout <<"Play Devices: "<<name<<endl;
    }

    for(int j = 0; j < lve->LoganVoe_GetNumOfRecordingDevices(); j++)
    {
        lve->LoganVoe_GetRecordingDeviceName(j, name, guid);
        cout <<"Record Devices:"<<name<<endl;
    }
    cout << "���롰0������:";
    while(true)
    {
        int i;
        cin >>i;
        if (i == 0) 
        {
            break;
        }
    }

    LoganVoiceEngine::Destroy(lve);
}


//#endif