
#include "common.hpp"
#include <iostream>
#include <cstdint>
#include <zmq.hpp>

int main()
{
	zmq::context_t ctx(1);

	zmq::socket_t recver(ctx, ZMQ_PULL);
	uint64_t hwm = 128;
	recver.setsockopt(ZMQ_HWM, &hwm, sizeof(hwm));
	recver.connect("tcp://localhost:" PORT_VENT);

	zmq::socket_t sender(ctx, ZMQ_PUSH);
	sender.connect("tcp://localhost:" PORT_SINK);

	while(1) {
		zmq::message_t msg;
		recver.recv(&msg);
		InputData *input = (InputData*)msg.data();

		Complex c = input->c;
		Complex cn = 0;
		int step = -1;
		do {
			step ++;
			cn = pow(cn, NUM_POW) + c;
		} while( (step < MAX_STEP) && ( pow(cn.real(), 2.) + pow(cn.imag(), 2.) < 4 ) );

		OutputData output(input->x, input->y, step);
		msg.rebuild(sizeof(output));
		*(OutputData*)(msg.data()) = output;
		sender.send(msg);

		if(output.y == -2) {
			std::cout << output.x << std::endl;
		}
		//std::cout << "(" << output.x << ", " << output.y << ") = "
		//          << input->c.real() << " + I (" << input->c.imag() << ") -> " << output.d
		//          << std::endl;
		//std::cout.flush();
	}

	return 0;
}

