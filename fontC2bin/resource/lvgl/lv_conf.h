#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>
//#include "lvgl_support.h"

/* Color Settings */

#define LV_HOR_RES_MAX   (800)
#define LV_VER_RES_MAX   (480)
#define LV_COLOR_DEPTH   32 // 色域
#define LV_COLOR_16_SWAP 0  // 色域修改 0:不使能
/*Enable more complex drawing routines to manage screens transparency.
 *Can be used if the UI is above another layer, e.g. an OSD menu or video player.
 *Requires `LV_COLOR_DEPTH = 32` colors and the screen's `bg_opa` should be set to non LV_OPA_COVER value*/
#define LV_COLOR_SCREEN_TRANSP 0
/* Adjust color mix functions rounding. GPUs might calculate color mix (blending) differently.
 * 0: round down, 64: round up from x.75, 128: round up from half, 192: round up from x.25, 254: round up */
#define LV_COLOR_MIX_ROUND_OFS (LV_COLOR_DEPTH == 32 ? 0 : 128) // 根据色域调整色彩混合选项 如:32bit 转 16位
/*Images pixels with this color will not be drawn if they are chroma keyed)*/
#define LV_COLOR_CHROMA_KEY lv_color_hex(0x00ff00) /*pure green*/

/* Memory Settings */

#define LV_MEM_CUSTOM 0
#if LV_MEM_CUSTOM == 0
/* Size of the memory available for `lv_mem_alloc` in bytes (>= 2kB)*/
// #define LV_MEM_SIZE (320U * 1024U) /*[bytes]*/
#define LV_MEM_SIZE (0x400000) /*2MB[bytes]*/
#else                          /* LV_MEM_CUSTOM */
// #define LV_MEM_CUSTOM_INCLUDE "FreeRTOS.h"
// #define LV_MEM_CUSTOM_ALLOC   pvPortMalloc
// #define LV_MEM_CUSTOM_FREE    vPortFree
// #define LV_MEM_CUSTOM_REALLOC realloc
#define LV_MEM_CUSTOM_INCLUDE "malloc.h"
#define LV_MEM_CUSTOM_ALLOC   MY_MALLOC
#define LV_MEM_CUSTOM_FREE    MY_FREE
#define LV_MEM_CUSTOM_REALLOC MY_REALLOC
#endif /* LV_MEM_CUSTOM */
/*Number of the intermediate memory buffer used during rendering and other internal processing mechanisms.
 *You will see an error log message if there wasn't enough buffers. */
#define LV_MEM_BUF_MAX_NUM 120 /* 16 */ /* 缓冲区数量 */
/*Use the standard `memcpy` and `memset` instead of LVGL's own functions. (Might or might not be faster).*/
#define LV_MEMCPY_MEMSET_STD 1 /* 是否使用标准的C库函数 */

/* HAL Settings */

#define LV_DISP_DEF_REFR_PERIOD  30 /*[ms] 刷新周期*/         // 20
#define LV_INDEV_DEF_READ_PERIOD 30 /*[ms] 输入设备读取周期*/ // 30
#define LV_TICK_CUSTOM           0
#if LV_TICK_CUSTOM
#define LV_TICK_CUSTOM_INCLUDE       "Arduino.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())
#endif /* LV_TICK_CUSTOM */

/*
 * LV_DPI_DEF 注意这里，虽然LVGL的作者说这个没这么重要，但他会严重影响到LVGL的动画效果
 * 你应该进行DPI的手动计算，例如800x480分辨率7英寸的屏幕，那么 DPI = ((√800*480) / 1.44) ≈ 89
 */
#define LV_DPI_DEF 89 /*[px/inch]*/ // 130

/* Drawing Configuration */

/*Enable complex draw engine.
 *Required to draw shadow, gradient, rounded corners, circles, arc, skew lines, image transformations or any masks*/
#define LV_DRAW_COMPLEX 0 // 启用复杂绘图引擎(绘制阴影，梯度，圆角，圆，弧，斜线，图像转换或任何蒙版)
#if LV_DRAW_COMPLEX

/*Allow buffering some shadow calculation.
 *LV_SHADOW_CACHE_SIZE is the max. shadow size to buffer, where shadow size is `shadow_width + radius`
 *Caching has LV_SHADOW_CACHE_SIZE^2 RAM cost*/
#define LV_SHADOW_CACHE_SIZE 0 /* 阴影计算缓存 */ // 0

