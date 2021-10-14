#define EEPROM_DATA_ADDRESS_LOW 0x00;
#define EEPROM_DATA_ADDRESS_HIGH 0x14;
//Start of the EEPROM address is 0x1400 = 0001 0100 0000 0000
int Light_Level = 0;
/************************************************************************/
/* This function reads a page from EEPROM memory. e tried to use a pointer to read the data.
But we did not succeeded.                                                                     */
/************************************************************************/
uint64_t EEPROM_read(){
	uint64_t * pointer = EEPROM_START;
	uint64_t data = 0;
	data = *pointer;
	
	
	return data;
}
/************************************************************************/
/* This function writes brightness level to the EEPROM memory.                                                                     */
/************************************************************************/
void EEPROM_write(BRIGHT_MODE_STATE_INFO level){
	while((NVMCTRL.STATUS&(0x01))==0x01); // Check for FBUSY
	while((NVMCTRL.STATUS&(0x02))==0x01); // Check for EEBUSY
	uint8_t data = 0;
	NVMCTRL.ADDRL = EEPROM_DATA_ADDRESS_LOW;
	NVMCTRL.ADDRH = EEPROM_DATA_ADDRESS_HIGH;
	switch(level.currentState){
		case(BRIGHT_ONE):
			data = 1;
			NVMCTRL.DATAL = data;
		break;
		case(BRIGHT_TWO):
		data = 2;
		NVMCTRL.DATAL = data;
		break;
		case(BRIGHT_THREE):
		data = 3;
		NVMCTRL.DATAL = data;
		break;
		case(BRIGHT_FOUR):
		data = 4;
		NVMCTRL.DATAL = data;
		break;
		case(BRIGHT_FIVE):
		data = 5;
		NVMCTRL.DATAL = data;
		break;
		case(BRIGHT_SIX):
		data = 6;
		NVMCTRL.DATAL = data;
		break;
		case(BRIGHT_SEVEN):
		data = 7;
		NVMCTRL.DATAL = data;
		break;
		case(BRIGHT_EIGHT):
		data = 8;
		NVMCTRL.DATAL = data;
		break;
		case(BRIGHT_NINE):
		data = 9;
		NVMCTRL.DATAL = data;
		break;
		case(BRIGHT_TEN):
		data = 10;
		NVMCTRL.DATAL = data;
		break;
		default:
		break;
	}
	
	NVMCTRL.DATAH = 0x00;
	CPU_CCP = CCP_SPM_gc;
	NVMCTRL.CTRLA = NVMCTRL_CMD_PAGEWRITE_gc;
}