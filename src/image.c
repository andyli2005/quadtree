#include "image.h"
#include <string.h>

Image *load_image(char *filename) {    
    FILE *fp = fopen(filename, "r");

    if(fp == NULL){
        return NULL;
    }

    char header[3];
    int width, height, max_intensity;
    fscanf(fp, "%s", header);

    while(1){
        char c;
        fscanf(fp, " %c", &c);
        if(c == '#'){
            while(fgetc(fp) != '\n');
        } else {
            ungetc(c, fp);
            break;
        }
    }

    fscanf(fp, "%d %d %d", &width, &height, &max_intensity);
    
    Image *image = malloc(sizeof(Image));
    strcpy(image->header, header);
    image->width = width;
    image->height = height;
    image->max_intensity = max_intensity;
    image->pixels = malloc(sizeof(int) * width * height);

    for(int i = 0; i < (width * height); i++){
        int r, g, b;
        fscanf(fp, "%d %d %d", &r, &g, &b);
        image->pixels[i] = b;
    }

    fclose(fp);
    return image;
}

void delete_image(Image *image) {
    free(image->pixels);
    free(image);
}

unsigned short get_image_width(Image *image) {
    return image->width;
}

unsigned short get_image_height(Image *image) {
    return image->height;
}

unsigned char get_image_intensity(Image *image, unsigned int row, unsigned int col) {
    int index = row * (image->width) + col; // OH: how to get specific index in a 1D array given row and column
    return image->pixels[index]; 
}

unsigned int hide_message(char *message, char *input_filename, char *output_filename) {
    Image *image = load_image(input_filename);
    FILE *write = fopen(output_filename, "w");
    if(image == NULL || write == NULL){
        return 0;
    }

    fprintf(write, "%s\n %d %d\n %d\n", image->header, image->width, image->height, image->max_intensity);

    int len = (int)strlen(message);
    int count = ((image->height * image->width) / 8) - 1; 

    if(count > len){
        count = len;
     }

    if(count < 1){
        for(int i = 0; i < 8; i++){
            image->pixels[i] &= ~(0x01); // only 8 bits
        }
    } else {
        int acc = 0;
        for(int i = 0; i < count; i++){
            int single_char = message[i]; 
            for(int j = 7; j >= 0; j--){
                image->pixels[acc] = (image->pixels[acc] & ~(0x01)) | ((single_char >> j) & 1);
                acc++;
            }
        }
        for(int i = 0; i < 8; i++){
            image->pixels[acc] &= ~(1);
            acc++;
        }
    }

    for(int i = 0; i < image->height * image->width; i++){
        fprintf(write, "%d %d %d ", image->pixels[i], image->pixels[i], image->pixels[i]);
    }
    
    delete_image(image);
    fclose(write);
    return count;
}

char *reveal_message(char *input_filename) {
    Image *image = load_image(input_filename);
    int image_size = image->width * image->height;
    char *message;

    if(image_size <= 8){
        message = malloc(1);
        message[0] = '\0';
    } else {
        message = malloc(image_size / 8 + 1); 
        int acc = 0, count = 0, index = 0;
        for(int i = 0; i < image_size; i++){ 
            int bit = image->pixels[i] & 0x01;
            acc = (acc << 1) | bit;
            count++;
            if(count == 8){
                char single_char = acc;
                message[index] = single_char;
                if(single_char == '\0'){
                    break;
                }
                acc = 0;
                count = 0;
                index++;
            }
        }
    }
    
    delete_image(image);
    return message;
}

unsigned int hide_image(char *secret_image_filename, char *input_filename, char *output_filename) {
    Image *image = load_image(input_filename);
    Image *secret_image = load_image(secret_image_filename);
    FILE *write = fopen(output_filename, "w");
 
    if(image == NULL || secret_image == NULL || write == NULL){
        return 0;
    }

    
    int width = secret_image->width;
    int height = secret_image->height;

    int size_of_secret_image = ((width * height) * 8) + 16;

    if(size_of_secret_image > (image->width * image->height)){
        delete_image(secret_image);
        delete_image(image);
        fclose(write);
        return 0;
    }

    fprintf(write, "%s\n %d %d\n %d\n", image->header, image->width, image->height, image->max_intensity);

    int index = 0;
    for(int i = 7; i >= 0; i--){
        image->pixels[index] = (image->pixels[index] & ~(0x01)) | ((width >> i) & 1);
        index++;
    }

    for(int i = 7; i >= 0; i--){
        image->pixels[index] = (image->pixels[index] & ~(0x01)) | ((height >> i) & 1);
        index++;
    }

    for(int i = 0; i < (width * height); i++){
        for(int j = 7; j >= 0; j--){
            image->pixels[index] = (image->pixels[index] & ~(0x01)) | ((secret_image->pixels[i] >> j) & 1);
            index++;
        }
    }

    for(int i = 0; i < image->height * image->width; i++){
        fprintf(write, "%d %d %d ", image->pixels[i], image->pixels[i], image->pixels[i]);
    }
    
    delete_image(secret_image);
    delete_image(image);
    fclose(write);
    return 1;
}

void reveal_image(char *input_filename, char *output_filename) {
    Image *image = load_image(input_filename);
    FILE *write = fopen(output_filename, "w");
    
    fprintf(write, "%s\n", image->header);

    int width = 0, height = 0, index = 0;

    for(int i = 0; i < 8; i++){
        int bit = image->pixels[index] & 0x01;
        width = (width << 1) | bit;
        index++;
    }

    for(int i = 0; i < 8; i++){
        int bit = image->pixels[index] & 0x01;
        height = (height << 1) | bit;
        index++;
    }

    fprintf(write, "%d %d\n %d\n", width, height, image->max_intensity);

    int secret_image_size = width * height * 8;
    int acc = 0, count = 0;

    for(int i = 0; i < secret_image_size; i++){
        int bit = image->pixels[index] & 0x01;
        acc = (acc << 1) | bit;
        count++;
        if(count == 8){
            fprintf(write, "%d %d %d ", acc, acc, acc);
            acc = 0;
            count = 0;
        }
        index++;
    }
    
    delete_image(image);
    fclose(write);
}