/* Set number of maximally cached circle data.
 * The circumference of 1/4 circle are saved for anti-aliasing
 * radius * 4 bytes are used per circle (the most often used radiuses are saved)
 * 0: to disable caching */
#define LV_CIRCLE_CACHE_SIZE 4 /* 设置最大的环形线束数据的缓存圈数 */ //4
#endif                         /* LV_DRAW_COMPLEX */

/*Default image cache size. Image caching keeps the images opened.
 *If only the built-in image formats are used there is no real advantage of caching. (I.e. if no new image decoder is added)
 *With complex image decoders (e.g. PNG or JPG) caching can save the continuous open/decode of images.
 *However the opened images might consume additional RAM.
 *0: to disable caching*/
#define LV_IMG_CACHE_DEF_SIZE 1 /* 图像解码缓存 PNG JPEG需要使用该缓存 */

/*Number of stops allowed per gradient. Increase this to allow more stops.
 *This adds (sizeof(lv_color_t) + 1) bytes per additional stop*/
#define LV_GRADIENT_MAX_STOPS 2 /* 允许进行颜色渐变的次数 */ // 2

/*Default gradient buffer size.
 *When LVGL calculates the gradient "maps" it can save them into a cache to avoid calculating them again.
 *LV_GRAD_CACHE_DEF_SIZE sets the size of this cache in bytes.
 *If the cache is too small the map will be allocated only while it's required for the drawing.
 *0 mean no caching.*/
#define LV_GRAD_CACHE_DEF_SIZE 0 /* 梯度缓冲区大小 */

/*Allow dithering the gradients (to achieve visual smooth color gradients on limited color depth display)
 *LV_DITHER_GRADIENT implies allocating one or two more lines of the object's rendering surface
 *The increase in memory consumption is (32 bits * object width) plus 24 bits * object width if using error diffusion */
#define LV_DITHER_GRADIENT 0 /* 渐变抖动 */
#if LV_DITHER_GRADIENT
/*Add support for error diffusion dithering.
 *Error diffusion dithering gets a much better visual result, but implies more CPU consumption and memory when drawing.
 *The increase in memory consumption is (24 bits * object's width)*/
#define LV_DITHER_ERROR_DIFFUSION 1 /* 错误扩散渐变抖动宽度 */
#endif                              /* LV_DITHER_GRADIENT */

/*Maximum buffer size to allocate for rotation.
 *Only used if software rotation is enabled in the display driver.*/
#define LV_DISP_ROT_MAX_BUF (10U * 1024U) // 旋转功能缓存区大小

/* GPU Configuration */
/*Use NXP's PXP GPU iMX RTxxx platforms*/
#define LV_USE_GPU_NXP_PXP 0
#if LV_USE_GPU_NXP_PXP
#define LV_USE_GPU_NXP_PXP_AUTO_INIT 1
#endif /* LV_USE_GPU_NXP_PXP */

#define LV_USE_GPU_SDL 0
#if LV_USE_GPU_SDL
#define LV_GPU_SDL_INCLUDE_PATH      <SDL2/SDL.h>
#define LV_GPU_SDL_LRU_SIZE          (1024 * 1024 * 8)
#define LV_GPU_SDL_CUSTOM_BLEND_MODE (SDL_VERSION_ATLEAST(2, 0, 6))
#endif /* LV_USE_GPU_SDL */

/* Logging Configuration */

#define LV_USE_LOG 0
#if LV_USE_LOG
/*How important log should be added:
 *LV_LOG_LEVEL_TRACE       A lot of logs to give detailed information
 *LV_LOG_LEVEL_INFO        Log important events
 *LV_LOG_LEVEL_WARN        Log if something unwanted happened but didn't cause a problem
 *LV_LOG_LEVEL_ERROR       Only critical issue, when the system may fail
 *LV_LOG_LEVEL_USER        Only logs added by the user
 *LV_LOG_LEVEL_NONE        Do not log anything*/
#define LV_LOG_LEVEL            LV_LOG_LEVEL_WARN
#define LV_LOG_PRINTF           1
#define LV_LOG_TRACE_MEM        1
#define LV_LOG_TRACE_TIMER      1
#define LV_LOG_TRACE_INDEV      1
#define LV_LOG_TRACE_DISP_REFR  1
#define LV_LOG_TRACE_EVENT      1
#define LV_LOG_TRACE_OBJ_CREATE 1
#define LV_LOG_TRACE_LAYOUT     1
#define LV_LOG_TRACE_ANIM       1
#endif /* LV_USE_LOG */

/* Asserts Configuration */

