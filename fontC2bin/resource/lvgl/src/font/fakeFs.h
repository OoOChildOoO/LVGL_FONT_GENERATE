/**
 * @file fakeFs.h
 *
 */

#ifndef FAKE_FS_H
#define FAKE_FS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*********************
 *      DEFINES
 *********************/
#define FAKE_FS_MAX_FN_LENGTH 64
#define FAKE_FS_MAX_PATH_LENGTH 256

/**********************
 *      TYPEDEFS
 **********************/

/**
 * Errors in the file system module.
 */
enum {
    FAKE_FS_RES_OK = 0,
    FAKE_FS_RES_HW_ERR,     /*Low level hardware error*/
    FAKE_FS_RES_FS_ERR,     /*Error in the file system structure*/
    FAKE_FS_RES_NOT_EX,     /*Driver, file or directory is not exists*/
    FAKE_FS_RES_FULL,       /*Disk full*/
    FAKE_FS_RES_LOCKED,     /*The file is already opened*/
    FAKE_FS_RES_DENIED,     /*Access denied. Check 'fs_open' modes and write protect*/
    FAKE_FS_RES_BUSY,       /*The file system now can't handle it, try later*/
    FAKE_FS_RES_TOUT,       /*Process time outed*/
    FAKE_FS_RES_NOT_IMP,    /*Requested function is not implemented*/
    FAKE_FS_RES_OUT_OF_MEM, /*Not enough memory for an internal operation*/
    FAKE_FS_RES_INV_PARAM,  /*Invalid parameter among arguments*/
    FAKE_FS_RES_UNKNOWN,    /*Other unknown error*/
};
typedef uint8_t fakeFs_res_t;

/* 字库目录信息类型 */
typedef struct
{
    char name[36];   /* 资源的名字 */
    uint32_t check;   /* 资源的和校验 */
    uint32_t size;   /* 资源的大小 */
    uint32_t offset; /* 资源相对于基地址的偏移 */
} CatalogTypeDef_fakeFs;

/**
 * Seek modes.
 */
typedef enum {
    FAKE_FS_SEEK_SET = 0x00,      /**< Set the position from absolutely (from the start of file)*/
    FAKE_FS_SEEK_END = 0x01,      /**< Set the position from the end of the file*/
} fakeFs_whence_t;

typedef struct {
    CatalogTypeDef_fakeFs catalog;
    void * file_add;
    uint32_t file_position;
} fakeFs_file_t;


/**********************
 * GLOBAL PROTOTYPES
 **********************/
/**
 * 初始化目录的地址范围
 * @param CatalogAdd    目录的地址
 * @param CatalogSize   目录的大小
 * @return  
 */
void fakeFs_init(uint32_t CatalogAdd, uint32_t CatalogSize);
uint32_t fakeFs_getBase(void);

/**
 * Open a file
 * @param file_p    pointer to a fakeFs_file_t variable
 * @param path      path to the file beginning with the driver letter (e.g. S:/folder/file.txt)
 * @param mode      read: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return          FAKE_FS_RES_OK or any error from fakeFs_res_t enum
 */
fakeFs_res_t fakeFs_open(fakeFs_file_t *file_p, const char *path);
fakeFs_res_t fakeFs_staticOpen(fakeFs_file_t *file_p, void *fileBuf, uint32_t fileSize, char * fileName);

/**
 * Close an already opened file
 * @param file_p    pointer to a fakeFs_file_t variable
 * @return          FAKE_FS_RES_OK or any error from fakeFs_res_t enum
 */
fakeFs_res_t fakeFs_close(fakeFs_file_t *file_p);

/**
 * Read from a file
 * @param file_p    pointer to a fakeFs_file_t variable
 * @param buf       pointer to a buffer where the read bytes are stored
 * @param btr       Bytes To Read
 * @param br        the number of real read bytes (Bytes Read). NULL if unused.
 * @return          FAKE_FS_RES_OK or any error from fakeFs_res_t enum
 */
fakeFs_res_t fakeFs_read(fakeFs_file_t *file_p, void *buf, uint32_t btr, uint32_t *br);
/**
 * Set the position of the 'cursor' (read write pointer) in a file
 * @param file_p    pointer to a fakeFs_file_t variable
 * @param pos       the new position expressed in bytes index (0: start of file)
 * @param whence    tells from where set the position. See @fakeFs_whence_t
 * @return          FAKE_FS_RES_OK or any error from fakeFs_res_t enum
 */
fakeFs_res_t fakeFs_seek(fakeFs_file_t *file_p, uint32_t pos, fakeFs_whence_t whence);

/**
 * Give the position of the read write pointer
 * @param file_p    pointer to a fakeFs_file_t variable
 * @param pos_p     pointer to store the position of the read write pointer
 * @return          FAKE_FS_RES_OK or any error from 'fs_res_t'
 */
uint32_t fakeFs_tell(fakeFs_file_t *file_p);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*FAKE_FS_H*/
