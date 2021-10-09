#include "PortDefinitions.h"

/************************************************************************/
/* Buzzer function outputs different sounds for different inputs from the remote controller                                                                     */
/************************************************************************/
typedef enum{
	POWER_ON_SOUND,
	POWER_OFF_SOUND,
	INC_SOUND,
	DEC_SOUND,
	LIGHT_SOUND,
	DEV_INIT_SOUND,
	DEV_MODE_SOUND,
	LIGHT_ADJ_SOUND
	
	}BUZZER_SOUND;
typedef struct BUZZER_SOUND_INFO{
	BUZZER_SOUND currentState;
	}BUZZER_SOUND_INFO;
BUZZER_SOUND_INFO BuzzerState;
volatile int32_t BuzzerFlag = 0;
volatile int32_t BuzzerCounter = 0;
volatile int32_t Pitch_flag=0;
void Buzzer(){
	BuzzerFlag=1;
	switch(BuzzerState.currentState){
		case(POWER_ON_SOUND):
			if(BuzzerCounter%2==0&&Pitch_flag==1){
				PORTD.OUT ^= (1<<BUZZER);
			}
			if(BuzzerCounter%10==0&&Pitch_flag==0){
				PORTD.OUT ^= (1<<BUZZER);
			}
			
						
			break;
		case(POWER_OFF_SOUND):
			if(BuzzerCounter%2==0&&Pitch_flag==0){
				PORTD.OUT ^= (1<<BUZZER);
			}
			if(BuzzerCounter%10==0&&Pitch_flag==1){
				PORTD.OUT ^= (1<<BUZZER);
			}
		break;
		case(INC_SOUND):
			if(BuzzerCounter%2==0){
				PORTD.OUT ^= (1<<BUZZER);
			}
			break;
		case(DEC_SOUND):
			if(BuzzerCounter%10==0){
				PORTD.OUT ^= (1<<BUZZER);
			}
			break;
		case(LIGHT_SOUND):
		
			break;
		case(DEV_INIT_SOUND):
		
			break;
		case(DEV_MODE_SOUND):
		
			break;
		case(LIGHT_ADJ_SOUND):
		
			break;
		default:	
		break;
	}
	
}