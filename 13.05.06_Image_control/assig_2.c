#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//structure�� ���� 1����Ʈ�� �޾ƿ��ڴٰ� ����
#define WIDTHBYTES(bits) (((bits)+31)/32*4)



//structure ����
#pragma pack(1)
typedef struct BITMAPFILEHEADER
{
	unsigned short identity; // 2 byte : ��BM�� is BMP
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


int main(int argc, char **argv)//argument���·� �Է��� (������)�޾ƿ´�.
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


	//if(argc !=2)//������ �޾ƿ��� �ʾҴٸ� ���α׷� ����
	//{
	//	return -1;
	//}

	if (!(fpread = fopen("D:\\VS-Project\\image\\Debug\\image1.bmp", "rb")))//�о�� ������ ����ٸ� 
	{
		printf("File Open Error\n");//�����޽��� ����� ����
		exit(1);
	}
	else
	{
		//printf("%s\nFile Open OK\n\n",fpread);
		fread(bitmap_file, sizeof(struct BITMAPFILEHEADER), 1, fpread);//head�� size��ŭ(structure size) fpread���� �о�ͼ� head�� ����
		//fseek ();//fpread �� ���������͸� ó����ġ���� head�� size��ŭ ���������� ��ġ�Ѵ�. 
		fread(bitmap_info, sizeof(struct BITMAPINFOHEADER), 1, fpread);//info�� size��ŭ(structure size) fpread���� �о�ͼ� info�� ����
		//fseek();//fpread �� ���������͸� ó����ġ���� head size+info size��ŭ ���������� ��ġ�Ѵ�.
		//praw=(unsigned char*) malloc();//�����迭 �Ҵ� (ũ��� �� info����ü���� data size������ ����ŭ)
		////data size�� �ǹ̰� �׸������� ũ���̴�. 
		//fread();//praw�� data size������ ����� �� ��ŭ(�׸����� ��üũ�⸸ŭ) fpread���� �о�´�.
		//fclose();//fpread(��������)�� �ݴ´�. 


		printf("\nFile Open OK\n\n");
	}


	image_data_width = WIDTHBYTES(bitmap_info->width * 24);  //char�� ũ�Ⱑ 8bit�̰�, �� �ȼ��� �� 3���� ä��(R,G,B)�� �����Ƿ� ���ϱ� 3���Ͽ� 8 * 3 = 24�����ؾ���. 
	image_channel_width = WIDTHBYTES(bitmap_info->width * 8);  //1���� channel ����

	buffer = (unsigned char *)malloc(sizeof(unsigned char)*image_data_width*bitmap_info->height);
	data_r = (unsigned char *)malloc(sizeof(unsigned char)*image_data_width*bitmap_info->height);
	data_g = (unsigned char *)malloc(sizeof(unsigned char)*image_data_width*bitmap_info->height);
	data_b = (unsigned char *)malloc(sizeof(unsigned char)*image_data_width*bitmap_info->height);

	memset(data_r, 0, sizeof(unsigned char)*image_data_width*bitmap_info->height);
	memset(data_g, 0, sizeof(unsigned char)*image_data_width*bitmap_info->height);
	memset(data_b, 0, sizeof(unsigned char)*image_data_width*bitmap_info->height);

	//line_size=(info.width*3);//���ٱ��� ����Ʈ ������ ����(����*3(bgr))
	//while()//���ٱ��̰� 4�� ����� �ɶ�����
	//{
	//	line_size++;//++�Ѵ�.
	//}

	printf("�����ϰ����ϴ� ������ �Է��� �ּ���: red:3, green:2, blue:1 gray:over 3\n");//������ ���� �����ϰ� �Ѵ�.
	scanf("%d", &k);

	if (k>3 || k<1)
	{
		k = 1;
	}


	printf("Width : %d  Height : %d\n", image_data_width, bitmap_info->height);

	//fflush(stdin);
	//(image_data_width*bitmap_info->height)
	printf("\nBitmap Data read :  %d\n", fread(buffer, sizeof(unsigned char), image_data_width*bitmap_info->height, fpread));




	for (y = 0; y<bitmap_info->height; y++){//�׸��� ���̸�ŭ �ݺ��Ѵ�.
		for (x = 0; x<bitmap_info->width; x++)//�׸��� �ʺ�ŭ �ݺ��Ѵ�.(���پ� ����)
		{
			data_b[y*image_data_width + (3 * x)] = buffer[y*image_data_width + (3 * x)];//b 	
			data_g[y*image_data_width + (3 * x) + 1] = buffer[y*image_data_width + (3 * x) + 1];//g
			data_r[y*image_data_width + (3 * x) + 2] = buffer[y*image_data_width + (3 * x) + 2];//r

			//bgr������ l��° ĭ�� �ش��ϴ� ���� ����Ǿ� �ִ� ������ 0���� ä���.

		}
	}
	fclose(fpread);

	fpr = fopen("color.bmp", "wb");//color.bmp��� ������ ��������� ����.
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