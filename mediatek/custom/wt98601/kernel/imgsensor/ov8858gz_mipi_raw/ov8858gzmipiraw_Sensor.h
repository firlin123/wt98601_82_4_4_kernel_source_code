/*******************************************************************************************/
  
  
/*******************************************************************************************/

/* SENSOR FULL SIZE */
#ifndef __OV8858GZ_SENSOR_H
#define __OV8858GZ_SENSOR_H   

typedef enum group_enum {
    PRE_GAIN=0,
    CMMCLK_CURRENT,
    FRAME_RATE_LIMITATION,
    REGISTER_EDITOR,
    GROUP_TOTAL_NUMS
} FACTORY_GROUP_ENUM;


#define ENGINEER_START_ADDR 10
#define FACTORY_START_ADDR 0

typedef enum engineer_index
{
    CMMCLK_CURRENT_INDEX=ENGINEER_START_ADDR,
    ENGINEER_END
} FACTORY_ENGINEER_INDEX;

typedef enum register_index
{
	SENSOR_BASEGAIN=FACTORY_START_ADDR,
	PRE_GAIN_R_INDEX,
	PRE_GAIN_Gr_INDEX,
	PRE_GAIN_Gb_INDEX,
	PRE_GAIN_B_INDEX,
	FACTORY_END_ADDR
} FACTORY_REGISTER_INDEX;

typedef struct
{
    SENSOR_REG_STRUCT	Reg[ENGINEER_END];
    SENSOR_REG_STRUCT	CCT[FACTORY_END_ADDR];
} SENSOR_DATA_STRUCT, *PSENSOR_DATA_STRUCT;

typedef enum {
    SENSOR_MODE_INIT = 0,
    SENSOR_MODE_PREVIEW,
    SENSOR_MODE_VIDEO,
    SENSOR_MODE_CAPTURE
} OV8858GZ_SENSOR_MODE;


typedef struct
{
	kal_uint32 DummyPixels;
	kal_uint32 DummyLines;
	
	kal_uint32 pvShutter;
	kal_uint32 pvGain;
	
	kal_uint32 pvPclk;  
	kal_uint32 videoPclk;
	kal_uint32 capPclk;
	
	kal_uint32 shutter;

	kal_uint16 sensorGlobalGain;
	kal_uint16 ispBaseGain;
	kal_uint16 realGain;

	kal_int16 imgMirror;

	OV8858GZ_SENSOR_MODE sensorMode;

	kal_bool OV8858GZAutoFlickerMode;
	kal_bool OV8858GZVideoMode;
	
}OV8858GZ_PARA_STRUCT,*POV8858GZ_PARA_STRUCT;


    #define OV8858GZ_SHUTTER_MARGIN 			(4)
	#define OV8858GZ_GAIN_BASE				(128)
	#define OV8858GZ_AUTOFLICKER_OFFSET_30 	(296)
	#define OV8858GZ_AUTOFLICKER_OFFSET_15 	(146)
	#define OV8858GZ_PREVIEW_PCLK 			(72000000)
	#define OV8858GZ_VIDEO_PCLK 				(72000000)
	#define OV8858GZ_CAPTURE_PCLK 			(72000000)
	
	#define OV8858GZ_MAX_FPS_PREVIEW			(300)
	#define OV8858GZ_MAX_FPS_VIDEO			(300)
	#define OV8858GZ_MAX_FPS_CAPTURE			(150)
	//#define OV8865_MAX_FPS_N3D				(300)


	//grab window
	#define OV8858GZ_IMAGE_SENSOR_PV_WIDTH					(1632)
	#define OV8858GZ_IMAGE_SENSOR_PV_HEIGHT					(1224)
	#define OV8858GZ_IMAGE_SENSOR_VIDEO_WIDTH 				(1632)
	#define OV8858GZ_IMAGE_SENSOR_VIDEO_HEIGHT				(1224)
	#define OV8858GZ_IMAGE_SENSOR_FULL_WIDTH					(3264)	
	#define OV8858GZ_IMAGE_SENSOR_FULL_HEIGHT 				(2448)

	#define OV8858GZ_FULL_X_START						    		(0)
	#define OV8858GZ_FULL_Y_START						    		(0)
	#define OV8858GZ_PV_X_START						    		(0)
	#define OV8858GZ_PV_Y_START						    		(0)
	#define OV8858GZ_VIDEO_X_START								(0)
	#define OV8858GZ_VIDEO_Y_START								(0)

//TODO~
	#define OV8858GZ_MAX_ANALOG_GAIN					(8)
	#define OV8858GZ_MIN_ANALOG_GAIN					(1)


	/* SENSOR PIXEL/LINE NUMBERS IN ONE PERIOD */
	#define OV8858GZ_PV_PERIOD_PIXEL_NUMS 				(1928)//0x0788	//1928
	#define OV8858GZ_PV_PERIOD_LINE_NUMS					(1244)//0x04DC	//1244

	#define OV8858GZ_VIDEO_PERIOD_PIXEL_NUMS				(1928)	//2566
	#define OV8858GZ_VIDEO_PERIOD_LINE_NUMS				(1244)	//1872

	#define OV8858GZ_FULL_PERIOD_PIXEL_NUMS				(1940)
	#define OV8858GZ_FULL_PERIOD_LINE_NUMS				(2474)
	
	#define OV8858GZMIPI_WRITE_ID 	(0x42)
	#define OV8858GZMIPI_READ_ID	(0x43)

	#define OV8858GZMIPI_SENSOR_ID            OV8858GZ_SENSOR_ID


	UINT32 OV8858GZMIPIOpen(void);
	UINT32 OV8858GZMIPIGetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution);
	UINT32 OV8858GZMIPIGetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_INFO_STRUCT *pSensorInfo, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
	UINT32 OV8858GZMIPIControl(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
	UINT32 OV8858GZMIPIFeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId, UINT8 *pFeaturePara,UINT32 *pFeatureParaLen);
	UINT32 OV8858GZMIPIClose(void);

#endif 

