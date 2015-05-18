# include <stdlib.h>
# include <stdio.h>
# include <time.h>
#include <string.h>
# include <fftw3.h>


// string append
char *append(const char *orig, char c)
{
    size_t sz = strlen(orig);
    char *str = malloc(sz + 2);
    strcpy(str, orig);
    str[sz] = c;
    str[sz + 1] = '\0';
    return str;
}




int main(void)
{
//input
int givenx=128;
int giveny=128;
int sparsity=50;
srand(1234567890);

 int i,j;
  int nx = givenx*2;
  int ny = giveny*2;
  double *out,*in;
  fftw_plan plan_backward;
  char ch;
  FILE *fp;





/*
  Create the input array.
*/
  in = malloc ( sizeof ( double ) * nx * ny  );
//initialize array to 0.0
  for ( i = 0; i < nx; i++ )
 {
   for ( j = 0; j < ny; j++ )
  {
   in[i*ny+j] = 0.0;
 }
}

// simple file reader for x,y,sigma,I\n per line
  fp = fopen("sample_NMR.txt","r");

  if( fp == NULL )
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }

  int type =0;
  //initialize array
  char *x = "0";
  char *y= "0";
  char *sigma = "0";
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
            in[i*ny+j] = strtod(I,NULL);
            *x='\0';
            *y='\0';
            *I='\0';
            *sigma='\0';
            type=0;
           break;
        default:
	   if(type==0){
  	     x=append(x,ch);
	   }else if(type==1){
              y=append(y,ch);
	   }else if(type==2){
             sigma=append(sigma,ch);
	   }else if(type==3){
            I=append(I,ch);
	   }
           break;


      }

  }


  fclose(fp);
 

  printf ( "\n" );
  printf ( "  Input Data Read\n" );
  printf ( "\n" );

 
// Create the output array initialized to 0.0

  out = malloc ( sizeof ( double ) * nx * ny );
  for ( i = 0; i < nx; i++ )
   {
   for ( j = 0; j < ny; j++ )
   {
   out[i*ny+j]= 0.0;
    }
  }
// Execute IFT


  plan_backward = fftw_plan_r2r_1d( (ny*nx), in,out, FFTW_REDFT01, FFTW_ESTIMATE );

  fftw_execute ( plan_backward );



//sparsify data

int max_count=(sparsity*nx*ny)/(100);
int count=0;
while (count < max_count){

     int select_x=nx+rand()/ (RAND_MAX / (0-nx+1)+1);
     int select_y=ny+rand()/ (RAND_MAX / (0-ny+1)+1);
     if(out[select_x*ny+select_y]!=0.0 ){
     out[select_x*ny+select_y]=0.0;
     count=count+1;
     }
}

printf("  Done generating sparse\n");

//Create dump file


FILE *f = fopen("sample_sparse_NMR.txt", "w");
if (f == NULL)
{
    printf("Error opening file!\n");
    exit(1);
}


  for ( i = 0; i < nx; i++ )
  {
    for ( j = 0; j < ny; j++ )
    {
     fprintf (f, "%4d,%4d,%8.15f\n", i, j, out[i*ny+j] );
    }
  } 

fclose(f);

  printf ( "\n" );
  printf ( "  Sparse file generated \n" );
  printf ( "\n" );

/*
  Free up the allocated memory.
*/
  fftw_destroy_plan ( plan_backward );
  free ( in );
  free ( out );
  free(x);
  free(y);
  free(sigma);
  free(I);




return 0;
}
