#include <iostream>
#include <Windows.h>
#include <cmath>
#include <conio.h>

using namespace std;

HANDLE hc = GetStdHandle(STD_OUTPUT_HANDLE);

string answer;
int timer = 0;
int StartWinLose = 1;
bool SuperTextures = false;

bool ESC = false;
bool W = false;
bool S = false;
bool A = false;
bool D = false;

int LevelNumber = 1;
int LevelHeight = 22;
int LevelWidth = 19;
int PelletTime = 400;

int FruitTimer;
int LastFruit;

int PlayerPos[3] = {0,9,16};
int PlayerWantedMomentum = 0;

int GhostPowerTimer[4];

int GhostPos[4][3];
int GhostPosTemplate[4][3] = {
{0,9,8},
{0,8,10},
{0,9,10},
{0,10,10}
};

int GhostTarget[4][3];
int GhostMode[4];

int GameMode=1;

int DotCount;
int Score = 0;
int Lives = 2;

int Level[22][19];
int LevelTemplate[22][19] = {
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,1},
{1,8,1,1,8,1,1,1,8,1,8,1,1,1,8,1,1,8,1},
{1,3,1,1,8,1,1,1,8,1,8,1,1,1,8,1,1,3,1},
{1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1},
{1,8,1,1,8,1,8,1,1,1,1,1,8,1,8,1,1,8,1},
{1,8,8,8,8,1,8,8,8,1,8,8,8,1,8,8,8,8,1},
{1,1,1,1,8,1,1,1,0,1,0,1,1,1,8,1,1,1,1},
{0,0,0,1,8,1,0,0,0,0,0,0,0,1,8,1,0,0,0},
{1,1,1,1,8,1,0,1,1,7,1,1,0,1,8,1,1,1,1},
{0,0,0,0,8,0,0,1,2,0,2,1,0,0,8,0,0,0,0},
{1,1,1,1,8,1,0,1,1,1,1,1,0,1,8,1,1,1,1},
{0,0,0,1,8,1,0,0,0,0,0,0,0,1,8,1,0,0,0},
{1,1,1,1,8,1,0,1,1,1,1,1,0,1,8,1,1,1,1},
{1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,1},
{1,8,1,1,8,1,1,1,8,1,8,1,1,1,8,1,1,8,1},
{1,3,8,1,8,8,8,8,8,0,8,8,8,8,8,1,8,3,1},
{1,1,8,1,8,1,8,1,1,1,1,1,8,1,8,1,8,1,1},
{1,8,8,8,8,1,8,8,8,1,8,8,8,1,8,8,8,8,1},
{1,8,1,1,1,1,1,1,8,1,8,1,1,1,1,1,1,8,1},
{1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

const char* art =
        " .----.  .--.   .---.   \n"
        " | {}  }/ {} \\ /  ___}  \n"
        " | .--'/  /\\  \\\\     }  \n"
        " `-'   `-'  `-' `---'   \n"
        ".-.   .-.  .--.  .-. .-.\n"
        "|  `.'  | / {} \\ |  `| |\n"
        "| |\\ /| |/  /\\  \\| |\\  |\n"
        "`-' ` `-'`-'  `-'`-' `-'\n";


void SetMaze(){
    FruitTimer = 0;
    LastFruit = 0;
    for(int i = 0; i < 22; i++){
        for(int j = 0; j < 19; j++){
            Level[i][j] = LevelTemplate[i][j];
        }
    }
}

void SetGhosts(){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 3; j++){
            GhostPos[i][j] = GhostPosTemplate[i][j];
        }
        GhostMode[i]=GameMode;
        GhostPowerTimer[i]=0-PelletTime;
    }
}

bool CheckForWalls(int X,int Y, int Momentum, int Type){
    if (abs(Momentum)==10){
        Y+=Momentum/10;
    } else {
        X+=Momentum;
    }

    if ((0 > X || X >= LevelWidth ||
         0 > Y || Y >= LevelHeight) && Type == 0)
        return true;
    {

    }
    if (Level[Y][(X+LevelWidth)%LevelWidth]==1){
        return false;
    }
    if (Type == 2){
        if (Level[Y][(X+LevelWidth)%LevelWidth]==2){
            return false;
        }
    }
    if (Type == 0||Type == 1){
        if (Level[Y][(X+LevelWidth)%LevelWidth]==7){
            return false;
        }
    }
    return true;
}

