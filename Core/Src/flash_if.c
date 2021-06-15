/**
  ******************************************************************************
  * @file    STM32F4xx_IAP/src/flash_if.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    10-October-2011
  * @brief   This file provides all the memory related operation functions.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/** @addtogroup STM32F4xx_IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"
#include "stm32h7xx_hal_flash.h"
#include "ymodem.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint32_t GetSector(uint32_t Address);
extern void               FLASH_MassErase(uint8_t VoltageRange, uint32_t Banks);
extern HAL_StatusTypeDef  FLASH_OB_EnableWRP(uint32_t WRPSector, uint32_t Banks);
extern HAL_StatusTypeDef  FLASH_OB_DisableWRP(uint32_t WRPSector, uint32_t Banks);
extern HAL_StatusTypeDef  FLASH_OB_RDP_LevelConfig(uint8_t Level);
extern HAL_StatusTypeDef  FLASH_OB_UserConfig(uint8_t Iwdg, uint8_t Stop, uint8_t Stdby);
extern HAL_StatusTypeDef  FLASH_OB_BOR_LevelConfig(uint8_t Level);
extern uint8_t            FLASH_OB_GetUser(void);
extern  uint16_t           FLASH_OB_GetWRP(void);
extern uint8_t            FLASH_OB_GetRDP(void);
extern uint8_t            FLASH_OB_GetBOR(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{ 
  HAL_FLASH_Unlock();

  /* Clear pending flags (if any) */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
  FLASH_FLAG_PGSERR);
                //  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
}

/**
  * @brief  This function does an erase of all user flash area
  * @param  StartSector: start of user flash area
  * @retval 0: user flash area successfully erased
  *         1: error occurred
  */
void FLASH_If_Erase(uint32_t StartSector)
{
	uint32_t UserStartSector = FLASH_SECTOR_1;
	uint32_t PageError;
	volatile uint32_t write_cnt=0, index=0;

	volatile HAL_StatusTypeDef status;

  /* Get the sector where start the user flash area */
   /*FLASH_EraseInitTypeDef EraseInitStruct;
  	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  	EraseInitStruct.Banks = FLASH_BANK_2;//0x08000000;
  	EraseInitStruct.Sector=FLASH_SECTOR_0;
  	EraseInitStruct.NbSectors=1;
  	EraseInitStruct.VoltageRange=FLASH_VOLTAGE_RANGE_3;*/

  	HAL_FLASH_Unlock();
  	HAL_FLASH_OB_Unlock();

  	FLASH_Erase_Sector(0, FLASH_BANK_2, VOLTAGE_RANGE_3);
  	FLASH_Erase_Sector(1, FLASH_BANK_2, VOLTAGE_RANGE_3);
/*while(status = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError)!=HAL_OK)
  	{
	EraseInitStruct.Sector++;
  	EraseInitStruct.NbSectors=1;
 		Send_Byte(ACK);
  	if(EraseInitStruct.Sector>=6)
  		break;
//  		HAL_Delay(100);
  	}*/
  	HAL_FLASH_OB_Lock();
  	HAL_FLASH_Lock();
  //UserStartSector = GetSector(APPLICATION_ADDRESS);
//for(i = 0; i <= 7; i ++)
//  {
//    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
//
//       be done by word */
//	  FLASH_Erase_Sector(i, FLASH_BANK_2,FLASH_VOLTAGE_RANGE_3);
//
////	  if (FLASH_Erase_Sector(i, FLASH_VOLTAGE_RANGE_3) != HAL_OK)//FLASH_COMPLETE)
////    {
////      /* Error occurred while page erase */
////      return (1);
////    }
//  }
  
//  return (0);
}


/**
  * @brief  Returns the write protection status of user flash area.
  * @param  None
  * @retval 0: No write protected sectors inside the user flash area
  *         1: Some sectors inside the user flash area are write protected
  */
uint16_t FLASH_If_GetWriteProtectionStatus(void)
{
  uint32_t UserStartSector = FLASH_SECTOR_1;

  /* Get the sector where start the user flash area */
  UserStartSector = GetSector(APPLICATION_ADDRESS);

  /* Check if there are write protected sectors inside the user flash area */
  if ((FLASH_OB_GetWRP() >> (UserStartSector/8)) == (0xFFF >> (UserStartSector/8)))
  { /* No write protected sectors inside the user flash area */
    return 1;
  }
  else
  { /* Some sectors inside the user flash area are write protected */
    return 0;
  }
}

/**
  * @brief  Disables the write protection of user flash area.
  * @param  None
  * @retval 1: Write Protection successfully disabled
  *         2: Error: Flash write unprotection failed
  */
uint32_t FLASH_If_DisableWriteProtection(void)
{
  __IO uint32_t UserStartSector = FLASH_SECTOR_1, UserWrpSectors = OB_WRP_SECTOR_1;

  /* Get the sector where start the user flash area */
  UserStartSector = GetSector(APPLICATION_ADDRESS);

  /* Mark all sectors inside the user flash area as non protected */  
  UserWrpSectors = 0xFFF-((1 << (UserStartSector/8))-1);
   
  /* Unlock the Option Bytes */
  HAL_FLASH_OB_Unlock();

  //HAL_FLASH
  /* Disable the write protection for all sectors inside the user flash area */
 // HAL_FLASH_OB_WRPConfig(UserWrpSectors, DISABLE);

  /* Start the Option Bytes programming process. */  
//  if (FLASH_OB_Launch() !=HAL_OK )//FLASH_COMPLETE)
//  {
//    /* Error: Flash write unprotection failed */
//    return (2);
//  }

  /* Write Protection successfully disabled */
  return (1);
}

/**
  * @brief  Gets the sector of a given address
  * @param  Address: Flash address
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;
  }
//  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
//  {
//    sector = FLASH_SECTOR_8;
//  }
//  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
//  {
//    sector = FLASH_SECTOR_9;
//  }
//  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
//  {
//    sector = FLASH_SECTOR_10;
//  }
//  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
//  {
//    sector = FLASH_SECTOR_11;
//  }
    return sector;
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
