#define AP_INT_MAX_W 9216
#include <ap_int.h>
#include"weights.h"
#include "components.h"

// My playground network
void conv_wrapper_tp(
  hls::stream<data_t_L1> &frame_in,
//  hls::stream<data_t_L8> &frame_in,
//  hls::stream<data_t_out> &frame_out
  hls::stream<data_t_L2> &frame_out
)
{

  /***********************************
   *  Initialise
   ***********************************/

  #pragma HLS DATAFLOW

  #pragma HLS INTERFACE ap_ctrl_none port=return
  #pragma HLS INTERFACE axis port=frame_in
  #pragma HLS INTERFACE axis port=frame_out

  // AXI Streaming Connections
  // layer 1 conv
  hls::stream<data_t_L1> swinput_0;
  #pragma HLS STREAM variable=swinput_0 depth=1
  hls::stream<data_t_L1> swoutput_0;
  #pragma HLS STREAM variable=swoutput_0 depth=1
  hls::stream<data_t_L2> swinput_1;
  #pragma HLS STREAM variable=swinput_1 depth=1
  hls::stream<data_t_L2> swoutput_1;
  #pragma HLS STREAM variable=swoutput_1 depth=1
  hls::stream<data_t_L3> mpinput_2;
  #pragma HLS STREAM variable=mpinput_2 depth=1

  hls::stream<data_t_L4> swinput_3;
  #pragma HLS STREAM variable=swinput_3 depth=1
  hls::stream<data_t_L4> swoutput_3;
  #pragma HLS STREAM variable=swoutput_3 depth=1
  hls::stream<data_t_L5> swinput_4;
  #pragma HLS STREAM variable=swinput_4 depth=1
  hls::stream<data_t_L5> swoutput_4;
  #pragma HLS STREAM variable=swoutput_4 depth=1
  hls::stream<data_t_L6> mpinput_5;
  #pragma HLS STREAM variable=mpinput_5 depth=1

  hls::stream<data_t_L7> swinput_6;
  #pragma HLS STREAM variable=swinput_6 depth=1
  hls::stream<data_t_L7> swoutput_6;
  #pragma HLS STREAM variable=swoutput_6 depth=1
  hls::stream<data_t_L8> swinput_7;
  #pragma HLS STREAM variable=swinput_7 depth=1
  hls::stream<data_t_L8> swoutput_7;
  #pragma HLS STREAM variable=swoutput_7 depth=1

  hls::stream<data_t_L9> fcinput_8;
  #pragma HLS STREAM variable=fcinput_8 depth=1

  hls::stream<data_t_L10> fcinput_9;
  #pragma HLS STREAM variable=fcinput_9 depth=1

  hls::stream<data_t_L11> fcinput_10;
  #pragma HLS STREAM variable=fcinput_10 depth=1

  /***********************************
   *  Weight memory partition
   ***********************************/

#pragma HLS ARRAY_PARTITION variable=weights_w_conv1_1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=weights_w_conv1_1 complete dim=4
#pragma HLS ARRAY_PARTITION variable=weights_w_conv2_1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=weights_w_conv2_1 complete dim=4

  /***********************************
   *  Network
   ***********************************/

  // First layer: conv layer
  LUTNET_SlidingWindow<
    L1_K, L1_N, L1_iR, L1_oR, L1_BW, 1>(
    frame_in,
    swoutput_0,
    //frame_out,
    L1_K,
    L1_iR,
    L1_oR,
    0);
  FXPMV<
    L1_iR, L1_iC, L1_N, L1_oR, L1_oC, L1_M, L1_K, L1_ST, L1_BW, L2_BW, 24, 16, L1_TN, L1_TM,
    hls::stream<data_t_L1>,
    hls::stream<data_t_L2>
    >(
    swoutput_0,
    swinput_1,
    //frame_out,
    weights_w_conv1_1,
    //pruning_mask_conv1_1,
    thresh_conv1,
    alpha_conv1,
    next_layer_means_conv1
    );
  // Second layer: conv layer
  LUTNET_SlidingWindow<
    L2_K, L2_N, L2_iR, L2_oR, L2_BW, 1>(
    swinput_1,
    //frame_in,
    swoutput_1,
    L2_K,
    L2_iR,
    L2_oR,
    0);
  LUTNET_LUT22MV_TM<
    L2_iR, L2_iC, L2_N, L2_oR, L2_oC, L2_M, L2_K, L2_ST, L2_BW, 24, 16, L2_TN, L2_TM,
    hls::stream<data_t_L2>,
    hls::stream<data_t_L3>
    >(
    swoutput_1,
    //mpinput_2,
    frame_out,
    weights_w_conv2_1,
    thresh_conv2,
    alpha_conv2,
    next_layer_means_conv2,
    rand_map_conv2
    );
//  // Third layer: maxpooling layer
//  MaxPool<
//    L3_iR, L3_iC, L3_N, L3_oR, L3_oC, L3_K, L3_BW,
//    hls::stream<data_t_L3>,
//    hls::stream<data_t_L4>
//    >(
//    mpinput_2,
//    swinput_3
//    //frame_out
//    );
//  // Fourth layer: conv layer
//  LUTNET_SlidingWindow<
//    L4_K, L4_N, L4_iR, L4_oR, L4_BW, 1>(
//    swinput_3,
//    swoutput_3,
//    L4_K,
//    L4_iR,
//    L4_oR,
//    0);
//  LUTNET_LUT22MV_TM<
//    L4_iR, L4_iC, L4_N, L4_oR, L4_oC, L4_M, L4_K, L4_ST, L4_BW, 24, 16, L4_TN, L4_TM,
//    hls::stream<data_t_L4>,
//    hls::stream<data_t_L5>
//    >(
//    swoutput_3,
//    swinput_4,
//    //frame_out,
//    weights_w_conv3_1,
//    thresh_conv3,
//    alpha_conv3,
//    next_layer_means_conv3,
//    rand_map_conv3
//    );
//  // Fifth layer: conv layer
//  LUTNET_SlidingWindow<
//    L5_K, L5_N, L5_iR, L5_oR, L5_BW, 1>(
//    swinput_4,
//    swoutput_4,
//    L5_K,
//    L5_iR,
//    L5_oR,
//    0);
//  LUTNET_LUT22MV_TM<
//    L5_iR, L5_iC, L5_N, L5_oR, L5_oC, L5_M, L5_K, L5_ST, L5_BW, 24, 16, L5_TN, L5_TM,
//    hls::stream<data_t_L5>,
//    hls::stream<data_t_L6>
//    >(
//    swoutput_4,
//    mpinput_5,
//    //frame_out,
//    weights_w_conv4_1,
//    thresh_conv4,
//    alpha_conv4,
//    next_layer_means_conv4,
//    rand_map_conv4
//    );
//  // Sixth layer: maxpooling layer
//  MaxPool<
//    L6_iR, L6_iC, L6_N, L6_oR, L6_oC, L6_K, L6_BW,
//    hls::stream<data_t_L6>,
//    hls::stream<data_t_L7>
//    >(
//    mpinput_5,
//    swinput_6
//    );
//  // Seventh layer: conv layer
//  LUTNET_SlidingWindow<
//    L7_K, L7_N, L7_iR, L7_oR, L7_BW, 1>(
//    swinput_6,
//    swoutput_6,
//    L7_K,
//    L7_iR,
//    L7_oR,
//    0);
//  LUTNET_LUT22MV_TM<
//    L7_iR, L7_iC, L7_N, L7_oR, L7_oC, L7_M, L7_K, L7_ST, L7_BW, 24, 16, L7_TN, L7_TM,
//    hls::stream<data_t_L7>,
//    hls::stream<data_t_L8>
//    >(
//    swoutput_6,
//    swinput_7,
//    //frame_out,
//    weights_w_conv5_1,
//    thresh_conv5,
//    alpha_conv5,
//    next_layer_means_conv5,
//    rand_map_conv5
//    );
//  // Eighth layer: conv layer
//  LUTNET_SlidingWindow<
//    L8_K, L8_N, L8_iR, L8_oR, L8_BW, 1>(
//    swinput_7,
//    //frame_in,
//    swoutput_7,
//    L8_K,
//    L8_iR,
//    L8_oR,
//    0);
//  LUTNET_LUT22MV_TM<
//    L8_iR, L8_iC, L8_N, L8_oR, L8_oC, L8_M, L8_K, L8_ST, L8_BW, 24, 16, L8_TN, L8_TM,
//    hls::stream<data_t_L8>,
//    hls::stream<data_t_L9>
//    >(
//    swoutput_7,
//    fcinput_8,
//    //frame_out,
//    weights_w_conv6_1,
//    thresh_conv6,
//    alpha_conv6,
//    next_layer_means_conv6,
//    rand_map_conv6
//    );
//  // Ninth layer: fc layer
//  LUTNET_LUT22MV_TM<
//    L9_iR, L9_iC, L9_N, L9_oR, L9_oC, L9_M, L9_K, L9_ST, L9_BW, 24, 16, L9_TN, L9_TM,
//    hls::stream<data_t_L9>,
//    hls::stream<data_t_L10>
//    >(
//    fcinput_8,
//    fcinput_9,
//    weights_w_fc7_1,
//    thresh_fc7,
//    alpha_fc7,
//    next_layer_means_fc7,
//    rand_map_fc7
//    );
//  // Tenth layer: fc layer
//  LUTNET_LUT22MV_TM<
//    L10_iR, L10_iC, L10_N, L10_oR, L10_oC, L10_M, L10_K, L10_ST, L10_BW, 24, 16, L10_TN, L10_TM,
//    hls::stream<data_t_L10>,
//    hls::stream<data_t_L11>
//    >(
//    fcinput_9,
//    fcinput_10,
//    //frame_out,
//    weights_w_fc8_1,
//    thresh_fc8,
//    alpha_fc8,
//    next_layer_means_fc8,
//    rand_map_fc8
//    );
//  // Eleventh layer: fc layer
////  XNORMV<
//    LUTNET_LUT22MV_TM_NOTH<
//    L11_iR, L11_iC, L11_N, L11_oR, L11_oC, L11_M, L11_K, L11_ST, L11_BW, 24, 16, OutputWidth_val, L11_TN, L11_TM,
//    hls::stream<data_t_L11>,
//    hls::stream<data_t_out>
//    >(
//    fcinput_10,
//    frame_out,
//    weights_w_fc9_1,
//    alpha_fc9,
//    rand_map_fc9
//    );


}