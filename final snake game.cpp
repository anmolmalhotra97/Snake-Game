
#include<iostream>
#include<stdio.h>
#include<cmath>
#include<stdlib.h>
#include<time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/select.h>
//#include<conio.h>
#include<math.h>
using namespace std;
#define M 25
#define N 50
int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
static int life=3;
static int counterf =0;
static int foodIndex;
static int counterp =0;
static int poisonIndex=0;
static int dist=0;
static int moves=0;
static int score=0;
void delay(int ms)
{
	clock_t stime=clock();
	while(clock()<stime+ms)
	{if(kbhit())
	break;
	}

}
struct snake
{
    int x,y;
    char value;
    struct snake *next;
};
struct food
{

    int x;
    int y;
    char val;
};
struct poison
{

    int x;
    int y;
    char val;
};

int checkPosOfFood(struct food f[],struct snake *head)
{
    for(int i=0;i<4;i++)
    {
    if((f[i].x==head->x) && (f[i].y==head->y))
        {
            foodIndex=i;
            return 1;
        }
    }
    return 0;
}
int checkFoodOverSnake(struct snake *head,int x,int y)
{
    int flag=0;
    struct snake *ptr=head;
    while(ptr!=NULL)
    {
        if((ptr->x == x) && (ptr->y==y))
        {
            flag=1;
            return 1;
        }
        ptr=ptr->next;
    }
    if(flag==0)
        return 0;
}
int checkFoodOverFood(struct food f[],int x, int y)
{
    for(int i=0;i<4;i++)
    {
        if(f[i].x==x && f[i].y==y)
            return 1;
    }
    return 0;
}

void genfood(struct food f[],struct snake *head)
{
    int z,x,y,t;
    for(int i=0;i<4;i++)
{
    if(counterf==0)
    {
        do
        {
            x=(rand()%M);
            y=(rand()%N);
            z=checkFoodOverSnake(head,x,y);
        }while(z!=0);
            f[0].x=x;
            f[0].y=y;
            f[0].val='F';
            counterf++;
        }
    else
        {
        do
        {
            x=(rand()%M);
            y=(rand()%N);
            z=checkFoodOverSnake(head,x,y);
            t=checkFoodOverFood(f,x,y);
        }while(z!=0 && t!=0);
            f[i].x=x;
            f[i].y=y;
            f[i].val='F';
        }
    }
}
int checkPoisonOverSnake(struct snake *head,int x,int y)
{
   int flag=0;
    struct snake *ptr=head;
    while(ptr!=NULL)
    {
        if((ptr->x == x) && (ptr->y==y))
        {
            flag=1;
            return 1;
        }
        ptr=ptr->next;
    }
    if(flag==0)
        return 0;
}
int checkPoisonOverPoison(struct poison p[],int x,int y)
{
    for(int i=0;i<4;i++)
    {
        if(p[i].x==x && p[i].y==y)
            return 1;
    }
    return 0;
}
int checkPoisonOverFood(struct food f[],int x,int y)
{
    for(int i=0;i<4;i++)
    {
        if(f[i].x==x && f[i].y==y)
            return 1;
    }
    return 0;
}
genpoison(struct food f[],struct snake *head,struct poison *p)
{
    int z,x,y,t,q;
    for(int i=0;i<2;i++)
{
    if(counterp==0)
    {
        do
        {
            x=(rand()%M);
            y=(rand()%N);
            z=checkPoisonOverSnake(head,x,y);
            q=checkPoisonOverFood(f,x,y);

        }while(z!=0 && q!=0);
            p[0].x=x;
            p[0].y=y;
            p[0].val='P';
            counterp++;
        }
    else
        {
        do
        {
            x=(rand()%M);
            y=(rand()%N);
            z=checkPoisonOverSnake(head,x,y);
            q=checkPoisonOverFood(f,x,y);
            t=checkPoisonOverPoison(p,x,y);
        }while(z!=0 && t!=0);
            p[i].x=x;
            p[i].y=y;
            p[i].val='P';
        }
    }
}
struct snake* start(struct snake *head,struct food f[],struct poison p[])
{
    struct snake *ptr;
    if(head==NULL)
    {
        struct snake *n;
        n=new snake;
        n->x=5;
        n->y=5;
        n->value='O';
        n->next = NULL;
        head=n;
        ptr=head;

        n=new snake;
        n->x=5;
        n->y=4;
        n->value='#';
        n->next=NULL;
        ptr->next=n;
        ptr=n;

        n=new snake;
        n->x=5;
        n->y=3;
        n->next=NULL;
        n->value='*';
        ptr->next=n;
        ptr=n;

        srand(time(NULL));
        genfood(f,head);
        genpoison(f,head,p);
    }
    return head;

};


