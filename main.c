#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void save_4bytes(uint32_t val, FILE *stream)
{
    fprintf(stream, "%c", (val & 0x000000FF) >> 0 * 8);
    fprintf(stream, "%c", (val & 0x0000FF00) >> 1 * 8);
    fprintf(stream, "%c", (val & 0x00FF0000) >> 2 * 8);
    fprintf(stream, "%c", (val & 0xFF000000) >> 3 * 8);
}

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


uint8_t greyscale(uint8_t rgb[3]){
    return (uint8_t)((0.3 * rgb[0]) + (0.59 * rgb[1]) + (0.11 * rgb[2]));
};

void RGBImageToGreyscale(uint32_t height, uint32_t width,  uint8_t image[height][width][3]){
    for (int i =0; i<=height-1; i++){
        for(int j = 0; j<=width-1; j++){             
            image[i][j][0] = greyscale(image[i][j]);
            image[i][j][1] = greyscale(image[i][j]);
            image[i][j][2] = greyscale(image[i][j]); 
        }
    }    

}

void createImage(struct BITMAP_header header, struct DIB_header dibheader, uint8_t image[dibheader.height][dibheader.width][3]){
    FILE *fpn = fopen("new.bmp", "wb");
    uint32_t padded_size= dibheader.width*3 * dibheader.height;
    fprintf(fpn, "BM");
    save_4bytes(padded_size + 54, fpn);
    save_4bytes(0, fpn);
    save_4bytes(54, fpn);
    save_4bytes(40, fpn);
    save_4bytes(dibheader.width, fpn);
    save_4bytes(dibheader.height, fpn);
    fprintf(fpn, "%c", 1);
    fprintf(fpn, "%c", 0);
    fprintf(fpn, "%c", 24);
    fprintf(fpn, "%c", 0);
    save_4bytes(0, fpn);
    save_4bytes(padded_size, fpn);
    save_4bytes(0, fpn);
    save_4bytes(0, fpn);
    save_4bytes(0, fpn);
    save_4bytes(0, fpn);

    RGBImageToGreyscale(dibheader.height, dibheader.width, image);

    for (int i = 0; i < dibheader.height; i++)
    {
        for (int j = 0; j < dibheader.width; j++)
        {
            for (int k = 2; k >= 0; k--)
            {
                fputc(image[dibheader.height-1-i][j][k],fpn);
            }     
        }     
    }
    fclose(fpn);

}

void main(){
    FILE *fp = fopen("sample1.bmp", "rb");
    struct BITMAP_header header;
    struct DIB_header dibheader;    
    
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
    uint8_t image[dibheader.height][dibheader.width][3];
    //the image is represented from bottom to top
    for (int row = dibheader.height - 1; row >= 0; row--) {
        fread(image[row], 1, dibheader.width * 3, fp);
    }
    createImage(header, dibheader, image);
    fclose(fp);
    
    
}