//User input and Display
#include <iostream>
#include "conio.h" //For getch

//Read Files (Level Maps)
#include <fstream>

// to translate from file to display
#include <map>

//So we do not have to use std::
//Stands for Standard
using namespace std;

class Map{
  /* 
  The Map is the displayed field for the actor 
  to interact with
  */
  public:
    char map[16][32];
  
  void getMap(string level){
    /*
    getMap will access the file of the current level 
    and store it in the class.
    Vars:
      level = Will pass in the level the player is in
      i = x axis
      j = y axis
    */
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
    /*
    displayMap will display the class's map[16][32] in the 
    console as something that looks nice to the user.
    */
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
      cout << '\n';
    }
    return;
  }
  
};

class Actor{
  /* 
  An actor can be any entity including the player
  Vars:
    token: what the display will display the actor as
    won: used to ask if the player has won
    current_level: used to ask what level the player is in.
    x: the x coordinate of actor
    y: the y coordinate of actor
  */
  public:
    char token;
    bool won = false;
    string current_level;
    int x;
    int y;
  
  Map drawActor(Map map){
    /*
    drawActor will draw the actor onto the map
    Vars:
      map: is the Map classand holds the map that will be displayed later
    Return:
      The Map class with the map with the actor on it.
    */
    map.map[y][x]=token;
    return map;
  }

  bool interact(char block){
    /*
    interact will tell the call how current actor can interact with a block
    Vars:
      block: block is the char of the block that needs to 
             be interacted with by this actor.
    Return:
      A bool whether you can walk on this block
      true = can move
      false = can't move
    */
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
    return false;
  }

  Map move(Map map, char input){
    /*
    move will move the actor on the map according to user input
    Vars:
      map: The map class that will be moved on
      input: the char of the users input, always one char because of getch
    */
    map.map[y][x]='0';
    
    auto shiftLeftWall = [&](){
      //Shifts the Left wall when moving up or left
      if(map.map[y][x-2] == '0'){
        for (int i = x; i > 0; --i){
          map.map[y][i] = map.map[y][i-1];
        }
        map.map[y][0]='#';
      }
    };

    auto shiftRightWall = [&](){
      //Shifts the Right wall when moving down or right
      if(map.map[y][x+2] == '0'){
        for (int i = x; i < 31; i++){
          map.map[y][i] = map.map[y][i+1];
        }
        map.map[y][31]='#';
      }
    };
    //interact() will tell actor how it interacts with target block
    if(input == 'w' && interact(map.map[y-1][x])){
      shiftLeftWall();
      y--;//up
    }
    else if(input == 's' && interact(map.map[y+1][x])){
      shiftRightWall();
      y++;//down
    }
    else if(input == 'a' && interact(map.map[y][x-1])){
      shiftLeftWall();
      x--;//left
    }
    else if(input == 'd' && interact(map.map[y][x+1])){
      shiftRightWall();
      x++;//right
    }
    
    map = drawActor(map);
    return map;
  }
};

class Display{
  //To display various repeated and space consuming things.
  public:

  void gap(){
  // gap is five new lines to give the items some space
    cout << "\n\n\n\n\n";
    return;
  }

  void displayStart(){
    //Will display on boot
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
    //Will dispaly after the player interacts with the goal ($)
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
};

int main() {
  //Start on level 1
  string level = "1";
  
  //init the player
  Actor player;
  player.x=2;
  player.y=2;
  player.token='@';
  player.current_level = level;  //Start on level 1

  //init the map
  Map levelmap;
  levelmap.getMap(player.current_level);//Get Level 1

  //init the display
  Display display;
  display.displayStart();

  while(!player.won){
    levelmap = player.drawActor(levelmap);//Spawn Player
    //getch stops console to waith for user input and takes in one char at a time automatically.
    levelmap = player.move(levelmap, getch());
    display.gap();
    levelmap.displayMap();
    display.gap();
    if (player.current_level != level){//If the player has moved levels
      level = player.current_level;
      levelmap.getMap(level);
      levelmap = player.drawActor(levelmap);//Spawn Player
      levelmap.displayMap();
    }
  }
  display.displayWin();
  return 0;
}