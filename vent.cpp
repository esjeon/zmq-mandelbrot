
#include "common.hpp"
#include <iostream>
#include <cstdint>
#include <zmq.hpp>

int main()
{
	zmq::context_t context(1);

	zmq::socket_t sender(context, ZMQ_PUSH);
	uint64_t hwm = 128;
	sender.setsockopt(ZMQ_HWM, &hwm, sizeof(hwm));
	sender.bind("tcp://*:" PORT_VENT);

	zmq::socket_t sink(context, ZMQ_PUSH);
	sink.connect("tcp://localhost:" PORT_SINK);

	std::cout << "Waiting..." << std::endl;
	std::getchar();
	std::cout << "Starting..." << std::endl;

	zmq::message_t msg(2);

	memcpy(msg.data(), "0", 1);
	sink.send(msg);

	Number a, b;
	int x, y;
	for(x = 0; x < IMG_WIDTH ; x ++)
	for(y = 0; y < IMG_HEIGHT; y ++) {
		a = (Number)(-2.) + ((Number)4. / (IMG_WIDTH - 1)) * x;
		b = (Number)(-2.) + ((Number)4. / (IMG_HEIGHT- 1)) * y;
		InputData input(x, y, Complex(a,b));

		msg.rebuild(sizeof(input));
		*(InputData*)(msg.data()) = input;
		sender.send(msg);
	}

	return 0;
}

