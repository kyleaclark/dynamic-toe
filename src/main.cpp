#include <iostream.h>
#include <stdlib.h>
#include <conio.c>   //not recommend to link to a .c (or .cpp) file!!!
#include <windows.h>

#define BLANK  0      //if the game hasn't been won yet
#define X_WIN  1      //if the game has been won by X
#define O_WIN  2      //if the game has been won by O

int map_size();  //this asks for the size of the grid and asks for a one player or two player game
void title();    //this displays the title for the game

short size;  //this is for the size of the grid
bool game=0;  //this is for whether game is going to be one player or two player
bool player;       //this tells whose turn it is

class init
 { 
 public: 
  char **grid;
  short **value_x;
  short **value_y; 
 }; 
  
class map : public init
 {
 //[AI functions]
 bool valid;    //used to check to see if a function is valid or not
 bool any();   //finding whatever possible square is left
 bool middle();        //this is to find the middle square of the grid
 bool horizontal_O();   //find the horizontal move when 2 or more Os are connected
 bool horizontal_X();   //find the horizontal move when 2 or more Xs are connected
 bool vertical_O();   //find the vertical move when 2 or more Os are connected
 bool vertical_X();   //find the vertical move when 2 or more Xs are connected
 short find_zero();     //find how many Xs are in a line
 bool do_zero(short line);      //draw to the line that has zero Xs in it
 short num;
 public:
 map(short s);           //constructor...sets size of grid
 bool move_check(short x, short y);           //to see if a move is valid or not
 int move(short x, short y, bool p);     //setting what the grid should equal
 int ai_move();                       //finding the move the ai should make
 int gotoxy(short x, short y);     //going to a certain point
 int clear_line(short x, short y, short length, short lines);     //clearing a line based on it's position and it's length
 void create();            //creating the map
 int display(short x, short y);       //displaying the map
 int win_check();           //check to see if the game has been won
 short win_ending(short winner);  //the ending of the game if a player has won
 }; 
 


map::map(short s)
 {
  size = s;
  grid = new char*[size]; 
  value_x = new short*[size];
  value_y = new short*[size];
  for (short i = 0 ; i < size; i++ )
   { 
    grid[i] = new char[size];
    value_x[i] = new short[size];
    value_y[i] = new short[size];
   }
   
  for(short i = 0; i < size; i++)
   {
   for(short j = 0; j < size; j++)
    {
     grid[i][j] = '.';
    }
   }

 }
 
int map::move(short x, short y, bool p)
 {
  if(p == 0)
   grid[x-1][y-1] = 'X';
  else if(p == 1 && game == 1)
   grid[x-1][y-1] = 'O';
 }

bool map::move_check(short x, short y)
 {
 bool move=true;

      if(grid[x-1][y-1] == 'X' || grid[x-1][y-1] == 'O')
       {
       Beep(200, 400);
       move=false;
       gotoxy(wherex(), wherey()+1);
       }

 return move;
 }
 
void map::create()
 {
 system("CLS");
 
  for(short i = 0, h = 0, c = 0; i < size;)
   {
    if(c % 2 == 0)
     {
       cout << i+1;
       value_x[i][h] = wherex();
       value_y[i][h] = wherey()+2;
       cout << grid[i][h];        
       cout << h+1;
     i++;
     }
    else
     {
      textcolor(LIGHTGREEN);      //setting color to lightgreen for grid
      cout << "|";
      textcolor(LIGHTGRAY);       //setting color to lightgray (normal)
     }
     
    if(i == size && h < size-1)
     {
     cout << endl;
   
     textcolor(LIGHTGREEN);   //setting color to lightgreen for grid
      for(int a = 0; i < (size*5)-1; i++)
       cout << "-";       
     textcolor(LIGHTGRAY);           //setting color to lightgray (normal)
       
     cout << endl;
     
     i = 0;
     h++;
     c = 0;
     
     }    
    else
     c++;
    }
    
 
  cout << endl << endl;
  textcolor(LIGHTMAGENTA);
  cout << "Col(accros)   Row(down)"; 
  textcolor(LIGHTGRAY);
              
  return;
 } 
 