void setBoard(char board[M][N],struct snake *head,struct food f[],struct poison p[])
{
   for(int i=0;i<M;i++)
    for(int j=0;j<N;j++)
        board[i][j]=' ';
   struct snake *ptr=head;
   while(ptr!=NULL)
   {
       board[ptr->x][ptr->y]=ptr->value;
       ptr=ptr->next;
   }
  for(int i=0;i<4;i++)
        board[f[i].x][f[i].y]=f[i].val;
for(int i=0;i<2;i++)
        board[p[i].x][p[i].y]=p[i].val;

}
void printBoard(char board[M][N])
{
    printf("dist : %d\tMOVES : %d\tLIFE : %d\tSCORE : %d\n",dist,moves,life,score);
   for(int i=0;i<=N+1;i++)
    {
        printf("!");
    }
    printf("\n");
    for(int i=0;i<M;i++)
    {
        printf("!");
        for(int j=0;j<N;j++)
        {
            cout<<board[i][j];
        }
        printf("!");
        cout<<endl;
    }
    for(int i=0;i<=N+1;i++)
    {
        printf("!");
    }
//printf("hx %d hy %d\n",head->x,head->y);
}
int validMove(struct snake *head,char dir)
{
    int f=1;
    int hx=head->x,hy=head->y;
    int bx=head->next->x,by=head->next->y;
   // printf("dir = %c\nhx = %d\thy = %d \n bx = %d\tby = %d\n",dir,hx,hy,bx,by);
    if(hy==by && abs(hx-bx)!=1)
    {

        if(hx==0 && bx==M-1)
        {
            bx=-1;
        }
        else
        {
            hx=-1;
        }

    }
    else if(hx==bx && abs(hy-by)!=1)
    {
        if(hy==0 && by==N-1)
        {
            by=-1;
        }
        else
        {
            hy=-1;
        }
    }
    if(dir!='a' && dir!='w' && dir!='s' && dir!='d')
        return 0;
    else if(dir=='s')
    {
       if(hx == (bx-1))
            return 0;
        else
            return 1;
    }
    else if(dir=='w')
    {
        if(hx == (bx+1))
            return 0;
        else
            return 1;
    }
    else if(dir == 'a')
    {
        if(hy == (by+1 ))
            return 0;
        else
            return 1;
    }
    else if(dir == 'd')
    {

        if( hy == (by-1) )
            {
                return 0;
            }
        else
            return 1;
    }


}
struct snake* insertNewNode(struct snake *head,int x,int y)
{
    if(head!=NULL)
	{
	struct snake *ptr=head,*temp;
	temp=(struct snake*)malloc(sizeof(struct snake));
	temp->next=NULL;
	temp->x=x;
	temp->y=y;
	temp->value='*';

	while(ptr->next!=NULL)
	{
	ptr=ptr->next;
	}
	ptr->next=temp;
	ptr->value='#';
	}
	return head;
};
void swap(int *t,int *snake)
{
    int blah = *t;
    *t=*snake;
    *snake=blah;
}
writeData(struct snake *head,struct food f[],struct poison p[])
{
    struct snake *efg=head;
            FILE *fptr;
            fptr=fopen("save.txt","w");
            while(efg!=NULL)
            {
            fprintf(fptr,"%d,%d,%c\n",efg->x,efg->y,efg->value);
            efg=efg->next;
            }
            for(int i=0;i<4;i++)
            fprintf(fptr,"%d,%d,%d,%c\n",f[i].x,f[i].y,i,f[i].val);

            for(int i=0;i<2;i++)
            fprintf(fptr,"%d,%d,%d,%c\n",p[i].x,p[i].y,i,p[i].val);

            fprintf(fptr,"%d,%c\n",life,'L');
            fprintf(fptr,"%d,%c\n",score,'S');
            fprintf(fptr,"%d,%c\n",moves,'M');
            fprintf(fptr,"%d,%c\n",dist,'D');
            fclose(fptr);
            exit(0);
}
void generateFood(struct food f[],struct snake *head)
{
    int x,y,z,t;
    do
        {
            x=(rand()%M);
            y=(rand()%N);
            z=checkFoodOverSnake(head,x,y);
            t=checkFoodOverFood(f,x,y);
        }while(z!=0 && t!=0);
            f[foodIndex].x=x;
            f[foodIndex].y=y;
            f[foodIndex].val='F';
}
int checkPosOfPoison(struct poison p[],struct snake *head)
{
    for(int i=0;i<2;i++)
    {
    if((p[i].x==head->x) && (p[i].y==head->y))
        {
            poisonIndex=i;
            return 1;
        }
    }
    return 0;
}
generatePosion(struct poison p[],struct snake *head)
{
    int x,y,z,t;
    do
        {
            x=(rand()%M);
            y=(rand()%N);
            z=checkPoisonOverSnake(head,x,y);
            t=checkPoisonOverPoison(p,x,y);
        }while(z!=0 && t!=0);
            p[poisonIndex].x=x;
            p[poisonIndex].y=y;
            p[poisonIndex].val='P';
}

