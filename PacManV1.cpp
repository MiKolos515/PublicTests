#include <iostream>
#include <Windows.h>
#include <cmath>
#include <conio.h>

using namespace std;

HANDLE hc = GetStdHandle(STD_OUTPUT_HANDLE);

int timer = 0;
int StartWinLose = 1;

bool W = false;
bool S = false;
bool A = false;
bool D = false;

int LevelNumber = 1;
int LevelHeight = 22;
int LevelWidth = 19;
int PelletTime = 400;

int PlayerPos[3] = {0,9,16};
int PlayerWantedMomentum;
int PlayerMomentum;

int GhostPowerTimer[4] = {0-PelletTime,0-PelletTime,0-PelletTime,0-PelletTime};

int GhostPos[4][3] = {
{0,9,8},
{0,8,10},
{0,9,10},
{0,10,10}
};
int GhostTarget[4][3];
int GhostMomentum[4];
int GhostMode[4];

int GameMode=1;

int Score = 0;

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

void SetMaze(){
    for(int i = 0; i < 22; i++){
        for(int j = 0; j < 19; j++){
            Level[i][j] = LevelTemplate[i][j];
        }
    }
}

bool CheckForWalls(int X,int Y, int Momentum, bool Ghost){
    if (abs(Momentum)==10){
        Y+=Momentum/10;
    } else {
        X+=Momentum;
    }
    if (Level[Y][(X+LevelWidth)%LevelWidth]==1){
        return false;
    }
    if (Level[Y][(X+LevelWidth)%LevelWidth]==2){
        return false;
    }
    if (!Ghost){
        if (Level[Y][(X+LevelWidth)%LevelWidth]==7){
            return false;
        }
    }
    return true;
}

void KeysCheck(){
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
    if(CheckForWalls(PlayerPos[1],PlayerPos[2],PlayerWantedMomentum,false)){
        PlayerMomentum=PlayerWantedMomentum;
    }

}

void AllGhostTurn(){
    for (int i=0;i<4;i++){
        if (GhostMode[i]<=1){
            GhostMomentum[i]=0-GhostMomentum[i];
        }
    }
}

void CollisionCheck(int Ghost){
    if (PlayerPos[1] == GhostPos[Ghost][1] && PlayerPos[2] == GhostPos[Ghost][2]){
        if ((timer - GhostPowerTimer[Ghost]) < PelletTime) {
            GhostPowerTimer[Ghost] = 0 - PelletTime;
            GhostMode[Ghost] = 3;
            Score += 520;
        } else if (GhostMode[Ghost]!=3){
            StartWinLose = 3;
        }
    }
}

