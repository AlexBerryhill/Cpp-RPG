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
    
    cout << fileName << '\n'; //Debug
    
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
    mymap['1']='#'; // Wall
    mymap['@']='@'; // Player

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
    int x;
    int y;
  
  Map drawActor(Map map){
    map.map[y][x]=token;
    return map;
  }

  Map move(Map map, char input){
    map.map[y][x]='0';
    if(input == 'w' && map.map[y-1][x] != '1'){
      y--;
    }
    else if(input == 's' && map.map[y+1][x] != '1'){
      y++;
    }
    else if(input == 'a' && map.map[y][x-1] != '1'){
      x--;
    }
    else if(input == 'd' && map.map[y][x+1] != '1'){
      x++;
    }
    
    map.map[y][x]=token;
    return map;
  }
};

int main() {
  // char x;
  // x=getch();
  // cout << x << '\n';

  bool gameon = true;
  Actor player;
  player.x=2;
  player.y=2;
  player.token='@';

  Map levelmap;
  levelmap.getMap("1");//Get Level 1
  while(gameon){
    levelmap = player.drawActor(levelmap);//Spawn Player

    levelmap = player.move(levelmap, getch());
    levelmap.displayMap();
  }
  return 0;
}