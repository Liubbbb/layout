#ifndef PUBLICDEFINE_H
#define PUBLICDEFINE_H

#include <QString>
#include <QList>
#include <memory>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include "pgmessage.h"

typedef struct SurfaceInformation
{
    int index;
    int horizontal_position;
    int vertical_position;
    int width;
    int height;
    QString layoutName;

}StSurfaceInf;

typedef struct WindowsInformation{

    int index;
    int horizontal_position;
    int vertical_position;
    int width;
    int height;
    //下面三个为可选参数
    int horizontal_offset;
    int vertical_offset;
    int target_surface;

    QString content;
    QString layoutName;

    enum{

        EM_HORIZONTAL_OFFSET,
        EM_VERTICAL_OFFSET,
        EM_TERGET_SURFANCE,
    };

    QList<QString> strKey;
    bool state_bit[EM_TERGET_SURFANCE+1];
    int selectContent[3];//存储可选变量

    WindowsInformation()
    {

        strKey<<"horizontal_offset"<<"vertical_offset"<<"target_surface";

//        //设置状态位
        memset(state_bit,false,sizeof(state_bit));
        memset(selectContent,0,sizeof(selectContent));

    }

}StWindowInf;


//视频输出格式参数2.5.3
typedef struct _ST_VIDEO_FORMAT_ARGUMENTS_
{
    enum
    {
        //以下前五个参数为字符串类型
        EM_ASPECT_RATIO,
        EM_FPS,
        EM_HORIZ_POLARITY,
        EM_VERT_POLARITY,
        EM_QUANTIZATION_RANGE,
        EM_ASPECT_RATIO_MODE,

        //其它25个参数为整型
        EM_HSYNC_FRONT_PORCH,
        EM_VSYNC_FRONT_PORCH,
        EM_HDMI_VIC,
        EM_KEEP_WIDTH,
        EM_KEEP_HEIGHT,
        EM_HORIZ_OFFSET,
        EM_VERT_OFFSET,
        EM_HSYNC_PULSE,
        EM_VSYNC_PULSE,
        EM_WIDTH,
        EM_HEIGHT,
        EM_TOTAL_WIDTH,
        EM_TOTAL_HEIGHT,
        EM_VIC,
        EM_VIEWPORT_HORIZ,
        EM_VIEWPORT_VERT,
        EM_VIEWPORT_WIDTH,
        EM_VIEWPORT_HEIGHT,
        EM_GRID_WIDTH,
        EM_GRID_HEIGHT,
        EM_GRID_INDEX_HORIZ,
        EM_GRID_INDEX_VERT,
        EM_BEZEL_HORIZ,
        EM_BEZEL_VERT
    };

    QList<QString> strKey;
    bool blSetStatus[30];
    QString strVideoArguments[6];
    int nVideoArguments[24];

    _ST_VIDEO_FORMAT_ARGUMENTS_()
    {
        strKey<<"aspect_ratio"<<"fps"<<"horiz_polarity"<<"vert_polarity"<<"quantization_range"<<"aspect_ratio_mode"
             <<"hsync_front_porch"<<"vsync_front_porch"<<"hdmi_vic"<<"keep_width"<<"keep_height"<<"horiz_offset"
             <<"vert_offset"<<"hsync_pulse"<<"vsync_pulse"<<"width"<<"height"<<"total_width"<<"total_height"<<"vic"
             <<"viewport_horiz"<<"viewport_vert"<<"viewport_width"<<"viewport_height"<<"grid_width"<<"grid_height"
             <<"grid_index_horiz"<<"grid_index_vert"<<"bezel_horiz"<<"bezel_vert";
        //初始化
        for(int i=0;i<30;i++)
        {
            blSetStatus[i]=false;
            if(i<EM_HSYNC_FRONT_PORCH)
                strVideoArguments[i]="";
            else
               nVideoArguments[i-6]=0;
        }
    }

}StVideoFormatArguments;

typedef struct _ST_COMMAND_MULTIVIEW_
{
   //指定为单个设备或设备组
   EmDeviceGroups DeviceGroups;
   //只有给单个设备设置时需要给其赋值，默认为空
   QString strDeviceId;
   QString layout;
   StVideoFormatArguments stVideoFormatArguments;
   //设置状态位
   bool subscriptions_state = false;
   //关联的译码器个数
   int length_subscriptions;
   //键subscriptions需指定的参数
   QList<QString> source_device;
   QVector<int> stream_index;
   QVector<int> subscriptions_index;



}StMultiviewCommand;


typedef struct
{
    QString layout_name;
    int width;
    int height;
    bool read_only;
    QVector<StSurfaceInf> sfObj;
    QVector<StWindowInf> wdObj;

}StLayoutInf;





extern QNetworkAccessManager *manager;


#endif // PUBLICDEFINE_H