int SuperWallPrint(int X,int Y){

    int TileSum = 0;

    if (!CheckForWalls(X,Y,10,0)){TileSum+=1;}//^
    if (!CheckForWalls(X,Y,1,0)){TileSum+=2;}//<
    if (!CheckForWalls(X,Y,-10,0)){TileSum+=4;}//V
    if (!CheckForWalls(X,Y,-1,0)){TileSum+=8;}//>

    switch(TileSum){
        case 0: return 462; //┼
        case 1:
        case 4:
        case 5: return 442; //─
        case 2:
        case 8:
        case 10: return 461; //│
        case 3: return 457; //┌
        case 9: return 443; //┐
        case 6: return 456; //└
        case 12: return 444; //┘
        case 11: return 459; //┬
        case 7: return 460; //├
        case 13: return 441; //┤
        case 14: return 458; //┴


        default: return 219; //█
    }
}

int SuperPacManPrint(int X,int Y, int Momentum){
    if ((PlayerWantedMomentum!=Momentum&&CheckForWalls(X,Y,PlayerWantedMomentum,1))||!CheckForWalls(X,Y,Momentum,1)||StartWinLose!=0){
        return 335; //O
    }
    switch(Momentum){
        case 10: return 350; //^
        case 1: return 316; //<
        case -1: return 318; //>
        case -10: return 374; //v

        default: return 335; //O
    }
}

int SuperGhostPrint(int Momentum){
    switch(Momentum){
        case 10: return 333; //M
        case 1: return 325; //E
        case -1: return 307; //3
        case -10: return 343; //W

        default: return 321; //A
    }
}

void KeysCheck(){
    ESC = GetKeyState(VK_ESCAPE) & 0x8000;
    W = GetKeyState('W') & 0x8000;
    S = GetKeyState('S') & 0x8000;
    A = GetKeyState('A') & 0x8000;
    D = GetKeyState('D') & 0x8000;
    if(W){
        PlayerWantedMomentum=-10;
    }else if(S){
        PlayerWantedMomentum=10;
    }else if(A){
        PlayerWantedMomentum=-1;
    }else if(D){
        PlayerWantedMomentum=1;
    }
    if(CheckForWalls(PlayerPos[1],PlayerPos[2],PlayerWantedMomentum,1)){
        PlayerPos[0]=PlayerWantedMomentum;
    }

}

void DotsCheck(){
    int Dots = 0;
    for(int i = 0; i < 22; i++){
        for(int j = 0; j < 19; j++){
            if (Level[i][j]==3||Level[i][j]==8){
                Dots++;
            }
        }
    }
    DotCount = Dots;
    if (Dots==0){StartWinLose=2;}
}

void FruitControl(){
    if(FruitTimer>0)
        FruitTimer--;
    if(LastFruit==0&&DotCount<=111){
        LastFruit=1;
        FruitTimer=35;
    } else if(LastFruit==1&&DotCount<=45){
        LastFruit=2;
        FruitTimer=35;
    }
}

void AllGhostTurn(){
    for (int i=0;i<4;i++){
        if (!(GhostPos[i][2]>=8&&GhostPos[i][2]<=10&&GhostPos[i][1]==9)&&GhostMode[i]>1){
            GhostPos[i][0]=0-GhostPos[i][0];
        }
    }
}

void CollisionCheck(int Ghost){
    if (PlayerPos[1] == GhostPos[Ghost][1] && PlayerPos[2] == GhostPos[Ghost][2]){
        if ((timer - GhostPowerTimer[Ghost]) <= PelletTime) {
            GhostPowerTimer[Ghost] = 0 - PelletTime;
            GhostMode[Ghost] = 3;
            Score += 520;
        } else if (GhostMode[Ghost]!=3){
            StartWinLose=3;
        }
    }
}

