/*************************************************/
/* Tony Nguyen*/
/* CS-241 001 */
/*  Sudoku solver which takes a 81
    char puzzle and solves,
    returns errors if invalid puzzle, 
    roughly takes around 30 sec :/
    tried to comment my best by in terms of
    code readability I think it's pretty awful
 */
/*************************************************/

#include <stdio.h>

#define MAX_PUZZLE_CHAR 81
#define INVALID_CHAR 1
#define INVALID_LENGTH 2
#define INVALID_PUZZLE 4
#define EMPTY 42 

int error = 0;
int done = 0;
int count = 0;
int status = 0;
int beststatus = 0;
int bestindex  = 100;

void readInt(char array[]);
void printArray(char array[]);
int checkRow(int row, char array[], int flag);
void printError();
int checkCol(int col, char array[], int flag);
int checkBox(int whichbox, char array[], int flag);
int mask(int temp);
int checkSquare(int index, char array[], int flag);
int validPuzzle(char array[]);
int playSquare(int index, int c,  char array[]);
void reset(char array[]);
int isComplete(char array[], int *index, int flag);
int sudokuSolv( char array[]);
void bestmove( int *index, char array[], int *bestflag, int *comflag);

int main()
{
  char myArray[MAX_PUZZLE_CHAR  ];
  int foo = 0;

  while(!done)
  {
    /* preps for next puzzle */
    reset(myArray);

    /* reads in chars */
    readInt(myArray); 

    /* EOF char read */
    if(done) break;


    if(error == 0)
    {
      /* checks if the puzzle is already completed */
      if(isComplete(myArray, &foo, 0))
      {
        printArray(myArray);
        continue;
      }

      /* checks if duplicates are given in the puzzle */
      validPuzzle(myArray);
    } 

    /* prints error */
    if(error)
    {
      printError();
      /*  printf("error: %d\n\n" , error); */
      continue;
    }

    /* tries to solve puzzle */
    if(sudokuSolv(myArray)) 
    {
      printArray(myArray);
    }
    else
    {
      printf("No solution\n\n");
    }

  }





  return 0;
}


/* reads chars, throws error if invalid char */
void readInt(char array[])
{
  int c = getchar();

  while( c != EOF && c !='\n')
  {

    printf("%c" , c);


    if(count <= MAX_PUZZLE_CHAR)
    {
      array[count] = c ;

    } 
    count++; 

    if(!((c >= '0' && c <= '9') ||  c == '.') )
    {
      error |= INVALID_CHAR;
    }

    c = getchar();
  } 
  if(!( count == MAX_PUZZLE_CHAR || count == 0))  error |= INVALID_LENGTH; 
  if( c == '\n') printf("%c" , c);
  if( c == EOF) done = 1;
}


void printArray(char array[])
{
  int i;

  for(i = 0 ; i < MAX_PUZZLE_CHAR ; i++)
  {
    printf("%c",  array[i]); 
  }  

  printf("\n\n");
}

/* obtains a mask value that is used to find duplicates in check methods */
int mask(int temp)
{
  return ( 1 << (temp - 1));  
}

void printError()
{
  printf("Error\n\n");  

}

/* checks rows for duplicates by looking at bits */
int checkRow(int row, char array[], int flag)
{
  /*0th bit is symbol '1', same thing up to bit 8 = symb '9'*/
  int r = 9 * row;
  int i;
  int temp = 0;
  int tempbit = 0;
  int duplicheck = 0;
  int localstatus = 0;

  for( i = r ; i < r + 9 ; i++)
  {
    if(array[i] == '.')
    {
      continue;   
    }

    temp = (array[i] - '0');
    tempbit = mask(temp);

    duplicheck = tempbit & localstatus;  /*duplicate check whenever there is a     */
    /*2nd occurance of same value             */

    if(flag)
    {
      status |= tempbit;      
    }

    /*duplicate is found return */
    if(duplicheck)
    {
      /*  printf("DUPLICATE: %u\n", temp);*/
      return 1;

    }

    /*stores what symbols have already been found */
    localstatus |= tempbit;
  }
  return 0;  /* 0 means that there are no duplicates*/

}



/* checks col for duplicates similarly as in checkrow */
int checkCol(int col, char array[], int flag)
{
  int i ;
  int temp = 0;
  int tempbit = 0;
  int duplicheck = 0;
  int localstatus = 0;



  for( i = col; i <  MAX_PUZZLE_CHAR   ; i += 9)
  {
    if(array[i] == '.')
    {
      continue;   
    }

    temp = array[i] - '0';
    tempbit = mask(temp);

    duplicheck = tempbit & localstatus;  /*duplicate check whenever there is a     */
    /*2nd occurance of same value             */
    if(flag)
    {
      status |= tempbit;      
    }


    if(duplicheck)
    {
      /*  printf("DUPLICATE COL: %u\n", temp); */
      return 1;

    }

    localstatus |= tempbit;

  }
  return 0; /* no duplicates*/

}

/*checks box for duplicates similarly as other check methods */
int checkBox(int whichbox, char array[], int flag)
{
  int i;
  int temp = 0;
  int tempbit = 0;
  int duplicheck = 0;
  int c = 0;
  int r = 0;
  int localstatus = 0;

  switch(whichbox)   /* I wasn't sure how to do this so I had to brute force T.T*/
  {
    case 0:
      c = 0;
      break;
    case 1:
      c = 3;
      break;
    case 2:
      c = 6;
      break;

    case 3:
      c = 27;
      break;
    case 4:
      c = 30;
      break;
    case 5:
      c = 33;
      break;

    case 6:
      c = 54;
      break;
    case 7:
      c = 57;
      break;
    case 8:
      c = 60;
      break;

  }  


  for(i = c  ; i <= c + 18 ; i += 9)    /*nested loop to check a "box" within array  */
  {
    for( r = i ; r < i + 3; r++)
    {
      if(array[r] == '.')
      {
        continue;   
      }

      temp = array[r] - '0';
      tempbit = mask(temp);

      duplicheck = tempbit & localstatus; 

      if(flag)
      {
        status |= tempbit; 
      }

      if(duplicheck)
      {

        /*  printf("DUPLICATE BOX : %u\n", temp); */
        return 1;

      }

      localstatus |= tempbit; 
    }

  }

  return 0;   /* no duplicates*/
}


