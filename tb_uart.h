#include <systemc.h>
SC_MODULE(tb_uart)
{
	sc_in<bool> clk;
	sc_in<bool> reset;
	sc_out<bool> rx;
	sc_out<sc_uint<8> > tx_data;
	sc_out<bool> tx_ack;

	sc_in<sc_uint<8> > rx_data;
	sc_in<bool> rx_ack;
	sc_in<bool> tx;

	FILE* transmit_file,*receive_file,*diff_file,*receive_file_read;

	void uart_transmit(void);
	void uart_receive(void);
	void compare_data_receive(void);
	SC_CTOR(tb_uart)
	{
		SC_CTHREAD(uart_transmit,clk.pos());
		reset_signal_is(reset,true);
		SC_CTHREAD(uart_receive,clk.pos());
		reset_signal_is(reset,true);
	}
	~tb_uart(){};
};