int map::display(short x, short y)
 {
short x_coord, y_coord;
short repos_x, repos_y;
 
  for(short h = 0; h < size; h++)
   {
    for(short i = 0; i < size; i++)
     {
      if(grid[i][h] == 'X' || grid[i][h] == 'O')
       {
      // x_coord = x*x + ;
     //  y_coord = y; 
       gotoxy(value_x[i][h]-1, value_y[i][h]);
       cout << " ";
       if(grid[i][h] == 'X')
        {
         textcolor(LIGHTBLUE);
         cout << grid[i][h];
         textcolor(LIGHTGRAY);
        }
       else if(grid[i][h] == 'O')
        {
         textcolor(LIGHTRED);
         cout << grid[i][h];
         textcolor(LIGHTGRAY);
        }
       else
        cout << grid[i][h];
       cout << " ";
       }
     }
   }
 
  gotoxy((size*2)+size-1, (size*2));  
  
 }
 
int map::gotoxy(short x, short y)
 {
   COORD coord;  //this is for declaring a coordinate
   coord.X = x;  //this is declaring the x-axis as the the value of x
   coord.Y = y;  //this is declaring the y-axis as the the value of y
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);  //this is positioning the cursor
} 

int map::clear_line(short x, short y, short length, short lines)
 {
  short loop=0;
 /* COORD coord;
  coord.X = x;
  coord.Y = y;    */
  while(loop < lines)
  {
   gotoxy(x, y);
   for(short li = 0; li < lines; li++)
    {
     for(short le = 0; le < length; le++)
      {
       cout << " ";
      }
     cout << endl;
    }
   loop++;
   y++;
  }
 }  
 
int map::ai_move()
 {
bool iter=true;

find_zero();

 if(middle() == 0)     //if the middle spot of the grid isn't open the keep looking
  {
  if(horizontal_O() == 0)        //if trying to stop two Xs or connect two Os turns false then continue
   {
   if(horizontal_X() == 0)
    {
    if(vertical_O() == 0)      //if trying to stop two Xs or connect two Os turns false then continue
     {
      if(vertical_X() == 0)
       {
      if(num != size)   //if there is at least one line that doesn't have an X on it
       {
        if(do_zero(num) == 1)     //take that line from find_zero() and try to find an open spot on it
         iter=false;                      //this iter doesn't allow anythign else to be done...sort of breaks the loop
       }
      if(num == size && iter==true)      //if the find_zero() couldn't find an open spot and iter still equals
        any();                //try to find any open spot available...will start from top left corner then move to right
     }
    }
    }
   }
  }

}
  
bool map::middle()
 {
  short mid;
  valid = 0;
  
  mid = (short)(size/2);
  
  if(grid[mid][mid] != 'X' && grid[mid][mid] != 'O')
   {
    grid[mid][mid] = 'O';
    valid = 1;
   }
    
  return valid;
 } 
  
bool map::any()
 {
  short check = 0;
  valid = 0;
   
  for(short h = 0; h < size; h++)
   {
    for(short i = 0; i < size; i++)
     {
      if(grid[i][h] != 'X' && grid[i][h] != 'O')
       {
        grid[i][h] = 'O';
        i = size;
        h = size;
        break;
       }
      }
    }  
 }
 
short map::find_zero()
 {
  short check = 0;
  short line[size];
  valid = 0;
  num = 0;

    while(check < size)
     {
      for(short i = 0; i < size; i++)
       {
        if(grid[i][check] == 'X')
         {
          line[check]++;
         }
       }
       check++;
     //check = 0;
    }

  check = 0;
  
  while(check < size)
   {
    if(line[check] == 0)
     {
      num = check;
      check = size;
      break;
     }
    else if(line[check] != 0 && check == size - 1)
     num = size; 
     
    check++;    
   }
    
  return num;
 }
 
