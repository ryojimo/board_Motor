/**************************************************************************//*!
 *  @file           hal_motorST.c
 *  @brief          [HAL] ステッピングモータ・モータ・ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             http://fum125.hatenablog.com/entry/2018/06/26/221558
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2019.04.22
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

#define DBG_PRINT
#define MY_NAME "HAL"
#include "../app/log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
#define MOTOR_ST_A1        (22)
#define MOTOR_ST_A2        (27)
#define MOTOR_ST_B1        (17)
#define MOTOR_ST_B2        (4)

// AOR = Angle Of Rotation
// PULSE = 1 PULSE で 0.17578125°回転。
#define ANGLE_MIN               (1.8)       ///< @def :  指定可能な最小の回転角度
#define PULSE_ANGLE_360         (200)       ///< @def : 360°回転するパルス数 ( 360 / 1.8 = 200 )
#define PULSE_ERROR             (1)         ///< @def : 誤差


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

static void         StepCw( void );
static void         StepCcw( void );




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
    void    ///< [in] ナシ
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

    pinMode( MOTOR_ST_A1, OUTPUT );
    pinMode( MOTOR_ST_A2, OUTPUT );
    pinMode( MOTOR_ST_B1, OUTPUT );
    pinMode( MOTOR_ST_B2, OUTPUT );

    return EN_TRUE;
}


/**************************************************************************//*!
 * @brief     ステッピング・モータを初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalMotorST_Init(
    void  ///< [in] ナシ
){
    EHalBool_t          ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    InitParam();
    ret = InitReg();
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "Unable to initialize GPIO port. \n\r" );
        return ret;
    }

    digitalWrite( MOTOR_ST_A2, EN_HIGH );
    digitalWrite( MOTOR_ST_B2, EN_HIGH );
    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     ステッピング・モータを終了する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalMotorST_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     CW に 1 Step 移動する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
StepCw(
    void  ///< [in] ナシ
){
//    DBG_PRINT_TRACE( "\n\r" );
    digitalWrite( MOTOR_ST_A1, EN_HIGH );
    usleep( 5 * 1000 );
    digitalWrite( MOTOR_ST_A1, EN_LOW );

    digitalWrite( MOTOR_ST_B1, EN_HIGH );
    usleep( 5 * 1000 );
    digitalWrite( MOTOR_ST_B1, EN_LOW );

    digitalWrite( MOTOR_ST_A2, EN_HIGH );
    usleep( 5 * 1000 );
    digitalWrite( MOTOR_ST_A2, EN_LOW );

    digitalWrite( MOTOR_ST_B2, EN_HIGH );
    usleep( 5 * 1000 );
    digitalWrite( MOTOR_ST_B2, EN_LOW );
    return;
}


/**************************************************************************//*!
 * @brief     CCW に 1 Step 移動する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
StepCcw(
    void  ///< [in] ナシ
){
//    DBG_PRINT_TRACE( "\n\r" );
    digitalWrite( MOTOR_ST_B2, EN_HIGH );
    usleep( 5 * 1000 );
    digitalWrite( MOTOR_ST_B2, EN_LOW );

    digitalWrite( MOTOR_ST_A2, EN_HIGH );
    usleep( 5 * 1000 );
    digitalWrite( MOTOR_ST_A2, EN_LOW );

    digitalWrite( MOTOR_ST_B1, EN_HIGH );
    usleep( 5 * 1000 );
    digitalWrite( MOTOR_ST_B1, EN_LOW );

    digitalWrite( MOTOR_ST_A1, EN_HIGH );
    usleep( 5 * 1000 );
    digitalWrite( MOTOR_ST_A1, EN_LOW );
    return;
}


/**************************************************************************//*!
 * @brief     ステッピング・モータの状態と回転角度をセットする。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalMotorST_SetPosition(
    EHalDirection_t   dir,    ///< [in] 回転方向
    unsigned int      deg     ///< [in] 回転角度
){
    static  int       cnt = 1;    // 回転角度の誤差を修正するためのカウンタ
                                  // この関数が 3 回呼ばれるたびに value の値を + PULSE_ERROR して回転角度の誤差を正す
    unsigned int      value = 0;

    DBG_PRINT_TRACE( "dir = %d \n\r", dir );
    DBG_PRINT_TRACE( "deg = %d \n\r", deg );

    value = PULSE_ANGLE_360 * deg / 360 / 4;

    cnt = ( cnt + 1 ) % 3;
    if( cnt == 0 )
    {
        value += PULSE_ERROR;
    }

    if( dir == EN_LEFT )
    {
        while( value-- )
        {
            StepCcw();
        }
    } else if( dir == EN_RIGHT )
    {
        while( value-- )
        {
            StepCw();
        }
    }
    return;
}


#ifdef __cplusplus
    }
#endif

