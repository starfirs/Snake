#include<cstdio>
#include<iostream>
#include<algorithm>
#include<stack>
#include<vector>
#include<queue>
#include<cstring>
#include<cmath>
#include<set>
#include<conio.h>
#include<windows.h>
using namespace std;
const int N = 10;
void SetBWhite(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
}
void SetBRed(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_RED);
}
void SetBGreen(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}
void ReSet(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}
void gotoxy(int x, int y) {
    COORD pos = {x,y}; 
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleCursorPosition(hOut, pos); 
}
struct Mov{
	int x, y;
	Mov Make(char ch){
		x = 0, y = 0;
		if(ch == 'a' || ch == 'A'){
			x = 0, y = -1;
		} else if(ch == 'w' || ch == 'W'){
			x = -1, y = 0;
		} else if(ch == 's' || ch == 'S'){
			x = 1, y = 0;
		} else if(ch == 'd' || ch == 'D'){
			x = 0, y = 1;
		} else {
			x = 1, y = 0;
		}
	}
};
struct Pos{
	int x, y;
	Pos operator+(const Mov b)const{
		return (Pos){x + b.x, y + b.y}; 
	}
	
	bool operator==(const Pos b)const{
		if(x == b.x && y == b.y) return 1;
		return 0;
	}
	Pos Make(int x, int y){
		x = x, y = y;
	}
	void change(string t){
		gotoxy(2 * y - 2, x - 1);
		cout << t;
	}
	void MakeAva(){
		if(x == 0) x = N;
		if(y == 0) y = N;
		if(x == N + 1) x = 1;
		if(y == N + 1) y = 1;
	}
};
class Mob{
public:
	Pos mob;
	void NewMob(){
		mob = (Pos){rand() % N + 1, rand() % N + 1};
	}
	Mob(){
		NewMob();
	}
};

class Snake{
public:
	deque<Pos> q;
	int add;
	int AvaPos[N + 5][N + 5];
	bool CheckAva(Pos cnt){
		if(AvaPos[cnt.x][cnt.y] == 1) return false;
		return true; 
	} 
	void Ava(Pos cnt, int val){
		AvaPos[cnt.x][cnt.y] = val;
	}
	Snake(){
		add = 0;
		while(!q.empty()) q.pop_front();
		memset(AvaPos, 0, sizeof AvaPos);
		Pos st = (Pos){rand() % N + 1, rand() % N + 1}; 
		q.push_back(st);
		Ava(st, 1);
	}
	bool Moving(Mov cnt){
		Pos NewHead = q.front() + cnt;
		NewHead.MakeAva();
		if(!add){
			Ava(q.back(), 0);
			SetBGreen();
			q.back().change("  ");
			ReSet();
			q.pop_back();
		} else add--; 
		if(!CheckAva(NewHead)) return false;
		q.push_front(NewHead);
		Ava(NewHead, 1);
		SetBWhite();
		NewHead.change("  ");
		ReSet();
		return true;
	}
};
Snake snake;
Mob mob;
char LastIn;
void print(){
	system("cls");
	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= N; j++){
			if(snake.CheckAva((Pos){i, j}) == 0){
				SetBWhite();
				cout << "  ";
			} else if(mob.mob == (Pos){i, j}){
				SetBRed();
				cout << "  ";
			} else {
				SetBGreen();
				cout << "  ";
			}
			ReSet();
		}
		cout << endl; 
	}
}
bool turn(){
	if(kbhit()) LastIn = getch();//�û�����
	Mov ord;
	ord.Make(LastIn);//�ƶ�����
	
	if(snake.Moving(ord) == 0){
		return 0;
	} 
	if(snake.CheckAva(mob.mob) == 0){//���Ѿ�ռ�� 
		snake.add++;
		while(snake.CheckAva(mob.mob) == 0)
			mob.NewMob();
		SetBRed();
		mob.mob.change("  ");
		ReSet();
	}
	return 1;
} 

void noedit(){
	HANDLE hStdin=GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin,&mode);
	mode&=~ENABLE_QUICK_EDIT_MODE;
	mode&=~ENABLE_INSERT_MODE;
	mode&=~ENABLE_MOUSE_INPUT;
	SetConsoleMode(hStdin,mode);
}
void HideCursor(){
	CONSOLE_CURSOR_INFO cur={1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cur);
}
void SetBig(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (NULL != hConsole)
    {
        COORD cSize = {40 * 2, 40};
        if (SetConsoleScreenBufferSize(hConsole, cSize))
        {
            SMALL_RECT rSize = { 0, 0, cSize.X - 1, cSize.Y - 1 };
            SetConsoleWindowInfo(hConsole, TRUE, &rSize);
        }
    }
}
	
int main(){
	noedit();
	HideCursor();
	LastIn = 'a';
	print();
	ReSet();
	while(1){
		if(turn() == 0){
			system("cls");
			cout << "score:" << snake.q.size() << "\n";
			cout << "游戏结束 下一局输入任意字符串\n";
			string s;
			cin >> s;
			main();
			return 0;	
		}
		Sleep(200);
	}
    return 0;
}

