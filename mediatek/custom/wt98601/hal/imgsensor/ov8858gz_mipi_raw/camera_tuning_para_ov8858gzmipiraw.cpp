/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/
#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_ov8858gzmipiraw.h"
#include "camera_info_ov8858gzmipiraw.h"
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
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    	}
    },
    ISPPca: {
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
        72120,    // i4R_AVG
        15943,    // i4R_STD
        113240,    // i4B_AVG
        30247,    // i4B_STD
        {  // i4P00[9]
            4810000, -2218000, -28000, -842000, 3556000, -154000, -208000, -2708000, 5476000
        },
        {  // i4P10[9]
            1451475, -1648548, 183562, 24874, 50397, -75271, 11876, -473131, 461255
        },
        {  // i4P01[9]
            831054, -879355, 35944, -138302, -44296, 182598, -286390, -1503931, 1790321
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
			 1136,	  // u4MinGain, 1024 base = 1x
			 10240,    // u4MaxGain, 16x
            100,    // u4MiniISOGain, ISOxx  
			 128,	 // u4GainStepUnit, 1x/8 
			 27,	// u4PreExpUnit 
			 30,	// u4PreMaxFrameRate
			 27,	// u4VideoExpUnit  
			 30,	// u4VideoMaxFrameRate 
			 1024,	  // u4Video2PreRatio, 1024 base = 1x 
			 27,	// u4CapExpUnit 
			 15,	// u4CapMaxFrameRate
			 1024,	  // u4Cap2PreRatio, 1024 base = 1x
			 20,	// u4LensFno, Fno = 2.8
			 350	// u4FocusLength_100x
		 },

        
        // rHistConfig
        {
            2,   // u4HistHighThres
            40,  // u4HistLowThres
            2,   // u4MostBrightRatio
            1,   // u4MostDarkRatio
            160, // u4CentralHighBound
            20,  // u4CentralLowBound
            {240, 230, 220, 210, 200}, // u4OverExpThres[AE_CCT_STRENGTH_NUM]
            {86, 108, 128, 148, 170},  // u4HistStretchThres[AE_CCT_STRENGTH_NUM]
            {18, 22, 26, 30, 34}       // u4BlackLightThres[AE_CCT_STRENGTH_NUM]
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
            76,    // u4InitIndex
            4,    // u4BackLightWeight
            32,    // u4HistStretchWeight
            4,    // u4AntiOverExpWeight
            2,    // u4BlackLightStrengthIndex
            2,    // u4HistStretchStrengthIndex
            2,    // u4AntiOverExpStrengthIndex
            2,    // u4TimeLPFStrengthIndex
            {1, 3, 5, 7, 8},    // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM] 
            90,    // u4InDoorEV = 9.0, 10 base 
            -9,    // i4BVOffset delta BV = value/10 
            64,    // u4PreviewFlareOffset
            64,    // u4CaptureFlareOffset
            5,    // u4CaptureFlareThres
            64,    // u4VideoFlareOffset
            5,    // u4VideoFlareThres
            5,    // u4StrobeFlareOffset
            5,    // u4StrobeFlareThres
            160,    // u4PrvMaxFlareThres
            0,    // u4PrvMinFlareThres
            160,    // u4VideoMaxFlareThres
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
    			0,	// i4R
    			0,	// i4G
    			0	// i4B
    		},
    		// rGoldenGain (golden sample gain: 1.0 = 512)
    		{
	            0,	// i4R
	            0,	// i4G
	            0	// i4B
            },
    		// rTuningUnitGain (Tuning sample unit gain: 1.0 = 512)
    		{
	            0,	// i4R
	            0,	// i4G
	            0	// i4B
            },
            // rD65Gain (D65 WB gain: 1.0 = 512)
            {
                979,    // i4R
                512,    // i4G
                645    // i4B
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
                -387,    // i4X
                -358    // i4Y
            },
            // A
            {
                -269,    // i4X
                -368    // i4Y
            },
            // TL84
            {
                -120,    // i4X
                -336    // i4Y
            },
            // CWF
            {
                -95,    // i4X
                -405    // i4Y
            },
            // DNP
            {
                -31,    // i4X
                -366    // i4Y
            },
            // D65
            {
                154,    // i4X
                -325    // i4Y
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
                -411,    // i4X
                -329    // i4Y
            },
            // A
            {
                -294,    // i4X
                -348    // i4Y
            },
            // TL84
            {
                -143,    // i4X
                -326    // i4Y
            },
            // CWF
            {
                -123,    // i4X
                -397    // i4Y
            },
            // DNP
            {
                -57,    // i4X
                -362    // i4Y
            },
            // D65
            {
                131,    // i4X
                -335    // i4Y
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
                533,    // i4G
                1460    // i4B
            },
            // A 
            {
                586,    // i4R
                512,    // i4G
                1212    // i4B
            },
            // TL84 
            {
                686,    // i4R
                512,    // i4G
                950    // i4B
            },
            // CWF 
            {
                780,    // i4R
                512,    // i4G
                1007    // i4B
            },
            // DNP 
            {
                806,    // i4R
                512,    // i4G
                876    // i4B
            },
            // D65 
            {
                979,    // i4R
                512,    // i4G
                645    // i4B
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
            4,    // i4RotationAngle
            255,    // i4Cos
            18    // i4Sin
        },
        // Daylight locus parameter
        {
            -148,    // i4SlopeNumerator
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
            -228,    // i4RightBound
            -843,    // i4LeftBound
            -285,    // i4UpperBound
            -429    // i4LowerBound
            },
            // Warm fluorescent
            {
            -228,    // i4RightBound
            -843,    // i4LeftBound
            -429,    // i4UpperBound
            -550    // i4LowerBound
            },
            // Fluorescent
            {
            -115,    // i4RightBound
            -228,    // i4LeftBound
            -310,    // i4UpperBound
            -410    // i4LowerBound
            },
            // CWF
            {
            -115,    // i4RightBound
            -228,    // i4LeftBound
            -410,    // i4UpperBound
            -500    // i4LowerBound
            },
            // Daylight
            {
            160,    // i4RightBound
            -115,    // i4LeftBound
            -290,    // i4UpperBound
            -440    // i4LowerBound
            },
            // Shade
            {
            516,    // i4RightBound
            160,    // i4LeftBound
            -290,    // i4UpperBound
            -440    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            40,    // i4RightBound
            -115,    // i4LeftBound
            -440,    // i4UpperBound
            -530    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            516,    // i4RightBound
            -843,    // i4LeftBound
            0,    // i4UpperBound
            -550    // i4LowerBound
            },
            // Daylight
            {
            185,    // i4RightBound
            -115,    // i4LeftBound
            -290,    // i4UpperBound
            -440    // i4LowerBound
            },
            // Cloudy daylight
            {
            285,    // i4RightBound
            110,    // i4LeftBound
            -290,    // i4UpperBound
            -440    // i4LowerBound
            },
            // Shade
            {
            385,    // i4RightBound
            110,    // i4LeftBound
            -290,    // i4UpperBound
            -440    // i4LowerBound
            },
            // Twilight
            {
            -115,    // i4RightBound
            -275,    // i4LeftBound
            -290,    // i4UpperBound
            -440    // i4LowerBound
            },
            // Fluorescent
            {
            181,    // i4RightBound
            -243,    // i4LeftBound
            -276,    // i4UpperBound
            -447    // i4LowerBound
            },
            // Warm fluorescent
            {
            -194,    // i4RightBound
            -394,    // i4LeftBound
            -276,    // i4UpperBound
            -447    // i4LowerBound
            },
            // Incandescent
            {
            -194,    // i4RightBound
            -394,    // i4LeftBound
            -290,    // i4UpperBound
            -440    // i4LowerBound
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
            908,    // i4R
            512,    // i4G
            770    // i4B
            },
            // Cloudy daylight
            {
            1113,    // i4R
            512,    // i4G
            609    // i4B
            },
            // Shade
            {
            1185,    // i4R
            512,    // i4G
            566    // i4B
            },
            // Twilight
            {
            680,    // i4R
            512,    // i4G
            1074    // i4B
            },
            // Fluorescent
            {
            831,    // i4R
            512,    // i4G
            844    // i4B
            },
            // Warm fluorescent
            {
            597,    // i4R
            512,    // i4G
            1234    // i4B
            },
            // Incandescent
            {
            600,    // i4R
            512,    // i4G
            1240    // i4B
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
            6406    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            0,    // i4SliderValue
            5903    // i4OffsetThr
            },
            // Shade
            {
            9,    // i4SliderValue
            1216    // i4OffsetThr
            },
            // Daylight WB gain
            {
            774,    // i4R
            512,    // i4G
            847    // i4B
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
                -542,    // i4RotatedXCoordinate[0]
                -425,    // i4RotatedXCoordinate[1]
                -274,    // i4RotatedXCoordinate[2]
                -188,    // i4RotatedXCoordinate[3]
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
}};  //  NSFeature


