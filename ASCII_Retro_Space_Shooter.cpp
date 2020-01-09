//ASCII Retro Space Shooter Project.
// Made by Smit Barmase (@smitbarmase). CSE.
// v1.0 on 7 Jan 2020.
// Learn. Modify. Share.
// I will keep updating this repo, adding more things to it.
// Thank you! Giving credit is appreciated. :)
// Current version. 7 Jan 2020 (v1.0)

#include<iostream>
#include<windows.h> //For cursor info
#include<conio.h> //For get keyboard hit
#include<chrono> //For clock
using namespace std;
using namespace std::chrono;

//Game setting data
//--------------------------
  const int H_size = 20; //Predefined canvas size.
  const int W_size = 40;

  const float bulletWaitTime = 0.1;
  const float bulletSpawnTime = 0.3;

  const float enemyWaitTime = 0.3;
  const float enemySpawnTime = 2;

  const string gameOverTitle = "Game Over";
  const string gameStartAgain = "Starts in ";
//--------------------------

//Private data no need to change.
//--------------------------
  high_resolution_clock::time_point t1, t2, t3, t4, t5, t6, t7, t8; //For keeping track of time duration
  bool gameOver = false;
  int score = 0;
  int x_Cord = 1;  //Default x cordinate of player.
  int dir = 0; //Direction to remove traces.

  char workspace[W_size][H_size] = {}; //2D workspace
//---------------------------

//Function to Show/Hide console cursor. Used it for hiding blinking cursor.
void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

//Function to set cursor position in console.
void setCursorPosition(int x, int y)
{
    static const HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(out, coord);
}

//Function to link between 2D Array and Console window.
void printObject(int i, int j, char c)
{
  workspace[i][j] = c; //Save char to 2d array.
  setCursorPosition(i,j); //Set cursor position on console.
  cout<<workspace[i][j]; //print that char from 2d array on console.
}

void MakeCanvas()
{
  //Making canvas wall here.
  for(int i=0;i<W_size;i++)
  {
    for(int j=0;j<H_size;j++)
    {
      if(i==0 || i==W_size-1)
      {
        printObject(i,j,']');
      }
      else
      {
        if(j==0 || j==H_size-1)
        {
          printObject(i,j,']');
        }
        else
        {
          printObject(i,j,' ');
        }
      }
    }
  }
  //Make Score panel.
  setCursorPosition(W_size+3,(H_size/2)); //Align string in center-right.
  cout<<"Score : "<<score;
}

//    #
//    #
// # ### #
// #######   4 x 7 grid (Reference on top-left)

void setPlayerPosition()
{
  const int y = H_size-2; //y is the bottom most co-ordinate
  const char symbol = '!';

  if(x_Cord<1) //Min bound.
    x_Cord = 1;
  else if(x_Cord>W_size-8) //Max bound.
    x_Cord = W_size-8; //8 due to player width 7 + 1

  //Draw player according to X and Y.
  for(int j=1; j<=3; j++)
  {
    for(int i=0;i<7;i++)
    {
      switch(j)
      {
        case 1: //Layer 1
                if(i==3)
                {
                  if(workspace[x_Cord+i][y-2]=='#')
                    gameOver = true;
                  printObject(x_Cord+i,y-2,symbol);
                }
                else
                  printObject(x_Cord+i,y-2,' ');
        break;
        case 2: //Layer 2
                if(i!=1 && i!=5)
                {
                  if(workspace[x_Cord+i][y-1]=='#')
                    gameOver = true;
                  printObject(x_Cord+i,y-1,symbol);
                }
                else
                  printObject(x_Cord+i,y-1,' ');
        break;
        case 3: //Layer 3
                if(workspace[x_Cord+i][y]=='#')
                  gameOver = true;
                printObject(x_Cord+i,y,symbol);
        break;
      }
    }
  }

  //Remove traces.
  for(int i=0;i<2;i++)
  {
    if(dir==1)
      printObject(x_Cord-1,y-i,' ');
    else if(dir==-1)
      printObject(x_Cord+7,y-i,' ');
  }
}

void SpawnEnemy()
{
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  srand((time_t)t.tv_nsec); // Random seed value from nanosec. //used time.h.

  int randomPos = (rand() % (W_size-8)) + 4;  //(W_size - 8) gives range. And + 4 shift that range to middle.
  printObject(randomPos,1,'#'); //Spawn enemy.
}