/* checks box, row, col, given a index */
int checkSquare(int index, char array[], int flag)
{
  int row = index / 9;
  int col = index % 9;
  int br = row / 3;
  int bc = col / 3;
  int bnum = br * 3 + bc;

  int rcheck; 
  int ccheck;
  int bcheck; 


  /* flag is used to indicate whether I need to see the options of square*/
  /* this is done by bitwise 'or' of a global value, which is used in sudokusolv */
  if(flag)
  {
    rcheck = checkRow( row, array, 1);

    ccheck = checkCol( col, array, 1);

    bcheck = checkBox( bnum, array, 1); 


  /* I could have just done it with status but I was testing on how to find
     the best first move ( the one with the least options )  */
    if(status > beststatus)
    {
      beststatus = status;
      bestindex = index; 
    }   
    status = 0;

  }
  else
  {
  /* here I know I'm just testing for duplicate so as soon as I see
     a duplicate I return, I thought this would make things faster
     but I'm not sure */
    rcheck = checkRow( row, array, 0);
    if(rcheck)
    {
      return 1;
    } 
    ccheck = checkCol( col, array, 0);
    if(ccheck)
    {
      return 1;
    } 
    bcheck = checkBox( bnum, array, 0); 
    if(bcheck)
    {
      return 1;
    } 

  }

  /* looks if any duplicates are found given flag is on */
  if( rcheck || ccheck || bcheck)
  {
    return 1;
  } 


  return 0; 
  /*returns square options, 0 bit means available possible play */
  /* need to use pointers to giver info on all checks*/


}

/* checks if given puzzle already has duplicate, if so it's invalid */
int validPuzzle(char array[])
{
  int i;
  int sqStatus;
  for( i = 0; i < MAX_PUZZLE_CHAR ; i++)
  {
    sqStatus = checkSquare( i , array, 0);
    if(sqStatus == 1)
    {
      error |= INVALID_PUZZLE;
      return 0;    /* not a valid puzzle    */
    } 


  } 

  return 1;

}

/* checks whether a hypothetical play would be the correct choice */
int playSquare(int index, int num,  char array[])
{
  int sqStatus;
  char c = num + '0'; 

  array[index] = c;

  sqStatus = checkSquare( index , array , 0);
  if(sqStatus == 1)
  {
    array[index] = '.';
    return 0;
  }   

  array[index] = '.';
  return 1;

  /* return 0 if play is invalid also sets to empty, 1 if play is successful */
}


/*resets array, error, count, to prep for next iteration in main   */ 
void reset(char array[])
{
  int i;

  error = 0;
  count = 0;
  bestindex = 100;
  beststatus = 0;

  for( i = 0; i < MAX_PUZZLE_CHAR; i++)
  {
    array[i] = '.';
  }

  return;
}


/*checks if the puzzle is already complete    */ 
int isComplete(char array[], int *index, int flag)
{
  int i;
  for(i = 0; i < MAX_PUZZLE_CHAR ; i++)
  {
    if(array[i] == '.')
    {
      if(flag)
      {
        *index = i;
      }
      return 0;
    }
  }  

  return 1;
}


/* algorithm that solves puzzle, which is kinda slow and lame compared to all
   the other cool peoplez code who can solve it in a couple of seconds ._. */
int sudokuSolv( char array[])
{
  int i;  
  int index;
/*  int bestflag = 0; */
  int compflag = 1;
  int tempbit = 0;  
  int localstatus = 0;

  /* reset beststatus so a best status can be assigned for a specific stack */
  beststatus = 0;

  /*Whenever sudoku is complete, solver is finnished     */
  if(isComplete(array, &index, compflag))
  {
    return 1;  /*sucess*/
  }

  /* checks current empty index and obtains a beststatus which is used to 
     look for a play */
  checkSquare(index, array, 1);
  /* remembers beststatus during a certain time */
  localstatus = beststatus;

  /*consider digits 1 through 9*/
  for(i = 1; i <= 9 ; i++)
  {
    tempbit = mask(i);
    /* looks for and empty available play, if found try it */
    if(tempbit&(~localstatus))
    { 

      /* test hypothetical available play */
      if(playSquare(index,  i, array) )
      {
        /* play is safe hypothetical change to array */
        array[index] = (i + '0'); 

        /* sucess if previous call is sucess */
        if(sudokuSolv(array))
        {
          return 1;
        }

        /* failure, undo go to next iteration */
        array[index] = '.';

      } 
    }
  }
  return 0;  /*failed goes back into previous sudoku call */

}





/* test stuff */
/*
void bestmove( int *index, char array[], int *bestflag, int *comflag)
{
  int i;

  for(i = 0; i < MAX_PUZZLE_CHAR; i++)
  {
    if(array[i] == '.')
    {
      checkSquare( i , array, 1);
    }
  }
  *index = bestindex;

  *bestflag = 0;
  *comflag = 1; 
  return;


}

*/

 /* testing stuff */
 /* if(bestflag)
  {
    bestmove(&index, array, &bestflag, &compflag);
  } */






