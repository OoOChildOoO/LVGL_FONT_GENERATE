/**
 * @file fakeFs.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "fakeFs.h"

#include <string.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
//static const char *fakeFs_get_real_path(const char *path);

/**********************
 *  STATIC VARIABLES
 **********************/
volatile static uint32_t g_catalogAdd = 0;  // 目录地址
volatile static uint32_t g_catalogSize = 0; // 目录大小

/**********************
 *      MACROS
 **********************/
/**
 * @brief 从_Src复制_Size字节到_Dst
 * @param {void} *_Dst
 * @param {void} *_Src
 * @param {uint32_t} _Size
 * @return {*}
 * @note
 */
void fakeFs_memcpy(void *_Dst, void *_Src, uint32_t _Size)
{
//    uint32_t complete = _Size / 4;
//    uint32_t lost = _Size % 4;
//    uint32_t i = 0;
//    uint32_t * Dst = _Dst;
//    uint32_t * Src = _Src;
//    for (i = 0; i < complete; i += 1)
//    {
//        *Dst = *Src;
//        Dst += 4;
//        Src += 4;
//    }
//    if (lost != 0)
//    {
//        memcpy(Dst, Src, lost);
//    }
    memcpy(_Dst, _Src, _Size);
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void fakeFs_init(uint32_t CatalogAdd, uint32_t CatalogSize)
{
    g_catalogAdd = CatalogAdd;
    g_catalogSize = CatalogSize;
}

uint32_t fakeFs_getBase(void)
{
    return g_catalogAdd;
}

fakeFs_res_t fakeFs_staticOpen(fakeFs_file_t *file_p, void *fileBuf, uint32_t fileSize, char * fileName)
{
    if (file_p == NULL || fileBuf == NULL)
    {
        return FAKE_FS_RES_INV_PARAM;
    }

    /* 设置文件信息 */
    memset(&file_p->catalog, 0, sizeof(CatalogTypeDef_fakeFs));
    file_p->catalog.size = fileSize;
    file_p->catalog.offset = 0;
    file_p->catalog.check = 0;
    strcpy(file_p->catalog.name,fileName);
    file_p->file_add = (void *)(fileBuf);
    g_catalogAdd = 0;
    file_p->file_position = 0;

    return FAKE_FS_RES_OK;
}

fakeFs_res_t fakeFs_open(fakeFs_file_t *file_p, const char *path)
{
    if (file_p == NULL || path == NULL || g_catalogAdd == 0 || g_catalogSize == 0)
    {
        return FAKE_FS_RES_INV_PARAM;
    }

    /* 在指定地址范围查询文件 */
    uint32_t i, len;
    CatalogTypeDef_fakeFs dir;
    len = strlen(path);
    for (i = 0; i < g_catalogSize; i += sizeof(CatalogTypeDef_fakeFs))
    {
        memcpy((uint8_t *)&dir, (uint8_t *)(g_catalogAdd + i), sizeof(CatalogTypeDef_fakeFs));
        if (strncmp(dir.name, path, len) == 0)
        {
            break;
        }
    }

    /* 无指定文件 */
    if (i >= g_catalogSize)
    {
        return FAKE_FS_RES_UNKNOWN;
    }

    /* 设置文件信息 */
    memcpy(&file_p->catalog, &dir, sizeof(CatalogTypeDef_fakeFs));
    file_p->file_add = (void *)(g_catalogAdd + dir.offset);
    file_p->file_position = 0;

    return FAKE_FS_RES_OK;
}

fakeFs_res_t fakeFs_close(fakeFs_file_t *file_p)
{
    fakeFs_res_t res = FAKE_FS_RES_OK;
    if (file_p == NULL)
    {
        return FAKE_FS_RES_INV_PARAM;
    }

    memset(&file_p->catalog, 0, sizeof(CatalogTypeDef_fakeFs));
    file_p->file_add = 0;
    file_p->file_position = 0;

    return res;
}

fakeFs_res_t fakeFs_read(fakeFs_file_t *file_p, void *buf, uint32_t btr, uint32_t *br)
{
    if (file_p == NULL || btr == 0)
    {
        return FAKE_FS_RES_INV_PARAM;
    }
    if (br != NULL)
        *br = 0;

    fakeFs_res_t res = FAKE_FS_RES_OK;
    uint32_t pRead = (uint32_t)file_p->file_add;

    /* 判断文件读取范围 */
    if (file_p->file_position + btr >= file_p->catalog.size)
    {
        btr = file_p->catalog.size - file_p->file_position;
    }

    /* 读取文件 */
    if (btr != 0)
    {
        pRead += file_p->file_position;
        fakeFs_memcpy(buf, (void *)pRead, btr);
        file_p->file_position += btr;
    }

    if (br != NULL)
        *br = btr;

    return res;
}

fakeFs_res_t fakeFs_seek(fakeFs_file_t *file_p, uint32_t pos, fakeFs_whence_t whence)
{
    fakeFs_res_t res = FAKE_FS_RES_OK;
    if (file_p == NULL)
    {
        return FAKE_FS_RES_INV_PARAM;
    }
    switch (whence)
    {
    case FAKE_FS_SEEK_SET:
        if (pos < file_p->catalog.size)
        {
            file_p->file_position = pos;
        }
        else
        {
            res = FAKE_FS_RES_UNKNOWN;
        }
        break;
    case FAKE_FS_SEEK_END:
        file_p->file_position = file_p->catalog.size;
        break;

    default:
        res = FAKE_FS_RES_UNKNOWN;
        break;
    }

    return res;
}

uint32_t fakeFs_tell(fakeFs_file_t *file_p)
{
    if (file_p == NULL)
    {
        return 0;
    }
    uint32_t pos;
    pos = file_p->file_position;
    return pos;
}