/*Enable asserts if an operation is failed or an invalid data is found.
 *If LV_USE_LOG is enabled an error message will be printed on failure*/
#define LV_USE_ASSERT_NULL          1 /*Check if the parameter is NULL. (Very fast, recommended)*/
#define LV_USE_ASSERT_MALLOC        1 /*Checks is the memory is successfully allocated or no. (Very fast, recommended)*/
#define LV_USE_ASSERT_STYLE         0 /*Check if the styles are properly initialized. (Very fast, recommended)*/
#define LV_USE_ASSERT_MEM_INTEGRITY 0 /*Check the integrity of `lv_mem` after critical operations. (Slow)*/
#define LV_USE_ASSERT_OBJ           0 /*Check the object's type and existence (e.g. not deleted). (Slow)*/
#define LV_ASSERT_HANDLER_INCLUDE   <stdint.h>
#define LV_ASSERT_HANDLER \
    while (1)             \
        ;

/* Other Features Configuration */

#define LV_USE_PERF_MONITOR 0 /* 显示CPU使用帧率 */
#if LV_USE_PERF_MONITOR
#define LV_USE_PERF_MONITOR_POS LV_ALIGN_BOTTOM_RIGHT
#endif                       /* LV_USE_PERF_MONITOR */
#define LV_USE_MEM_MONITOR 0 /* 当LV_MEM_CUSTOM=0时,显示已用内存与内存碎片 */
#if LV_USE_MEM_MONITOR
#define LV_USE_MEM_MONITOR_POS LV_ALIGN_BOTTOM_LEFT
#endif /* LV_USE_MEM_MONITOR */

/*1: Draw random colored rectangles over the redrawn areas*/
#define LV_USE_REFR_DEBUG 0 /* 修改 0 */

#define LV_SPRINTF_CUSTOM 0
#if LV_SPRINTF_CUSTOM
#define LV_SPRINTF_INCLUDE <stdio.h>
#define lv_snprintf        snprintf
#define lv_vsnprintf       vsnprintf
#else /* LV_SPRINTF_CUSTOM */
#define LV_SPRINTF_USE_FLOAT 1
#endif /* LV_SPRINTF_CUSTOM */
#define LV_USE_USER_DATA 1
#define LV_ENABLE_GC     0
#if LV_ENABLE_GC
#define LV_GC_INCLUDE "gc.h"
#endif /* LV_ENABLE_GC */

/* Compiler Settings */

/*Define a custom attribute to `lv_tick_inc` function  MSB为大端系统 LSB为小端系统*/
#define LV_BIG_ENDIAN_SYSTEM 0

/*Define a custom attribute to `lv_timer_handler` function*/
#define LV_ATTRIBUTE_TICK_INC

/*Define a custom attribute to `lv_timer_handler` function*/
#define LV_ATTRIBUTE_TIMER_HANDLER

/*Define a custom attribute to `lv_disp_flush_ready` function*/
#define LV_ATTRIBUTE_FLUSH_READY

/*Required alignment size for buffers 缓冲区对齐大小*/
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE 32//64

/*Will be added where memories needs to be aligned (with -Os data might not be aligned to boundary by default).
 * E.g. __attribute__((aligned(4)))*/
#define LV_ATTRIBUTE_MEM_ALIGN

/*Attribute to mark large constant arrays for example font's bitmaps 标记大型常量数组*/
#ifndef LV_ATTRIBUTE_LARGE_CONST
#define LV_ATTRIBUTE_LARGE_CONST
#endif /* LV_ATTRIBUTE_LARGE_CONST */

/*Compiler prefix for a big array declaration in RAM 大数组申明的前缀*/
#define LV_ATTRIBUTE_LARGE_RAM_ARRAY

/*Place performance critical functions into a faster memory (e.g RAM) 将关键信息放入运行速度更快的区域*/
#define LV_ATTRIBUTE_FAST_MEM

/*Prefix variables that are used in GPU accelerated operations, often these need to be placed in RAM sections that are DMA accessible*/
/* 可进行GPU加速的前缀,放在DMA可以方位的区域 */
#define LV_ATTRIBUTE_DMA

#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning

/*Extend the default -32k..32k coordinate range to -4M..4M by using int32_t for coordinates instead of int16_t*/
/* 扩展坐标范围 */
#define LV_USE_LARGE_COORD 0

/* Font Usage */

#define LV_FONT_MONTSERRAT_8  1
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 0
#define LV_FONT_MONTSERRAT_14 0
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0

