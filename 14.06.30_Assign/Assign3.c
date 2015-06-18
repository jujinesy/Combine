#include <stdio.h>
#include <stdlib.h>

#pragma pack(1)	// ����ü ���� ������ 1����Ʈ�� ���� �̰� ���ϸ� ������
struct WAVHEAD //44bytes��� �����־����� 58bytes���� �迭�� 14bytes�� �߰� �߽��ϴ�.
{
	char ChunkID[4]; // �������Դϴ�. Ascii�� "RIFF" �� ���ϴ�.
	int ChunkSize; // �� ����(s) + ���÷���Ʈ *2(���׷����� ��) + 36 Ȥ��, ��ü���� ũ�� - 8
	char Format[8]; // �������Դϴ�. "WAVEfmt "�� ���ϴ�.
	int Subchunk1Size; // �������Դϴ�. 16�� ���ϴ�.(0x10)
	short AudioFormat; // �������Դϴ�. 1�� ���ϴ�.
	short NumChannels; // Mono�϶���  1, Stereo�϶��� 2�� ���ϴ�.
	int SampleRate; // ���÷���Ʈ ���� ���ϴ�.
	int ByteRate; // ���÷���Ʈ * Numchannels
	short BlockAlign; // ��Ʈ����Ʈ / 8
	short BitsPerSample; // ��Ʈ����Ʈ ���� ���ϴ�.
	char Subchunk2ID[4]; // �������Դϴ�. "data"�� ���ϴ�.
	int Subchunk2Size; // ����(s) * ���÷���Ʈ * NumChannels
	char temp[14];
};


int main(int argc, char **argv){
	int i, length;			//������ �����ͱ��� ����
	char *pWav, *pRevers;	// �Ϲ� wav���ϰ� ��wav���� ������ ����
	FILE *pFileTemp1, *pFileTemp2;	//���� ������ 2�� ����
	struct WAVHEAD waveH;	// ����� ����ü�� waveH�� ����

	if (argc != 2 || (pFileTemp1 = fopen(argv[1], "rb")) == NULL){		// �ҷ��� ������ ������ ����
		exit(1);
	}
	else {
		fread(&waveH, sizeof(struct WAVHEAD), 1, pFileTemp1);	// ����� ũ���� 58��ŭ ����
		length = waveH.ChunkSize + 8 - 58;	//���� ���̴� ��ü���� ��������
		pWav = (char*)calloc(length, sizeof(char));	// ���� Ȯ���԰� ���ÿ� 0���� �ʱ�ȭ
		pRevers = (char*)calloc(length, sizeof(char));	// ���� Ȯ���԰� ���ÿ� 0���� �ʱ�ȭ
		fread(pWav, sizeof(char), length, pFileTemp1);	// ���������� pWav�� ����
		fclose(pFileTemp1);	//�б� ��Ʈ�� ����
	}
	for (i = 0; i<length;){
		*(pRevers + length - i - 2) = *(pWav + i);
		*(pRevers + length - i++ - 1) = *(pWav + i + 1); //2����Ʈ ������ ��� ������ ����
		i++; // ���� ++�� 1�������Ƿ� ���⼱ 1������
	}
	for (i = 0; i<length;){
		*(pWav + i++) = *(pWav + i) - *(pRevers + i);	//���̰��� ���� pWav�� ����
	}
	pFileTemp1 = fopen("Revers.wav", "wb");	//���� ����
	pFileTemp2 = fopen("Differ.wav", "wb");	//���� ����
	fwrite(&waveH, sizeof(struct WAVHEAD), 1, pFileTemp1);		//��� �ۼ�
	fwrite(pRevers, sizeof(char), length, pFileTemp1);			//�� wav ����
	fwrite(&waveH, sizeof(struct WAVHEAD), 1, pFileTemp2);		//����ۼ�
	fwrite(pWav, sizeof(char), length, pFileTemp2);			//���� ����
	fclose(pFileTemp1);	//���� ����
	fclose(pFileTemp2);	//���� ����
	free(pWav);		//�޸��Ҵ� ����
	free(pRevers);	//�޸��Ҵ� ����
	return 0;
}