void PlayerMove(){
    if(CheckForWalls(PlayerPos[1],PlayerPos[2],PlayerMomentum,false)){
        if (abs(PlayerMomentum)==10){
            PlayerPos[2]+=PlayerMomentum/10;
        } else {
            PlayerPos[1]+=PlayerMomentum;
        }
    }

    PlayerPos[1]=(PlayerPos[1]+LevelWidth)%LevelWidth;

    if (Level[PlayerPos[2]][PlayerPos[1]]==8){
        Level[PlayerPos[2]][PlayerPos[1]]=0;
        Score+=10;
    } else if (Level[PlayerPos[2]][PlayerPos[1]]==3){
        Level[PlayerPos[2]][PlayerPos[1]]=0;
        Score+=50;

        AllGhostTurn();

        for (int i=0;i<4;i++){
            if (GhostMode[i]!=3){
                GhostPowerTimer[i]=timer;
            }
        }
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

    if (Momentum!=-10&&CheckForWalls(X,Y,10,Mode==3||OnExit)){
        UpDistance=abs((Y+1)-TargetY)+abs(X-TargetX);
    }
    if (Momentum!=1&&CheckForWalls(X,Y,-1,Mode==3||OnExit)){
        LeftDistance=abs(Y-TargetY)+abs((X-1)-TargetX);
    }
    if (Momentum!=10&&CheckForWalls(X,Y,-10,Mode==3||OnExit)){
        DownDistance=abs((Y-1)-TargetY)+abs(X-TargetX);
    }
    if (Momentum!=-1&&CheckForWalls(X,Y,1,Mode==3||OnExit)){
        RightDistance=abs(Y-TargetY)+abs((X+1)-TargetX);
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

    GhostMomentum[Ghost]=GetGhostMomentum(X,Y,Momentum,TargetX,TargetY,Mode);

    if(GhostMode[Ghost]!=2||timer%40==0){
        if (abs(GhostMomentum[Ghost])==10){
            GhostPos[Ghost][2]+=GhostMomentum[Ghost]/10;
        } else {
            GhostPos[Ghost][1]+=GhostMomentum[Ghost];
        }
    }

    GhostPos[Ghost][1]=(GhostPos[Ghost][1]+LevelWidth)%LevelWidth;
    CollisionCheck(Ghost);

    if (GhostMode[Ghost]==3){
        if (GhostPos[Ghost][1]==9&&GhostPos[Ghost][2]==10){
            GhostMomentum[Ghost]=-10;
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
                GhostTarget[2][1] = PlayerPos[1] + (abs(PlayerMomentum)!=10 ? PlayerMomentum * 4 : 0);
                GhostTarget[2][2] = PlayerPos[2] + (abs(PlayerMomentum)==10 ? PlayerMomentum/10 * 4 : 0);
            } else if (Ghost==3){
                if (pow(PlayerPos[1] - GhostPos[3][1], 2) + pow(PlayerPos[2] - GhostPos[3][2], 2) < 64){
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
        GhostMove(GhostPos[i][1],GhostPos[i][2],GhostMomentum[i],GhostTarget[i][1],GhostTarget[i][2],GhostMode[i],i);

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

void PrintScore(){
    SetConsoleTextAttribute(hc, 0x0F);
    cout<<"     HIGH SCORE    \n";
    cout<<GameMode<<"     "<<Score<<"\n";
    cout<<"                   \n";
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
                Symbol("C",0x06);
            } else if (GhostPos[0][1]==j&&GhostPos[0][2]==i){
                if (GhostMode[0] == 2){
                    Symbol("A",0x09);
                } else if (GhostMode[0] == 3){
                    Symbol("~",0x0F);
                } else {
                    Symbol("A",0x04);
                }
            } else if (GhostPos[1][1]==j&&GhostPos[1][2]==i){
                if (GhostMode[1] == 2){
                    Symbol("A",0x09);
                } else if (GhostMode[1] == 3){
                    Symbol("~",0x0F);
                } else {
                    Symbol("A",0x03);
                }
            } else if (GhostPos[2][1]==j&&GhostPos[2][2]==i){
                if (GhostMode[2] == 2){
                    Symbol("A",0x09);
                } else if (GhostMode[2] == 3){
                    Symbol("~",0x0F);
                } else {
                    Symbol("A",0x0D);
                }
            } else if (GhostPos[3][1]==j&&GhostPos[3][2]==i){
                if (GhostMode[3] == 2){
                    Symbol("A",0x09);
                } else if (GhostMode[3] == 3){
                    Symbol("~",0x0F);
                } else {
                    Symbol("A",0x0C);
                }
            } else if (Level[i][j]==1){
                Symbol("#",0x01);
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

void PrintMaze(){
    PrintScore();
    PrintLevel();
}

int main()
{
SetMaze();
while (true) {
    timer++;
    KeysCheck();
    if ((timer % (2000 / LevelNumber)) < (1000 / LevelNumber))
        if (GameMode!=1){
            AllGhostTurn();
            GameMode = 1;
        }
    else
        if (GameMode!=0){
            AllGhostTurn();
            GameMode = 0;
        }
    if(timer%20==0){
        if (StartWinLose==0){
            PlayerMove();
            GhostControl();
        }
        SetConsoleCursorPosition(hc, {0, 0});
        PrintMaze();
    }
    if (StartWinLose==1&&timer>160){
        StartWinLose = 0;
    }
    Sleep(1);
}
return 0;
}