void PlayerMove(){
    if(CheckForWalls(PlayerPos[1],PlayerPos[2],PlayerPos[0],1)){
        if (abs(PlayerPos[0])==10){
            PlayerPos[2]+=PlayerPos[0]/10;
        } else {
            PlayerPos[1]+=PlayerPos[0];
        }
    }

    PlayerPos[1]=(PlayerPos[1]+LevelWidth)%LevelWidth;

    if (Level[PlayerPos[2]][PlayerPos[1]]==8){
        Level[PlayerPos[2]][PlayerPos[1]]=0;
        Score+=10;
        DotsCheck();
    } else if (Level[PlayerPos[2]][PlayerPos[1]]==3){
        Level[PlayerPos[2]][PlayerPos[1]]=0;
        Score+=50;
        DotsCheck();

        AllGhostTurn();

        for (int i=0;i<4;i++){
            if (GhostMode[i]!=3){
                GhostPowerTimer[i]=timer;
            }
        }
    }
    if (PlayerPos[1]==9&&PlayerPos[2]==12&&FruitTimer>0){
        FruitTimer=0;
        Score+=500;
    }

    for (int i=0;i<4;i++){
        CollisionCheck(i);
    }
}

int GetGhostMomentum(int X,int Y,int Momentum,int TargetX,int TargetY,int Mode){

    bool OnExit = X==9&&Y==10;

    int UpDistance = 9999;
    int LeftDistance = 9999;
    int DownDistance = 9999;
    int RightDistance = 9999;

    if (Momentum != -10 && CheckForWalls(X, Y, 10, (Mode == 3 || OnExit) ? 2 : 1)) {
        UpDistance = ((Y+1)-TargetY)*((Y+1)-TargetY) + (X-TargetX)*(X-TargetX);
    }
    if (Momentum != 1 && CheckForWalls(X, Y, -1, (Mode == 3 || OnExit) ? 2 : 1)) {
        LeftDistance = (Y-TargetY)*(Y-TargetY) + ((X-1)-TargetX)*((X-1)-TargetX);
    }
    if (Momentum != 10 && CheckForWalls(X, Y, -10, (Mode == 3 || OnExit) ? 2 : 1)) {
        DownDistance = ((Y-1)-TargetY)*((Y-1)-TargetY) + (X-TargetX)*(X-TargetX);
    }
    if (Momentum != -1 && CheckForWalls(X, Y, 1, (Mode == 3 || OnExit) ? 2 : 1)) {
        RightDistance = (Y-TargetY)*(Y-TargetY) + ((X+1)-TargetX)*((X+1)-TargetX);
    }

    if (Mode==2){
        int Moves[4];
        int PossibleMoves= 0;

        if (UpDistance != 9999)    Moves[PossibleMoves++] = 10;
        if (LeftDistance != 9999)  Moves[PossibleMoves++] = -1;
        if (DownDistance != 9999)  Moves[PossibleMoves++] = -10;
        if (RightDistance != 9999) Moves[PossibleMoves++] = 1;

        if (PossibleMoves == 0){
            return Momentum;
        }
        return Moves[rand() % PossibleMoves];
    } else {
        int Best = UpDistance;
        int FinalMomentum = 10;

        if (LeftDistance < Best){
            Best = LeftDistance;
            FinalMomentum = -1;
        }
        if (DownDistance < Best){
            Best = DownDistance;
            FinalMomentum = -10;
        }
        if (RightDistance < Best){
            Best = RightDistance;
            FinalMomentum = 1;
        }
        return FinalMomentum;
    }
}

void GhostMove(int X,int Y,int Momentum,int TargetX,int TargetY,int Mode,int Ghost){

    if(GhostMode[Ghost]!=2||timer%40==0){
    GhostPos[Ghost][0]=GetGhostMomentum(X,Y,Momentum,TargetX,TargetY,Mode);

        if (abs(GhostPos[Ghost][0])==10){
            GhostPos[Ghost][2]+=GhostPos[Ghost][0]/10;
        } else {
            GhostPos[Ghost][1]+=GhostPos[Ghost][0];
        }
    }

    GhostPos[Ghost][1]=(GhostPos[Ghost][1]+LevelWidth)%LevelWidth;
    CollisionCheck(Ghost);

    if (GhostMode[Ghost]==3){
        if (GhostPos[Ghost][1]==9&&GhostPos[Ghost][2]==10){
            GhostPos[Ghost][0]=-10;
            GhostMode[Ghost]=GameMode;
        }
    }
}

