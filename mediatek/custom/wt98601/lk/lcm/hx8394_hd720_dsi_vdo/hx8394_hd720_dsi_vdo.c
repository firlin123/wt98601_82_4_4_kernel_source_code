/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"


#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#elif defined(BUILD_UBOOT)
#include <asm/arch/mt_gpio.h>
#else
#include <mach/mt_gpio.h>
#endif


// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1280)

#define REGFLAG_DELAY             							0xFE
#define REGFLAG_END_OF_TABLE      							0xFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE									0

#define LCM_ID_HX8394A                                      0x94


// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)   			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size) 


static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[128];
};

static struct LCM_setting_table lcm_initialization_setting[] = {
	
	/*
	Note :

	Data ID will depends on the following rule.
	
		count of parameters > 1	=> Data ID = 0x39
		count of parameters = 1	=> Data ID = 0x15
		count of parameters = 0	=> Data ID = 0x05

	Structure Format :

	{DCS command, count of parameters, {parameter list}}
	{REGFLAG_DELAY, milliseconds of time, {}},

	...

	Setting ending by predefined flag
	
	{REGFLAG_END_OF_TABLE, 0x00, {}}
	*/
	/*
	{0xB9,3,{0xFF,0x83,0x94}}, 
	{0xBA,16,{0x12,0x82,0x00,0x16,0xC5,0x00,0x10,0xFF,0x0F,0x24,0x03,0x21,0x24,0x25,0x20,0x08}},
	{0xB1,16,{0x01,0x00,0x04,0x87,0x01,0x11,0x11,0x2F,0x37,0x3F,0x3F,0x47,0x12,0x01,0xE6,0xE2}},
	{0xB2,6,{0x00,0xC8,0x08,0x04,0x00,0x22}},
	{0xB4,22,{0x80,0x06,0x32,0x10,0x03,0x32,0x15,0x08,0x32,0x10,0x08,0x33,0x04,0x43,0x05,0x37,0x04,0x43,0x06,0x61,0x61,0x06}},
	{0xBF,4,{0x06,0x00,0x10,0x04}},
	{0xC0,2,{0x0C,0x17}},
	{0xB6,1,{0x0B}},
	{0xD5,32,{0x00,0x00,0x00,0x00,0x0A,0x00,0x01,0x00,0xCC,0x00,0x00,0x00,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x01,0x67,0x45,0x23,0x01,0x23,0x88,0x88,0x88,0x88}},
	{0xCC,1,{0x01}},
	{0xC7,4,{0x00,0x10,0x00,0x10}},
	{0xE0,42,{0x00,0x04,0x06,0x2B,0x33,0x3F,0x13,0x34,0x0A,0x0E,0x0D,0x11,0x13,0x11,0x13,0x10,0x17,0x00,0x04,0x06,0x2B,0x33,0x3F,0x13,0x34,0x0A,0x0E,0x0D,0x11,0x13,0x11,0x13,0x10,0x17,0x0B,0x17,0x07,0x11,0x0B,0x17,0x07,0x11}},
	{0xD4,1,{0x32}},
	{0x11,1,{0x00}},
	//{0x00,	1,	{0x00}},
//	{0x3A,	1,	{0x77}},
//	{0x11,	1,	{0x00}},
	{REGFLAG_DELAY, 200, {}},
	{0x29,	1,	{0x00}},
	{REGFLAG_DELAY, 50, {}},
//	{0x2C,	0,	{}},
	
	// Note
	// Strongly recommend not to set Sleep out / Display On here. That will cause messed frame to be shown as later the backlight is on.
  
  
	// Setting ending by predefined flag
	{REGFLAG_END_OF_TABLE, 0x00, {}}*/
	/////QQQQQ
	{0xB9,3,{0xFF,0x83,0x94}}, 
	{0xBA,16,{0x12,0x82,0x00,0x16,0xC5,0x00,0x10,0xFF,0x0F,0x24,0x03,0x21,0x24,0x25,0x20,0x08}},
	{0xB1,16,{0x01,0x00,0x04,0x87,0x01,0x11,0x11,0x2F,0x37,0x3F,0x3F,0x47,0x12,0x01,0xE6,0xE2}},
	{0xB2,6,{0x00,0xC8,0x08,0x04,0x00,0x22}},
	{0xB4,22,{0x80,0x06,0x32,0x10,0x03,0x32,0x15,0x08,0x32,0x10,0x08,0x33,0x04,0x43,0x05,0x37,0x04,0x43,0x06,0x61,0x61,0x06}},
	{0xBF,4,{0x06,0x00,0x10,0x04}},
	{0xC0,2,{0x0C,0x17}},
	{0xB6,1,{0x0B}},
	{0xD5,32,{0x00,0x00,0x00,0x00,0x0A,0x00,0x01,0x00,0xCC,0x00,0x00,0x00,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x01,0x67,0x45,0x23,0x01,0x23,0x88,0x88,0x88,0x88}},
	{0xCC,1,{0x01}},
	{0xC7,4,{0x00,0x10,0x00,0x10}},
	{0xE0,42,{0x00,0x04,0x06,0x2B,0x33,0x3F,0x13,0x34,0x0A,0x0E,0x0D,0x11,0x13,0x11,0x13,0x10,0x17,0x00,0x04,0x06,0x2B,0x33,0x3F,0x13,0x34,0x0A,0x0E,0x0D,0x11,0x13,0x11,0x13,0x10,0x17,0x0B,0x17,0x07,0x11,0x0B,0x17,0x07,0x11}},
	{0xD4,1,{0x32}},
	{0x11,1,{0x00}},
	//{0x00,	1,	{0x00}},
//	{0x3A,	1,	{0x77}},
//	{0x11,	1,	{0x00}},
	{REGFLAG_DELAY, 200, {}},
	{0x29,	1,	{0x00}},
	{REGFLAG_DELAY, 50, {}},
//	{0x2C,	0,	{}},
	
