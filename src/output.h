#ifndef OUTPUT_H_
#define OUTPUT_H_

struct output_config {
	int out_fd;
};

void perform_conversion( struct output_config outconf );

#endif // OUTPUT_H_