bool map::do_zero(short line)
 {
 valid = 0;

       for(short i = 0; i < size; i++)
        {
         if(grid[i][line] != 'X' && grid[i][line] != 'O')
          {
           grid[i][line] = 'O';
           valid = 1;
         //  i = size;
           break;
          }
         }

  
  return valid;
}    
    
bool map::horizontal_O()
 {
  short check = 0;
  valid = 0;
  
  for(short h = 0; h < size; h++)
   {
   check = 0;
    while(check < size)
     {
      if(check < size-2)
       {
        //for the O
        //finding three in a row with the first and second being O
        if(grid[0+check][h] == 'O' && grid[0+check+1][h] == 'O' 
           && grid[0+check+2][h] != 'O' && grid[0+check+2][h] != 'X')
         {
          grid[0+check+2][h] = 'O';
          valid = 1;
          check = size;
          h = size;
          break;
         }
        //finding three in a row with the first and last being O
        else if(grid[0+check][h] == 'O' && grid[0+check+2][h] == 'O'
                && grid[0+check+1][h] != 'O' && grid[0+check+1][h] != 'X' )
         {
          grid[0+check+1][h] = 'O';
          valid = 1;
          check = size;
          h = size;
          break;
         }
        //finding three in a row with the second and last being O
        else if(grid[0+check+1][h] == 'O' && grid[0+check+2][h] == 'O'
                && grid[0+check][h] != 'O' && grid[0+check][h] != 'X' )
         {
          grid[0+check][h] = 'O';
          valid = 1;
          check = size;
          h = size;
          break;
         }
       }
      check++;
     }
   }

 return valid;
}

bool map::horizontal_X()
 {
  short check = 0;
  valid = 0;
  
  for(short h = 0; h < size; h++)
   {
   check = 0;
    while(check < size)
     {
      if(check < size-2)
       {
        //for the X
        //finding three in a row with the first and second being X
        if(grid[0+check][h] == 'X' && grid[0+check+1][h] == 'X'
                && grid[0+check+2][h] != 'O' && grid[0+check+2][h] != 'X' )
         {
          grid[0+check+2][h] = 'O';
          valid = 1;
          check = size;
          h = size;
          break;
         }
        //finding three in a row with the first and last being X
        else if(grid[0+check][h] == 'X' && grid[0+check+2][h] == 'X'
                && grid[0+check+1][h] != 'O' && grid[0+check+1][h] != 'X' )
         {
          grid[0+check+1][h] = 'O';
          valid = 1;
          check = size;
          h = size;
          break;
         }
        //finding three in a row with the second and last being X
        else if(grid[0+check+1][h] == 'X' && grid[0+check+2][h] == 'X'
                && grid[0+check][h] != 'O' && grid[0+check][h] != 'X' )
         {
          grid[0+check][h] = 'O';
          valid = 1;
          check = size;
          h = size;
          break;
         }     
       }
      check++;
     }
   }

  return valid;
 }

bool map::vertical_O()
 {
  short check = 0;
  valid = 0;
  
  for(short i = 0; i < size; i++)
   {
   check = 0;
    while(check < size)
     {
      if(check < size-2)
       {
        //for the O
        //finding three in a row with the first and second being O
        if(grid[i][0+check] == 'O' && grid[i][0+check+1] == 'O' 
           && grid[i][0+check+2] != 'O' && grid[i][0+check+2] != 'X')
         {
          grid[i][0+check+2] = 'O';
          valid = 1;
          check = size;
          i = size;
          break;
         }
        //finding three in a row with the first and last being O
        else if(grid[i][0+check] == 'O' && grid[i][0+check+2] == 'O'
                && grid[i][0+check+1] != 'O' && grid[i][0+check+1] != 'X' )
         {
          grid[i][0+check+1] = 'O';
          valid = 1;
          check = size;
          i = size;
          break;
         }
        //finding three in a row with the second and last being O
        else if(grid[i][0+check+1] == 'O' && grid[i][0+check+2] == 'O'
                && grid[i][0+check] != 'O' && grid[i][0+check] != 'X' )
         {
          grid[i][0+check] = 'O';
          valid = 1;
          check = size;
          i = size;
          break;
         }
       }        
      check++;
     }
   }
   
 return valid;
} 