	// Note
	// Strongly recommend not to set Sleep out / Display On here. That will cause messed frame to be shown as later the backlight is on.
  
  
	// Setting ending by predefined flag
	{REGFLAG_END_OF_TABLE, 0x00, {}}	
};
  
static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{ 
	unsigned int i;
  
    for(i = 0; i < count; i++) {
		
        unsigned cmd;
        cmd = table[i].cmd;
		
        switch (cmd) {
			
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
				
            case REGFLAG_END_OF_TABLE :
                break;
				
            default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
       	}
    }
	
}


static void init_lcm_registers(void)
{
   unsigned int data_array[16];



    data_array[0] = 0x00043902;                          
    data_array[1] = 0x9483FFB9; //send one param for test
    dsi_set_cmdq(&data_array, 2, 1);
    MDELAY(20);


//0x12,0x82,0x00,0x16,0xC5,0x00,0x10,0xFF,0x0F,0x24,0x03,0x21,0x24,0x25,0x20,0x08

    
   // 0x01,0x00,0x04,0x87,0x01,0x11,0x11,0x2F,0x37,0x3F,0x3F,0x47,0x12,0x01,0xE6,0xE2
    
    data_array[0] = 0x00033902;                          
    data_array[1] = 0x008132BA; 
    dsi_set_cmdq(&data_array, 2, 1); 
    MDELAY(20);
    
    //0x00,0xC8,0x08,0x04,0x00,0x22



//0x80,0x06,0x32,0x10,0x03,0x32,0x15,0x08,0x32,0x10,0x08,0x33,0x04,0x43,0x05,0x37,0x04,0x43,0x06,0x61,0x61,0x06
    data_array[0] = 0x00103902;                          
    data_array[1] = 0x10106CB1; 
    data_array[2] = 0xF1110426;
    data_array[3] = 0x23543A81;
    data_array[4] = 0x58D2C080;
    dsi_set_cmdq(&data_array, 5, 1); 
    MDELAY(20);

	//{0xBF,4,{0x06,0x00,0x10,0x04}},
	


    data_array[0] = 0x000C3902;                          
    data_array[1] = 0x0E6400B2; 
	data_array[2] = 0x081C220D;
	data_array[3] = 0x004D1C08;
    dsi_set_cmdq(&data_array, 4, 1);
    MDELAY(20);



    data_array[0] = 0x000D3902;                          
    data_array[1] = 0x51FF00B4; 
    data_array[2] = 0x03525952;
    data_array[3] = 0x20600152;
    data_array[4] = 0x00000060;

    dsi_set_cmdq(&data_array, 5, 1); 
    MDELAY(20);

//0x00,0x00,0x00,0x00,0x0A,0x00,0x01,0x00,0xCC,0x00,0x00,0x00,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x01,0x67,0x45,0x23,0x01,0x23,0x88,0x88,0x88,0x88}},
	


         data_array[0] = 0x07BC1500;//              
            dsi_set_cmdq(&data_array, 1, 1);

         MDELAY(20);

   

    data_array[0] = 0x00043902;                          
    data_array[1] = 0x010E41BF; 
 //   data_array[1] = 0x00000001; 
    dsi_set_cmdq(&data_array, 2, 1);
    MDELAY(20);



//#if use TE function,please open it
    data_array[0] = 0x00043902;                          
    data_array[1] = 0x9483FFB9; //send one param for test
    dsi_set_cmdq(&data_array, 2, 1);
    MDELAY(20);
    data_array[0] = 0x001F3902;                          
    data_array[1] = 0x000F00D3; 
    data_array[2] = 0x00100740;                          
    data_array[3] = 0x00081008;
    data_array[4] = 0x0E155408;                          
    data_array[5] = 0x15020E05;
    data_array[6] = 0x47060506;                          
    data_array[7] = 0x4B0A0A44;
    data_array[8] = 0x00070710;                          
    dsi_set_cmdq(&data_array, 9, 1);
    MDELAY(20);

//0x00,0x04,0x06,0x2B,0x33,0x3F,0x13,0x34,0x0A,0x0E,0x0D,0x11,0x13,0x11,0x13,0x10,0x17,0x00,0x04,0x06,0x2B,0x33,0x3F,0x13,0x34,0x0A,0x0E,0x0D,0x11,0x13,0x11,0x13,0x10,0x17,0x0B,0x17,0x07,0x11,0x0B,0x17,0x07,0x11}


    
   // data_array[0] = 0x00110500; // Sleep Out
  //  dsi_set_cmdq(&data_array, 1, 1); 
    //MDELAY(400);

   // data_array[0] = 0x00290500; // Display On
 //   dsi_set_cmdq(&data_array, 1, 1); 
  //  MDELAY(10);
        data_array[0] = 0x00043902;                          
    data_array[1] = 0x9483FFB9; //send one param for test
    dsi_set_cmdq(&data_array, 2, 1);
    MDELAY(20);
       data_array[0] = 0x002D3902;                          
       data_array[1] = 0x1B1A1AD5; 
	data_array[2] = 0x0201001B; 
	data_array[3] = 0x06050403;
	data_array[4] = 0x0A090807;
	data_array[5] = 0x1825240B;
	data_array[6] = 0x18272618;
	data_array[7] = 0x18181818;
	data_array[8] = 0x18181818;
	data_array[9] = 0x18181818;
 data_array[10] = 0x20181818;
 data_array[11] = 0x18181821;
 data_array[12] = 0x00000018;
	dsi_set_cmdq(&data_array, 13, 1);
    MDELAY(20);

    data_array[0] = 0x00043902;                          
    data_array[1] = 0x9483FFB9; //send one param for test
    dsi_set_cmdq(&data_array, 2, 1);
    MDELAY(20);
	data_array[0] = 0x002D3902;                          
      data_array[1] = 0x1B1A1AD6; 
	data_array[2] = 0x090A0B1B; 
	data_array[3] = 0x05060708;
	data_array[4] = 0x01020304;
	data_array[5] = 0x58202100;
	data_array[6] = 0x18262758;
	data_array[7] = 0x18181818;
	data_array[8] = 0x18181818;
	data_array[9] = 0x18181818;
      data_array[10] = 0x25181818;
	data_array[11] = 0x18181824;
	data_array[12] = 0x00000018;
    dsi_set_cmdq(&data_array, 13, 1);
    MDELAY(20);





	 data_array[0] = 0x002B3902;                          
       data_array[1] = 0x100B00E0; 
	data_array[2] = 0x1D3F322C; 
	data_array[3] = 0x0D0B0639;
	data_array[4] = 0x14110E17;
	data_array[5] = 0x13081311;
	data_array[6] = 0x0B001614;
	data_array[7] = 0x3F322C10;
	data_array[8] = 0x0B06391D;
	data_array[9] = 0x110E170D;
 data_array[10] = 0x08131114;
 data_array[11] = 0x00161413;
    dsi_set_cmdq(&data_array, 12, 1);
    MDELAY(20);







     data_array[0] = 0x09CC1500;//              
     dsi_set_cmdq(&data_array, 1, 1);

         MDELAY(20);




     data_array[0] = 0x00033902;                          
    data_array[1] = 0x001430C0; 
    dsi_set_cmdq(&data_array, 2, 1);
    MDELAY(20);
    

	  data_array[0] = 0x00053902;                          
    data_array[1] = 0x40C000C7; 
	  data_array[2] = 0x000000C0;
    dsi_set_cmdq(&data_array, 3, 1);
    MDELAY(5);



	 data_array[0] = 0x00033902;                          
   data_array[1] = 0x005454B6; 
    dsi_set_cmdq(&data_array, 2, 1);
    MDELAY(10);

	


     data_array[0] = 0x00110500; 			   
		dsi_set_cmdq(&data_array, 1, 1); 
		MDELAY(200);
		     data_array[0] = 0x00290500; 			   
		dsi_set_cmdq(&data_array, 1, 1); 
		MDELAY(20);		
}
// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
    memset(params, 0, sizeof(LCM_PARAMS));

    params->type   = LCM_TYPE_DSI;

    params->width  = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;

    // enable tearing-free
    params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;
    //params->dbi.te_mode 				= LCM_DBI_TE_MODE_DISABLED;
    params->dbi.te_edge_polarity			= LCM_POLARITY_RISING;