/*Demonstrate special features 特殊字体*/
#define LV_FONT_MONTSERRAT_12_SUBPX      0
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0 /*bpp = 3*/
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0 /*Hebrew, Arabic, Persian letters and all their forms*/
#define LV_FONT_SIMSUN_16_CJK            0 /*1000 most common CJK radicals*/

/*Pixel perfect monospace fonts 像素完美的单行字体*/
#define LV_FONT_UNSCII_8  0
#define LV_FONT_UNSCII_16 0

/*Optionally declare custom fonts here.
 *You can use these fonts as default font too and they will be available globally.
 *E.g. #define LV_FONT_CUSTOM_DECLARE   LV_FONT_DECLARE(my_font_1) LV_FONT_DECLARE(my_font_2)*/
/* 自定义字体的申明 */
#define LV_FONT_CUSTOM_DECLARE

/*Always set a default font 默认字体设置*/
#define LV_FONT_DEFAULT (&lv_font_montserrat_16) /* &lv_font_montserrat_14 */

/*Enable handling large font and/or fonts with a lot of characters.
 *The limit depends on the font size, font face and bpp.
 *Compiler error will be triggered if a font needs it.*/
/* 允许超大容量字体 */
#define LV_FONT_FMT_TXT_LARGE 1

/*Enables/disables support for compressed fonts. 字体压缩功能*/
#define LV_USE_FONT_COMPRESSED 0

/*Enable subpixel rendering 字体的像素级渲染*/
#define LV_USE_FONT_SUBPX 0 /* 修改 0 */

#if LV_USE_FONT_SUBPX
/*Set the pixel order of the display. Physical order of RGB channels. Doesn't matter with "normal" fonts.*/
#define LV_FONT_SUBPX_BGR 0 /*0: RGB; 1:BGR order*/
#endif                      /* LV_USE_FONT_SUBPX */

/* Text Settings */
/**
 * Select a character encoding for strings.
 * Your IDE or editor should have the same character encoding
 * - LV_TXT_ENC_UTF8
 * - LV_TXT_ENC_ASCII
 */
#define LV_TXT_ENC LV_TXT_ENC_UTF8

/*Can break (wrap) texts on these chars 可以在如下字符上换行文本*/
#define LV_TXT_BREAK_CHARS " ,.;:-_"

/*If a word is at least this long, will break wherever "prettiest" To disable, set to a value <= 0*/
/* 如果一个字符串显示过长,会在适当位置打断 , <=0禁用 */
#define LV_TXT_LINE_BREAK_LONG_LEN 0

/*Minimum number of characters in a long word to put on a line before a break.Depends on LV_TXT_LINE_BREAK_LONG_LEN.*/
/* 换行前,长单词在一行中的最小显示字符数 */
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN 3

/*Minimum number of characters in a long word to put on a line after a break.Depends on LV_TXT_LINE_BREAK_LONG_LEN.*/
/* 换行后长单词在一行中的最小显示字符数 */
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3

/* 颜色指令字符 */
#define LV_TXT_COLOR_CMD "#"

/*Support bidirectional texts. Allows mixing Left-to-Right and Right-to-Left texts.
 *The direction will be processed according to the Unicode Bidirectional Algorithm:
 *https://www.w3.org/International/articles/inline-bidi-markup/uba-basics*/
/* 支持双向文本。允许混合从左到右和从右到左的文本。 */
#define LV_USE_BIDI 0
#if LV_USE_BIDI
/*Set the default direction. Supported values:
 *`LV_BASE_DIR_LTR` Left-to-Right
 *`LV_BASE_DIR_RTL` Right-to-Left
 *`LV_BASE_DIR_AUTO` detect texts base direction*/
#define LV_BIDI_BASE_DIR_DEF LV_BIDI_DIR_AUTO
#endif /* LV_USE_BIDI */

/*Enable Arabic/Persian processing
 *In these languages characters should be replaced with an other form based on their position in the text*/
/*启用阿拉伯语/波斯语处理在这些语言中，字符应根据其在文本中的位置替换为其他形式*/
#define LV_USE_ARABIC_PERSIAN_CHARS 0 /* 修改 0 */

/* Widget Usage */