void GhostAi(int Ghost){
    if (GhostMode[Ghost]<=2){
        if (GameMode==1){
            if (Ghost==0){
                GhostTarget[0][1] = PlayerPos[1];
                GhostTarget[0][2] = PlayerPos[2];
            } else if (Ghost==1){
                GhostTarget[1][1] = 2 * PlayerPos[1] - GhostPos[0][1];
                GhostTarget[1][2] = 2 * PlayerPos[2] - GhostPos[0][2];
            } else if (Ghost==2){
                GhostTarget[2][1] = PlayerPos[1] + (abs(PlayerPos[0])!=10 ? PlayerPos[0] * 2 : 0);
                GhostTarget[2][2] = PlayerPos[2] + (abs(PlayerPos[0])==10 ? PlayerPos[0]/10 * 2 : 0);
            } else if (Ghost==3){
                if (pow(PlayerPos[1] - GhostPos[3][1], 2) + pow(PlayerPos[2] - GhostPos[3][2], 2) < 6){
                    GhostTarget[3][1] = 0;
                    GhostTarget[3][2] = 21;
                } else {
                    GhostTarget[3][1] = PlayerPos[1];
                    GhostTarget[3][2] = PlayerPos[2];
                }
            }
        } else if (GameMode==0){
            if (Ghost==0){
                GhostTarget[0][1]= 17;
                GhostTarget[0][2]= -2;
            } else if (Ghost==1){
                GhostTarget[1][1] = 17;
                GhostTarget[1][2] = 23;
            } else if (Ghost==2){
                GhostTarget[2][1] = 1;
                GhostTarget[2][2] = -2;
            } else if (Ghost==3){
                GhostTarget[3][1] = 1;
                GhostTarget[3][2] = 23;
            }
        }
    } else if (GhostMode[Ghost]==3){
        GhostTarget[Ghost][1]=9;
        GhostTarget[Ghost][2]=9;
    }
}

void GhostControl(){
    for (int i=0;i<4;i++){

        GhostAi(i);
        GhostMove(GhostPos[i][1],GhostPos[i][2],GhostPos[i][0],GhostTarget[i][1],GhostTarget[i][2],GhostMode[i],i);

        if ((timer-GhostPowerTimer[i])<PelletTime){
            GhostMode[i] = 2;
        } else if (GhostMode[i] == 2){
            GhostMode[i] = GameMode;
        }
    }
}

void Symbol(string symbol, int color){
    SetConsoleTextAttribute(hc, color);
    cout<<symbol;
}

void SuperSymbol(int symbol, int color){
    SetConsoleTextAttribute(hc, color);
    cout<<char(symbol);
}

void Fruit(int FruitNumber){
    switch (FruitNumber){
        case 1: Symbol("%",0x0C); break;
        case 2: Symbol("V",0x04); break;
        case 3:
        case 4: Symbol("Q",0x06); break;
        case 5:
        case 6: Symbol("@",0x0C); break;
        case 7:
        case 8: Symbol("6",0x02); break;
        case 9:
        case 10: Symbol("Y",0x0B); break;
        case 11:
        case 12: Symbol("^",0x0E); break;
        default: Symbol("+",0x08); break;
    }
}

void PrintScore(){
    SetConsoleTextAttribute(hc, 0x0F);
    cout<<"     HIGH SCORE    "<<endl;
    cout<<DotCount<<"     "<<Score<<endl;
    cout<<"                   "<<endl;
}

void PrintGhost(int Ghost){
    int GhostColor;
    switch(Ghost){
        case 0: GhostColor=0x04; break;
        case 1: GhostColor=0x03; break;
        case 2: GhostColor=0x0D; break;
        case 3: GhostColor=0x0C; break;
    }



    if (GhostMode[Ghost] == 2){
        if (timer - GhostPowerTimer[Ghost] > 2*PelletTime/3 && timer % 40 > 10){
            if (SuperTextures){
                SuperSymbol(SuperGhostPrint(GhostPos[Ghost][0]),0x0F);
            } else {
                Symbol("M",0x0F);
            }
        } else {
            if (SuperTextures){
                SuperSymbol(SuperGhostPrint(GhostPos[Ghost][0]),0x09);
            } else {
                Symbol("M",0x09);
            }
        }
    } else if (GhostMode[Ghost] == 3){
        Symbol("~",0x0F);
    } else {
        if (SuperTextures){
            SuperSymbol(SuperGhostPrint(GhostPos[Ghost][0]),GhostColor);
        } else {
            Symbol("M",GhostColor);
        }
    }
}