#if (LCM_DSI_CMD_MODE)
    params->dsi.mode   = CMD_MODE;
#else
    params->dsi.mode   = SYNC_PULSE_VDO_MODE;
#endif

    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM				= LCM_THREE_LANE;
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;


    // Highly depends on LCD driver capability.
    // Not support in MT6573
    params->dsi.packet_size = 256;

    // Video mode setting		
    params->dsi.intermediat_buffer_num = 2;

    params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;


    params->dsi.word_count = 720*3;	
/*
	params->dsi.vertical_sync_active				= 4; // 1
	params->dsi.vertical_backporch					= 16; // 16
	params->dsi.vertical_frontporch 				= 16; // 15
	params->dsi.vertical_active_line				= FRAME_HEIGHT; 

	params->dsi.horizontal_sync_active				= 90; // 60
	params->dsi.horizontal_backporch				= 90; // 72
	params->dsi.horizontal_frontporch				= 129; // 108
	params->dsi.horizontal_active_pixel 			= FRAME_WIDTH;


    // Bit rate calculation
    params->dsi.pll_div1=0;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
    params->dsi.pll_div2=1;		// div2=0,1,2,3;div1_real=1,2,4,4	
    params->dsi.fbk_div =24;    // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	*/
/////QQQQQ
params->dsi.vertical_sync_active	=16;//
	params->dsi.vertical_backporch		= 16;//
	params->dsi.vertical_frontporch		= 16;//
	params->dsi.vertical_active_line	= FRAME_HEIGHT;

	params->dsi.horizontal_sync_active	= 40;//
	params->dsi.horizontal_backporch	= 80;//
	params->dsi.horizontal_frontporch	= 80;//
	params->dsi.horizontal_active_pixel	= FRAME_WIDTH;
        //params->dsi.HS_PRPR = 4;
    //    params->dsi.CLK_HS_PRPR = 6;
	// Bit rate calculation
	
	params->dsi.pll_div1=0;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
	params->dsi.pll_div2=1;		// div2=0,1,2,3;div1_real=1,2,4,4
	params->dsi.fbk_div =17;    // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)
}


