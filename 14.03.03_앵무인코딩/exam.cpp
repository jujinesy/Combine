#include <iostream>
#include <fstream>
#include <vector>
#include <tchar.h>
#include <locale>
#include <string.h>

#include <conio.h> // getch()를 위해서

#define MAX 1000

using namespace std; 

void reverse(int start, int end, char* word, char* temp);

void main()
{
	//std::locale::global(std::locale("ko_KR.UTF-8"));
	//char* han[10];
	//han[0]="한글";
	//cout << *han << endl;
	//system("chcp 65001"); 
	//setlocale(LC_ALL, "korean"); 
	char word[MAX];
	char temp[MAX];
	char ch;
	//char reverse[MAX];
	ifstream file;
	file.open("input.txt");
	int start=0;
	int end = 0;


	//while (file.get(ch)){  // 파일로 부터 한 문자씩 읽기
	//	wcout << ch;    // 읽어온 한 문자 출력
	//}

	while(!file.eof()) 
	{
		file.getline(word, MAX);

		for(int i =0; i < MAX; i++){
			if(!word[i])
				break;
			wcout << word[i];
			if(word[i]== ' '){
				end = i;
				reverse(start, end, word, temp);
			}
			start =  end;
		}
		for(int i =0; i<end; i++){
			//cout<<temp[i];
		}
		cout<<endl;
		start = 0;
		end = 0;

	}



	file.close();
	system("pause");

}
void reverse(int start, int end, char* word, char* temp){
	int pWord = end;
	int pTemp = start;

	for(int i = end-1; i>=start; i--){
		pTemp++; 
		temp[pTemp] = word[i];
		//cout << word[i];
		//cout << temp[pTemp];;

	}
}