void PrintLevel(){
    for (int i=0;i<LevelHeight;i++){
        for (int j=0;j<LevelWidth;j++){
            if (StartWinLose==1&&i==12&&j==7){
                Symbol("READY",0x0F);
                j+=4;
            } else if (StartWinLose==3&&i==12&&j==7){
                Symbol("LOSE!",0x0F);
                j+=4;
            } else if (StartWinLose==2&&i==12&&j==8){
                Symbol("WIN",0x0F);
                j+=2;
            } else if (PlayerPos[1]==j&&PlayerPos[2]==i){
                if (SuperTextures){
                SuperSymbol(SuperPacManPrint(PlayerPos[1],PlayerPos[2],PlayerPos[0]),0x06);
                } else {
                Symbol("C",0x06);
                }
            } else if (GhostPos[0][1]==j&&GhostPos[0][2]==i){
                PrintGhost(0);
            } else if (GhostPos[1][1]==j&&GhostPos[1][2]==i){
                PrintGhost(1);
            } else if (GhostPos[2][1]==j&&GhostPos[2][2]==i){
                PrintGhost(2);
            } else if (GhostPos[3][1]==j&&GhostPos[3][2]==i){
                PrintGhost(3);
            } else if (i==12&&j==9&&FruitTimer>0){
                Fruit(LevelNumber);
            } else if (Level[i][j]==1){
                if (SuperTextures){
                SuperSymbol(SuperWallPrint(j,i),0x01);
                } else {
                Symbol("#",0x01);
                }
            } else if (Level[i][j]==8){
                Symbol(".",0x0E);
            } else if (Level[i][j]==3){
                Symbol("o",0x0E);
            } else if (Level[i][j]==7){
                Symbol("-",0x05);
            } else {
                Symbol(" ",0x00);
            }
        }
        cout<<endl;
    }
}

void PrintExtra(){
    Symbol(" ",0x00);
    for (int i=0;i<Lives;i++){
        Symbol("C",0x06);
    }
    for (int i=0;LevelNumber>6?i<(16-Lives-6):i<(16-Lives-LevelNumber);i++){
        Symbol(" ",0x00);
    }
    for (int i=0;LevelNumber>6?i<6:i<LevelNumber;i++){
        Fruit(LevelNumber-i);
    }
        Symbol("  ",0x00);
}

void PrintMaze(){
    PrintScore();
    PrintLevel();
    PrintExtra();
}

int main()
{
    SetConsoleTextAttribute(hc, 0x60);
    cout<<art<<endl;
    Symbol("Dou you wish for \ndetailed textures?\n",0x0F);
    cin>>answer;

    if (answer=="1"||answer=="yes"||answer=="Yes"||answer=="Y"||answer=="true"||answer=="TRUE") {
        SuperTextures = true;
    }

    system("cls");

    SetMaze();
    SetGhosts();
    DotsCheck();

    while (!ESC) {
        KeysCheck();
        if(timer%20==0){
            if (GameMode!=((timer % 2000) > (2000 >> LevelNumber))){
                AllGhostTurn();
                GameMode = ((timer % 2000) > (2000 >> LevelNumber));
            }

            if (StartWinLose==0){
                PlayerMove();
            }
            if (StartWinLose==0){
                GhostControl();
            }
            FruitControl();

            SetConsoleCursorPosition(hc, {0, 0});
            PrintMaze();
        }

        if (StartWinLose==1&&timer>160){
            StartWinLose = 0;
        }

        timer++;

        if (StartWinLose==2||StartWinLose==3){
            Sleep(1500);
            SetGhosts();

            PlayerWantedMomentum=0;
            PlayerPos[0]=0;
            PlayerPos[1]=9;
            PlayerPos[2]=16;

            timer=0;
            Lives-=1;

            system("cls");

            if (StartWinLose==3){
                if (Lives<0){
                    Score=0;
                    Lives=2;
                    LevelNumber=1;
                    SetMaze();
                }
            } else {
                LevelNumber++;
                SetMaze();
            }

            StartWinLose = 1;
        }


        Sleep(1);
    }
    return 0;
}
