/*
 * @Author WJ 240997293@qq.com
 * @Date 2023-04-10
 * @LastEditors WJ 240997293@qq.com
 * @LastEditTime 2023-08-30
 * @FilePath \font\lvgl\src\font\lv_font_map_loader.h
 * @Description 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**
 * @file lv_font_loader.h
 *
 */

#ifndef LV_FONT_MAP_LOADER_H
#define LV_FONT_MAP_LOADER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lv_font.h"
#include "fakeFs.h"
/*********************
 *      DEFINES
 *********************/
//#include <malloc.h>
// #define FONT_MALLOC(size) malloc(size)
// #define FONT_FREE(p) free(p)

#include <lv_mem.h>
#define FONT_MALLOC(size) lv_mem_alloc(size)
#define FONT_FREE(p) lv_mem_free(p)

// #include "malloc2.h"
// #define FONT_MALLOC(size) MY_MALLOC(size)
// #define FONT_FREE(p) MY_FREE(p)

/**********************
 *      TYPEDEFS
 **********************/

typedef struct
{
    char name[32];     /* 资源的名字 */
    uint32_t cnt;      /* 词条数量 */
    uint32_t add;      /* 资源假地址 */
    uint32_t size;     /* 资源的大小 */
    uint32_t offset;   /* 资源相对文字的地址 */
} Catalog_Font_Typedef;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
lv_font_t *lv_load_map_font(fakeFs_file_t *fp, uint32_t fileBase);
lv_font_t *lv_font_map_load(const char *font_name);
void lv_font_map_free(lv_font_t *font);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_FONT_MAP_LOADER_H*/