bool map::vertical_X()
 {
  short check = 0;
  valid = 0;
  
  for(short i = 0; i < size; i++)
   {
   check = 0;
    while(check < size)
     {
      if(check < size-2)
       {
        //for the X
        //finding three in a row with the first and second being X
        if(grid[i][0+check] == 'X' && grid[i][0+check+1] == 'X'
           && grid[i][0+check+2] != 'O' && grid[i][0+check+2] != 'X')
         {
          grid[i][0+check+2] = 'O';
          valid = 1;
          check = size;
          i = size;
          break;
         }
        //finding three in a row with the first and last being O
        else if(grid[i][0+check] == 'X' && grid[i][0+check+2] == 'X'
                && grid[i][0+check+1] != 'O' && grid[i][0+check+1] != 'X' )
         {
          grid[i][0+check+1] = 'O';
          valid = 1;
          check = size;
          i = size;
          break;
         }
        //finding three in a row with the second and last being O
        else if(grid[i][0+check+1] == 'X' && grid[i][0+check+2] == 'X'
                && grid[i][0+check] != 'O' && grid[i][0+check] != 'X' )
         {
          grid[i][0+check] = 'O';
          valid = 1;
          check = size;
          i = size;
          break;
         }

       }        
      check++;
     }
   }
   
 return valid;
}
   
 
int map::win_check()
 {
  short win_grid[size][size];
  short win_flag = BLANK;
  short check = 0;
 
 
 if(win_flag == BLANK)
 {
  //for checking horizontal win        
  for(short h = 0; h < size; h++)
   {
   check = 0;
    while(check < size)
     {
      if(grid[0+check][h] == 'X')
       { 
        win_grid[0+check][h] = X_WIN;
       }
      else if(grid[0+check][h] == 'O')
       {
        win_grid[0+check][h] = O_WIN;
       }
      else
       {
        win_grid[0+check][h] = BLANK;
       }
       check++;
      }     
    check = 0;   
   }
 
 //for checking horizontal win  
  for(short h = 0; h < size; h++)
   {
    check = 0;  
    while(check < size)
     {
      if(win_grid[0+check][h] == X_WIN && win_flag != O_WIN)
       {
        win_flag = X_WIN;
       }
      else if(win_grid[0+check][h] == O_WIN && win_flag != X_WIN)
       {
        win_flag = O_WIN;
       }
      else
       {
        win_flag = BLANK;
        check = size;
       }
       if(check == size - 1 && win_flag != BLANK)
        {
         h = size;
         break;
        }
       check++;
      }
    } 
 }
 
if(win_flag == BLANK)
 {
  //for checking vertical win        
  for(short i = 0; i < size; i++)
   {
   check = 0;
    while(check < size)
     {
      if(grid[i][0+check] == 'X')
       { 
        win_grid[i][0+check] = X_WIN;
       }
      else if(grid[i][0+check] == 'O')
       {
        win_grid[i][0+check] = O_WIN;
       }
      else
       {
        win_grid[i][0+check] = BLANK;
       }
       check++;
      }     
    check = 0;   
   }
 
 //for checking vertical win  
  for(short i = 0; i < size; i++)
   {
    check = 0;  
    while(check < size)
     {
      if(win_grid[i][0+check] == X_WIN && win_flag != O_WIN)
       {
        win_flag = X_WIN;
       }
      else if(win_grid[i][0+check] == O_WIN && win_flag != X_WIN)
       {
        win_flag = O_WIN;
       }
      else
       {
        win_flag = BLANK;
        check = size;
       }
       if(check == size - 1 && win_flag != BLANK)
        {
         i = size;
         break;
        }
       check++;
      }
    } 
 }
 
if(win_flag == BLANK)
 {
  //for checking diagonal win        
    while(check < size)
     {
      if(grid[0+check][0+check] == 'X')
       { 
        win_grid[0+check][0+check] = X_WIN;
       }
      else if(grid[(size-1)-check][(size-1)-check] == 'X')
       {
        win_grid[(size-1)-check][(size-1)-check] = X_WIN;
       }
      else if(grid[0+check][0+check] == 'O')
       {
        win_grid[0+check][0+check] = O_WIN;
       }
      else if(grid[(size-1)-check][(size-1)-check] == 'O')
       {
        win_grid[(size-1)-check][(size-1)-check] = O_WIN;
       }
      else
       {
        win_grid[0+check][0+check] = BLANK;
       }
       check++;
      }     
    check = 0;   
 
    while(check < size)
     {
      if(win_grid[0+check][0+check] == X_WIN && win_flag != O_WIN)
       {
        win_flag = X_WIN;
       }
      else if(win_grid[0+check][0+check] == O_WIN && win_flag != X_WIN)
       {
        win_flag = O_WIN;
       }
      else
       {
        win_flag = BLANK;
        break;
       }
      check++;
     }
     
   if(win_flag == BLANK)
    {
     check = 0;
     while(check < size)
     {
      if(win_grid[(size-1)-check][(size-1)-check] == X_WIN && win_flag != O_WIN)
       {
        win_flag = X_WIN;
       }
      else if(win_grid[(size-1)-check][(size-1)-check] == O_WIN && win_flag != X_WIN)
       {
        win_flag = O_WIN;
       }
      else
       {
        win_flag = BLANK;
        break;
       }
      check++;
     }
    }
 }
    
return win_flag;

}
    
