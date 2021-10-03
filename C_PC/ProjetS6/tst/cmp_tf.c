/******************************************************************************
			PROJET 	S6
	EL-KHARROUBI 	GRAVES 	LEFEBVRE 	NEZET
*******************************************************************************/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

float
cmp_tf (float *ref, float *tf2, int N)
{
  int i;
  float tmp, f = 0;
  for (i = 0; i < N * 2; i++)
    {
      if (*(ref + i) != *(tf2 + i)){
	tmp = fabs (*(ref + i)) /fabs( *(tf2 + i))-1;
	}else{
	tmp=0;
	}
      f += tmp;

    }
  return f/N ;

}

// Etudie l'erreur par rapport aux résultats Matlab
float
cmp_fftw (float *ref, float *tf2, int N)
{
  int i;
  float tmp, f = 0;
  for (i = 0; i < N; i++)
    {
      if (*(ref + i) != *(tf2 + i))
	tmp = fabs (*(ref + i*2) / *(tf2 + i));
      if (tmp > 1)
	f += (tmp - 1);
      else
	{
	  f += 1 - tmp;
	}

    }
  return f /= N;

}
