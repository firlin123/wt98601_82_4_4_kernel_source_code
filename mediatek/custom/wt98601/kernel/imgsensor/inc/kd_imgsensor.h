#ifndef _KD_IMGSENSOR_H
#define _KD_IMGSENSOR_H

#include <linux/ioctl.h>

#ifndef ASSERT
    #define ASSERT(expr)        BUG_ON(!(expr))
#endif

#define IMGSENSORMAGIC 'i'
//IOCTRL(inode * ,file * ,cmd ,arg )
//S means "set through a ptr"
//T means "tell by a arg value"
//G means "get by a ptr"
//Q means "get by return a value"
//X means "switch G and S atomically"
//H means "switch T and Q atomically"

/*******************************************************************************
*
********************************************************************************/

/*******************************************************************************
*
********************************************************************************/

//sensorOpen
//This command will TBD
#define KDIMGSENSORIOC_T_OPEN            _IO(IMGSENSORMAGIC,0)
//sensorGetInfo
//This command will TBD
#define KDIMGSENSORIOC_X_GETINFO            _IOWR(IMGSENSORMAGIC,5,ACDK_SENSOR_GETINFO_STRUCT)
//sensorGetResolution
//This command will TBD
#define KDIMGSENSORIOC_X_GETRESOLUTION      _IOWR(IMGSENSORMAGIC,10,ACDK_SENSOR_RESOLUTION_INFO_STRUCT)
//sensorFeatureControl
//This command will TBD
#define KDIMGSENSORIOC_X_FEATURECONCTROL    _IOWR(IMGSENSORMAGIC,15,ACDK_SENSOR_FEATURECONTROL_STRUCT)
//sensorControl
//This command will TBD
#define KDIMGSENSORIOC_X_CONTROL            _IOWR(IMGSENSORMAGIC,20,ACDK_SENSOR_CONTROL_STRUCT)
//sensorClose
//This command will TBD
#define KDIMGSENSORIOC_T_CLOSE            _IO(IMGSENSORMAGIC,25)
//sensorSearch 
#define KDIMGSENSORIOC_T_CHECK_IS_ALIVE     _IO(IMGSENSORMAGIC, 30) 
//set sensor driver
#define KDIMGSENSORIOC_X_SET_DRIVER         _IOWR(IMGSENSORMAGIC,35,SENSOR_DRIVER_INDEX_STRUCT)
//get socket postion
#define KDIMGSENSORIOC_X_GET_SOCKET_POS     _IOWR(IMGSENSORMAGIC,40,u32)
//set I2C bus 
#define KDIMGSENSORIOC_X_SET_I2CBUS     _IOWR(IMGSENSORMAGIC,45,u32)
//set I2C bus 
#define KDIMGSENSORIOC_X_RELEASE_I2C_TRIGGER_LOCK     _IO(IMGSENSORMAGIC,50)
//Set Shutter Gain Wait Done
#define KDIMGSENSORIOC_X_SET_SHUTTER_GAIN_WAIT_DONE   _IOWR(IMGSENSORMAGIC,55,u32)//HDR
//set mclk
#define KDIMGSENSORIOC_X_SET_MCLK_PLL         _IOWR(IMGSENSORMAGIC,60,ACDK_SENSOR_MCLK_STRUCT)
/*******************************************************************************
*
********************************************************************************/
/* SENSOR CHIP VERSION */
#define SP2529_SENSOR_ID				0x2529
#define NT99252_SENSOR_ID                        0x2520
#define A5142MIPI_SENSOR_ID                     0x4800
#define OV8858_SENSOR_ID       0x8858
#define OV8858GZ_SENSOR_ID       (0x8858+1)
#define GC2355_SENSOR_ID       0x2355
#define S5K4H5YX_SENSOR_ID   0x485b
/* CAMERA DRIVER NAME */
#define CAMERA_HW_DEVNAME            "kd_camera_hw"

/* SENSOR DEVICE DRIVER NAME */
#define SENSOR_DRVNAME_SP2529_YUV   "sp2529yuv"
#define SENSOR_DRVNAME_NT99252_YUV   	"nt99252yuv"
#define SENSOR_DRVNAME_A5142_MIPI_RAW   "a5142mipiraw"
#define SENSOR_DRVNAME_OV8858_MIPI_RAW   	"ov8858raw"
#define SENSOR_DRVNAME_OV8858GZ_MIPI_RAW   	"ov8858rawgz"
#define SENSOR_DRVNAME_GC2355_RAW   	"gc2355raw"
#define SENSOR_DRVNAME_S5K4H5YX_MIPI_RAW  "s5k4h5yxmipiraw"

/*******************************************************************************
*
********************************************************************************/

void KD_IMGSENSOR_PROFILE_INIT(void); 
void KD_IMGSENSOR_PROFILE(char *tag); 

#define mDELAY(ms)     mdelay(ms) 
#define uDELAY(us)       udelay(us) 
#endif //_KD_IMGSENSOR_H