void SpawnBullet()
{
    printObject(x_Cord+3,H_size-5,'*'); //Spawn bullet.
}

void UpdateBulletState()
{
  //This loop travels from top-left to bottom-right.
  for(int i=1;i<W_size;i++)
  {
    for(int j=1;j<H_size;j++)
    {
      if(workspace[i][j]=='*')
      {
        if((j-1)>0)
        {
            if(workspace[i][j-1]=='#')
            {
              score += 10;
              setCursorPosition(W_size+3,(H_size/2)); //Align string in center-right.
              cout<<"Score : "<<score;
            }
            printObject(i,j-1,'*');
        }
        printObject(i,j,' ');
      }
    }
  }
}

void UpdateEnemyState()
{
  //This loop travels from bottom-right to top-left.
  for(int i=W_size-1;i>0;i--)
  {
    for(int j=H_size-1;j>0;j--)
    {
      if(workspace[i][j]=='#')
      {
        if(j<H_size-1)
        {
          if(workspace[i][j+1]=='!' || workspace[i][j+1]==']')
          {
            gameOver = true;
          }
          else
          {
            printObject(i,j+1,'#');
          }
        }
        printObject(i,j,' ');
      }
    }
  }
}

void CheckInput()  //Keyboard input function.
{
  if(_kbhit())
  {
    switch(_getch())
    {
      case 75: x_Cord += -1; //68 Left Arrow
               dir = -1; //Remove trace on opposite side.
          break;
      case 77: x_Cord += 1; //67 Right Arrow
               dir = 1; //Remove trace on opposite side.
          break;
      default:
          break;
    }
    setPlayerPosition(); //Update player position on key pressed.
  }
}

int main()
{
  ShowConsoleCursor(false);  //Hide cursor.
  MakeCanvas();
  while(true) //For replaying game after game over.
  {
    setPlayerPosition(); //Player position on start.
    while(gameOver==false) //Runs every frame.
    {
        CheckInput(); //Get input.
        t2 = high_resolution_clock::now();
        t4 = high_resolution_clock::now();
        t6 = high_resolution_clock::now();
        t8 = high_resolution_clock::now();
        if(duration_cast<duration<float>>(t2 - t1).count()>=bulletWaitTime)
        {
            UpdateBulletState();  //Runs every defautWaitTime.
            t1 = high_resolution_clock::now();
            t2 = {};
        }
        if(duration_cast<duration<float>>(t4 - t3).count()>=bulletSpawnTime)
        {
            SpawnBullet();  //Runs every spwantime.
            t3 = high_resolution_clock::now();
            t4 = {};
        }
        if(duration_cast<duration<float>>(t6 - t5).count()>=enemySpawnTime)
        {
            SpawnEnemy();  //Runs every spwantime.
            t5 = high_resolution_clock::now();
            t6 = {};
        }
        if(duration_cast<duration<float>>(t8 - t7).count()>=enemyWaitTime)
        {
            UpdateEnemyState();
            t7 = high_resolution_clock::now();
            t8 = {};
        }
    }
    //Game over here...
    //Score.
    score = 0; //Resetting score.

    //Display score panel.
    setCursorPosition(W_size+3,(H_size/2)); //Align string in center-right.
    cout<<"Score : 0    "; //Spaces to clear everything.

    //Clearing existing snake.
    for(int i=1;i<W_size-1;i++)
    {
      for(int j=1;j<H_size-1;j++)
      {
        printObject(i,j,' ');
      }
    }

    //Game over screen.
    setCursorPosition((W_size/2)-(gameOverTitle.size()/2),(H_size/2)-1); //Align string in center.
    cout<<gameOverTitle;
    for(int i=3;i>0;i--)
    {
      setCursorPosition((W_size/2)-(gameStartAgain.size()/2)-2,(H_size/2));
      cout<<gameStartAgain;
      cout<<i<<"...";
      Sleep(1000);
    }
    //Clearing all text from canvas.
    setCursorPosition((W_size/2)-(gameOverTitle.size()/2),(H_size/2)-1);
    for(int i=gameOverTitle.size();i>0;i--)
    {
      cout<<" ";
    }
    setCursorPosition((W_size/2)-(gameStartAgain.size()/2)-2,(H_size/2));
    for(int i=(gameStartAgain.size()+4);i>0;i--)  //+4 for "x..."
    {
      cout<<" ";
    }
    //Setting gameOver to flase.
    gameOver = false;
  }
  return 0;
}
