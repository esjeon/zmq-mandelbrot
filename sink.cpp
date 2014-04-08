
#include "common.hpp"
#include <iostream>
#include <cstdio>
#include <cstdint>
#include <zmq.hpp>

int
pgm_write (const char *path, unsigned int width, unsigned int height, uint8_t gray[], uint8_t maxval, bool plain)
{
	FILE *fp;
	int i, c, len;

	fp = fopen(path, "wb");
	if (!fp)
		return 1;

	fprintf(fp, "P%d %d %d %d\n",
			((plain)? 2: 5), width, height, maxval);

	len = width * height;
	if (plain)
		for (i = 0, c = 0; i < len; i ++) {
			fprintf (fp, "%3d", gray[i]);
			if ((++c) == width)
				c = 0, fputc('\n', fp);
			else
				fputc(' ', fp);
		}
	else
		for (i = 0; i < width * height; i ++)
			fwrite(&gray[i], sizeof(uint8_t), 1, fp);

	fclose (fp);

	return 0;
}

int main()
{
	zmq::context_t ctx(1);

	zmq::socket_t recver(ctx, ZMQ_PULL);
	recver.bind("tcp://*:" PORT_SINK);
	
	zmq::message_t msg;
	recver.recv(&msg);

	uint8_t *buf = new uint8_t[IMG_WIDTH * IMG_HEIGHT];

	for(int i = 0; i < IMG_WIDTH * IMG_HEIGHT; i ++) {
		recver.recv(&msg);
		OutputData *output = (OutputData*)msg.data();
		buf[output->x + output->y * IMG_WIDTH] = output->d;
		//std::cout << "(" << output->x << ", " << output->y << ") = " << output->d << std::endl;
	}

	pgm_write("output.pgm", IMG_WIDTH, IMG_HEIGHT, buf, MAX_STEP, true);

	//for(int x = 0; x < IMG_WIDTH ; x ++) {
	//	for(int y = 0; y < IMG_HEIGHT; y ++)
	//		std::cout << buf[x][y] << "\t";
	//	std::cout << std::endl;
	//}
}
