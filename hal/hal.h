/**************************************************************************//*!
 *  @file           hal.h
 *  @brief          [HAL] 外部公開 API を宣言したヘッダファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *                  関数命名規則
 *                      通常         : Hal[デバイス名]_処理名()
 *                      割込ハンドラ : Hal[デバイス名]_IH_処理名()
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.06.05
 *************************************************************************** */

// 多重コンパイル抑止
#ifndef _HAL_H_
#define _HAL_H_


//********************************************************
/* include                                               */
//********************************************************
#include "hal_cmn.h"


//********************************************************
/*! @def                                                 */
//********************************************************
/* NULL */
#ifndef NULL
  #define NULL              (0)
#endif

/* OFF */
#ifndef OFF
  #define OFF               (0)
#endif

/* ON */
#ifndef ON
  #define ON                (1)
#endif

/* OK */
#ifndef OK
  #define OK                (1)
#endif

/* EOF */
#ifndef EOF
  #define EOF               (-1)
#endif


//********************************************************
/*! @enum                                                */
//********************************************************
//*************************************
// モード選択のための型
//*************************************
// LCD の動作モードに使用する型
typedef enum tagEHalLcdMode
{
    EN_LCD_CMD = 0,         ///< @var : コマンドモード設定の定義 ( RS = 0 )
    EN_LCD_DAT              ///< @var : データモード  設定の定義 ( RS = 1 )
} EHalLcdMode_t;


//*************************************
// ステータスの型
//*************************************
// モーターのステータスに使用する型
typedef enum tagEHalMotorState
{
    EN_MOTOR_STANDBY = 0,   ///< @var : スタンバイ (= 初期値 )
    EN_MOTOR_BRAKE,         ///< @var : ブレーキ中
    EN_MOTOR_CCW,           ///< @var : CCW 方向に回転中
    EN_MOTOR_CW,            ///< @var : CW  方向に回転中
    EN_MOTOR_STOP           ///< @var : 停止中
} EHalMotorState_t;


//*************************************
// デバイスを区別するための型
//*************************************
// プッシュ・スイッチの区別に使用する型
typedef enum tagEHalPushSw
{
    EN_PUSH_SW_0 = 0,               ///< @var : プッシュ・スイッチ 0
    EN_PUSH_SW_1,                   ///< @var : プッシュ・スイッチ 1
    EN_PUSH_SW_2                    ///< @var : プッシュ・スイッチ 2
} EHalPushSw_t;


//********************************************************
/*! @struct                                              */
//********************************************************
// なし


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
// I2C LCD API
EHalBool_t      HalI2cLcd_Init( void );
void            HalI2cLcd_Fini( void );
EHalBool_t      HalI2cLcd_Write( EHalLcdMode_t rs, unsigned char code );

// I2C PCA9685 API
EHalBool_t      HalI2cPca9685_Init( void );
void            HalI2cPca9685_Fini( void );
EHalBool_t      HalI2cPca9685_SetPwmDuty( unsigned char ch, EHalMotorState_t status, int rate );

// LED API
EHalBool_t      HalLed_Init( void );
void            HalLed_Fini( void );
void            HalLed_Set( unsigned char value );

// DC モータ API
EHalBool_t      HalMotorDC_Init( void );
void            HalMotorDC_Fini( void );
void            HalMotorDC_SetPwmDuty( EHalMotorState_t status, int rate );

// ステッピングモータ API
EHalBool_t      HalMotorST_Init( void );
void            HalMotorST_Fini( void );
void            HalMotorST_SetStatus( EHalMotorState_t status );
void            HalMotorST_SetAngle( EHalMotorState_t status, double angle );
void            HalMotorST_SetSpeed( unsigned int time_us );

// サーボモータ API
EHalBool_t      HalMotorSV_Init( void );
void            HalMotorSV_Fini( void );
void            HalMotorSV_SetPwmDuty( EHalMotorState_t status, int rate );

// プッシュ・スイッチ API
EHalBool_t      HalPushSw_Init( void );
void            HalPushSw_Fini( void );
EHalBool_t      HalPushSw_Get( EHalPushSw_t );

// SENSOR (ADC) ポテンショメータ API
EHalBool_t      HalSensorPm_Init( void );
void            HalSensorPm_Fini( void );
SHalSensor_t*   HalSensorPm_Get( void );


#endif /* _HAL_H_ */

