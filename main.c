#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct BITMAP_header{
    char name[2];//should be BM
    uint32_t size;
    uint32_t garbage;
    uint32_t image_offset;//from where the pixel info start since there can be other header info

};

struct DIB_header{
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t colorplanes;
    uint16_t bitsperpixel;
    uint32_t compression;
    uint32_t image_size;
    uint32_t temp[4]; //to compensate hus dibheader actually 40 bytes

};

// struct RGB {
//     uint8_t blue;
//     uint8_t green;
//     uint8_t red;
// };

// uint8_t greyscale(struct RGB rgb){
//     return ((0.3 * rgb.red)+ (0.6 *rgb.green) + (0.1*rgb.blue))/3;
// };

// void RGBImageToGreyscale(uint32_t height, uint32_t width, struct RGB image[height][width]){
//     struct RGB imagenew[height][width];
    // printf("%d", image[0][100].red);
    // for (int i =0; i<height-1; i++){
    //     for(int j = 0; j<width-1; j++){ 

    //         // printf("%d %d ", i , j);  
    //         // printf("%d \n", greyscale(image[i][j]));      
    //         // imagenew[i][j].red = greyscale(image[i][j]);
    //         // imagenew[i][j].green = greyscale(image[i][j]);
    //         // imagenew[i][j].blue = greyscale(image[i][j]); 
    //     }
    // }

// }

// void createImage(struct BITMAP_header header, struct DIB_header dibheader, struct RGB image[dibheader.height][dibheader.width]){
//     FILE *fpn = fopen("new.bmp", "w");
//     fwrite(header.name, 2,1,fpn);
//     fwrite(&header.size,4,1,fpn);
//     fwrite(&dibheader, sizeof(struct DIB_header),1,fpn);
//     printf("%d", image[0][100].red);
//     RGBImageToGreyscale(dibheader.height, dibheader.width, image);
//     for (int i = dibheader.height-1; i<0; i--){
//         for (int j=0; j >= dibheader.width; j++){
//             fwrite(&image[i][j], sizeof(struct RGB), 1, fpn);
//             }
//     }
//     fclose(fpn);

// }

void openbmpfile(){
    FILE *fp = fopen("cat.bmp", "rb");
    struct BITMAP_header header;
    struct DIB_header dibheader;
    uint8_t image[dibheader.height][dibheader.width][3];
    
    fread(header.name, 2,1,fp); //cant read the entire structure &header because it is 16byte instead of 14byte cuz C make all 4 byte for efficiency
    fread(&header.size, 4,1,fp);
    fread(&header.garbage, 4,1,fp);
    fread(&header.image_offset, 4,1,fp);
    printf("Firsts two chatecters:%c%c\n",header.name[0], header.name[1]);
    printf("size = %d\n", header.size);
    printf("Offset %d\n", header.image_offset);    
    
    fread(&dibheader, sizeof(struct DIB_header),1, fp);//can read the entire structure because total multiple of 4
    printf("header size %d width %d height %d color planes %d bits per pixel %d \n compression %d image size %d \n"
    ,dibheader.header_size, dibheader.width, dibheader.height, dibheader.colorplanes, dibheader.bitsperpixel, dibheader.compression, dibheader.image_size);
    
    fseek(fp, header.image_offset, SEEK_SET);//starting from the beginning go ahead by image offset
    //the image is represented from bottom to top

    // fread(image,1,dibheader.image_size,fp);
    
    // int i=dibheader.height -1,j=0;
    // printf(" %d %d \n", i, j);
    // fread(&image[i][j], sizeof(image[i][j]), 1, fp);    
    // j++;
    // printf(" %d %d \n", i, j);
    // fread(&image[i][j], sizeof(image[i][j]), 1, fp);
    // j++;
    // printf(" %d %d \n", i, j);
    // fread(&image[i][j], sizeof(image[i][j]), 1, fp);

    // printf("%d", dibheader.height-1);    
    // for (i = dibheader.height -1; i>=0; i=i-1){
    //     for (j=0; j < dibheader.width; j++){ 
    //         printf(" %d %d \n", i, j);               
    //         fread(&image[i][j], sizeof(image[i][j]), 1, fp);
    //         }        
    // }   
    // printf("%d \n", image[0][100].red);
    

    //createImage(header, dibheader, image);

    

    fclose(fp);
}

void main(){
    openbmpfile();
    
}