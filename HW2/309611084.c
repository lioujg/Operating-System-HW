#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main(int argc, char *argv[])
{
   char ch;
   FILE *source, *target;

   if (argc != 3) {
	   printf("Usage: ./simple-cp SOURCE DEST\n");
      	   exit(EXIT_FAILURE);
   }

 
   source = fopen(argv[1], "rb");
 
   if(source == NULL)
   {
      printf("Error: cannot stat %s: No such file or directory\n", argv[1]);
      exit(EXIT_FAILURE);
   }
 
   target = fopen(argv[2], "wb");
 
   if(target == NULL)
   {
      fclose(source);
      printf("Error: cannot create regular file %s: No such file or directory\n", argv[2]);
      exit(EXIT_FAILURE);
   }
   size_t bytes;
   char buffer[512];   
   while ((bytes = fread(buffer, 1, 512, source)) != 0) {
        if(fwrite(buffer, 1, bytes, target) != bytes) {
            return 1;                                           // or other action
        }
   } 
   fclose(source);
   fclose(target);
 
   return 0;
}