struct snake*newsnake(struct snake *head,int x,int y,struct food f[],struct poison p[])
{
    int temp1=x,temp2=y;
    struct snake *ptr=head;
    while(ptr!=NULL)
    {
        swap(&temp1,&(ptr->x));
        swap(&temp2,&(ptr->y));
        ptr=ptr->next;
    }
    if(checkPosOfFood(f,head))
    {
        score=score+5;
        head=insertNewNode(head,temp1,temp2);
        generateFood(f,head);
    }
    if(checkPosOfPoison(p,head))
    {
        life=life-1;
        generatePosion(p,head);
    }
    return head;
}
void cut(struct snake *head)
{
    int x=head->x,y=head->y;
    struct snake *ptr=head->next;
    struct snake *temp=head;
    while(ptr!=NULL)
    {
        if((ptr->x==x)&&(ptr->y==y))
        {   /*cout<<"\nGame over\n";
            FILE *fptr;
            fptr=fopen("save.txt","w");
            exit(0);*/
            life=life-1;
            temp->value='*';
            temp->next=NULL;
            break;
        }
        temp=ptr;
        ptr=ptr->next;
    }
}
char currdir(struct snake *head)
{
    int f=1;
    int hx=head->x,hy=head->y;
    int bx=head->next->x,by=head->next->y;
    if(hy==by && abs(hx-bx)!=1)
    {

        if(hx==0 && bx==M-1)
        {
            bx=-1;
        }
        else
        {
            hx=-1;
        }

    }
    else if(hx==bx && abs(hy-by)!=1)
    {
        if(hy==0 && by==N-1)
        {
            by=-1;
        }
        else
        {
            hy=-1;
        }
    }

       if(hx == (bx-1))
            return 'w';
        else if(hx == (bx+1))
            return 's';
        else if(hy == (by+1 ))
            return 'd';
        else if( hy == (by-1) )
            return 'a';


}
struct snake *movement(struct snake *head,struct food f[],struct poison p[])
{

