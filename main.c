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
#include "./sys/sys.h"


#define DBG_PRINT
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
static void         Run_Version( void );

static void         Run_I2cLcd( int argc, char *argv[] );
static void         Run_Led( char* str );
static void         Run_MotorDC( char* str );
static void         Run_MotorST( int argc, char *argv[] );

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
    printf( "  -h, --help                  display the help menu. \n\r" );
    printf( "  -v, --version               display the version information. \n\r" );
    printf( "                                                               \n\r" );
    printf( "  -c, --i2clcd                control the (I2C) LCD.           \n\r" );
    printf( "    -x number, --dir_x=number                                  \n\r" );
    printf( "                              the value of x-axis.             \n\r" );
    printf( "    -y number, --dir_y=number                                  \n\r" );
    printf( "                              the value of y-axis.             \n\r" );
    printf( "    -s string, --string=string                                 \n\r" );
    printf( "                              the string to display on LCD.    \n\r" );
    printf("\x1b[32m");
    printf( "                              Ex) -c        -x      <number>  -y      <number>  -s       <string> \n\r" );
    printf( "                                  --i2clcd  --dir_x=<number>  --dir_y=<number>  --string=<string> \n\r" );
    printf("\x1b[39m");
    printf( "                                                               \n\r" );
    printf( "  -d number, --motordc=number control the DC motor.            \n\r" );

    printf( "  -e, --motorst               control the STEPPING motor.      \n\r" );
    printf( "    -d number, --deg=number                                    \n\r" );
    printf( "                              degree of rotation.              \n\r" );
    printf( "    -r {ccw|cw}, --rol={ccw|cw}                                \n\r" );
    printf( "                              direction of rotation.           \n\r" );
    printf( "                              ccw : left direction.            \n\r" );
    printf( "                              cw  : right direction.           \n\r" );
    printf("\x1b[32m");
    printf( "                              Ex) -e        -d    90  -r    left \n\r" );
    printf( "                                  --motorst --deg=90  --rol=left \n\r" );
    printf("\x1b[39m");


    printf( "  -e {left|right}, --motorst={left|right}                      \n\r" );
    printf( "                              control the STEPPING motor.      \n\r" );
    printf( "                              left  : left rotation.           \n\r" );
    printf( "                              right : right rotation.          \n\r" );
    printf( "                                                               \n\r" );
    printf( "  -l number, --led=number     control the LED.                 \n\r" );
    printf( "  -p [json], --sa_pm=[json]                                                  \n\r" );
    printf( "                              get the value of a sensor(A/D), Potentiometer. \n\r" );
    printf( "                              json : get the all values of json format.      \n\r" );
    printf( "\n\r" );

    return;
}


