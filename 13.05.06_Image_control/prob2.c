#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma pack(1)		//����ü�� ������ 1����Ʈ ������ ������ ����~ �̰� ���ϸ� ������
struct BITMAPFILEHEADER //14byte
{
	unsigned short identity; // 2 byte : ��BM�� is BMP
	unsigned int file_size; // 4 byte : File size in bytes
	unsigned short reserved1; // 2 byte : unused(=0)
	unsigned short reserved2; // 2 byte : unused(=0)
	unsigned int data_offset; // 4 byte : File offset to Raster Data
};
struct BITMAPINFOHEADER	//40byte
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

int main(int argc, char **argv)
{
	struct BITMAPFILEHEADER BitmapFH;	//fileheader ����
	struct BITMAPINFOHEADER BitmapIH;	//infoheadeer ����
	int input, i, j, k, pSize, nWidth;
	unsigned char *praw;	//bmp ���� ����� ������ ����
	FILE *fpread, *fpr;	//�б� ���� ���� ��Ʈ�� ����

	if (argc != 2 || !(fpread = fopen(argv[1], "rb")))//�о�� ������ ����� ���ڰ��� ���ٸ� 
	{
		printf("File Open Error\n");//�����޽��� ����� ����
		exit(1);
	}
	else	//���� �б� �κ�
	{
		fread(&BitmapFH, sizeof(struct BITMAPFILEHEADER), 1, fpread);//head�� size��ŭ(structure size) fpread���� �о�ͼ� head�� ����
		//fseek ();//fpread �� ���������͸� ó����ġ���� head�� size��ŭ ���������� ��ġ�Ѵ�. 
		fread(&BitmapIH, sizeof(struct BITMAPINFOHEADER), 1, fpread);//info�� size��ŭ(structure size) fpread���� �о�ͼ� info�� ����

		pSize = (4 - BitmapIH.width % 4) % 4;	//�е� ������ ���
		nWidth = 3 * BitmapIH.width + pSize; // ���� ���ϱ� 3���ؼ� ��������

		praw = (unsigned char*)malloc(3 * nWidth*BitmapIH.height);	//praw ���� ����
		fread(praw, sizeof(unsigned char), 3 * nWidth*BitmapIH.height, fpread);	//�׸� ���� �б�
		fclose(fpread);	//���� �б� ��Ʈ�� ����
	}
	puts("�����ϰ��� �ϴ� ������ �Է��� �ּ��� : red:3, green:2, blue:1, gray:default");	//���ù� ���
	scanf("%d", &input);	//���� �Է� ����

	for (k = 0; k<BitmapIH.height; k++){	//���� ����
		for (j = 0; j<BitmapIH.width; j++){	//���� ����
			if (input == 1) //���
			{
				*(praw + nWidth*k + 3 * j + 1) = 0;	//�׸�
				*(praw + nWidth*k + 3 * j + 2) = 2;	//����
			}
			else if (input == 2) { //�׸�
				*(praw + nWidth*k + 3 * j + 0) = 0;	//��� 
				*(praw + nWidth*k + 3 * j + 2) = 2;	//����
			}
			else if (input == 3) { //����
				*(praw + nWidth*k + 3 * j + 0) = 0;	//���
				*(praw + nWidth*k + 3 * j + 1) = 2;	//����
			}
			else {
				*(praw + nWidth*k + 3 * j) = 0.0126*(*(praw + nWidth*k + 3 * j)) + 0.7152*(*(praw + nWidth*k + 3 * j + 1)) + 0.0722*(*(praw + nWidth*k + 3 * j + 2));
				*(praw + nWidth*k + 3 * j + 1) = *(praw + nWidth*k + 3 * j);	//�׷��� �����
				*(praw + nWidth*k + 3 * j + 2) = *(praw + nWidth*k + 3 * j);
			}
		}
		for (i = 0; i<pSize; i++){	//�е� ��Ʈ ����
			*(praw + (nWidth*i + 3 * j) + i) = 0;
		}
	}
	fpr = fopen("color.bmp", "wb");	//���� ���� ��Ʈ�� ����
	fwrite(&BitmapFH, sizeof(struct BITMAPFILEHEADER), 1, fpr);//head�� size��ŭ(structure size) fpread���� �о�ͼ� head�� ����
	fwrite(&BitmapIH, sizeof(struct BITMAPINFOHEADER), 1, fpr);//info�� size��ŭ(structure size) fpread���� �о�ͼ� info�� ����
	fwrite(praw, sizeof(unsigned char), 3 * nWidth*BitmapIH.height, fpr);	//�׸� ����
	fclose(fpr);	//���� ����Ű�� ������ ����
	free(praw);	//praw �޸� ȯ��
	return 0;
}