    char quit;
    char choice;
    cout<<"\nMake a move: ";
    char dir;
   delay(50);
    cout<<"\n\nEnter the direction in which you want to move the snake: ";
    if(kbhit())
    {
    moves=moves+1;
     dir=getch();
     if(dir=='q')
    {
        cout<<"\ndo you want to quit\n";
        cin>>quit;
        if(quit=='y'){
        cout<<"\ndo you want to save the game?";
        cin>>choice;
        if(choice=='y')
        {
            writeData(head,f,p);
        }
        else
            exit(0);
    }
    }
    }
    else
    {
        dir=currdir(head);
    }/*

    cin>>dir;
    moves=moves+1;
    if(dir=='q')
    {
        cout<<"\ndo you want to save the game?";
        cin>>choice;
        if(choice=='y')
        {
            writeData(head,f,p);
        }
        else
            exit(0);
    }*/
    if(validMove(head,dir))
    {
        dist=dist+1;
        int x,y;
        x=head->x;
        y=head->y;
        if(dir=='w')
            {
                x--;
                if(x<0)
                    x=M-1;
            }

        else if(dir=='s')
            {
                x++;
                if(x>=M)
                    x=0;
            }
        else if(dir=='a')
            {
                y--;
                if(y<0)
                    y=N-1;
            }
        else if(dir=='d')
            {
                y++;
                if(y>=N)
                y=0;

            }
            head=newsnake(head,x,y,f,p);
            cut(head);
    }
    else
    {
        printf("invalid dir\n");
        head=movement(head,f,p);

    }

