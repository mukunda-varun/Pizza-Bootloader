# Pizza-Bootloader
Pizza Bootloader Code - STM32H743ZI
STM32H743ZI Bootloader code with Version check
STM32H743ZI bootloader code with firmware version check at address 0x8125800(ADDRESS_TO_READ)

Memory Partition for the Wokie project is done as below:


Section       |   Start Address   |     Size
Bootloader    |    0x8000000      |     512KB
Application   |    0x8100000      |     150KB
FW Version    |    0x8125800      |     4KB

Partitions are done in respective code in flash.ld file
The VECT_TAB_OFFSET value in application code has to be changed to VECT_TAB_OFFSET  0x00100000UL from 0x00000000UL