short map::win_ending(short winner)
 {
  if(winner != BLANK)
   {
    clear_line(0, wherey()+1, 23, 2);

    if(winner == X_WIN)
     {
     textcolor(LIGHTBLUE);
     cout << "X ";
     textcolor(WHITE);
     cout << "wins the game\n";
     }
    else
     {
     textcolor(LIGHTRED);
     cout << "O ";
     textcolor(WHITE);
     cout << "wins the game\n";
     }

    system("PAUSE");
    system("CLS");
    cout << endl;
   // textcolor(WHITE);
    cout << "   A      L           L              H       H      A      I I I I I   L\n"
            "  A A     L           L              H       H     A A         I       L\n"
            " A   A    L           L              H       H    A   A        I       L\n"
            "A A A A   L           L              H H H H H   A A A A       I       L\n"
            "A     A   L           L              H       H   A     A       I       L\n"
            "A     A   L           L              H       H   A     A       I       L\n"
            "A     A   L L L L L   L L L L L      H       H   A     A   I I I I I   L L L L L\n";
     if(winner == X_WIN)
      {
       textcolor(LIGHTBLUE);
       cout << "                               X       X\n"
               "                                X     X\n"
               "                                 X   X\n"
               "                                  X X\n"
               "                                   X\n"
               "                                  X X\n"
               "                                 X   X\n"
               "                                X     X\n"
               "                               X       X\n";
       textcolor(LIGHTGRAY);
      }
     else if(winner == O_WIN)
      {
       textcolor(LIGHTRED);
       cout << "                               O O\n"
               "                             O     O\n"
               "                           O         O\n"
               "                          O           O\n"
               "                         O             O\n"
               "                         O             O\n"
               "                         O             O\n"
               "                         O             O\n"
               "                          O           O\n"
               "                           O         O\n"
               "                             O     O\n"
               "                               O O\n";
       textcolor(LIGHTGRAY);
      }
      cout << endl;
      getch();
  }

  return winner;
}

