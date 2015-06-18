#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma pack(1)		//구조체의 공간을 1바이트 단위로 나누기 위해~ 이거 안하면 오류남
struct BITMAPFILEHEADER //14byte
{
	unsigned short identity; // 2 byte : “BM” is BMP
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
	struct BITMAPFILEHEADER BitmapFH;	//fileheader 선언
	struct BITMAPINFOHEADER BitmapIH;	//infoheadeer 선언
	int input, i, j, k, pSize, nWidth;
	unsigned char *praw;	//bmp 내용 저장용 포인터 생성
	FILE *fpread, *fpr;	//읽기 쓰기 파일 스트림 생성

	if (argc != 2 || !(fpread = fopen(argv[1], "rb")))//읽어올 파일이 비었고 인자값이 없다면 
	{
		printf("File Open Error\n");//에러메시지 출력후 종료
		exit(1);
	}
	else	//파일 읽기 부분
	{
		fread(&BitmapFH, sizeof(struct BITMAPFILEHEADER), 1, fpread);//head의 size만큼(structure size) fpread에서 읽어와서 head에 저장
		//fseek ();//fpread 의 파일포인터를 처음위치에서 head의 size만큼 떨어진곳에 배치한다. 
		fread(&BitmapIH, sizeof(struct BITMAPINFOHEADER), 1, fpread);//info의 size만큼(structure size) fpread에서 읽어와서 info에 저장

		pSize = (4 - BitmapIH.width % 4) % 4;	//패딩 사이즈 계산
		nWidth = 3 * BitmapIH.width + pSize; // 가로 곱하기 3을해서 공간생성

		praw = (unsigned char*)malloc(3 * nWidth*BitmapIH.height);	//praw 공간 생성
		fread(praw, sizeof(unsigned char), 3 * nWidth*BitmapIH.height, fpread);	//그림 내용 읽기
		fclose(fpread);	//파일 읽기 스트림 종료
	}
	puts("변경하고자 하는 색상을 입력해 주세요 : red:3, green:2, blue:1, gray:default");	//선택문 출력
	scanf("%d", &input);	//숫자 입력 받음

	for (k = 0; k<BitmapIH.height; k++){	//높이 증가
		for (j = 0; j<BitmapIH.width; j++){	//가로 증가
			if (input == 1) //블루
			{
				*(praw + nWidth*k + 3 * j + 1) = 0;	//그린
				*(praw + nWidth*k + 3 * j + 2) = 2;	//레드
			}
			else if (input == 2) { //그린
				*(praw + nWidth*k + 3 * j + 0) = 0;	//블루 
				*(praw + nWidth*k + 3 * j + 2) = 2;	//레드
			}
			else if (input == 3) { //레드
				*(praw + nWidth*k + 3 * j + 0) = 0;	//블루
				*(praw + nWidth*k + 3 * j + 1) = 2;	//레드
			}
			else {
				*(praw + nWidth*k + 3 * j) = 0.0126*(*(praw + nWidth*k + 3 * j)) + 0.7152*(*(praw + nWidth*k + 3 * j + 1)) + 0.0722*(*(praw + nWidth*k + 3 * j + 2));
				*(praw + nWidth*k + 3 * j + 1) = *(praw + nWidth*k + 3 * j);	//그레이 계산함
				*(praw + nWidth*k + 3 * j + 2) = *(praw + nWidth*k + 3 * j);
			}
		}
		for (i = 0; i<pSize; i++){	//패딩 비트 세팅
			*(praw + (nWidth*i + 3 * j) + i) = 0;
		}
	}
	fpr = fopen("color.bmp", "wb");	//파일 쓰기 스트림 생성
	fwrite(&BitmapFH, sizeof(struct BITMAPFILEHEADER), 1, fpr);//head의 size만큼(structure size) fpread에서 읽어와서 head에 저장
	fwrite(&BitmapIH, sizeof(struct BITMAPINFOHEADER), 1, fpr);//info의 size만큼(structure size) fpread에서 읽어와서 info에 저장
	fwrite(praw, sizeof(unsigned char), 3 * nWidth*BitmapIH.height, fpr);	//그림 저장
	fclose(fpr);	//파일 가르키는 포인터 종료
	free(praw);	//praw 메모리 환원
	return 0;
}