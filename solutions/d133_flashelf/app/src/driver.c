/*******************************************************************************
 * Copyright (c) 2012-2019 Hangzhou C-SKY Microsystems Co., Ltd.
 * 
 * All rights reserved. This software is proprietary and confidential to
 * Hangzhou C-SKY Microsystems Co., Ltd. and its licensors.
 *
 * Contributors:
 *     Hangzhou C-SKY Microsystems Co., Ltd.
 *
 * 2019.6.18   Jiang Long(long_jiang@c-sky.com)
 *     Initial API and implementation
 *******************************************************************************/

/**
 * Driver for flash program.
 */

/**
 * ERROR TYPE. MUST NOT BE MODIFIED
 */
#define ERROR_INIT             -200
#define ERROR_READID           -201
#define ERROR_PROGRAM          -202
#define ERROR_READ             -203
#define ERROR_ERASE            -204
#define ERROR_CHIPERASE        -205
#define ERROR_UNINIT           -206
#define ERROR_CHECKSUM         -207
#define ERROR_FLASH_INDEX      -208

/**
 * Customize this method to perform any initialization
 * needed to access your flash device.
 *
 * @return: if this method returns an error,MUST RUTURN ERROR_INIT,
 * Otherwise return 0.
 */
int  flashInit(){
	// TODO 
  return 0;
}

/**
 * Customize this method to perform any un-initialization
 * needed to access your flash device.
 *
 * @return: if this method returns an error,MUST RUTURN ERROR_UNINIT,
 * Otherwise return 0.
 */
int  flashUnInit(){
	// TODO 
  return 0;
}

/**
 * Customize this method to read flash ID
 *
 * @param flashID: returns for flash ID
 *
 * @return: if this method returns an error,MUST RUTURN ERROR_READID,
 * Otherwise return 0.
 */
int  flashID(unsigned int* flashID){
	// TODO 
  return 0;
}

/**
 * This method takes the data pointed to by the src parameter
 * and writes it to the flash blocks indicated by the
 * dst parameter.
 *
 * @param dst : destination address where flash program
 * @param src : address of data
 * @param length : data length
 *
 * @return : if this method returns an error,MUST RUTURN ERROR_PROGRAM,
 * Otherwise return 0.
 */
int flashProgram(char* dst, char *src, int length){
	// TODO 
  return 0;
}

/**
 * Customize this method to read data from a group of flash blocks into a buffer
 *
 * @param dst : reads the contents of those flash blocks into the address pointed to by
 * the dst parameter.
 * @param src : a pointer to a single flash.
 * @param length : data length
 *
 *  @return: if this method returns an error,MUST RUTURN ERROR_READ,
 * Otherwise return 0.
 */
int flashRead(char* dst, char *src, int length){
	// TODO 
  return 0;
}

/**
 * Customize this method to erase a group of flash blocks.
 *
 * @param dst : a pointer to the base of the flash device.
 * NOTE: dst will always be sector aligned, the sector size is stored in FlashDev.c#FlashDevices#Devs#PageSize
 * @param length : erase length
 * NOTE: length will always be sector aligned, the sector size is stored in FlashDev.c#FlashDevices#Devs#PageSize
 *
 * @return : if this method returns an error,MUST RUTURN ERROR_ERASE,
 * Otherwise return 0
 */
int flashErase(char *dst, int length){
	// TODO 
  return 0;
}

/**
 * Customize this method to erase the whole flash.
 *
 * @return : if this method returns an error,MUST RUTURN ERROR_CHIPERASE,
 * Otherwise return 0.
 */
int flashChipErase( ){
	// TODO 
  return 0;
}

/**
 * Customize this method to make the veryfiy process more quickly.
 * 
 * @param dst : a pointer to the base of the flash device.
 * NOTE: dst will always be 4 aligned.
 * @param length : the lenght of the data which will be used for checksum
 * NOTE: the length will always be 4 aligned.
 * @param checksum : the expected checksum value in the programmed file(ihex,bin,elf format)
 * 
 * @return : if the specified data's checksum from dst and lenght is checksum, return 0, else return ERROR_CHECKSUM
 * @example if the flash can be read directly, you can copy following code to replace current "return 0;"
 *
  int i, sum = 0;
  for (i = 0; i < length; i++) {
   sum += dst[i];
  }
  return sum == checksum ? 0 : ERROR_CHECKSUM;
 * 
 */
int flashChecksum(char*dst, int length, int checksum) {
  return 0;
}

/**
 * Customize this method to set flash index, indicate which flash to use
 *
 * @param index: index of flash
 *
 * @return: if this method returns an error,MUST RUTURN ERROR_FLASH_INDEX,
 * Otherwise return 0.
 */
int  flashIndex(int index){
	// TODO 
  return 0;
}

// NOTING: when debug the driver, this macro defined as 1, and then
// it must be set as 0, for release to flash programmer library
#define DEBUG_DRIVER	0

/**
 * Debug entry for driver.
 *
 * @return : if this method returns an error,MUST RUTURN ERROR_CHIPERASE,
 * Otherwise return 0.
 */
int flashTest(){
#if DEBUG_DRIVER

	unsigned int ID;
	// read flash id
	flashID(&ID);

    // other drivers test
    return ID;
#else
	return 0;
#endif
}