        return head;

};
struct snake * createAgain(struct snake *head,int x,int y,char value)
{
    int flag=0;
    struct snake *ptr=head;
    if(head==NULL)
    {
        struct snake *n;
        n=new snake;
        n->x=x;
        n->y=y;
        n->value=value;
        n->next=NULL;
        head=n;
        ptr=head;
    }
    else
    {
        struct snake *n;
        n=new snake;
        n->x=x;
        n->y=y;
        n->value=value;
        n->next=NULL;
        while(ptr->next!=NULL)
            ptr=ptr->next;
        ptr->next=n;
        ptr=n;
        //cout<<ptr->x<<"  "<<ptr->y<<"   "<<ptr->value<<endl ;
    }
    //cout<<head->x<<"  "<<head->y<<"   "<<head->value<<endl<<"------------"<<endl ;
    return head;

}
void genfoodfromfile(struct food f[],int x,int y,char value,int i)
{
    f[i].x=x;
    f[i].y=y;
    f[i].val=value;
}
void genpoisonfromfile(struct poison p[],int x,int y,char value,int i)
{
     p[i].x=x;
    p[i].y=y;
    p[i].val=value;
}
struct snake *loadSaved(struct snake *head,struct food f[],struct poison p[])
{
     int x,y,count=0,j;
    int i=0;
    FILE *fptr;
    char value;
    char ch;
    char name[20];
    fptr = fopen("save.txt", "r");
    if(fptr!=NULL){
        ch = fgetc(fptr);
     if(ch==EOF)
    {
        printf("You have no save games! so hit any key to start new game\n");
        getch();
        head=start(head,f,p);
        return head;
    }
    else
    {
    while (ch != EOF)
    {
        if(ch=='\n')
        {
                //cout<<x<<"\t"<<y<<"\t"<<j<<"\t"<<value<<endl;
                count=0;
                if(value!='F' && value!='P' && value!='L' && value!='D' && value!='S' && value!='M')
                {
                    head=createAgain(head,x,y,value);
                }
                else if(value == 'F')
                    genfoodfromfile(f,x,y,value,j);
                else if(value == 'P')
                    genpoisonfromfile(p,x,y,value,j);
                else if(value=='L')
                    life=x;
                else if(value=='D')
                    dist=x;
                else if(value=='M')
                    moves=x;
                else if(value=='S')
                    score=x;
                ch=fgetc(fptr);
                continue;
        }
        else if(ch!=',')
        {
            if((ch>='A' && ch<='Z' ) || ( ch >= 'a' && ch<='z')  || ch=='#' || ch=='*')
            {
                value=ch;
                count++;
            }
               else
                {
                name[i] = ch;
                i++;
                }
        }
        else
        {
            name[i]='\0';
            i=0;
            int ans=atoi(name);
            if(count==0)
            {
                x=ans;
                count++;
            }
            else if(count==1)
            {
                y=ans;
                count++;
            }
            else if(count == 2)
            {
                j=ans;
            }
        }
            ch = fgetc(fptr);
    }
        fclose(fptr);
        return head;
    }
    }
    else
    {
        printf("You have no save games! so hit any key to start new game\n");
        getch();
        head=start(head,f,p);
        return head;
    }
}
int main()
{
int newGame=1;
struct food f[4];
struct poison p[2];
struct snake *head=NULL;
cout<<"\n\n--------------------------SNAKE GAME----------------------------\n\n\n\t\t> Press 0 to load a saved game\n\n\t\t> Press 1 to start a new game\n\n\t\t> Press 2 to view Records\n\n\t\t> Press 3 to clear all Records\n\n\t\t> Press 4 to clear all saved Games\n";
cin>>newGame;
if(newGame==1)
{
    FILE *fptr;
    fptr=fopen("save.txt","w");
    head=start(head,f,p);
}
else if(newGame==0)
{
    head=loadSaved(head,f,p);
}
else if(newGame==2)
{
    FILE *fptr;
    system("cls");
    fptr=fopen("record.txt","r");
    char ch;
    ch=fgetc(fptr);
    cout<<"\t\t   RECORDS\n";
    if(ch==EOF)
    {
        cout<<"There is no record as of now\n";
    }
    while(ch!=EOF)
        {
            printf("%c",ch);
            ch=fgetc(fptr);
        }
    fclose(fptr);
}
else if(newGame==3)
{
    FILE *fptr;
    system("cls");
    fptr=fopen("record.txt","w");
    fclose(fptr);
    cout<<"ALL RECORDS HAVE BEEN DELETED";
    fptr=fopen("save.txt","a");
    fprintf(fptr,"\t\t\tRECORDS\n");
    fclose(fptr);
}
else if(newGame==4)
{
    FILE *fptr;
    system("cls");
    fptr=fopen("save.txt","w");
    fclose(fptr);
    cout<<"ALL SAVE GAMES HAVE BEEN DELETED!";

}
char board[M][N];
while(1)
{

setBoard(board,head,f,p);
printBoard(board);
if(life>0)
head=movement(head,f,p);
else
{
    cout<<"\n\n---------------------GAME OVER--------------------\n\n!";
    printf("dist : %d\tMOVES : %d\tLIFE : %d\tSCORE : %d\n",dist,moves,life,score);
    getch();
    FILE *fptr;
    fptr=fopen("save.txt","w");
    fclose(fptr);
    fptr=fopen("record.txt","a");
    char user[30];
    system("cls");
    cout<<"Enter your name for the record!\n";
    scanf("%s",user);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(fptr,"\n___________________________________________________\nDATE: %d-%d-%d\t\tTIME %d:%d:%d\n",tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(fptr,"NAME : %s\t\tSCORE : %d\n___________________________________________________",user,score);
    system("cls");
    fclose(fptr);
    fptr=fopen("record.txt","r");
    char ch;
    ch=fgetc(fptr);
    while(ch!=EOF)
        {
            printf("%c",ch);
            ch=fgetc(fptr);
        }
    fclose(fptr);
    break;
}
system("cls");
}

return 0;
}
