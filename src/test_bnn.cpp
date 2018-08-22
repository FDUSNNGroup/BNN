#include <iostream>
#include <fstream>
#include <cstdlib>
#include "bnn.h"
#include "timer.h"
#include "sds_lib.h"
#ifdef _WIN32
#define TESTROUTE "e:/Computer/HLS/BNN/data/test_b.dat"
#define LABELROUTE "e:/Computer/HLS/BNN/data/label.dat"
#else
#define TESTROUTE "data/test_b.dat"
#define LABELROUTE "data/label.dat"
#endif // WIN32

using namespace std;
const int TEST_SIZE = 500;

/* void reshape(int* input, float* output) {
	for (int c = 0; c < 64; c++) {
		for (int y = 0; y < 7; y++) {
			for (int x = 0; x < 7; x++) {
				int o_index = c + (x + y * 7 ) * 64;
				int i_index = x + y * 7 + c * 49;
				output[o_index] = input[i_index];
			}
		}
	}
}

void dense(float* input, float* output, const float* weight, const float* bias, int M, int N, bool use_relu){
	float var_w = 2. / M;
	float c = sqrt(var_w);

	for (int n = 0; n < N; n++){
		float one_out = 0;
		for (int m = 0; m < M; m++) {
			int w_index = n * M + m;
			//output[n] += input[m] * weight[w_index] * c;
			one_out += (input[m] == weight[w_index]) ? 1 : 0; //XNOR
		}
		output[n] = (2 * one_out - M)*c;
		float biased = output[n] + bias[n];
		if (use_relu) output[n] = (biased > 0) ? 1 : 0;
		else output[n] = biased;
	}

} */

void read_test_images(int8_t** test_images) {
	std::ifstream infile(TESTROUTE);
	if (infile.is_open()) {
		for (int index = 0; index < TEST_SIZE; index++) {
			for (int pixel = 0; pixel < 784; pixel++) {
				int i;
				infile >> i;
				test_images[index][pixel] = i;
			}
		}
		infile.close();
	}
}

void read_test_labels(int test_labels[TEST_SIZE]) {
	std::ifstream infile(LABELROUTE);
	if (infile.is_open()) {
		for (int index = 0; index < TEST_SIZE; index++) {
			infile >> test_labels[index];
		}
		infile.close();
	}
}

void read_fc1_weights(bit8_t* w_fc1) {
	std::ifstream infile("data/weight_10bp");
	if (infile.is_open()) {
		for (int index = 0; index < MAX_W_FC / 8; index++) {
			int i;
			infile >> i;
			w_fc1[index] = i;
		}
		infile.close();
	}
}

void read_fc2_weights(bit8_t* w_fc2) {
	std::ifstream infile("data/weight_12bp");
	if (infile.is_open()) {
		for (int index = 0; index < 640; index++) {
			int i;
			infile >> i;
			w_fc2[index] = i;
		}
		infile.close();
	}
}

int main(){

	int8_t** test_images;
	bit8_t* w_fc1 = (bit8_t*)sds_alloc(MAX_W_FC / 8 * sizeof(bit8_t));
	bit8_t* w_fc2 = (bit8_t*)sds_alloc(640 * sizeof(bit8_t));
	test_images = new int8_t*[TEST_SIZE];
	for(int i = 0; i < TEST_SIZE; i++)
		test_images[i] = new int8_t[784];
	int test_labels[TEST_SIZE];
	read_test_images(test_images);
	read_test_labels(test_labels);
	read_fc1_weights(w_fc1);
	read_fc2_weights(w_fc2);

	float correct = 0.0;

	bit8_t input_image[I_WIDTH1*I_WIDTH1];
	bit8_t output_image[O_WIDTH*O_WIDTH * 64] = { 0 };
	int output_image_f[O_WIDTH*O_WIDTH * 64] = { 0 };
	float layer1_out[512] = { 0 };
	float reshape_image[3136] = {0};
	fixo out[10];
	float result[10];

	Timer timer("conv timer");
	Timer timer_("bnn timer");

	for (int test = 0; test < TEST_SIZE; test++) {

		for (int i = 0; i < 784; i++){
			input_image[i] = test_images[test][i];
		}

		timer_.start();
		timer.start();

		bnn(input_image, out, w_fc1, w_fc2);

		for (int i = 0; i < 10; i++)
			result[i] = out[i].to_float();

		timer.stop();

		/* for (int i = 0; i < O_WIDTH*O_WIDTH * 64; i++) output_image_f[i] = output_image[i].to_int(); //in this case, no need to add a "-"
		reshape(output_image_f, reshape_image);
		dense(reshape_image, layer1_out, w_fc1, b_fc1, O_WIDTH*O_WIDTH*64, 512, true);
		dense(layer1_out, out, w_fc2, b_fc2, 512, 10, false); */

		int max_id = 0;
		for (int i = 1; i < 10; i++) {
			if (result[i] > result[max_id])
				max_id = i;
		}
		timer_.stop();
		if (max_id == test_labels[test]) correct += 1.0;
		cout << test << ": " << max_id << " " << test_labels[test] << endl;
	}
	cout << correct/TEST_SIZE << endl;

	return 0;
}