#define LV_USE_ARC       1
#define LV_USE_ANIMIMG   1
#define LV_USE_BAR       1
#define LV_USE_BTN       1
#define LV_USE_BTNMATRIX 1
#define LV_USE_CANVAS    1
#define LV_USE_CHECKBOX  1
#define LV_USE_DROPDOWN  1 /*Requires: lv_label*/
#define LV_USE_IMG       1 /*Requires: lv_label*/
#define LV_USE_LABEL     1
#if LV_USE_LABEL
#define LV_LABEL_TEXT_SELECTION 1 /*Enable selecting text of the label*/
#define LV_LABEL_LONG_TXT_HINT  1 /*Store some extra info in labels to speed up drawing of very long texts >=40k个字符*/
#endif                            /* LV_USE_LABEL */
#define LV_USE_LINE   1
#define LV_USE_ROLLER 1 /*Requires: lv_label*/
#if LV_USE_ROLLER
#define LV_ROLLER_INF_PAGES 7 /*Number of extra "pages" when the roller is infinite*/
#endif                        /* LV_USE_ROLLER */
#define LV_USE_SLIDER   1     /*Requires: lv_bar*/
#define LV_USE_SWITCH   1
#define LV_USE_TEXTAREA 1 /*Requires: lv_label*/
#if LV_USE_TEXTAREA
#define LV_TEXTAREA_DEF_PWD_SHOW_TIME 1500 /*ms*/
#endif                                     /* LV_USE_TEXTAREA */
#define LV_USE_TABLE 1

/*==================
 * EXTRA COMPONENTS
 *==================*/

/*-----------
 * Widgets
 *----------*/
#define LV_USE_ANALOGCLOCK 1

#define LV_USE_CALENDAR    1
#if LV_USE_CALENDAR
#define LV_CALENDAR_WEEK_STARTS_MONDAY 0

#if LV_CALENDAR_WEEK_STARTS_MONDAY
#define LV_CALENDAR_DEFAULT_DAY_NAMES            \
    {                                            \
        "Mo", "Tu", "We", "Th", "Fr", "Sa", "Su" \
    }
#else
#define LV_CALENDAR_DEFAULT_DAY_NAMES            \
    {                                            \
        "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa" \
    }
#endif /* LV_CALENDAR_WEEK_STARTS_MONDAY */

#define LV_CALENDAR_DEFAULT_MONTH_NAMES                                                                                          \
    {                                                                                                                            \
        "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" \
    }
#define LV_USE_CALENDAR_HEADER_ARROW    1
#define LV_USE_CALENDAR_HEADER_DROPDOWN 1
#endif /* LV_USE_CALENDAR */

#define LV_USE_CAROUSEL   1
#define LV_USE_CHART      1
#define LV_USE_COLORWHEEL 1
#define LV_USE_DCLOCK     1
#define LV_USE_IMGBTN     1
#define LV_USE_KEYBOARD   1
#define LV_USE_LED        1
#define LV_USE_LIST       1
#define LV_USE_MENU       1
#define LV_USE_METER      1
#define LV_USE_MSGBOX     1
#define LV_USE_RADIOBTN   1
#define LV_USE_SPAN       1
#if LV_USE_SPAN
/*A line text can contain maximum num of span descriptor */
#define LV_SPAN_SNIPPET_STACK_SIZE 64
#endif /* LV_USE_SPAN */
#define LV_USE_SPINBOX     1
#define LV_USE_SPINNER     1
#define LV_USE_TABVIEW     1
#define LV_USE_TILEVIEW    1
#define LV_USE_WIN         1
#define LV_USE_ZH_KEYBOARD 1
#if LV_USE_ZH_KEYBOARD
#define LV_ZH_KEYBOARD_MINI 1
#endif /* LV_USE_ZH_KEYBOARD */

/* Themes */

#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT
#define LV_THEME_DEFAULT_DARK            0
#define LV_THEME_DEFAULT_GROW            1
#define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif /* LV_USE_THEME_DEFAULT */
#define LV_USE_THEME_BASIC 1
#define LV_USE_THEME_MONO  1

/* Layouts */
/*A layout similar to Flexbox in CSS.*/
#define LV_USE_FLEX 1
/*A layout similar to Grid in CSS.*/
#define LV_USE_GRID 1

/* 3rd party libraries */

/*API for fopen, fread, etc*/
#define LV_USE_FS_STDIO 0
#if LV_USE_FS_STDIO
#define LV_FS_STDIO_LETTER     '\0' /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#define LV_FS_STDIO_PATH       ""   /*Set the working directory. File/directory paths ill be appended to it.*/
#define LV_FS_STDIO_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif                              /* LV_USE_FS_STDIO */

