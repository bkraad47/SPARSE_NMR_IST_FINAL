/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
    
    by Badruddin Kamal developed as part of my ANU COMP8715 Project
*/


# include <stdlib.h>
# include <stdio.h>
# include <time.h>
#include <string.h>
# include <fftw3.h>
#include <time.h>
#include <math.h>

char *append(const char *orig, char c)
{
    size_t sz = strlen(orig);
    char *str = malloc(sz + 2);
    strcpy(str, orig);
    str[sz] = c;
    str[sz + 1] = '\0';
    return str;
}


void calc_ist(int nx,int ny,int iter,char* file_in,char* file_out,double thresh_percent)
{
  int i,j;
  int sparse_len=0;
  double *out,*in,*final;
  int *sparse=malloc ( sizeof ( int ) * nx * ny  );
  fftw_plan plan_backward,plan_forward;
  char ch;
  FILE *fp;
  int iter_count=0;
  double max,thresh;


//Create the input & final array.

  in = malloc ( sizeof ( double ) * nx * ny  );


  final = malloc ( sizeof ( double ) * nx * ny  );

//initialize input & final array to 0.0
  for ( i = 0; i < nx; i++ )
 {
   for ( j = 0; j < ny; j++ )
  {
   if((j+4)<ny){
   final[i*ny+j]=0.0;
   final[i*ny+(j+1)]=0.0;
   final[i*ny+(j+2)]=0.0;
   final[i*ny+(j+3)]=0.0;
   in[i*ny+j]=0.0;
   in[i*ny+(j+1)]=0.0;
   in[i*ny+(j+2)]=0.0;
   in[i*ny+(j+3)]=0.0;
   j=j+3;
   }else{
   final[i*ny+j]=0.0;
   in[i*ny+j] = 0.0;
   }
 }
}

// simple file reader for x,y,sigma,I\n per line
  fp = fopen(file_in,"r");

  if( fp == NULL )
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }

  int type =0;
  //initialize array
  char *x = "0";
  char *y= "0";
  char *I = "0";
  //read csv file
  while( ( ch = fgetc(fp) ) != EOF ){

      switch (ch ){
       case '"':
       case '\'': /* can ' start a quoted string? */
           break;
       case ',':
            type= type+1;
            break;
       case '\n':
            i=atoi(x);
            j=atoi(y);
            in[i*ny+j]= strtod(I,NULL);

            // keep track of sparse points 
            if(strtod(I,NULL)==0.0){
              sparse[sparse_len]=i*ny+j;
              sparse_len=sparse_len+1;
            }
            *x='\0';
            *y='\0';
            *I='\0';
            type=0;
           break;
        default:
	   if(type==0){
  	     x=append(x,ch);
	   }else if(type==1){
              y=append(y,ch);
	   }else if(type==2){
            I=append(I,ch);
	   }
           break;


      }

  }


  fclose(fp);
 

  printf ( "\n" );
  printf ( "  Input Data Read\n" );
  printf ( "\n" );



  //  out array initialization

  out = malloc ( sizeof ( double ) * nx * ny  );

  //iterate for IST
  clock_t start = clock();

  while(iter_count<iter){

  //Do FFT
  plan_forward =  fftw_plan_r2r_1d( (ny*nx), in,out, FFTW_REDFT10, FFTW_ESTIMATE );

  fftw_execute ( plan_forward );


  // initialize max in every itteration
  max=0.0;
  //Format out data & find max data point

    for ( i = 0; i < nx; i++ )
  {
    for ( j = 0; j < ny; j++ )
    {
     if((j+4)<ny){
     out[i*ny+j]= out[i*ny+j] / ( nx * ny*2 );
     out[i*ny+(j+1)]= out[i*ny+(j+1)] / ( nx * ny*2 );
     out[i*ny+(j+2)]= out[i*ny+(j+2)] / ( nx * ny*2 );
     out[i*ny+(j+3)]= out[i*ny+(j+3)] / ( nx * ny*2 );
     if(out[i*ny+j]>max){
      max=out[i*ny+j];
      }
     if(out[i*ny+(j+1)]>max){
      max=out[i*ny+(j+1)];
      }
     if(out[i*ny+(j+2)]>max){
      max=out[i*ny+(j+2)];
      }
     if(out[i*ny+(j+3)]>max){
      max=out[i*ny+(j+3)];
      }
     j=j+3;
     }else{
     out[i*ny+j]= out[i*ny+j] / ( nx * ny*2 );
     if(out[i*ny+j]>max){
      max=out[i*ny+j];
      }
     }
    }
  } 


   // define max threshold and chop off chunks
  

   thresh=max*thresh_percent;

   //chop off peaks and store in final


   for ( i = 0; i < nx; i++ )
  {
    for ( j = 0; j < ny; j++ )
    { 
     if((j+4)<ny){

      if(out[i*ny+j]>=thresh){
        final[i*ny+j] = final[i*ny+j]+ (out[i*ny+j]-thresh) ;
        out[i*ny+j] =thresh;
      }

      if(out[i*ny+(j+1)]>=thresh){
        final[i*ny+(j+1)] = final[i*ny+(j+1)]+ (out[i*ny+(j+1)]-thresh) ;
        out[i*ny+(j+1)] =thresh;
      }

      if(out[i*ny+(j+2)]>=thresh){
        final[i*ny+(j+2)] = final[i*ny+(j+2)]+ (out[i*ny+(j+2)]-thresh) ;
        out[i*ny+(j+2)] =thresh;
      }


      if(out[i*ny+(j+3)]>=thresh){
        final[i*ny+(j+3)] = final[i*ny+(j+3)]+ (out[i*ny+(j+3)]-thresh) ;
        out[i*ny+(j+3)] =thresh;
      }

     j=j+3;
     }else{
      if(out[i*ny+j]>=thresh){
        final[i*ny+j] = final[i*ny+j]+ (out[i*ny+j]-thresh) ;
        out[i*ny+j] =thresh;
      }
     }
    }
  }



  //Do IFT


  plan_backward = fftw_plan_r2r_1d( (ny*nx), out,in, FFTW_REDFT01, FFTW_ESTIMATE );

  fftw_execute ( plan_backward );

  //set empty points

  for ( i = 0; i < sparse_len; i++ )
  {
    if((i+4)<sparse_len){
     j=sparse[i];
     in[j] =0.0;
     j=sparse[i+1];
     in[j] =0.0;
     j=sparse[i+2];
     in[j] =0.0;
     j=sparse[i+3];
     in[j] =0.0;
     i=i+3;
     }else{
     j=sparse[i];
     in[j] =0.0;
     }  
  }


 iter_count=iter_count+1;
}

printf("  Time elapsed: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC);


  printf ( "\n" );
  printf ( "  End of iteration storing data\n" );
  printf ( "\n" );

//print output

FILE *f = fopen(file_out, "w");
if (f == NULL)
{
    printf("Error opening file!\n");
    exit(1);
}
  for ( i = 0; i < nx/2; i++ )
   {
   for ( j = 0; j < ny/2; j++ )
    {
     fprintf (f, "%4d,%4d,%15.8f\n", i, j, final[i*ny+j] );
    }
  } 
fclose(f);



//Free up the allocated memory.

  fftw_destroy_plan ( plan_forward );
  fftw_destroy_plan ( plan_backward );

  free ( in );
  free ( out );
  free ( final );
  free ( sparse );
  free(x);
  free(y);
  free(I);

}
