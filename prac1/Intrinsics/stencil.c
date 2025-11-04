#include "stencil.h"
#include "immintrin.h"



void ApplyStencil(unsigned char *img_in, unsigned char *img_out, int width, int height) {
  
	short val;
	unsigned char val_out;

	for (int i = 1; i < height-1; i++)
		for (int j = 1; j < width-1; j++) {
			val = img_in[(i  )*width + j];
			val +=	-img_in[(i-1)*width + j-1] -   img_in[(i-1)*width + j] - img_in[(i-1)*width + j+1] 
					-img_in[(i  )*width + j-1] + 7*img_in[(i  )*width + j] - img_in[(i  )*width + j+1] 
					-img_in[(i+1)*width + j-1] -   img_in[(i+1)*width + j] - img_in[(i+1)*width + j+1];
			if (val<0){
				val=0;
			}else val_out = (unsigned char)val;
			if (val>255){
				val_out=255;
			}else val_out = (unsigned char)val;

			img_out[i*width + j] = (unsigned char)(val_out);
		}
}

void ApplyStencil_Intrinceca(unsigned char *img_in, unsigned char *img_out, int width, int height) {

	short val;
	unsigned char val_out;
	__m128i sse_1, sse_2, sse_3, sse_4, sse_5, sse_6, sse_7, sse_8, sse_9;//, sse_cte;
	__m128i sse_val;

	//sse_cte = _mm_set1_epi8(8)
	__m128i zeros = _mm_setzero_si128();
	__m128i sse_val_hi, sse_val_lo;

	for (int i = 1; i < height-1; i++)
		for (int j = 1; j < width-16; j+=16) {
	        sse_1 = _mm_loadu_si128((const __m128i_u *)&img_in[(i-1)*width + j-1]);
			sse_2 = _mm_loadu_si128((const __m128i_u *)&img_in[(i-1)*width + j]);
			sse_3 = _mm_loadu_si128((const __m128i_u *)&img_in[(i-1)*width + j+1]);
			sse_4 = _mm_loadu_si128((const __m128i_u *)&img_in[(i  )*width + j-1]);
			sse_5 = _mm_loadu_si128((const __m128i_u *)&img_in[(i  )*width + j]);
			sse_6 = _mm_loadu_si128((const __m128i_u *)&img_in[(i  )*width + j+1]);
			sse_7 = _mm_loadu_si128((const __m128i_u *)&img_in[(i+1)*width + j-1]);
			sse_8 = _mm_loadu_si128((const __m128i_u *)&img_in[(i+1)*width + j]);
			sse_9 = _mm_loadu_si128((const __m128i_u *)&img_in[(i+1)*width + j+1]);

			//unpack into a 16-bit style
			__m128i lo16_1 = _mm_unpacklo_epi8(sse_1, zeros);
			__m128i hi16_1 = _mm_unpackhi_epi8(sse_1, zeros);
			__m128i lo16_2 = _mm_unpacklo_epi8(sse_2, zeros);
			__m128i hi16_2 = _mm_unpackhi_epi8(sse_2, zeros);
			__m128i lo16_3 = _mm_unpacklo_epi8(sse_3, zeros);
			__m128i hi16_3 = _mm_unpackhi_epi8(sse_3, zeros);
			__m128i lo16_4 = _mm_unpacklo_epi8(sse_4, zeros);
			__m128i hi16_4 = _mm_unpackhi_epi8(sse_4, zeros);
			__m128i lo16_5 = _mm_unpacklo_epi8(sse_5, zeros);
			__m128i hi16_5 = _mm_unpackhi_epi8(sse_5, zeros);
			__m128i lo16_6 = _mm_unpacklo_epi8(sse_6, zeros);
			__m128i hi16_6 = _mm_unpackhi_epi8(sse_6, zeros);
			__m128i lo16_7 = _mm_unpacklo_epi8(sse_7, zeros);
			__m128i hi16_7 = _mm_unpackhi_epi8(sse_7, zeros);
			__m128i lo16_8 = _mm_unpacklo_epi8(sse_8, zeros);
			__m128i hi16_8 = _mm_unpackhi_epi8(sse_8, zeros);

			//operate with 16-bits to not get saturated
			lo16_1 = _mm_adds_epi16(lo16_5, lo16_1);
			hi16_1 = _mm_adds_epi16(hi16_5, hi16_1);
			lo16_2 = _mm_adds_epi16(lo16_5, lo16_2);
			hi16_2 = _mm_adds_epi16(hi16_5, hi16_2);
			lo16_3 = _mm_adds_epi16(lo16_3, lo16_3);
			hi16_3 = _mm_adds_epi16(hi16_3, hi16_3);
			lo16_4 = _mm_adds_epi16(lo16_5, lo16_4);
			hi16_4 = _mm_adds_epi16(hi16_5, hi16_4);
			lo16_6 = _mm_adds_epi16(lo16_5, lo16_6);
			hi16_6 = _mm_adds_epi16(hi16_5, hi16_6);
			lo16_7 = _mm_adds_epi16(lo16_5, lo16_7);
			hi16_7 = _mm_adds_epi16(hi16_5, hi16_7);
			lo16_8 = _mm_adds_epi16(lo16_5, lo16_8);
			hi16_8 = _mm_adds_epi16(hi16_5, hi16_8);

			sse_val_lo =  _mm_adds_epi16(lo16_1, lo16_2);
			sse_val_hi =  _mm_adds_epi16(hi16_1, hi16_2);
			sse_val_lo =  _mm_adds_epi16(sse_val_lo, lo16_3);
			sse_val_hi =  _mm_adds_epi16(sse_val_hi, hi16_3);
			sse_val_lo =  _mm_adds_epi16(sse_val_lo, lo16_4);
			sse_val_hi =  _mm_adds_epi16(sse_val_hi, hi16_4);
			sse_val_lo =  _mm_adds_epi16(sse_val_lo, lo16_6);
			sse_val_hi =  _mm_adds_epi16(sse_val_hi, hi16_6);
			sse_val_lo =  _mm_adds_epi16(sse_val_lo, lo16_7);
			sse_val_hi =  _mm_adds_epi16(sse_val_hi, hi16_7);
			sse_val_lo =  _mm_adds_epi16(sse_val_lo, lo16_8);
			sse_val_hi =  _mm_adds_epi16(sse_val_hi, hi16_8);

            __m128i sse_val = _mm_packus_epi16(sse_val_lo, sse_val_hi);
			/*
			sse_1 = _mm_subs_epu8(sse_5, sse_1);
			sse_2 = _mm_subs_epu8(sse_5, sse_2);
			sse_3 = _mm_subs_epu8(sse_5, sse_3);
			sse_4 = _mm_subs_epu8(sse_5, sse_4);
			sse_6 = _mm_subs_epu8(sse_5, sse_6);
			sse_7 = _mm_subs_epu8(sse_5, sse_7);
			sse_8 = _mm_subs_epu8(sse_5, sse_8);
			sse_9 = _mm_subs_epu8(sse_5, sse_9);
			sse_val = _mm_adds_epu8(sse_1, sse_2);
			sse_val = _mm_adds_epu8(sse_val, sse_3);
			sse_val = _mm_adds_epu8(sse_val, sse_4);
			sse_val = _mm_adds_epu8(sse_val, sse_6);
			sse_val = _mm_adds_epu8(sse_val, sse_7);
			sse_val = _mm_adds_epu8(sse_val, sse_8);
			sse_val = _mm_adds_epu8(sse_val, sse_9);
			*/

			_mm_store_si128((__m128i *)&img_out[i*width + j], sse_val);
		}
}

void CopyImage(unsigned char *img_in, unsigned char *img_out, int width, int height) {

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			img_in[i*width + j] = img_out[i*width + j];
}
