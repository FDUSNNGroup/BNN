#ifndef LAYER_H
#define LAYER_H

#include"typedef.h"

const int I_WIDTH1 = 28; //conv1 input width
const int I_WIDTH2 = 14; //conv2 input width
const int O_WIDTH = 7; //conv output width
const int F = 5; //filter width
const int FILTER_SIZE = F*F;
const int PADDING = F - 1;
const int MAX_FMAP = 65536; //32*32*64
const int MAX_WIDTH = 28;
const int MAX_F = 64; //{32, 64} num of conv2 output fmaps
const int MAX_W_CONV = 51200;//5*5*32*64 num of conv2 weights
const int FC1_UNITS = O_WIDTH*O_WIDTH * 64; //num of fc1 input units
const int FC2_UNITS = 512;
const int MAX_W_FC = FC1_UNITS*FC2_UNITS;
const int OUT = 10;
const fix con1 = sqrt(2.0/(F*F*1));//0.28284271;
const fix con2 = sqrt(2.0/(F*F*32));//0.05;

//void pad(bit input[64][32][32], bit output[64][32][32], int M, int I);

//void conv_2d(bit input[64][32][32], fix output[64][32][32], const bit weight[MAX_W_CONV], int M, int N, int I, fix con);
void conv_1(bit input[64][28][28], bit output[64][28][28], const bit weight[MAX_W_CONV], const fix k[MAX_F], const fix h[MAX_F], int M, int N, int I, fix con);

void max_pool(bit input[64][28][28], bit32_t output[2][14][14], int M, int I);

void conv_2(bit32_t input[14][14], bit output[64][28][28], const bit weight[MAX_W_CONV], const fix k[MAX_F], const fix h[MAX_F], fix con);

//void batch_norm(fix input[64][32][32], bit output[64][32][32], const fix miu[MAX_F], const float sigma[MAX_F], const fix gamma[MAX_F], const fix beta[MAX_F], int M, int I);
//void batch_norm(fix input[64][32][32], bit output[64][32][32], const fix k[MAX_F], const fix h[MAX_F], int M, int I);

void reshape(float* input, float* output);

void dense(float* input, float*output, const float*weight, const float* bias, int M, int N, bool use_relu);

#endif
