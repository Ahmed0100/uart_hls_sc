#include "uart.h"
#include "tb_uart.h"
#include <systemc.h>
#define WAVE_DUMP

int sc_main(int argc, char *argv[])
{
	sc_clock clk("clk",25,SC_NS,0.5,12.5,SC_NS,true);
	sc_signal<bool> reset;
	sc_signal<bool> rx;
	sc_signal<bool> tx;
	sc_signal<bool> rx_ack;
	sc_signal<bool> tx_ack;
	sc_signal<sc_uint<8> > tx_data;
	sc_signal<sc_uint<8> > rx_data;
	
	uart u_uart("uart_inst");
	tb_uart test("tb_uart_inst");
    //connect to DUT

    u_uart.clk(clk);
    u_uart.reset(reset);
    u_uart.rx(rx);
    u_uart.tx_ack(tx_ack);
    u_uart.tx_data(tx_data);
    u_uart.rx_data(rx_data);
    u_uart.rx_ack(rx_ack);
    u_uart.tx(tx);

    //connect to test_bench
    test.clk(clk);
    test.reset(reset);
    test.rx(rx);
    test.tx_ack(tx_ack);
    test.tx_data(tx_data);
    test.rx_data(rx_data);
    test.rx_ack(rx_ack);
    test.tx(tx);
	
#ifdef WAVE_DUMP
    sc_trace_file *trace_file = sc_create_vcd_trace_file("trace_file");
    sc_trace(trace_file,clk,"clk");
    sc_trace(trace_file,reset,"reset");
    sc_trace(trace_file,tx,"tx");
    sc_trace(trace_file,rx,"rx");
    sc_trace(trace_file,rx_ack,"rx_ack");
    sc_trace(trace_file,tx_ack,"tx_ack");
    sc_trace(trace_file,rx_data,"rx_data");
    sc_trace(trace_file,tx_data,"tx_data");
#endif
    reset.write(1);
    sc_start(25,SC_NS);
    reset.write(0);
    rx=1;
    sc_start();
#ifdef WAVE_DUMP
    sc_close_vcd_trace_file(trace_file);
#endif
    return 0;
}
