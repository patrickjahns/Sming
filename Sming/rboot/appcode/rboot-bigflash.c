//////////////////////////////////////////////////
// rBoot open source boot loader for ESP8266.
// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.
//////////////////////////////////////////////////

#ifdef RBOOT_INTEGRATION
#include <rboot-integration.h>
#endif

typedef unsigned int uint32;
typedef unsigned char uint8;

#include <rboot.h>

#ifdef BOOT_BIG_FLASH

// plain sdk defaults to iram
#ifndef IRAM_ATTR
#define IRAM_ATTR
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void Cache_Read_Disable(void);
extern uint32 SPIRead(uint32, void*, uint32);
extern void ets_printf(const char*, ...);
extern void Cache_Read_Enable(uint32, uint32, uint32);

#ifdef BOOT_RTC_ENABLED

static uint8 calc_chksum(uint8 *start, uint8 *end) {
	uint8 chksum = CHKSUM_INIT;
	while(start < end) {
		chksum ^= *start;
		start++;
	}
	return chksum;
}

uint32 system_rtc_mem(int32 addr, void *buff, int32 length, uint32 mode) {

    int32 blocks;

    // validate reading a user block
    if (addr < 64) return 0;
    if (buff == 0) return 0;
    // validate 4 byte aligned
    if (((uint32)buff & 0x3) != 0) return 0;
    // validate length is multiple of 4
    if ((length & 0x3) != 0) return 0;

    // check valid length from specified starting point
    if (length > (0x300 - (addr * 4))) return 0;

    // copy the data
    for (blocks = (length >> 2) - 1; blocks >= 0; blocks--) {
        volatile uint32 *ram = ((uint32*)buff) + blocks;
        volatile uint32 *rtc = ((uint32*)0x60001100) + addr + blocks;
		if (mode == RBOOT_RTC_WRITE) {
			*rtc = *ram;
		} else {
			*ram = *rtc;
		}
    }

    return 1;
}

#endif

uint8 rBoot_mmap_1 = 0xff;
uint8 rBoot_mmap_2 = 0xff;

// this function must remain in iram
void IRAM_ATTR Cache_Read_Enable_New(void) {

	if (rBoot_mmap_1 == 0xff) {
		uint32 addr;
		rboot_config conf;
#ifdef BOOT_RTC_ENABLED
		rboot_rtc_data rtc;
#endif
		Cache_Read_Disable();

		SPIRead(BOOT_CONFIG_SECTOR * SECTOR_SIZE, &conf, sizeof(rboot_config));

#ifdef BOOT_RTC_ENABLED
		if (system_rtc_mem(RBOOT_RTC_ADDR, &rtc, sizeof(rboot_rtc_data), RBOOT_RTC_READ) &&
			(rtc.chksum == calc_chksum((uint8*)&rtc, (uint8*)&rtc.chksum))) {
			if (rtc.last_mode & MODE_TEMP_ROM) {
				addr = conf.roms[rtc.temp_rom];
			}
			else
			{
				addr = conf.roms[conf.current_rom];
			}
		}
		else
		{
			addr = conf.roms[conf.current_rom];
		}
#else
		addr = conf.roms[conf.current_rom];
#endif
		addr /= 0x100000;

		rBoot_mmap_2 = addr / 2;
		rBoot_mmap_1 = addr % 2;

		//ets_printf("mmap %d,%d,1\r\n", rBoot_mmap_1, rBoot_mmap_2);
	}

	Cache_Read_Enable(rBoot_mmap_1, rBoot_mmap_2, 1);
}

#ifdef __cplusplus
}
#endif

#endif