/*API for open, read, etc*/
#define LV_USE_FS_POSIX 0
#if LV_USE_FS_POSIX
#define LV_FS_POSIX_LETTER     '\0' /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#define LV_FS_POSIX_PATH       ""   /*Set the working directory. File/directory paths ill be appended to it.*/
#define LV_FS_POSIX_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif                              /* LV_USE_FS_POSIX */

/*API for CreateFile, ReadFile, etc*/
#define LV_USE_FS_WIN32 0
#if LV_USE_FS_WIN32
#define LV_FS_WIN32_LETTER     '\0' /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#define LV_FS_WIN32_PATH       ""   /*Set the working directory. File/directory path ill be appended to it.*/
#define LV_FS_WIN32_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif                              /* LV_USE_FS_WIN32 */

/*API for FATFS (needs to be added separately). Uses f_open, f_read, etc*/
#define LV_USE_FS_FATFS 0
#if LV_USE_FS_FATFS
#define LV_FS_FATFS_LETTER     'S' /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#define LV_FS_FATFS_CACHE_SIZE 0   /*>0 to cache this number of bytes in lv_fs_read()*/
#endif                             /* LV_USE_FS_FATFS */

/*PNG decoder library*/
#define LV_USE_PNG 1

/*BMP decoder library*/
#define LV_USE_BMP 1

/* 自定义的压缩BMP格式 */
#define LV_USE_BMPRLE 1

/* JPG + split JPG decoder library.
 * Split JPG is a custom format optimized for embedded systems. */
#define LV_USE_SJPG 1

/*GIF decoder library*/
#define LV_USE_GIF 1

/*QR code library*/
#define LV_USE_QRCODE 0

/*FreeType library*/
#define LV_USE_FREETYPE 0

#if LV_USE_FREETYPE
#define LV_FREETYPE_CACHE_SIZE (16U * 1024U) /*Memory used by FreeType to cache characters [bytes] (-1: no caching)*/
#if LV_FREETYPE_CACHE_SIZE >= 0
/* 1: bitmap cache use the sbit cache, 0:bitmap cache use the image cache. */
/* sbit cache:it is much more memory efficient for small bitmaps(font size < 256) */
/* if font size >= 256, must be configured as image cache */
#define LV_FREETYPE_SBIT_CACHE 0
/* Maximum number of opened FT_Face/FT_Size objects managed by this cache instance. */
/* (0:use system defaults) */
#define LV_FREETYPE_CACHE_FT_FACES 0
#define LV_FREETYPE_CACHE_FT_SIZES 0
#endif /* LV_FREETYPE_CACHE_SIZE */
#endif /* LV_USE_FREETYPE */

/*Rlottie library*/
#define LV_USE_RLOTTIE 0

/*FFmpeg library for image decoding and playing videos
 *Supports all major image formats so do not enable other image decoder with it*/
#define LV_USE_FFMPEG 0
#if LV_USE_FFMPEG
#define LV_FFMPEG_AV_DUMP_FORMAT 0
#endif /* LV_USE_FFMPEG */

/* Other Components */

/*1: Enable API to take snapshot for object*/
#define LV_USE_SNAPSHOT 0

/*1: Enable Monkey test*/
#define LV_USE_MONKEY 0

/*1: Enable grid navigation*/
#define LV_USE_GRIDNAV 0

/*1: Enable lv_obj fragment*/
#define LV_USE_FRAGMENT 0

/*1: Support using images as font in label or span widgets */
#define LV_USE_IMGFONT 1

/*1: Enable a published subscriber based messaging system */
#define LV_USE_MSG 0

/*1: Enable Pinyin input method*/
/*Requires: lv_keyboard*/
#define LV_USE_IME_PINYIN 1
#if LV_USE_IME_PINYIN
/*1: Use default thesaurus*/
/*If you do not use the default thesaurus, be sure to use `lv_ime_pinyin` after setting the thesauruss*/
#define LV_IME_PINYIN_USE_DEFAULT_DICT 1
/*Set the maximum number of candidate panels that can be displayed*/
/*This needs to be adjusted according to the size of the screen*/
#define LV_IME_PINYIN_CAND_TEXT_NUM 6

/*Use 9 key input(k9)*/
#define LV_IME_PINYIN_USE_K9_MODE 0
#if LV_IME_PINYIN_USE_K9_MODE == 1
#define LV_IME_PINYIN_K9_CAND_TEXT_NUM 3
#endif // LV_IME_PINYIN_USE_K9_MODE
#endif

#define LV_USE_GUIDER_SIMULATOR 0

extern void (*playSounds_event_handler)(void);

#endif
