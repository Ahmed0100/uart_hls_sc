#include <systemc.h>
#include "tb_uart.h"
#include "define.h"

void tb_uart::uart_transmit()
{
	int l=0;
	unsigned int data_in_read;
	transmit_file = fopen(TRANSMITFILENAME,"rt");
	if(!transmit_file)
	{
		cout<<"could not open "<<TRANSMITFILENAME<<endl;
		sc_stop();
		exit(-1);
	}
	tx_ack.write(0);
	wait();
	while(true)
	{
		while(fscanf(transmit_file,"%u",&data_in_read) != EOF)
		{
			tx_data.write(data_in_read);
			tx_ack.write(1);
			wait();
			tx_ack.write(0);
			for(l=0;l<10;l++)
				wait();
		}
		wait();
		wait();
		tx_ack.write(0);
		wait();
		wait();
		wait();
		fclose(transmit_file);
		cout<<endl<<"compare data"<<endl;
		compare_data_receive();
		sc_stop();
		wait();
	}
}
void tb_uart::uart_receive()
{
	int k=0;
	sc_uint<1> loop_data;
	unsigned int data_out_read;
	receive_file = fopen(RECEIVEFILENAME,"w");
	if(!receive_file)
	{
		cout<<"could not open "<<RECEIVEFILENAME<<endl;
		sc_stop();
		exit(-1);
	}
	wait();
	wait();
	wait();
	while(true)
	{
		for(k=0;k<10;k++){
			loop_data=tx.read();
			rx.write(loop_data);
			wait();
		}
		wait();
		data_out_read=rx_data.read();
		fprintf(receive_file,"%u \n",data_out_read);
	}
}

void tb_uart::compare_data_receive()
{
	unsigned int golden_data,receive_data,line=1,errors=0;
	fclose(receive_file);
	receive_file = fopen(RECEIVEFILENAME,"rt");
    if(!receive_file)
    {
        cout << "could not open " << RECEIVEFILENAME << "\n";
        sc_stop();
        exit (-1);
    }

	transmit_file = fopen(TRANSMITFILENAME,"rt");
    if(!receive_file)
    {
        cout << "could not open " << TRANSMITFILENAME << "\n";
        sc_stop();
        exit (-1);
    }
    diff_file = fopen(DIFFERENCEFILENAME,"rt");
    if(!receive_file)
    {
        cout << "could not open " << DIFFERENCEFILENAME << "\n";
        sc_stop();
        exit (-1);
    }

    while(fscanf(transmit_file,"%u",&golden_data) != EOF)
    {
    	fscanf(receive_file,"%u",&receive_data);
        cout << endl <<"cycle["<< line << "]: " << golden_data << "-- "<< receive_data;
        if(receive_data != golden_data)
        {
            cout << "\noutput missmatch [line:" << line << "] golden:" << golden_data << " -- Output:" << receive_data;
            fprintf(diff_file,"\noutput missmatch[line:%d] golden: %d -- Output: %d",line, golden_data, receive_data);
            errors++;
        }
        line ++;
    }
    if(errors == 0)
    cout << endl << "finished simulation SUCCESSFULLY" << endl;
    else
    cout << endl << "finished simulation " << errors << " MISSMATCHES between Golden and Simulation" << endl;
    fclose(receive_file);
    fclose(diff_file);
    fclose(transmit_file);
}
