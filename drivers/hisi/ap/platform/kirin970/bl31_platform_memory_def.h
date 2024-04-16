#ifndef __PLATFORM_MEMORY_DEF_H__
#define __PLATFORM_MEMORY_DEF_H__ 
#include <global_ddr_map.h>
#define HISI_BL31_BASE ATF_RESERVED_BL31_PHYMEM_BASE
#define HISI_BL31_RAM_ADDR HISI_BL31_BASE
#define HISI_BL31_RAM_SIZE (0xE0000)
#define ATF_BL31_LOG_ADDR (HISI_BL31_BASE + HISI_BL31_RAM_SIZE)
#define ATF_BL31_LOG_SIZE (0x20000)
#define ATF_BL31_PARAMS_FROM_FASTBOOT_PARAMS_ADDR (ATF_BL31_LOG_ADDR + ATF_BL31_LOG_SIZE)
#define HISI_BL31_PARAMS_FROM_FASTBOOT_PARAMS_SIZE (0x110)
#define HISI_BL31_RPMB_KEY_INFO_ADDR (ATF_BL31_PARAMS_FROM_FASTBOOT_PARAMS_ADDR + \
                HISI_BL31_PARAMS_FROM_FASTBOOT_PARAMS_SIZE)
#define HISI_BL31_RPMB_KEY_INFO_SIZE (0x20)
#define HISI_BL31_RPMB_CRYPTOGRAPHIC_KEY_ADDR (HISI_BL31_RPMB_KEY_INFO_ADDR + \
                HISI_BL31_RPMB_KEY_INFO_SIZE)
#define HISI_BL31_RPMB_CRYPTOGRAPHIC_KEY_SIZE (0x20)
#define HISI_BL31_HUK_CODE_ADDR (HISI_BL31_RPMB_CRYPTOGRAPHIC_KEY_ADDR + HISI_BL31_RPMB_CRYPTOGRAPHIC_KEY_SIZE)
#define HISI_BL31_HUK_CODE_SIZE (0x20)
#define HISI_BL31_RPMB_KEY_ADDR (HISI_BL31_HUK_CODE_ADDR + HISI_BL31_HUK_CODE_SIZE)
#define HISI_BL31_RPMB_KEY_SIZE (0x20)
#define HISI_BL31_DDRC_INFO_ADDR (HISI_BL31_RPMB_KEY_ADDR + HISI_BL31_RPMB_KEY_SIZE)
#define HISI_BL31_DDRC_INFO_SIZE (0x200)
#define HISI_BL31_UFS_KEY_ADDR (HISI_BL31_DDRC_INFO_ADDR + HISI_BL31_DDRC_INFO_SIZE)
#define HISI_BL31_UFS_KEY_SIZE (0x800)
#define HISI_BL31_RPMB_DEVICE_INFO_ADDR (HISI_BL31_UFS_KEY_ADDR + HISI_BL31_UFS_KEY_SIZE)
#define HISI_BL31_RPMB_DEVICE_INFO_SIZE (0x28)
#define HISI_BL31_RPMB_BOOT_INFO_ADDR (HISI_BL31_RPMB_DEVICE_INFO_ADDR + HISI_BL31_RPMB_DEVICE_INFO_SIZE)
#define HISI_BL31_RPMB_BOOT_INFO_SIZE (0x20)
#define BL31_FASTBOOT_HITEST_MNTN_ADDR (HISI_BL31_RPMB_BOOT_INFO_ADDR + HISI_BL31_RPMB_BOOT_INFO_SIZE)
#define BL31_FASTBOOT_HITEST_MNTN_SIZE (0x20)
#define HISI_BL31_RESERVE_MEM_ADDR (BL31_FASTBOOT_HITEST_MNTN_ADDR + BL31_FASTBOOT_HITEST_MNTN_SIZE)
#define HISI_BL31_RESERVE_MEM_SIZE (0x408)
#define HISI_BL31_HISEE_ADDR (HISI_BL31_RESERVE_MEM_ADDR + HISI_BL31_RESERVE_MEM_SIZE)
#define HISI_BL31_HISEE_SIZE (0x2FF000)
#define HISI_BL31_SIZE (ATF_RESERVED_BL31_PHYMEM_SIZE - HISI_BL31_HISEE_SIZE)
#endif