/**************************************************************************//*!
 * @brief     VERSION を表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_Version(
    void
){
    DBG_PRINT_TRACE( "\n\r" );
    printf( "Copyright (C) 2019 Uz Foundation, Inc. \n\r" );
    printf( "Licence: Free. \n\r" );
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
    int   argc,
    char  *argv[]
){
    int             opt = 0;
    const char      optstring[] = "s:x:y:";
    const struct    option longopts[] = {
      //{ *name,    has_arg,           *flag, val }, // 説明
        { "string", required_argument, NULL,  's' },
        { "dir_x",  required_argument, NULL,  'x' },
        { "dir_y",  required_argument, NULL,  'y' },
        { 0,        0,                 NULL,   0  }, // termination
    };
    int             longindex = 0;
    int             x = 0;
    int             y = 0;
    char            str[16];

    DBG_PRINT_TRACE( "argc    = %d \n\r", argc );
    DBG_PRINT_TRACE( "argv[0] = %s \n\r", argv[0] );
    DBG_PRINT_TRACE( "argv[1] = %s \n\r", argv[1] );
    DBG_PRINT_TRACE( "argv[2] = %s \n\r", argv[2] );
    DBG_PRINT_TRACE( "argv[3] = %s \n\r", argv[3] );
    DBG_PRINT_TRACE( "argv[4] = %s \n\r", argv[4] );
    DBG_PRINT_TRACE( "argv[5] = %s \n\r", argv[5] );
    DBG_PRINT_TRACE( "argv[6] = %s \n\r", argv[6] );

    while( 1 )
    {
        opt = getopt_long( argc, argv, optstring, longopts, &longindex );
        DBG_PRINT_TRACE( "optind = %d \n\r", optind );
        DBG_PRINT_TRACE( "opt    = %c \n\r", opt );

        if( opt == -1 )   // 処理するオプションが無くなった場合
        {
            break;
        } else if( opt == '?' )  // optstring で指定していない引数が見つかった場合
        {
            DBG_PRINT_TRACE( "optopt = %c \n\r", optopt );
            break;
        }

        switch( opt )
        {
        case 's': DBG_PRINT_TRACE( "optarg = %s \n\r", optarg ); strncpy( str, (const char*)optarg, 16 ); break;
        case 'x': DBG_PRINT_TRACE( "optarg = %s \n\r", optarg ); x = strtol( (const char*)optarg, NULL, 10 ); break;
        case 'y': DBG_PRINT_TRACE( "optarg = %s \n\r", optarg ); y = strtol( (const char*)optarg, NULL, 10 ); break;
        default:
            DBG_PRINT_ERROR( "invalid command/option. : \"%s\" \n\r", argv[1] );
            Run_Help();
        break;
        }
    }

    DBG_PRINT_TRACE( "(x, y) = (%d, %d) \n\r", x, y );
    DBG_PRINT_TRACE( "str    = %s \n\r", str );
    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( "                " );
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "                " );

    AppIfLcd_CursorSet( x, y );
    AppIfLcd_Printf( str );

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
    EHalBool_t      flag = EN_FALSE;


    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( 0 == strncmp( str, "standby", strlen("standby") ) )
    {
        HalMotorDC_SetPwmDuty( EN_MOTOR_STANDBY, 0 );
        HalMotorDC2_SetPwmDuty( EN_MOTOR_STANDBY, 0 );
    } else if( 0 == strncmp( str, "pm", strlen("pm") ) )
    {
        while( EN_FALSE == HalPushSw_Get( EN_PUSH_SW_0 ) )
        {
            value = HalSensorPm_Get();
            DBG_PRINT_TRACE( "value->cur_rate = %3d %% \n", value->cur_rate );

            AppIfLcd_CursorSet( 0, 1 );
            AppIfLcd_Printf( "%3d%%", value->cur_rate );

            // SW1 が押されたら、MotorDC  の PWM Duty を変更する
            // SW2 が押されたら、MotorDC2 の PWM Duty を変更する
            if( EN_TRUE == HalPushSw_Get( EN_PUSH_SW_1 ) )
            {
                flag = EN_TRUE;
            } else if( EN_TRUE == HalPushSw_Get( EN_PUSH_SW_2 ) )
            {
                flag = EN_FALSE;
            }

            if( flag == EN_TRUE )
            {
                HalMotorDC_SetPwmDuty( EN_MOTOR_CW, value->cur_rate );
            } else
            {
                HalMotorDC2_SetPwmDuty( EN_MOTOR_CW, value->cur_rate );
            }
        }

        HalMotorDC_SetPwmDuty( EN_MOTOR_STOP, 0 );
        HalMotorDC2_SetPwmDuty( EN_MOTOR_STOP, 0 );
    } else if( 0 != isdigit( str[0] ) )
    {
        data = atoi( (const char*)str );
        DBG_PRINT_TRACE( "data = %d \n", data );
        HalMotorDC_SetPwmDuty( EN_MOTOR_CW, data );
        HalMotorDC2_SetPwmDuty( EN_MOTOR_CW, data );
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
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "d:r:";
    const struct    option longopts[] = {
      //{ *name,  has_arg,           *flag, val }, // 説明
        { "rol",  required_argument, NULL,  'r' },
        { "deg",  required_argument, NULL,  'd' },
        { 0,      0,                 NULL,   0  }, // termination
    };
    int             longindex = 0;
    int             deg = 0;
    char            rol[16];

    while( 1 )
    {
        opt = getopt_long( argc, argv, optstring, longopts, &longindex );
        DBG_PRINT_TRACE( "optind = %d \n\r", optind );
        DBG_PRINT_TRACE( "opt    = %c \n\r", opt );

        if( opt == -1 )   // 処理するオプションが無くなった場合
        {
            break;
        } else if( opt == '?' )  // optstring で指定していない引数が見つかった場合
        {
            DBG_PRINT_TRACE( "optopt = %c \n\r", optopt );
            break;
        }

        switch( opt )
        {
        case 'd': DBG_PRINT_TRACE( "optarg = %s \n\r", optarg ); deg = strtol( (const char*)optarg, NULL, 10 ); break;
        case 'r': DBG_PRINT_TRACE( "optarg = %s \n\r", optarg ); strncpy( rol, (const char*)optarg, 16 ); break;
        default:
            DBG_PRINT_ERROR( "invalid command/option. : \"%s\" \n\r", argv[1] );
            Run_Help();
        break;
        }
    }

    DBG_PRINT_TRACE( "deg = %d \n\r", deg );
    DBG_PRINT_TRACE( "rol = %s \n\r", rol );

    if( 0 == strncmp( rol, "ccw", strlen("ccw") ) )
    {
        HalMotorST_SetPosition( EN_MOTOR_CCW, deg );
    } else if( 0 == strncmp( rol, "cw", strlen("cw") ) )
    {
        HalMotorST_SetPosition( EN_MOTOR_CW, deg );
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", rol );
        goto err;
    }

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
    const char      optstring[] = "hvc:d:e:l:p::x:y:z:";
    const struct    option longopts[] = {
      //{ *name,           has_arg,           *flag, val }, // 説明
        { "help",          no_argument,       NULL,  'h' },
        { "version",       no_argument,       NULL,  'v' },
        { "i2clcd",        required_argument, NULL,  'c' },
        { "motordc",       required_argument, NULL,  'd' },
        { "motorst",       required_argument, NULL,  'e' },
        { "led",           required_argument, NULL,  'l' },
        { "sa_pm",         optional_argument, NULL,  'p' },
        { 0,               0,                 NULL,   0  }, // termination
    };
    int longindex = 0;

    Sys_Init();

    DBG_PRINT_TRACE( "argc    = %d \n\r", argc );
    DBG_PRINT_TRACE( "argv[0] = %s \n\r", argv[0] );
    DBG_PRINT_TRACE( "argv[1] = %s \n\r", argv[1] );
    DBG_PRINT_TRACE( "argv[2] = %s \n\r", argv[2] );
    DBG_PRINT_TRACE( "argv[3] = %s \n\r", argv[3] );


    AppIfLcd_Clear();
    AppIfLcd_Ctrl( 1, 0, 0 );
    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( "cmd:%s", argv[1] );

    while( 1 )
    {
        opt = getopt_long( argc, argv, optstring, longopts, &longindex );
//      opt = getopt( argc, argv, optstring );
        DBG_PRINT_TRACE( "optind = %d \n\r", optind );
        DBG_PRINT_TRACE( "opt    = %c \n\r", opt );

        if( opt == -1 )   // 処理するオプションが無くなった場合
        {
            break;
        } else if( opt == '?' )  // optstring で指定していない引数が見つかった場合
        {
            DBG_PRINT_TRACE( "optopt = %c \n\r", optopt );
            break;
        } else if( opt == 'c' )
        {
            optind = 1;
            argc = argc - optind;
            argv = argv + optind;
            Run_I2cLcd( argc, argv );
            break;
        } else if( opt == 'e' )
        {
            optind = 1;
            argc = argc - optind;
            argv = argv + optind;
            Run_MotorST( argc, argv );
            break;
        }

        switch( opt )
        {
        case 'h': Run_Help(); break;
        case 'v': Run_Version(); break;
        case 'd': Run_MotorDC( optarg ); break;
        case 'l': Run_Led( optarg ); break;
        case 'p': Run_Sa_Pm( optarg ); break;
        default:
            DBG_PRINT_ERROR( "invalid command/option. : \"%s\" \n\r", argv[1] );
            Run_Help();
        break;
        }
    }

    Sys_Fini();
    return 0;
}


#ifdef __cplusplus
    }
#endif
