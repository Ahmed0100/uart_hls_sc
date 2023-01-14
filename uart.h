#ifndef UART_H
#define UART
#include<systemc.h>
#include "define.h"
SC_MODULE(uart)
{
	sc_in<bool> clk;
	sc_in<bool> reset;
	sc_in<bool> rx;
	sc_in<bool> tx_ack;
	sc_in<sc_uint<8> > tx_data;

	sc_out<bool> tx;
	sc_out<bool> rx_ack;
	sc_out<sc_uint<8> > rx_data;

	void uart_main(void);
	SC_CTOR(uart)
	{
		SC_CTHREAD(uart_main,clk.pos());
		reset_signal_is(reset,true);
	}
	~uart(){};
};
#endif
