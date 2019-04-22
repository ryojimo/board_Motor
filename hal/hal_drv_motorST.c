/**************************************************************************//*!
 *  @file           hal_motorST.c
 *  @brief          [HAL] ステッピングモータ・モータ・ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
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

//#define DBG_PRINT
#define MY_NAME "HAL"
#include "../app/log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
#define MOTOR_ST_A1        (22)
#define MOTOR_ST_A2        (27)
#define MOTOR_ST_B1        (17)
#define MOTOR_ST_B1        (4)


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
    digitalWrite( MOTOR_ST_A2, EN_HIGH );
    digitalWrite( MOTOR_ST_B2, EN_HIGH );

    digitalWrite( MOTOR_ST_A1, EN_HIGH );
    sleep( 0.01 );
    digitalWrite( MOTOR_ST_B1, EN_HIGH );
    sleep( 0.01 );
    digitalWrite( MOTOR_ST_A1, EN_LOW );
    sleep( 0.01 );
    digitalWrite( MOTOR_ST_B1, EN_LOW );
    sleep( 0.01 );
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
    digitalWrite( MOTOR_ST_A2, EN_HIGH );
    digitalWrite( MOTOR_ST_B2, EN_HIGH );

    digitalWrite( MOTOR_ST_B1, EN_HIGH );
    sleep( 0.01 );
    digitalWrite( MOTOR_ST_A1, EN_HIGH );
    sleep( 0.01 );
    digitalWrite( MOTOR_ST_B1, EN_LOW );
    sleep( 0.01 );
    digitalWrite( MOTOR_ST_A1, EN_LOW );
    sleep( 0.01 );
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
SetPosition(
    void  ///< [in] ナシ
){
    while( 1 )
    {
        StepCcw();
    }
    return;
}


#ifdef __cplusplus
    }
#endif

