#ifndef RCP_IO_CONFIG_HPP             		
#define RCP_IO_CONFIG_HPP

#define NUM_KEYS_ON_PAD 12
String IO_Keypad_Group_1 = "1ABC";
String IO_Keypad_Group_2 = "2DEF";
String IO_Keypad_Group_3 = "3GHI";
String IO_Keypad_Group_4 = "4JKL";
String IO_Keypad_Group_5 = "5MNO";
String IO_Keypad_Group_6 = "6PQR";
String IO_Keypad_Group_7 = "7STU";
String IO_Keypad_Group_8 = "8VWX";
String IO_Keypad_Group_9 = "9YZ";
String IO_Keypad_Group_S = "* .?!";
String IO_Keypad_Group_0 = "0@$%:";
String IO_Keypad_Group_P = "#-/^";
String IO_Keypad_Groups[12] = {
  IO_Keypad_Group_1,
  IO_Keypad_Group_2,
  IO_Keypad_Group_3,
  IO_Keypad_Group_4,
  IO_Keypad_Group_5,
  IO_Keypad_Group_6,
  IO_Keypad_Group_7,
  IO_Keypad_Group_8,
  IO_Keypad_Group_9,
  IO_Keypad_Group_S,
  IO_Keypad_Group_0,
  IO_Keypad_Group_P
};

char cycleChar(char input);
bool sameCharGroup(char inputA, char inputB);
// int getCharGroup(char input);
// int getCharIndex(char input, char* array);

bool sameCharGroup(char inputA, char inputB){
  int x = 0;
  for(;x<NUM_KEYS_ON_PAD;x++){
    int index = IO_Keypad_Groups[x].indexOf(inputA);
    if(index !=-1)
    {  
      if(IO_Keypad_Groups[x].indexOf(inputB) != -1)
        return true;
      else
        return false;
    }
  }
  return false;
}

char cycleChar(char input){
  int x = 0;
  for(;x<NUM_KEYS_ON_PAD;x++){
    int index = IO_Keypad_Groups[x].indexOf(input);
    if(index !=-1){
      index++;
      Serial.println(index);
      if(index >= IO_Keypad_Groups[x].length())
        index = 0;
      Serial.println(index);
      return IO_Keypad_Groups[x].charAt(index);
    }
  }
  return input;
}

// int getCharGroup(char input){
//   int x = 0;
//   int offset = input - '0';
//   for(;x<NUM_KEYS_ON_PAD;x++){
//     int gIndex = IO_Keypad_Groups;
//     int index = IO_Keypad_Groups[gIndex].indexOf();
//     if(index !=-1)
//       return index;
//   }
//   return -1;
// }

// int getCharIndex(char input, char* array){
  
// }


/*
char cycleChar(char input){
  switch(input){
// Button 1
    case '1':
      return 'A';
    case 'A':
      return 'B';
    case 'B':
      return 'C';
    case 'C':
      return '1';

// Button 2
    case '2':
      return 'D';
    case 'D':
      return 'E';
    case 'E':
      return 'F';
    case 'F':
      return '2';


// Button 3

    case '3':
      return 'G';
    case 'G':
      return 'H';
    case 'H':
      return 'I';
    case 'I':
      return '3';

// Button 4
    case '4':
      return 'J';
    case 'J':
      return 'K';
    case 'K':
      return 'L';
    case 'L':
      return '4';

// Button 5
    case '5':
      return 'M';
    case 'M':
      return 'N';
    case 'N':
      return 'O';
    case 'O':
      return '5';

// Button 6
    case '6':
      return 'P';
    case 'P':
      return 'Q';
    case 'Q':
      return 'R';
    case 'R':
      return '6';

// Button 7
    case '7':
      return 'S';
    case 'S':
      return 'T';
    case 'T':
      return 'U';
    case 'U':
      return '7';

// Button 8
    case '8':
      return 'Y';
    case 'Y':
      return 'W';
    case 'W':
      return 'X';
    case 'X':
      return '8';

// Button 9
    case '9':
      return 'Y';
    case 'Y':
      return 'Z';
    case 'Z':
      return '9';

// Button 0
    case '0':
      return '@';
    case '@':
      return '$';
    case '$':
      return '%';
    case '%':
      return ':';
    case ':':
      return '0';

// Button *
    case '*':
      return ' ';
    case ' ':
      return '.';
    case '.':
      return '?';
    case '?':
      return '!';
    case '!':
      return '*';

// Button #
    case '#':
      return '-';
    case '-':
      return '/';
    case '/':
      return '^';
    case '^':
      return '#';
  }
}
*/

#endif //RCP_IO_CONFIG_HPP