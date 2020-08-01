#include "sys_config.h"


void clocks_start( void ){
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;

    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
}


void sys_info(void){

    switch (NRF_FICR->INFO.VARIANT)
    {
    case 0x41414141:
        NRF_LOG_INFO("Chip: %X AAAA",NRF_FICR->INFO.PART);
        break;

    case 0x41414142:
        NRF_LOG_INFO("Chip: %X AAAB",NRF_FICR->INFO.PART);
        break;

    case 0x41414241:
        NRF_LOG_INFO("Chip: %X AABA",NRF_FICR->INFO.PART);
        break;

    case 0x41414242:
        NRF_LOG_INFO("Chip: %X AABB",NRF_FICR->INFO.PART);
        break;

    case 0x41414230:
        NRF_LOG_INFO("Chip: %X AAB0",NRF_FICR->INFO.PART);
        break;

    case 0x41414530:
        NRF_LOG_INFO("Chip: %X AAE0",NRF_FICR->INFO.PART);
        break;

    default:
        NRF_LOG_INFO("Chip: %X Unspecified",NRF_FICR->INFO.PART);
        break;
    }

    switch (NRF_FICR->INFO.PACKAGE) {

        case 0x2000: 
            NRF_LOG_INFO("Package: QFxx - 48-pin QFN"); 
            break;

        case 0x2001: 
            NRF_LOG_INFO("Package: CHxx - 7x8 WLCSP 56 balls"); 
            break;

        case 0x2002: 
            NRF_LOG_INFO("Package: CIxx - 7x8 WLCSP 56 balls"); 
            break;

        case 0x2005: 
            NRF_LOG_INFO("Package: CKxx - 7x8 WLCSP 56 balls with backside coating"); 
            break;

        default:
            NRF_LOG_INFO("Package: unknown");
            break;
    }


  switch (NRF_FICR->INFO.RAM) {
    case 0x10: 
        NRF_LOG_INFO("RAM: 16 kByte"); 
        break;

    case 0x20: 
        NRF_LOG_INFO("RAM: 32 kByte"); 
        break;

    case 0x40: 
        NRF_LOG_INFO("RAM: 64 kByte"); 
        break;
    
    default:
        NRF_LOG_INFO("RAM: unknown");
        break;
  }


  switch (NRF_FICR->INFO.FLASH) {
    case 0x80:  
        NRF_LOG_INFO("FLASH: 128 kByte"); 
        break;
    case 0x100: 
        NRF_LOG_INFO("FLASH: 256 kByte"); 
        break;
    case 0x200: 
        NRF_LOG_INFO("FLASH: 512 kByte"); 
        break;

    default:
        NRF_LOG_INFO("FLASH: unknown");
        break;

  }
}
