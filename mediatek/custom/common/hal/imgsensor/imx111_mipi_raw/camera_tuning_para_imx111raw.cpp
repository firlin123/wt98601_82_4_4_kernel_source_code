#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_imx111raw.h"
#include "camera_info_imx111raw.h"
#include "camera_custom_AEPlinetable.h"
#include "camera_custom_tsf_tbl.h"
const NVRAM_CAMERA_ISP_PARAM_STRUCT CAMERA_ISP_DEFAULT_VALUE =
{{
    //Version
    Version: NVRAM_CAMERA_PARA_FILE_VERSION,
    //SensorId
    SensorId: SENSOR_ID,
    ISPComm:{
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    },
    ISPPca:{
        #include INCLUDE_FILENAME_ISP_PCA_PARAM
    },
    ISPRegs:{
        #include INCLUDE_FILENAME_ISP_REGS_PARAM
        },
    ISPMfbMixer:{{
        {//00: MFB mixer for ISO 100
            0x00000000, 0x00000000
        },
        {//01: MFB mixer for ISO 200
            0x00000000, 0x00000000
        },
        {//02: MFB mixer for ISO 400
            0x00000000, 0x00000000
        },
        {//03: MFB mixer for ISO 800
            0x00000000, 0x00000000
        },
        {//04: MFB mixer for ISO 1600
            0x00000000, 0x00000000
        },
        {//05: MFB mixer for ISO 2400
            0x00000000, 0x00000000
        },
        {//06: MFB mixer for ISO 3200
            0x00000000, 0x00000000
        }
    }},
    ISPCcmPoly22:{
        71275,    // i4R_AVG
        16046,    // i4R_STD
        102075,    // i4B_AVG
        24099,    // i4B_STD
        {  // i4P00[9]
            5312500, -2762500, 7500, -905000, 4047500, -577500, -80000, -2250000, 4890000
        },
        {  // i4P10[9]
            2333686, -2685379, 350559, -205446, -273217, 473894, 34653, 314530, -330362
        },
        {  // i4P01[9]
            1841234, -2168741, 338976, -363640, -387740, 753649, -33228, -324909, 381626
        },
        {  // i4P20[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P11[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P02[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    }
}};

const NVRAM_CAMERA_3A_STRUCT CAMERA_3A_NVRAM_DEFAULT_VALUE =
{
    NVRAM_CAMERA_3A_FILE_VERSION, // u4Version
    SENSOR_ID, // SensorId

    // AE NVRAM
    {
        // rDevicesInfo
        {
            1260,    // u4MinGain, 1024 base = 1x
            8192,    // u4MaxGain, 16x
            162,    // u4MiniISOGain, ISOxx  
            128,    // u4GainStepUnit, 1x/8 
            27,    // u4PreExpUnit 
            30,    // u4PreMaxFrameRate
            18,    // u4VideoExpUnit  
            30,    // u4VideoMaxFrameRate 
            1024,    // u4Video2PreRatio, 1024 base = 1x 
            27,    // u4CapExpUnit 
            15,    // u4CapMaxFrameRate
            1024,    // u4Cap2PreRatio, 1024 base = 1x
            24,    // u4LensFno, Fno = 2.8
            350    // u4FocusLength_100x
        },
        // rHistConfig
        {
            2,    // u4HistHighThres
            40,    // u4HistLowThres
            2,    // u4MostBrightRatio
            1,    // u4MostDarkRatio
            160,    // u4CentralHighBound
            20,    // u4CentralLowBound
            {240, 230, 220, 210, 200},    // u4OverExpThres[AE_CCT_STRENGTH_NUM] 
            {86, 108, 128, 148, 170},    // u4HistStretchThres[AE_CCT_STRENGTH_NUM] 
            {18, 22, 26, 30, 34}    // u4BlackLightThres[AE_CCT_STRENGTH_NUM] 
        },
        // rCCTConfig
        {
            TRUE,    // bEnableBlackLight
            TRUE,    // bEnableHistStretch
            FALSE,    // bEnableAntiOverExposure
            TRUE,    // bEnableTimeLPF
            TRUE,    // bEnableCaptureThres
            TRUE,    // bEnableVideoThres
            TRUE,    // bEnableStrobeThres
            47,    // u4AETarget
            0,    // u4StrobeAETarget
            50,    // u4InitIndex
            4,    // u4BackLightWeight
            32,    // u4HistStretchWeight
            4,    // u4AntiOverExpWeight
            2,    // u4BlackLightStrengthIndex
            2,    // u4HistStretchStrengthIndex
            2,    // u4AntiOverExpStrengthIndex
            2,    // u4TimeLPFStrengthIndex
            {1, 3, 5, 7, 8},    // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM] 
            90,    // u4InDoorEV = 9.0, 10 base 
            -25,    // i4BVOffset delta BV = value/10 
            64,    // u4PreviewFlareOffset
            64,    // u4CaptureFlareOffset
            5,    // u4CaptureFlareThres
            64,    // u4VideoFlareOffset
            5,    // u4VideoFlareThres
            2,    // u4StrobeFlareOffset
            2,    // u4StrobeFlareThres
            8,    // u4PrvMaxFlareThres
            0,    // u4PrvMinFlareThres
            8,    // u4VideoMaxFlareThres
            0,    // u4VideoMinFlareThres
            18,    // u4FlatnessThres    // 10 base for flatness condition.
            75    // u4FlatnessStrength
        }
    },
    // AWB NVRAM
    {
        // AWB calibration data
        {
            // rUnitGain (unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rGoldenGain (golden sample gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rTuningUnitGain (Tuning sample unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rD65Gain (D65 WB gain: 1.0 = 512)
            {
                987,    // i4R
                512,    // i4G
                623    // i4B
            }
        },
        // Original XY coordinate of AWB light source
        {
           // Strobe
            {
                0,    // i4X
                0    // i4Y
            },
            // Horizon
            {
                -414,    // i4X
                -351    // i4Y
            },
            // A
            {
                -264,    // i4X
                -378    // i4Y
            },
            // TL84
            {
                -108,    // i4X
                -336    // i4Y
            },
            // CWF
            {
                -92,    // i4X
                -401    // i4Y
            },
            // DNP
            {
                -35,    // i4X
                -360    // i4Y
            },
            // D65
            {
                170,    // i4X
                -315    // i4Y
            },
            // DF
            {
                0,    // i4X
                0    // i4Y
            }
        },
        // Rotated XY coordinate of AWB light source
        {
            // Strobe
            {
                0,    // i4X
                0    // i4Y
            },
            // Horizon
            {
                -443,    // i4X
                -314    // i4Y
            },
            // A
            {
                -295,    // i4X
                -354    // i4Y
            },
            // TL84
            {
                -136,    // i4X
                -325    // i4Y
            },
            // CWF
            {
                -126,    // i4X
                -392    // i4Y
            },
            // DNP
            {
                -66,    // i4X
                -356    // i4Y
            },
            // D65
            {
                142,    // i4X
                -328    // i4Y
            },
            // DF
            {
                0,    // i4X
                0    // i4Y
            }
        },
        // AWB gain of AWB light source
        {
            // Strobe 
            {
                512,    // i4R
                512,    // i4G
                512    // i4B
            },
            // Horizon 
            {
                512,    // i4R
                558,    // i4G
                1572    // i4B
            },
            // A 
            {
                597,    // i4R
                512,    // i4G
                1220    // i4B
            },
            // TL84 
            {
                697,    // i4R
                512,    // i4G
                935    // i4B
            },
            // CWF 
            {
                778,    // i4R
                512,    // i4G
                997    // i4B
            },
            // DNP 
            {
                794,    // i4R
                512,    // i4G
                874    // i4B
            },
            // D65 
            {
                987,    // i4R
                512,    // i4G
                623    // i4B
            },
            // DF 
            {
                512,    // i4R
                512,    // i4G
                512    // i4B
            }
        },
        // Rotation matrix parameter
        {
            5,    // i4RotationAngle
            255,    // i4Cos
            22    // i4Sin
        },
        // Daylight locus parameter
        {
            -153,    // i4SlopeNumerator
            128    // i4SlopeDenominator
        },
        // AWB light area
        {
            // Strobe:FIXME
            {
            0,    // i4RightBound
            0,    // i4LeftBound
            0,    // i4UpperBound
            0    // i4LowerBound
            },
            // Tungsten
            {
            -186,    // i4RightBound
            -836,    // i4LeftBound
            -284,    // i4UpperBound
            -384    // i4LowerBound
            },
            // Warm fluorescent
            {
            -186,    // i4RightBound
            -836,    // i4LeftBound
            -384,    // i4UpperBound
            -504    // i4LowerBound
            },
            // Fluorescent
            {
            -116,    // i4RightBound
            -186,    // i4LeftBound
            -260,    // i4UpperBound
            -358    // i4LowerBound
            },
            // CWF
            {
            -116,    // i4RightBound
            -186,    // i4LeftBound
            -358,    // i4UpperBound
            -442    // i4LowerBound
            },
            // Daylight
            {
            167,    // i4RightBound
            -116,    // i4LeftBound
            -248,    // i4UpperBound
            -408    // i4LowerBound
            },
            // Shade
            {
            527,    // i4RightBound
            167,    // i4LeftBound
            -248,    // i4UpperBound
            -408    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            170,    // i4RightBound
            -116,    // i4LeftBound
            -408,    // i4UpperBound
            -530    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            527,    // i4RightBound
            -836,    // i4LeftBound
            0,    // i4UpperBound
            -530    // i4LowerBound
            },
            // Daylight
            {
            192,    // i4RightBound
            -116,    // i4LeftBound
            -248,    // i4UpperBound
            -408    // i4LowerBound
            },
            // Cloudy daylight
            {
            292,    // i4RightBound
            117,    // i4LeftBound
            -248,    // i4UpperBound
            -408    // i4LowerBound
            },
            // Shade
            {
            392,    // i4RightBound
            117,    // i4LeftBound
            -248,    // i4UpperBound
            -408    // i4LowerBound
            },
            // Twilight
            {
            -116,    // i4RightBound
            -276,    // i4LeftBound
            -248,    // i4UpperBound
            -408    // i4LowerBound
            },
            // Fluorescent
            {
            192,    // i4RightBound
            -236,    // i4LeftBound
            -275,    // i4UpperBound
            -442    // i4LowerBound
            },
            // Warm fluorescent
            {
            -195,    // i4RightBound
            -395,    // i4LeftBound
            -275,    // i4UpperBound
            -442    // i4LowerBound
            },
            // Incandescent
            {
            -195,    // i4RightBound
            -395,    // i4LeftBound
            -248,    // i4UpperBound
            -408    // i4LowerBound
            },
            // Gray World
            {
            5000,    // i4RightBound
            -5000,    // i4LeftBound
            5000,    // i4UpperBound
            -5000    // i4LowerBound
            }
        },
        // PWB default gain	
        {
            // Daylight
            {
            868,    // i4R
            512,    // i4G
            726    // i4B
            },
            // Cloudy daylight
            {
            1065,    // i4R
            512,    // i4G
            568    // i4B
            },
            // Shade
            {
            1133,    // i4R
            512,    // i4G
            528    // i4B
            },
            // Twilight
            {
            650,    // i4R
            512,    // i4G
            1022    // i4B
            },
            // Fluorescent
            {
            843,    // i4R
            512,    // i4G
            823    // i4B
            },
            // Warm fluorescent
            {
            602,    // i4R
            512,    // i4G
            1227    // i4B
            },
            // Incandescent
            {
            576,    // i4R
            512,    // i4G
            1182    // i4B
            },
            // Gray World
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            }
        },
        // AWB preference color	
        {
            // Tungsten
            {
            0,    // i4SliderValue
            6971    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            0,    // i4SliderValue
            5486    // i4OffsetThr
            },
            // Shade
            {
            0,    // i4SliderValue
            1342    // i4OffsetThr
            },
            // Daylight WB gain
            {
            763,    // i4R
            512,    // i4G
            845    // i4B
            },
            // Preference gain: strobe
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: tungsten
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: warm fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: CWF
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: shade
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            }
        },
        {// CCT estimation
            {// CCT
                2300,    // i4CCT[0]
                2850,    // i4CCT[1]
                4100,    // i4CCT[2]
                5100,    // i4CCT[3]
                6500    // i4CCT[4]
            },
            {// Rotated X coordinate
                -585,    // i4RotatedXCoordinate[0]
                -437,    // i4RotatedXCoordinate[1]
                -278,    // i4RotatedXCoordinate[2]
                -208,    // i4RotatedXCoordinate[3]
                0    // i4RotatedXCoordinate[4]
            }
        }
    },
    {0}
};