static void lcm_init(void)
{
    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(200);

   init_lcm_registers();
  
  	//push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
    unsigned int data_array[16];

#if 1
    SET_RESET_PIN(1);
        MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(120);
#endif

#if 0
    // Display Off
	data_array[0]=0x00280500;
	dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(120);

    // Sleep In
    data_array[0] = 0x00100500;
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(50);
#endif
}


static void lcm_resume(void)
{  
    lcm_init();

}


static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
    unsigned int x0 = x;
    unsigned int y0 = y;
    unsigned int x1 = x0 + width - 1;
    unsigned int y1 = y0 + height - 1;

    unsigned char x0_MSB = ((x0>>8)&0xFF);
    unsigned char x0_LSB = (x0&0xFF);
    unsigned char x1_MSB = ((x1>>8)&0xFF);
    unsigned char x1_LSB = (x1&0xFF);
    unsigned char y0_MSB = ((y0>>8)&0xFF);
    unsigned char y0_LSB = (y0&0xFF);
    unsigned char y1_MSB = ((y1>>8)&0xFF);
    unsigned char y1_LSB = (y1&0xFF);

    unsigned int data_array[16];

    data_array[0]= 0x00053902;
    data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
    data_array[2]= (x1_LSB);
    dsi_set_cmdq(&data_array, 3, 1);

    data_array[0]= 0x00053902;
    data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
    data_array[2]= (y1_LSB);
    dsi_set_cmdq(&data_array, 3, 1);

    data_array[0]= 0x00290508; //HW bug, so need send one HS packet
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0]= 0x002c3909;
    dsi_set_cmdq(&data_array, 1, 0);
}

