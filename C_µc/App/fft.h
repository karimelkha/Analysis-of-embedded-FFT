/******************************************************************************
			PROJET 	S6
	EL-KHARROUBI 	GRAVES 	LEFEBVRE 	NEZET
*******************************************************************************/
#ifndef __FFT_H__
#define __FFT_H__
#define PI 3.1415927
#define X3(n) ((n<<1) + n)
#define RE(n,b) *(n+(b<<1))
#define I(n) *(n+1)
#define IM(n,b) *(n+(b<<1)+1)
#define W_0_4R(n) (RE(n,0)+RE(n,1)+RE(n,2)+RE(n,3))
#define W_0_4I(n) (IM(n,0)+IM(n,1)+IM(n,2)+IM(n,3))
#define W_1_4R(n) (*n+IM(n,1))-RE(n,2)-IM(n,3)
#define W_1_4I(n) (I(n)-RE(n,1)-IM(n,2)+RE(n,3))
#define W_2_4R(n) (RE(n,0)-RE(n,1)+RE(n,2)-RE(n,3))
#define W_2_4I(n) (IM(n,0)-IM(n,1)+IM(n,2)-IM(n,3))
#define W_3_4R(n) (RE(n,0)-IM(n,1)-RE(n,2)+IM(n,3))
#define W_3_4I(n) (IM(n,0)+RE(n,1)-IM(n,2)-RE(n,3))

void fftf_rdx2 (short *sig, float *TF, int N);
void fftf_rdx4 (short *sig, float *TF, int N, float *twiddles);
float *get_twiddles (int b);


#endif