#include INCLUDE_FILENAME_ISP_LSC_PARAM
//};  //  namespace

const CAMERA_TSF_TBL_STRUCT CAMERA_TSF_DEFAULT_VALUE =
{
    #include INCLUDE_FILENAME_TSF_PARA
    #include INCLUDE_FILENAME_TSF_DATA
};


typedef NSFeature::RAWSensorInfo<SENSOR_ID> SensorInfoSingleton_T;


namespace NSFeature {
template <>
UINT32
SensorInfoSingleton_T::
impGetDefaultData(CAMERA_DATA_TYPE_ENUM const CameraDataType, VOID*const pDataBuf, UINT32 const size) const
{
    UINT32 dataSize[CAMERA_DATA_TYPE_NUM] = {sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT),
                                             sizeof(NVRAM_CAMERA_3A_STRUCT),
                                             sizeof(NVRAM_CAMERA_SHADING_STRUCT),
                                             sizeof(NVRAM_LENS_PARA_STRUCT),
                                             sizeof(AE_PLINETABLE_T),
                                             0,
                                             sizeof(CAMERA_TSF_TBL_STRUCT)};

    if (CameraDataType > CAMERA_DATA_TSF_TABLE || NULL == pDataBuf || (size < dataSize[CameraDataType]))
    {
        return 1;
    }

    switch(CameraDataType)
    {
        case CAMERA_NVRAM_DATA_ISP:
            memcpy(pDataBuf,&CAMERA_ISP_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_3A:
            memcpy(pDataBuf,&CAMERA_3A_NVRAM_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_3A_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_SHADING:
            memcpy(pDataBuf,&CAMERA_SHADING_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_SHADING_STRUCT));
            break;
        case CAMERA_DATA_AE_PLINETABLE:
            memcpy(pDataBuf,&g_PlineTableMapping,sizeof(AE_PLINETABLE_T));
            break;
        case CAMERA_DATA_TSF_TABLE:
            memcpy(pDataBuf,&CAMERA_TSF_DEFAULT_VALUE,sizeof(CAMERA_TSF_TBL_STRUCT));
            break;
        default:
            break;
    }
    return 0;
}}; // NSFeature


