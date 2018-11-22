/**************************************************************************//*!
 *  @file           hal_motorDC.c
 *  @brief          [HAL] DC モータ・ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
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
// なし


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
static EHalMotorDC_t  g_target;


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
    g_target = EN_MOTOR_DC_GPIO13;
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

    pinMode( g_target, PWM_OUTPUT );
    pwmSetMode( PWM_MODE_MS );
    pwmSetClock( 192 );
    pwmSetRange( 100 );

    return EN_TRUE;
}


/**************************************************************************//*!
 * @brief     DC モータを初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalMotorDC_Init(
    EHalMotorDC_t   which   ///< [in] 対象のモータ
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    InitParam();

    g_target = which;
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
 * @brief     DC モータを終了する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalMotorDC_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );

    HalMotorDC_SetPwmDuty( EN_MOTOR_STANDBY, 0 );
    return;
}


/**************************************************************************//*!
 * @brief     DC モータを回す。
 * @attention なし。
 * @note      PWM のカウントアップ・カウンタは 5kHz (= 0.2ms ) の速さでカウントアップ
 *            100 カウントアップ (= 20ms ) で 1 周期なので
 *            PWM 周波数は 50Hz ( 20ms x 50Hz = 1000ms = 1s )
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalMotorDC_SetPwmDuty(
    EHalMotorState_t    status, ///< [in] モータの状態
    double              rate    ///< [in] デューティ比 : 0.0% ～ 100.0% まで
){
    unsigned int        clock = 0;  // PWM のカウンタのカウントアップ周期を設定するために使用するパラメータ
    unsigned int        range = 0;  // PWM の周期を設定するために使用するパラメータ
    unsigned int        value = 0;

    DBG_PRINT_TRACE( "rate    = %d%% \n\r", rate );

    // PWM のカウンタのカウントアップ周期
    // 19.2MHz / clock(=3840) = 5kHz でカウントアップ ( 0.2ms )
    clock = 3840;   // 2 - 4095 まで

    // PWM 周期
    // 5kHz / range(=100) = 50Hz
    range = 100;    // 1 - 4096 まで

    // pwmWrite() にセットするのは 0~100 ではなくて 0~1024 の値をとる
    // とのことなので、値を変換する。
    value = 1024 * rate / 100;

    pwmSetClock( clock );
    pwmSetRange( range );

    if( status == EN_MOTOR_CCW ||
        status == EN_MOTOR_CW   )
    {
        pwmWrite( g_target, value );
    } else
    {
        pwmWrite( g_target, 0 );
    }

    return EN_TRUE;
}


#ifdef __cplusplus
    }
#endif

