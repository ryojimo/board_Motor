/**************************************************************************//*!
 *  @file           main.c
 *  @brief          main() を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.12.03
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <getopt.h>

#include "./app/if_lcd/if_lcd.h"
#include "./hal/hal.h"


//#define DBG_PRINT
#define MY_NAME "MAI"
#include "./app/log/log.h"


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
// getopt() で使用
extern char *optarg;
extern int  optind, opterr, optopt;


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         Run_Help( void );

static void         Run_I2cLcd( char* str );
static void         Run_I2cPca9685( char* str );
static void         Run_Led( char* str );
static void         Run_MotorDC( char* str );
static void         Run_MotorST( char* str );
static void         Run_MotorSV( char* str );

static void         Run_Sa_Pm( char* str );




/**************************************************************************//*!
 * @brief     HELP を表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_Help(
    void
){
    DBG_PRINT_TRACE( "\n\r" );

    printf( " COMMAND                   : DESCRIPTION                                    \n\r" );
    printf( "============================================================================\n\r" );
    printf( " -h, --help                : display the command option list.               \n\r" );
    printf( "                                                                            \n\r" );
    printf( " -c, --i2clcd <value>      : Control the (I2C) LCD.                         \n\r" );
    printf( " -g, --i2cpca9685 <value>  : Control the (I2C) PCA9685.                     \n\r" );
    printf( "                                                                            \n\r" );
    printf( " -d, --motordc <value>     : Control the DC motor.                          \n\r" );
    printf( " -e, --motorst <value>     : Control the STEPPING motor.                    \n\r" );
    printf( " -f, --motorsv <value>     : Control the SAVO motor.                        \n\r" );
    printf( "                                                                            \n\r" );
    printf( " -l, --led <value>         : Control the LED.                               \n\r" );
    printf( " -p, --sa_pm               : Get the value of a sensor(A/D), Potentiometer. \n\r" );
    printf( "                      json : value of json format.                          \n\r" );
    printf( "\n\r" );

    return;
}


/**************************************************************************//*!
 * @brief     I2C LCD を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_I2cLcd(
    char*  str     ///< [in] 文字列
){
    DBG_PRINT_TRACE( "str = %s \n\r", str );

    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( str );
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "                " );

    return;
}


/**************************************************************************//*!
 * @brief     I2C PCA9685 を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_I2cPca9685(
    char*  str     ///< [in] 文字列
){
    DBG_PRINT_TRACE( "str = %s \n\r", str );

    HalI2cPca9685_SetPwm( 1, 0, 120 );

    return;
}


/**************************************************************************//*!
 * @brief     LED を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_Led(
    char*           str     ///< [in] 文字列
){
    unsigned int    num;
    DBG_PRINT_TRACE( "str = %s \n\r", str );

    sscanf( str, "%X", &num );
    HalLed_Set( num );

    return;
}


/**************************************************************************//*!
 * @brief     DC MOTOR を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_MotorDC(
    char*           str     ///< [in] 文字列
){
    int             data = 0;
    SHalSensor_t*   value;


    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( 0 == strncmp( str, "standby", strlen("standby") ) )
    {
        HalMotorDC_SetPwmDuty( EN_MOTOR_STANDBY, 0 );
    } else if( 0 == strncmp( str, "pm", strlen("pm") ) )
    {
        while( EN_FALSE == HalPushSw_Get( EN_PUSH_SW_0 ) )
        {
            value = HalSensorPm_Get();

            AppIfLcd_CursorSet( 0, 1 );
            AppIfLcd_Printf( "%3d %%", value->cur_rate );
            HalMotorDC_SetPwmDuty( EN_MOTOR_CW, value->cur_rate );
        }
    } else if( 0 != isdigit( str[0] ) )
    {
        data = atoi( (const char*)str );
        DBG_PRINT_TRACE( "data = %d \n", data );
        HalMotorDC_SetPwmDuty( EN_MOTOR_CW, data );
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

//  usleep( 1000 * 1000 );  // 2s 待つ

err :
    return;
}


/**************************************************************************//*!
 * @brief     STEPPING MOTOR を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_MotorST(
    char*   str     ///< [in] 文字列
){
    int     data = 0;

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    data = atoi( (const char*)str );
    DBG_PRINT_TRACE( "data = %d \n", data );

    HalMotorST_SetStatus( EN_MOTOR_CW );
//  usleep( 1000 * 1000 );  // 2s 待つ

    return;
}


/**************************************************************************//*!
 * @brief     SERVO MOTOR を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_MotorSV(
    char*           str     ///< [in] 文字列
){
    int             data = 0;
    SHalSensor_t*   value;

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( 0 == strncmp( str, "standby", strlen("standby") ) )
    {
        HalMotorSV_SetPwmDuty( EN_MOTOR_STANDBY, 0 );
    } else if( 0 == strncmp( str, "pm", strlen("pm") ) )
    {
        while( EN_FALSE == HalPushSw_Get( EN_PUSH_SW_0 ) )
        {
            value = HalSensorPm_Get();

            data = ( 12 - 3 ) * value->cur_rate / 100;
            data = 3 + data;

            AppIfLcd_CursorSet( 0, 1 );
            AppIfLcd_Printf( "%3d %%", data );
            HalMotorSV_SetPwmDuty( EN_MOTOR_CW, data );
        }
    } else if( 0 != isdigit( str[0] ) )
    {
        data = atoi( (const char*)str );
        DBG_PRINT_TRACE( "data = %d \n", data );
        HalMotorSV_SetPwmDuty( EN_MOTOR_CW, data );
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

//  usleep( 1000 * 1000 );  // 2s 待つ

err :
    return;
}


/**************************************************************************//*!
 * @brief     ポテンショメーターを実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_Sa_Pm(
    char*           str     ///< [in] 文字列
){
    SHalSensor_t*   data;

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( str == NULL )
    {
        data = HalSensorPm_Get();
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%3d %%", data->cur_rate );
        printf( "%3d", data->cur_rate );
    } else if( 0 == strncmp( str, "json", strlen("json") ) )
    {
        data = HalSensorPm_Get();

        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%3d %%", data->cur_rate );

        printf( "{ " );
        printf( "  \"sensor\": \"sa_pm\"," );
        printf( "  \"value\": %3d,", data->cur_rate );
        printf( "}" );
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

err :
    return;
}


/**************************************************************************//*!
 * @brief     メイン
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
int main(int argc, char *argv[ ])
{
    int             opt = 0;
    const char      optstring[] = "hc:d:e:f:g:l:p::t::";
    const struct    option longopts[] = {
      //{ *name,         has_arg,           *flag, val }, // 説明
        { "help",        no_argument,       NULL,  'h' },
        { "i2clcd",      required_argument, NULL,  'c' },
        { "i2cpca9685",  required_argument, NULL,  'g' },
        { "motordc",     required_argument, NULL,  'd' },
        { "motorst",     required_argument, NULL,  'e' },
        { "motorsv",     required_argument, NULL,  'f' },
        { "led",         required_argument, NULL,  'l' },
        { "sa_pm",       optional_argument, NULL,  'p' },
        { 0,             0,                 NULL,   0  }, // termination
    };
    int longindex = 0;

    HalCmnGpio_Init();
    HalCmnI2c_Init();
    HalCmnSpi_Init();

    HalI2cLcd_Init();
    HalI2cPca9685_Init();
    HalLed_Init();
    HalMotorDC_Init();
    HalMotorST_Init();
    HalMotorSV_Init();
    HalPushSw_Init();

    // SENSOR (ADC)
    HalSensorPm_Init();

    while( 1 )
    {
        opt = getopt_long( argc, argv, optstring, longopts, &longindex );
//      opt = getopt( argc, argv, optstring );
        DBG_PRINT_TRACE( "opt = %c \n\r", opt );

        if( opt == -1 )   // 処理するオプションが無くなった場合
        {
            break;
        }

        if( opt == '?' )  // optstring で指定していない引数が見つかった場合
        {
            DBG_PRINT_TRACE( "optopt = %c \n\r", optopt );
            break;
        }

        switch( opt )
        {
        case 'h': Run_Help(); break;
        case 'c': Run_I2cLcd( optarg ); break;
        case 'd': Run_MotorDC( optarg ); break;
        case 'e': Run_MotorST( optarg ); break;
        case 'f': Run_MotorSV( optarg ); break;
        case 'g': Run_I2cPca9685( optarg ); break;
        case 'l': Run_Led( optarg ); break;
        case 'p': Run_Sa_Pm( optarg ); break;
        default:
            DBG_PRINT_ERROR( "invalid command/option. : \"%s\" \n\r", argv[1] );
            Run_Help();
        break;
        }
    }

    HalI2cLcd_Fini();
    HalI2cPca9685_Fini();
    HalLed_Fini();
    HalMotorDC_Fini();
    HalMotorST_Fini();
    HalMotorSV_Fini();
    HalPushSw_Fini();

    // SENSOR (ADC)
    HalSensorPm_Fini();

    HalCmnGpio_Fini();
    HalCmnI2c_Fini();
    HalCmnSpi_Fini();

    return 0;
}


#ifdef __cplusplus
    }
#endif
