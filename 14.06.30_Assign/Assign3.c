#include <stdio.h>
#include <stdlib.h>

#pragma pack(1)	// 구조체 내의 단위를 1바이트로 선언 이거 안하면 오류남
struct WAVHEAD //44bytes라고 쓰여있었으나 58bytes여서 배열로 14bytes를 추가 했습니다.
{
	char ChunkID[4]; // 고정값입니다. Ascii로 "RIFF" 가 들어갑니다.
	int ChunkSize; // 총 길이(s) + 샘플레이트 *2(스테레오일 때) + 36 혹은, 전체파일 크기 - 8
	char Format[8]; // 고정값입니다. "WAVEfmt "가 들어갑니다.
	int Subchunk1Size; // 고정값입니다. 16이 들어갑니다.(0x10)
	short AudioFormat; // 고정값입니다. 1이 들어갑니다.
	short NumChannels; // Mono일때는  1, Stereo일때는 2가 들어갑니다.
	int SampleRate; // 샘플레이트 값이 들어갑니다.
	int ByteRate; // 샘플레이트 * Numchannels
	short BlockAlign; // 비트레이트 / 8
	short BitsPerSample; // 비트레이트 값이 들어갑니다.
	char Subchunk2ID[4]; // 고정값입니다. "data"가 들어갑니다.
	int Subchunk2Size; // 길이(s) * 샘플레이트 * NumChannels
	char temp[14];
};


int main(int argc, char **argv){
	int i, length;			//변수와 데이터길이 저장
	char *pWav, *pRevers;	// 일반 wav파일과 역wav파일 포인터 선언
	FILE *pFileTemp1, *pFileTemp2;	//파일 포인터 2개 선언
	struct WAVHEAD waveH;	// 선언된 구조체를 waveH로 선언

	if (argc != 2 || (pFileTemp1 = fopen(argv[1], "rb")) == NULL){		// 불러올 파일이 없으면 종료
		exit(1);
	}
	else {
		fread(&waveH, sizeof(struct WAVHEAD), 1, pFileTemp1);	// 헤더의 크기인 58만큼 저장
		length = waveH.ChunkSize + 8 - 58;	//자주 쓰이는 전체길이 변수저장
		pWav = (char*)calloc(length, sizeof(char));	// 공간 확보함과 동시에 0으로 초기화
		pRevers = (char*)calloc(length, sizeof(char));	// 공간 확보함과 동시에 0으로 초기화
		fread(pWav, sizeof(char), length, pFileTemp1);	// 읽은내용을 pWav에 저장
		fclose(pFileTemp1);	//읽기 스트림 종료
	}
	for (i = 0; i<length;){
		*(pRevers + length - i - 2) = *(pWav + i);
		*(pRevers + length - i++ - 1) = *(pWav + i + 1); //2바이트 단위로 끈어서 역으로 저장
		i++; // 위에 ++이 1번있으므로 여기선 1번해줌
	}
	for (i = 0; i<length;){
		*(pWav + i++) = *(pWav + i) - *(pRevers + i);	//차이값을 기존 pWav에 저장
	}
	pFileTemp1 = fopen("Revers.wav", "wb");	//파일 생성
	pFileTemp2 = fopen("Differ.wav", "wb");	//파일 생성
	fwrite(&waveH, sizeof(struct WAVHEAD), 1, pFileTemp1);		//헤더 작성
	fwrite(pRevers, sizeof(char), length, pFileTemp1);			//역 wav 저장
	fwrite(&waveH, sizeof(struct WAVHEAD), 1, pFileTemp2);		//헤더작성
	fwrite(pWav, sizeof(char), length, pFileTemp2);			//차이 저장
	fclose(pFileTemp1);	//파일 종료
	fclose(pFileTemp2);	//파일 종료
	free(pWav);		//메모리할당 해제
	free(pRevers);	//메모리할당 해제
	return 0;
}