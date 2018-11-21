/**************************************************************************//*!
 *  @file           hal_motorSV.c
 *  @brief          [HAL] サーボモータ・ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      01. PWM サイクル : 20ms (= 50Hz )
 *                  02. 制御パルス   : 0.5ms ～ 2.4ms
 *                      0.5 ms : duty= 2.5 % の時 -90°
 *                      1.45ms : duty= 7.25% の時   0°
 *                      2.4 ms : duty=12.0 % の時 +90°
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.08.11
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include <wiringPi.h>

#include "hal_cmn.h"
#include "hal.h"


//#define DBG_PRINT
#define MY_NAME "HAL"
#include "../app/log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
#define MOTOR_OUT     (18)


//********************************************************
/*! @enum                                                */
//********************************************************
// なし


//********************************************************
/*! @struct                                              */
//********************************************************
// なし


//********************************************************
/* モジュールグローバル変数                              */
//********************************************************
// なし


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         InitParam( void );
static EHalBool_t   InitReg( void );




/**************************************************************************//*!
 * @brief     ファイルスコープ内のグローバル変数を初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
InitParam(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     H/W レジスタを初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
InitReg(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );

    pinMode( MOTOR_OUT, PWM_OUTPUT );
    pwmSetMode( PWM_MODE_MS );
    pwmSetClock( 192 );
    pwmSetRange( 100 );

    return EN_TRUE;
}


/**************************************************************************//*!
 * @brief     サーボモータを初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalMotorSV_Init(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    InitParam();
    ret = InitReg();
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "Unable to initialize GPIO port. \n\r" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     サーボモータを終了する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalMotorSV_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );

    HalMotorSV_SetPwmDuty( EN_MOTOR_STANDBY, 0 );
    return;
}


/**************************************************************************//*!
 * @brief     サーボモータを回す。
 * @attention なし。
 * @note      PWM のカウントアップ・カウンタは 5kHz (= 0.2ms ) の速さでカウントアップ
 *            100 カウントアップ (= 20ms ) で 1 周期なので
 *            PWM 周波数は 50Hz ( 20ms x 50Hz = 1000ms = 1s )
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalMotorSV_SetPwmDuty(
    EHalMotorState_t    status, ///< [in] モータの状態
    double              rate    ///< [in] デューティ比 : 0.0% ～ 100.0% まで
){
    unsigned int        clock = 0;  // PWM のカウンタのカウントアップ周期を設定するために使用するパラメータ
    unsigned int        range = 0;  // PWM の周期を設定するために使用するパラメータ
    unsigned int        value = 0;

    DBG_PRINT_TRACE( "rate    = %d%% \n\r", rate );

    if( rate < 2.5 || 12 < rate )
    {
        return EN_FALSE;
    }

    // PWM のカウンタのカウントアップ周期
    // 19.2MHz / clock(=3840) = 5kHz でカウントアップ ( 0.2ms )
    clock = 3840;   // 2 - 4095 まで

    // PWM 周期
    // 5kHz / range(=100) = 50Hz
    range = 100;    // 1 - 4096 まで

    // pwmWrite() にセットするのは 0~100 ではなくて 0~1024 の値をとる
    // とのことなので、値を変換する。
    // デューティ比 = value / range
    value = 1024 * rate / 100;

    pwmSetClock( clock );
    pwmSetRange( range );

    if( status == EN_MOTOR_CCW ||
        status == EN_MOTOR_CW   )
    {
        pwmWrite( MOTOR_OUT, value );
    } else
    {
        pwmWrite( MOTOR_OUT, 0 );
    }

    return EN_TRUE;
}


#ifdef __cplusplus
    }
#endif

