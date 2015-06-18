#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//structure의 값을 1바이트씩 받아오겠다고 선언
#define WIDTHBYTES(bits) (((bits)+31)/32*4)



//structure 선언
#pragma pack(1)
typedef struct BITMAPFILEHEADER
{
	unsigned short identity; // 2 byte : “BM” is BMP
	unsigned int file_size; // 4 byte : File size in bytes
	unsigned short reserved1; // 2 byte : unused(=0)
	unsigned short reserved2; // 2 byte : unused(=0)
	unsigned int data_offset; // 4 byte : File offset to Raster Data
};
struct BITMAPINFOHEADER
{
	unsigned int header_size; // 4 byte : size of struct BITMAPINFOHEADER
	int width; // 4 byte : Bitmap Width (pixel)
	int height; // 4 byte : Bitmap Height (pixel)
	unsigned short planes; // 2 byte : Number of Planes (=1)
	unsigned short bit_per_pixel; // 2 byte : Bit per pixel (1/4/8/16/24/32)
	unsigned int compression; // 4 byte : compression type, 0 :no compression
	unsigned int data_size; // 4 byte : (compressed) Size of Image
	int hresolution; // 4 byte : pixel/m
	int vresolution; // 4 byte : pixel/m
	unsigned int used_colors; // 4 byte : Number of actually used colors 
	unsigned int important_colors; // 4 byte : Number of important colors 0 = all
};

struct PALLETTE
{
	char blue;
	char green;
	char red;
	char reserved;
};


int main(int argc, char **argv)//argument형태로 입력을 (파일을)받아온다.
{
	struct BITMAPFILEHEADER *bitmap_file = (struct BITMAPFILEHEADER *)malloc(sizeof(struct BITMAPFILEHEADER));
	struct BITMAPINFOHEADER *bitmap_info = (struct BITMAPINFOHEADER *)malloc(sizeof(struct BITMAPINFOHEADER));

	FILE *fpread, *fpr;

	unsigned char *buffer;
	unsigned char *data_r;
	unsigned char *data_g;
	unsigned char *data_b;
	//unsigned int line_size;
	unsigned int k, x, y;
	unsigned int image_data_width;
	unsigned int image_channel_width;


	//if(argc !=2)//파일을 받아오지 않았다면 프로그램 종료
	//{
	//	return -1;
	//}

	if (!(fpread = fopen("D:\\VS-Project\\image\\Debug\\image1.bmp", "rb")))//읽어올 파일이 비었다면 
	{
		printf("File Open Error\n");//에러메시지 출력후 종료
		exit(1);
	}
	else
	{
		//printf("%s\nFile Open OK\n\n",fpread);
		fread(bitmap_file, sizeof(struct BITMAPFILEHEADER), 1, fpread);//head의 size만큼(structure size) fpread에서 읽어와서 head에 저장
		//fseek ();//fpread 의 파일포인터를 처음위치에서 head의 size만큼 떨어진곳에 배치한다. 
		fread(bitmap_info, sizeof(struct BITMAPINFOHEADER), 1, fpread);//info의 size만큼(structure size) fpread에서 읽어와서 info에 저장
		//fseek();//fpread 의 파일포인터를 처음위치에서 head size+info size만큼 떨어진곳에 배치한다.
		//praw=(unsigned char*) malloc();//동적배열 할당 (크기는 위 info구조체안의 data size변수의 값만큼)
		////data size의 의미가 그림정보의 크기이다. 
		//fread();//praw에 data size변수에 저장된 값 만큼(그림정보 전체크기만큼) fpread에서 읽어온다.
		//fclose();//fpread(원본파일)를 닫는다. 


		printf("\nFile Open OK\n\n");
	}


	image_data_width = WIDTHBYTES(bitmap_info->width * 24);  //char의 크기가 8bit이고, 한 픽셀당 총 3개의 채널(R,G,B)가 있으므로 곱하기 3을하여 8 * 3 = 24를곱해야함. 
	image_channel_width = WIDTHBYTES(bitmap_info->width * 8);  //1개의 channel 사용시

	buffer = (unsigned char *)malloc(sizeof(unsigned char)*image_data_width*bitmap_info->height);
	data_r = (unsigned char *)malloc(sizeof(unsigned char)*image_data_width*bitmap_info->height);
	data_g = (unsigned char *)malloc(sizeof(unsigned char)*image_data_width*bitmap_info->height);
	data_b = (unsigned char *)malloc(sizeof(unsigned char)*image_data_width*bitmap_info->height);

	memset(data_r, 0, sizeof(unsigned char)*image_data_width*bitmap_info->height);
	memset(data_g, 0, sizeof(unsigned char)*image_data_width*bitmap_info->height);
	memset(data_b, 0, sizeof(unsigned char)*image_data_width*bitmap_info->height);

	//line_size=(info.width*3);//한줄길이 바이트 단위로 설정(가로*3(bgr))
	//while()//한줄길이가 4의 배수가 될때까지
	//{
	//	line_size++;//++한다.
	//}

	printf("변경하고자하는 색상을 입력해 주세요: red:3, green:2, blue:1 gray:over 3\n");//변경할 색을 결정하게 한다.
	scanf("%d", &k);

	if (k>3 || k<1)
	{
		k = 1;
	}


	printf("Width : %d  Height : %d\n", image_data_width, bitmap_info->height);

	//fflush(stdin);
	//(image_data_width*bitmap_info->height)
	printf("\nBitmap Data read :  %d\n", fread(buffer, sizeof(unsigned char), image_data_width*bitmap_info->height, fpread));




	for (y = 0; y<bitmap_info->height; y++){//그림의 높이만큼 반복한다.
		for (x = 0; x<bitmap_info->width; x++)//그림의 너비만큼 반복한다.(한줄씩 진행)
		{
			data_b[y*image_data_width + (3 * x)] = buffer[y*image_data_width + (3 * x)];//b 	
			data_g[y*image_data_width + (3 * x) + 1] = buffer[y*image_data_width + (3 * x) + 1];//g
			data_r[y*image_data_width + (3 * x) + 2] = buffer[y*image_data_width + (3 * x) + 2];//r

			//bgr순서중 l번째 칸에 해당하는 색이 저장되어 있는 공간을 0으로 채운다.

		}
	}
	fclose(fpread);

	fpr = fopen("color.bmp", "wb");//color.bmp라는 파일을 쓰기용으로 연다.
	switch (k)
	{
	case 1:
		fwrite(bitmap_file, sizeof(struct BITMAPFILEHEADER), 1, fpr);
		fwrite(bitmap_info, sizeof(struct BITMAPINFOHEADER), 1, fpr);
		fwrite(data_b, 1, image_data_width*bitmap_info->height, fpr);
		fclose(fpr);
		break;
	case 2:
		fwrite(bitmap_file, sizeof(struct BITMAPFILEHEADER), 1, fpr);
		fwrite(bitmap_info, sizeof(struct BITMAPINFOHEADER), 1, fpr);
		fwrite(data_g, 1, image_data_width*bitmap_info->height, fpr);
		fclose(fpr);
		break;
	case 3:
		fwrite(bitmap_file, sizeof(struct BITMAPFILEHEADER), 1, fpr);
		fwrite(bitmap_info, sizeof(struct BITMAPINFOHEADER), 1, fpr);
		fwrite(data_r, 1, image_data_width*bitmap_info->height, fpr);
		fclose(fpr);
		break;
	}
	return 0;
}