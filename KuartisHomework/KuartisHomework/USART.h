#ifndef F_CPU
#define F_CPU 20000000UL //20 MHz clock
#endif


volatile  uint32_t uart_tx_busy = 1; // 1 means busy, 0 means not busy

/************************************************************************/
/*This function initializes UART interface. We are setting a baud rate at first, enabling TX and RX,
and enabling interrupt.                                                                   */
/************************************************************************/
void uart_init(uint32_t baud){
	uint16_t speed = 16;
	
	baud = (F_CPU/(speed*baud))-1;
	PORTC.DIR |= (1<<USART_TRAN);
	PORTC.OUT = 0;
	USART0.BAUDH = (9600>>8);
	USART0.BAUDL = (9600&0x00FF);
	USART0.CTRLB |= USART_TXEN_bm | USART_RXEN_bm ;
	USART0.CTRLA |= USART_RXCIE_bm | USART_TXCIE_bm;
	USART0.CTRLC |= USART_CHSIZE0_bm | USART_CHSIZE1_bm;
	
	
}
/************************************************************************/
/* This function sends one byte of data with USART.                                                                     */
/************************************************************************/
void uart_send_byte(uint8_t data){
	while(uart_tx_busy==0);
	uart_tx_busy=0;
	USART0.TXDATAL = data;
}
/************************************************************************/
/* This function sends byte array of data with USART.                                                                     */
/************************************************************************/
void uart_send_array(uint8_t *data_arr,uint16_t len){
	for(uint16_t i =0;i<len;i++){
		uart_send_byte(data_arr[i]);
	}
}

