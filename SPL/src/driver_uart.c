#include "driver_uart.h"
uint8_t _rx_buffer[SERIAL_RX_BUFFER_SIZE] ; // khai báo mảng
static _rx_buffer_head = 0, _rx_buffer_tail =0;

USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
void UART1_Init_A9A10()
{
  // Enable clock for UART1 & GPIOA
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);       // Configure the USART1
  USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);//
  NVIC_EnableIRQ(USART1_IRQn);//

  //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  // TX Empty
  //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // RX Not Empty

  USART_Cmd(USART1, ENABLE);  // enable UART1

  {
    GPIO_InitTypeDef	gpio_init_struct;

    // GPIOA PIN9 alternative function Tx
    gpio_init_struct.GPIO_Pin = GPIO_Pin_9;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_10MHz;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio_init_struct);

    // GPIOA PIN10 alternative function Rx
    gpio_init_struct.GPIO_Pin = GPIO_Pin_10;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &gpio_init_struct);
  }
}// end UART1_Init_A9A10

int _write(int file, char *ptr, int len)
{
  for (int i = len; i != 0; i--)
  {
    while ((USART1->SR & USART_FLAG_TXE) == 0);
    USART1->DR = *ptr++;
  }
  return len;
}
void USART1_IRQHandler()// đọc dữ liệu từ UART gửi xuống và lưu vào mảng
{
  uint8_t chartoreceive = USART_ReceiveData(USART1);// gán biến bằng dữ liệu uart gửi xuống
  _rx_buffer[_rx_buffer_head] = chartoreceive;// lưu dữ liệu vào mảngb
  _rx_buffer_head = (_rx_buffer_head + 1) % SERIAL_RX_BUFFER_SIZE; //reset rx_buffer_hear
}


int HardwareSerial_available()// xác định có bao nhiêu byte dữ liệu đã được gửi xuống lưu trong mảng
{
  return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
}//end HardwareSerial_available

int HardwareSerial_read(void)//đọc dữ liệu đã lưu trong mảng
{
  // if the head isn't ahead of the tail, we don't have any characters
  if (_rx_buffer_head == _rx_buffer_tail) {//ko có dữ liệu được gửi xuống
    return -1;
  } else {
    unsigned char c = _rx_buffer[_rx_buffer_tail];
    _rx_buffer_tail = (_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE;
    return c;
  }
}// end HardwareSerial_read
