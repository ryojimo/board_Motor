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
#define MOTOR_DC_OUT    (18)


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

    pinMode( MOTOR_DC_OUT, PWM_OUTPUT );
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
    return;
}


/**************************************************************************//*!
 * @brief     DC モータを回す。
 * @attention なし。
 * @note      PWM のカウントアップ・カウンタは 100kHz (= 0.01ms ) の速さでカウントアップ
 *            PWM 周波数は 1kHz
 *            100 カウントアップで 1 周期 (= 1kHz )
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalMotorDC_SetPwmDuty(
    EHalMotorState_t    status, ///< [in] モータの状態
    int                 rate    ///< [in] デューティ比 : 0% ～ 100% まで
){
    unsigned int        clock = 0;  // PWM のカウンタのカウントアップ周期を設定するために使用するパラメータ
    unsigned int        range = 0;  // PWM の周期を設定するために使用するパラメータ
    unsigned int        value = 0;

    DBG_PRINT_TRACE( "rate    = %d%% \n\r", rate );

    // PWM のカウンタのカウントアップ周期
    // 19.2MHz / clock(=192) = 100kHz でカウントアップ
    clock = 192;    // 2 - 4095 まで

    // PWM 周期
    // 100kHz / range(=100) = 1kHz
    range = 100;    // 1 - 4096 まで

    // デューティ比 = value / range
    value = rate;

    pwmSetClock( clock );
    pwmSetRange( range );

    if( status == EN_MOTOR_CCW ||
        status == EN_MOTOR_CW   )
    {
        pwmWrite( MOTOR_DC_OUT, value );
    } else
    {
        pwmWrite( MOTOR_DC_OUT, 0 );
    }

    return;
}


#ifdef __cplusplus
    }
#endif

