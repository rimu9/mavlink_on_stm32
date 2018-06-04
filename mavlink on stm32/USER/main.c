#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "mavlink_avoid_errors.h"
#include "mavlink_usart_fifo.h"
#include "open_tel_mavlink.h"

mavlink_system_t mavlink_system;
#define UART_TX_BUFFER_SIZE        255
#define UART_RX_BUFFER_SIZE        255
extern fifo_t uart_rx_fifo, uart_tx_fifo;
extern uint8_t uart_tx_buf[UART_TX_BUFFER_SIZE], uart_rx_buf[UART_RX_BUFFER_SIZE];


 int main(void)
 {		
//		 mavlink_heartbeat_t packet_in ={
//	963497464,17,84,151,218,3
//	};
//	mavlink_heartbeat_t packet1;
//	memset(&packet1,0,sizeof(packet1));
//	packet1.custom_mode = packet_in.custom_mode;
//	packet1.type = packet_in.type;
//	packet1.autopilot = packet_in.autopilot;
//	packet1.base_mode = packet_in.base_mode;
//	packet1.system_status = packet_in.system_status;
//	packet1.mavlink_version = packet_in.mavlink_version;

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	 
	fifo_init(&uart_tx_fifo, uart_tx_buf, UART_TX_BUFFER_SIZE);	
	fifo_init(&uart_rx_fifo, uart_rx_buf, UART_RX_BUFFER_SIZE);
	mavlink_system.sysid = MAV_TYPE_GENERIC;
	mavlink_system.compid = MAV_AUTOPILOT_GENERIC;
	 
	 
 	while(1)
	{
		//mavlink_msg_heartbeat_send(MAVLINK_COMM_1 , packet1.type , packet1.autopilot , packet1.base_mode , packet1.custom_mode , packet1.system_status );
		mavlink_send_message(0, MSG_HEARTBEAT, 0);
		mavlink_send_message(0, MSG_LOCATION, 0);
//		 printf("Received message%d",0);
		while(1)
		{
//			if(tranlTimer > 100) 
//			{
//				tranlTimer = 0;
//				mavlink_send_message(0, MSG_HEARTBEAT, 0);
//		    mavlink_send_message(0, MSG_ATTITUDE, 0);
//		    mavlink_send_message(0, MSG_AHRS, 0);
//			}
			update();
		}
	}	 
}

