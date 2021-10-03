/******************************************************************************
			PROJET 	S6
	EL-KHARROUBI 	GRAVES 	LEFEBVRE 	NEZET
*******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <complex.h>
#include <time.h>
#include "tst_func.h"
#include "smp_tst.h"
#include "string.h"
#define MAX_POW 14
#define MAX 1<<MAX_POW
#define MAX_SMP_VAL 1024
#define BUF_SZ 64
int
main (int argc, char **argv)
{


  char buf_c[BUF_SZ];
  struct timespec now, bf;
  FILE *results;
  clock_gettime (CLOCK_REALTIME, &now);


  results = fopen ("tst_results.txt", "w");


  if (argc == 1)
    no_arg (bf, now, results);
  else
    {
      if (strspn (argv[1], "0123456789") == strlen (argv[1]))
	{
	  tst_one_tf (argv[1], bf, now, results);
	}
      else
	{
	  if (argc == 3)
	    {

	      strcpy (buf_c, argv[1]);
	      if (!strcmp (buf_c, "init"))
		{
		  init (argv[2]);
		}
	      else
		{
		  ml_compare (argv[1], argv[2], results, bf, now);
		}
	    }
	  else
	    {

	      printf ("Pas assez d'arguments!\n");
	    }
	}

    }
  fclose (results);
  return 0;
}
