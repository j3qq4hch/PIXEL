#ifndef __BOOTLOADER
#define __BOOTLOADER

//12 Kbyte starting from address 0x1FFFC800, contain the bootloader firmware
#define BOOTLOADER_START_ADR    (u32)(0x1FFFC800 + 4)
#define BOOT_SIGN_ADR   0x20003FF0
#define BOOT_SIGN       0xDEADBEEF

#endif