static unsigned int lcm_compare_id(void)
{

  	unsigned int id = 0;
    unsigned char buffer[3];
    unsigned int array[16];  
    unsigned int data_array[16]; 
    SET_RESET_PIN(1);
	MDELAY(10);
    SET_RESET_PIN(0);
	MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(20);//Must over 6 ms
    #if defined(BUILD_LK)
   //  lcm_poweron(true);
    #endif	
    MDELAY(180);
    array[0] = 0x00043902;                          
    array[1] = 0x9483FFB9; //send one param for test
    dsi_set_cmdq(&data_array, 2, 1);
    MDELAY(20);
    array[0] = 0x00023902;                          
    array[1] = 0x000032BA; //send one param for test
    dsi_set_cmdq(&data_array, 2, 1);
    MDELAY(20);
    array[0] = 0x00033700;// read id return two byte,version and id
    dsi_set_cmdq(array, 1, 1);
    read_reg_v2(0x04, buffer, 3);


if((buffer[0] == 0x83)&&(buffer[1] == 0x94)&&(buffer[2] == 0x0D))
	
	/*
    array[0]=0x00043902;
    array[1]=0x9483FFB9;// page enable
    dsi_set_cmdq(&array, 2, 1);
    MDELAY(10);
    read_reg_v2(0x04, buffer, 2);
    id = buffer[0]; */

    return 1;
else
	return 0;
	}


LCM_DRIVER hx8394_hd720_dsi_vdo_lcm_drv = 
{
	.name		    = "hx8394_hd720_dsi_vdo",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id     = lcm_compare_id,
#if (LCM_DSI_CMD_MODE)
	.update         = lcm_update,
#endif
};


