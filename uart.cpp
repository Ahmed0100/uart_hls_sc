#include "uart.h"
#include "define.h"
void uart::uart_main(void)
{
	sc_uint<8> rx_data_buffer,tx_data_buffer;
	bool rx_buff="00000000",tx_buff="11111111";
	bool rx_flag=0,tx_flag=0;
	bool trans_flag=0,recv_flag=0;
	int cnt=0;
	int i=0,j=0;
	wait();
	while(true)
	{
		rx_buff = rx.read();
		if(rx_buff == STARTBIT && !recv_flag)
		{
			recv_flag=1;
			i=0;
		}
		else if(recv_flag && i !=WORDLEN)
		{
			rx_data_buffer[i]=rx_buff;
			i++;
		}
		else if(recv_flag && i==WORDLEN)
		{
			if(rx_buff == STOPBIT)
			{
				rx_data.write(rx_data_buffer);
				rx_flag = 1;
				rx_ack.write(rx_flag);
			}
			recv_flag=0;
		}
		else
		{
			i=0;
			recv_flag=0;
		}

		if(tx_ack.read() && !tx_flag)
		{
			tx_flag=1;
			tx_data_buffer = tx_data.read();
		}
		if(tx_flag && !trans_flag)
		{
			j=0;
			trans_flag=1;
			tx_buff = STARTBIT;
			tx.write(tx_buff);
		}
		else if(trans_flag && j!= WORDLEN)
		{
			tx_buff = tx_data_buffer[j];
			tx.write(tx_buff);
			j++;
		}
		else if(trans_flag && j==WORDLEN)
		{
			trans_flag=0;
			tx_flag=0;
			j=0;
			tx_buff = STOPBIT;
			tx.write(tx_buff);
		}
		else
		{
			trans_flag=0;
			tx_flag=0;
			j=0;
		}
		wait();
	}
}

