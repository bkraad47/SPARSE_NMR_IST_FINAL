%module ist
 %{
 void calc_ist(int nx,int ny,int iter,char* file_in,char* file_out,double thresh_percent);
 extern char *append(const char *orig, char c);
 %}
 
 void calc_ist(int nx,int ny,int iter,char* file_in,char* file_out,double thresh_percent);
 extern char *append(const char *orig, char c);

