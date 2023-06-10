//#include <stdio.h>
//#include "tinymaix.h"
//#include "lcd.h"
//#include <stdint.h>

#if TM_MDL_TYPE == TM_MDL_INT8
#include "../tools/tmdl/model-53270-final.h"
//#include "../../tools/tmdl/mnist_resnet_q.h"
#elif TM_MDL_TYPE == TM_MDL_FP32
#include "../../tools/tmdl/cnn_face_02.h"
//#include "../../tools/tmdl/mnist_resnet_f.h"
#elif TM_MDL_TYPE == TM_MDL_FP16
#include "../../tools/tmdl/mnist_valid_fp16.h"
#elif TM_MDL_TYPE == TM_MDL_FP8_143
#include "../../tools/tmdl/mnist_fp8_143.h"
#elif TM_MDL_TYPE == TM_MDL_FP8_152
#include "../../tools/tmdl/mnist_fp8_152.h"
#endif


extern unsigned int picture[64*64];

static tm_err_t layer_cb(tm_mdl_t* mdl_2, tml_head_t* lh)
{   //dump middle result
//    int h = lh->out_dims[1];
//    int w = lh->out_dims[2];
//    int ch= lh->out_dims[3];
//    mtype_t* output = TML_GET_OUTPUT(mdl_2, lh);
//    return TM_OK;
//    TM_PRINTF("Layer %d callback ========\n", mdl_2->layer_i);
//    #if 1
//    for(int y=0; y<h; y++){
//        TM_PRINTF("[");
//        for(int x=0; x<w; x++){
//            TM_PRINTF("[");
//            for(int c=0; c<ch; c++){
//            #if TM_MDL_TYPE == TM_MDL_FP32
//                TM_PRINTF("%.3f,", output[(y*w+x)*ch+c]);
//            #else
//                TM_PRINTF("%.3f,", TML_DEQUANT(lh,output[(y*w+x)*ch+c]));
//            #endif
//            }
//            TM_PRINTF("],");
//        }
//        TM_PRINTF("],\n");
//    }
//    TM_PRINTF("\n");
//    #endif
//    return TM_OK;
}

static void parse_output(tm_mat_t* outs)
{
//    tm_mat_t out = outs[0];
//    float* data  = out.dataf;
//    float maxp = 0;
//    int maxi = -1;
//		u8 char_maxi[32],char_maxp[32];
//    for(int i=0; i<10; i++){
//        printf("%d: %.3f\n", i, data[i]);
//        if(data[i] > maxp) {
//            maxi = i;
//            maxp = data[i];
//        }
//    }
//		//sprintf((char*)char_maxi,"Predict output is: Number %d",maxi);
//		//sprintf((char*)char_maxp,"prob %f",maxp);
//    TM_PRINTF("### Predict output is: Number %d, prob %.3f\n", maxi, maxp);
//		//LCD_ShowString(5,240,240,12,16,char_maxi);
//		//LCD_ShowString(5,280,240,12,16,char_maxp);
//    return;
}

int cnn_face_main(int argc, char** argv)
{	
//	tm_mdl_t mdl_2;
//	
//	TM_DBGT_INIT();
//  TM_PRINTF("mnist demo\n");
//	
//		tm_mat_t in_uint8 = {3,64,64,1, {(mtype_t*)picture}};//-128~127
//		//own_mat_t in_uint8 = {3,64,64,1, {(unsigned int*)picture}};//-128~127
//		tm_mat_t in = {3,64,64,1, {NULL}};//-128~127
//		tm_mat_t outs[1];
//		tm_err_t res;

//		tm_stat((tm_mdlbin_t*)mdl_data); 
//		
//		res = tm_load(&mdl_2, mdl_data, NULL, layer_cb, &in);
//    if(res != TM_OK) {
//        TM_PRINTF("tm model load err %d\n", res);
//        return -1;
//    }
//#if (TM_MDL_TYPE == TM_MDL_INT8) || (TM_MDL_TYPE == TM_MDL_INT16) 
//    res = tm_preprocess(&mdl_2, TMPP_UINT2INT, &in_uint8, &in); //0~256 -> -128~127
//#else
//    res = tm_preprocess(&mdl_2, TMPP_UINT2FP01, &in_uint8, &in); 
//#endif
//		TM_DBGT_START();
//		res = tm_run(&mdl_2, &in, outs);
//		TM_DBGT("tm_run");
//    if(res==TM_OK) parse_output(outs);  
//    else TM_PRINTF("tm run error: %d\n", res);
//		
//		tm_unload(&mdl_2);                 

//		return 0;


//	
}