int main()
 {
      short x, y; 
      char input;
      bool loop = true;

      title();
      
      map ob(map_size()); 
      
      ob.create();
      Sleep(500);

      while(loop == true)
      {
             
      if(player == 0 || (player == 1 && game == 1))
       { 
        cout << endl;    //new line after Row Col
        do
        {
          ob.clear_line(wherex(), wherey()+2, 23, 1);   //deleting numbers below Row Col
          ob.gotoxy(wherex()+5, wherey()+1);        //going to beginning of line
          textcolor(WHITE);     //setting color to white
          cin >> x;     //waiting for col number to be entered
          ob.gotoxy(wherex()+18, wherey()+1);     //going to spot where row number is entered
          cin >> y;              //waiting for row number to be entered
         }while(ob.move_check(x, y) == false);
        textcolor(LIGHTGRAY);           //setting color back to normal
        ob.move(x, y, player);
       }
      else if(player == 1 && game == 0)
       {
       ob.ai_move();
       player = 1;
       }
       
      ob.display(x, y);
      
     if(ob.win_ending(ob.win_check()) != BLANK)
       {
        loop = false;
       } 

      switch(player)
       {
        case 0: player = 1;
         break;
        case 1: player = 0;
         break;
       }
         
      } 
       
      return 0;
}

void title()
 {
 short loop=0;

 while(loop < 9)
 {

  switch(loop)
  {
   case 1: textcolor(LIGHTRED);
    break;
   case 2: textcolor(WHITE);
    break;
   case 3: textcolor(GREEN);
    break;
   case 4: textcolor(LIGHTMAGENTA);
    break;
   case 5: textcolor(YELLOW);
    break;
   case 6: textcolor(CYAN);
    break;
   case 7: textcolor(LIGHTGREEN);
    break;
   case 8: textcolor(LIGHTBLUE);
    break;
  }

  cout << endl << endl << "     D" << "Y" << "N" << "A" << "M" << "I" << "C\n"
       << "      " << " " << "T" << "O" << "E\n";
  Sleep(1000);
  system("CLS");
  loop++;
 }
 }

int map_size()
 {
  
  while(size < 3 || size > 9)
  {
  textcolor(LIGHTMAGENTA);
  cout << endl << endl << "     Between size 3 and 9 what size do you want the grid to be?" << endl;
  textcolor(WHITE);
  cout << "     ";
  cin >> size;
  textcolor(LIGHTGRAY);
  
   if(size < 3 || size > 9)
    Beep(200, 400);
    
  system("CLS");
  }

    textcolor(LIGHTMAGENTA);
    cout << endl << endl << "     Enter 0 for a one player game or enter 1 for a two player game" << endl;
    textcolor(WHITE);
    cout << "     ";
    cin >> game;
    textcolor(LIGHTGRAY);
  
  player = rand()%2;       //a number between 0 and 1

    if(player == 0)
     {
       textcolor(LIGHTMAGENTA);
       cout << endl << endl << "     Player X will go first" << endl;
       textcolor(WHITE);
       cout << "     ";
       system("PAUSE");
       textcolor(LIGHTGRAY);
     }
    else if(player == 1 && game == 0)
     {
       textcolor(LIGHTMAGENTA);
       cout << endl << endl << "     Enemy O will go first" << endl;
       textcolor(WHITE);
       cout << "     ";
       system("PAUSE");
       textcolor(LIGHTGRAY);
     }
    else if(player == 1 && game == 1)
     {
       textcolor(LIGHTMAGENTA);
       cout << endl << endl << "     Player O will go first" << endl;
       textcolor(WHITE);
       cout << "     ";
       system("PAUSE");
       textcolor(LIGHTGRAY);
     }

  system("CLS");
  
  return size;
 } 
