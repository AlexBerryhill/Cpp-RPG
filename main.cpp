//User input and Display
#include <iostream>
#include <iterator>
//Read Files (Level Maps)
#include <fstream>
#include <string>
#include <map>
#include "conio.h" //For getch
using namespace std;

class Map{
  public:
    char map[16][32];
  
  void getMap(string level){
    int i = 0;
    int j = 0;
    string fileName = "./levels/"+level+".txt";
    
    cout << "Welcome to level " << level << '\n'; //Debug
    
    ifstream myFile(fileName.c_str());
    if(myFile.is_open()){
      while(!myFile.eof()){
        myFile >> map[j][i];
        i++;
        if(i==32){
          i=0;
          j++;
        }
      }
      myFile.close();
    }
    else{
      cout << "Unable to open file";
    }
    return;
  }

  void displayMap(){
    std::map<char,char> mymap;
    std::map<char,char>::key_compare mycomp = mymap.key_comp();
    mymap['0']=' '; // Empty space
    mymap['#']='#'; // Wall
    mymap['@']='@'; // Player
    mymap['$']='$'; // Goal
    mymap['1']='H'; // Door to room 1
    mymap['2']='H'; // Door to room 2
    mymap['3']='H'; // Door to room 3

    for (int i = 0; i < 16; i++){
      for (int j = 0; j < 32; j++){
        cout << mymap[map[i][j]];
      }
      std::cout << '\n';
    }
    return;
  }
  
};

class Actor{
  // An actor can be any entity
  public:
    int health;
    char token;
    bool won = false;
    string current_level;
    int x;
    int y;
  
  Map drawActor(Map map){
    map.map[y][x]=token;
    return map;
  }

  bool interact(char block){
    switch (block)
    {
    case '0':
      return true;
    case '#':
      return false;
    case '$':
      won=true;
      return true;
    case '1':
      current_level="1";
      return false;
    case '2':
      current_level="2";
      return false;
    case '3':
      current_level="3";
      return false;
    default:
      return false;
    }
    return true;
  }

  Map move(Map map, char input){
    map.map[y][x]='0';
    
    auto shiftLeftWall = [&](){
      if(map.map[y][x-2] == '0'){
        for (int i = x; i > 0; --i){
          map.map[y][i] = map.map[y][i-1];
        }
        map.map[y][0]='#';
      }
    };

    auto shiftRightWall = [&](){
      if(map.map[y][x+2] == '0'){
        for (int i = x; i < 31; i++){
          map.map[y][i] = map.map[y][i+1];
        }
        map.map[y][31]='#';
      }
    };

    if(input == 'w' && interact(map.map[y-1][x])){
      shiftLeftWall();
      y--;
    }
    else if(input == 's' && interact(map.map[y+1][x])){
      shiftRightWall();
      y++;
    }
    else if(input == 'a' && interact(map.map[y][x-1])){
      shiftLeftWall();
      x--;
    }
    else if(input == 'd' && interact(map.map[y][x+1])){
      shiftRightWall();
      x++;
    }
    
    map.map[y][x]=token;
    return map;
  }
};

class Display{
  //To display various repeated and space consuming things.
  public:

  void displayStart(){
    gap();
    cout  << "/------------------------------\\\n"
          << "|                              |\n"
          << "|                              |\n"
          << "|                              |\n"
          << "|             _                |\n"
          << "|           _|W|_              |\n"
          << "|      Use |A|S|D| to move     |\n"
          << "|                              |\n"
          << "|      Find the $ and win!!    |\n"
          << "|                              |\n"
          << "|      Move to start           |\n"
          << "|                              |\n"
          << "|                              |\n"
          << "|                              |\n"
          << "|                              |\n"
          << "\\------------------------------/\n";
    gap();
  }

  void displayWin(){
    gap();
    cout  << "/------------------------------\\\n"
          << "|                              |\n"
          << "|                              |\n"
          << "|                              |\n"
          << "|                              |\n"
          << "|                              |\n"
          << "|                              |\n"
          << "|          You Win!!!!         |\n"
          << "|                              |\n"
          << "|  Press Any key to continue!  |\n"
          << "|                              |\n"
          << "|                              |\n"
          << "|                              |\n"
          << "|                              |\n"
          << "|                              |\n"
          << "\\------------------------------/\n";
    gap();
  }

  void gap(){
    cout << "\n\n\n\n\n";
    return;
  }
};

int main() {
  bool gameon = true;
  string level = "1";
  Actor player;
  player.x=2;
  player.y=2;
  player.token='@';
  player.current_level = "1";

  Map levelmap;
  levelmap.getMap(player.current_level);//Get Level 1

  Display display;
  display.displayStart();
  while(!player.won){
    levelmap = player.drawActor(levelmap);//Spawn Player
    levelmap = player.move(levelmap, getch());
    display.gap();
    levelmap.displayMap();
    display.gap();
    if (player.current_level != level){
      levelmap.getMap(player.current_level);
      level = player.current_level;
      levelmap = player.drawActor(levelmap);//Spawn Player
      levelmap.displayMap();
    }
  }
  display.displayWin();
  return 0;
}