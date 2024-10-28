#include "image.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
  //  FILE *fileptr;
    
    printf("Expected Values: \n");
    printf("Header: P3\nWidth: 8px\nHeight: 2px\n");

    printf("Your Function: \n");
   // fileptr = fopen("./images/originals/building1.ppm" , "r");
    Image *image = load_image("./images/originals/building1.ppm");
 
    printf("Header: %s\nWidth: %dpx\nHeight: %dpx\nMax Intensity: %d\n", image->header, 
     image->width, image->height, image->max_intensity);

    delete_image(image);   
  //  fclose(fileptr);
    return 0;
}