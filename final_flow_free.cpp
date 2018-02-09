#include <strings.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <queue>
#include <ctime>
#include <stdlib.h>

using namespace std;

vector<char> domain;
int length;
int height;
int attempt;

struct node{
  int wid;
  int hei;
  vector<int> own_domain;
  int remained;
  bool source;
  bool unassign;
};
vector<node*> unassigned;
vector<node*> maze_vec;

int close_to_edge(node* n){
  int wid = n->wid;
  int hei = n->hei;
  return wid*wid + (length-wid)*(length-wid) + hei*hei + (height - hei)*(height-hei);
}

bool has_adjacent(node* n, vector<string> maze){

  int curry = n->hei;
  int currx = n->wid;

  if (curry != 0 && maze[curry-1][currx] != '_')
      return true;

  if (curry != height-1 && maze[curry+1][currx] != '_')
      return true;

  if (currx != 0 && maze[curry][currx-1] != '_')
      return true;

  if (currx != length-1 && maze[curry][currx+1] != '_')
      return true;

  return false;
}

int find_min_domain(vector<string> maze){
  int min_index = 0;
  /*for(int i = 0; i < unassigned.size();i++){
    if (has_adjacent(unassigned[i], maze)){
      min_index = i;
      break;
    }
  }*/

//   && has_adjacent(unassigned[i], maze

  for(int i = 0; i < unassigned.size();i++){
    if(unassigned[i]->remained < unassigned[min_index]->remained ){
      min_index = i;
    }
    if(unassigned[i]->remained == unassigned[min_index]->remained ){
      if (close_to_edge(unassigned[i]) > close_to_edge(unassigned[min_index])){
        min_index = i;
      }
    }
  }
  return min_index;
}

bool checkconstrain(vector<string>& maze,int x,int y,char curr){
  if(x < 0 || x >= length || y < 0 || y >= height || curr == '_'){
    return true;
  }
  int consist = 0;
  int diff = 0;
  if(x != 0) {
    char left = maze[y][x-1];
    if(left != '_'){
      if(left == curr) consist++;
      else diff++;
    }
  }
  else diff++;
  if(x != length-1){
    char right = maze[y][x+1];
    if(right != '_'){
      if(right == curr) consist++;
      else diff++;
    }
  }
  else diff++;
  if(y != 0){
    char up = maze[y-1][x];
    if(up != '_'){
      if(up == curr) consist++;
      else diff++;
    }
  }
  else diff++;
  if(y != height-1) {
    char bottom = maze[y+1][x];
    if(bottom != '_'){
      if(bottom == curr) consist++;
      else diff++;
    }
  }
  else diff++;
  if(maze_vec[y*length+x]->source){
    if(consist >= 2 || diff == 4) return false;
    else return true;
  }
  else {
    if(diff >= 3 || consist >= 3) return false;
    else return true;
  }
}

void update_domain(vector<string>& maze,node* node){
  node->remained = domain.size();
  for(int i = 0; i< domain.size();i++){
    bool up = checkconstrain(maze,node->wid,node->hei,domain[i]);
    if(up == false) {
      node->own_domain[i] = 0;
      node->remained--;
    }
    else {
      node->own_domain[i] = 1;
    }
  }
}


bool dumb_recurse_function(vector<string>& maze){
  attempt++;
  if(unassigned.empty()) return true;
  int randomindex = rand() % unassigned.size();
  node* randomnode = unassigned[randomindex];
  unassigned[randomindex] = unassigned[unassigned.size()-1];
  unassigned.erase(unassigned.begin()+randomindex);
  int currx = randomnode->wid;
  int curry = randomnode->hei;
  bool bo[domain.size()];
  for(int i = 0; i < domain.size();i++){
    bo[i] = false;
  }
  int times = 0;
  while(times < domain.size()){
    int index =  rand() % domain.size();
    while(bo[index]){
      index =  rand() % domain.size();
    }
    bo[index] = true;
    times++;
    char currrandom = domain[index];
    if(checkconstrain(maze,currx,curry,currrandom)){
      bool checkleft;
      bool checkright;
      bool checkup;
      bool checkdown;
      maze[curry][currx] = currrandom;
      if(currx != 0) {
        checkleft = checkconstrain(maze,currx-1,curry,maze[curry][currx-1]);
      }
      else checkleft = true;
      if(currx != length-1) {
        checkright = checkconstrain(maze,currx+1,curry,maze[curry][currx+1]);
      }
      else checkright = true;
      if(curry != 0) {
        checkup = checkconstrain(maze,currx,curry-1,maze[curry-1][currx]);
      }
      else checkup = true;
      if(curry != height-1) {
        checkdown = checkconstrain(maze,currx,curry+1,maze[curry+1][currx]);
      }
      else checkdown = true;
      if(checkleft && checkright && checkdown && checkup){
        bool result = dumb_recurse_function(maze);
        if(result) return true;
      }
      maze[curry][currx] = '_';
    }
  }
  unassigned.push_back(randomnode);
  return false;
}

bool smart_recurse_function(vector<string>& maze){
    attempt++;
    if(unassigned.empty()) return true;
    int min_idx = find_min_domain(maze);
    node* currrandom = unassigned[min_idx];
    unassigned.erase(unassigned.begin()+min_idx);
    currrandom -> unassign = false;
    int currx = currrandom->wid;
    int curry = currrandom->hei;
    for(int q = 0; q < domain.size(); q++){
      if(currrandom->own_domain[q] == 1 && checkconstrain(maze,currx,curry,domain[q])){
      bool checkleft;
      bool checkright;
      bool checkup;
      bool checkdown;
      maze[curry][currx] = domain[q];
      bool check[4];
      if(currx != 0) {
        checkleft = checkconstrain(maze,currx-1,curry,maze[curry][currx-1]);
        check[0] = true;
      }
      else {
        checkleft = true;
        check[0] = false;
      }
      if(currx != length-1) {
        checkright = checkconstrain(maze,currx+1,curry,maze[curry][currx+1]);
        check[1] = true;
      }
      else {
        checkright = true;
        check[1] = false;
      }
      if(curry != 0) {
        checkup = checkconstrain(maze,currx,curry-1,maze[curry-1][currx]);
        check[2] = true;
      }
      else {
        checkup = true;
        check[2] = false;
      }
      if(curry != height-1) {
        checkdown = checkconstrain(maze,currx,curry+1,maze[curry+1][currx]);
        check[3] = true;
      }
      else {
        checkdown = true;
        check[3] = false;
      }
      if(checkleft && checkright && checkdown && checkup){
        if(check[0]) {
          if(maze_vec[curry*length + currx-1]->unassign){
            update_domain(maze,maze_vec[curry*length + currx-1]);
            //cout << 0 << endl;
          }
        }
        if(check[1]) {
          if(maze_vec[curry*length + currx+1]->unassign){
           update_domain(maze,maze_vec[curry*length + currx+1]);
           //cout << 1 << endl;
          }
        }
        if(check[2]) {
          if(maze_vec[(curry-1)*length + currx]->unassign){
           update_domain(maze,maze_vec[(curry-1)*length + currx]);
           //cout << 2 << endl;
          }
        }
        if(check[3]) {
          if(maze_vec[(curry+1)*length + currx]->unassign){
           update_domain(maze,maze_vec[(curry+1)*length + currx]);
           //cout << 3 << endl;
          }
        }
        //cout << "!!!" << endl;
        bool result = smart_recurse_function(maze);
        if(result) return true;
        else {
            maze[curry][currx] = '_';
            if(check[0]) {
              if(maze_vec[curry*length + currx-1]->unassign){
                update_domain(maze,maze_vec[curry*length + currx-1]);
              }
            }
            if(check[1]) {
              if(maze_vec[curry*length + currx+1]->unassign)
               update_domain(maze,maze_vec[curry*length + currx+1]);
            }
            if(check[2]) {
              if(maze_vec[(curry-1)*length + currx]->unassign)
               update_domain(maze,maze_vec[(curry-1)*length + currx]);
            }
            if(check[3]) {
              if(maze_vec[(curry+1)*length + currx]->unassign)
               update_domain(maze,maze_vec[(curry+1)*length + currx]);
            }
        }
      }
    }
    maze[curry][currx] = '_';
  }
  currrandom -> unassign = true;
  //update_domain(maze,currrandom);
  unassigned.push_back(currrandom);
  //cout << unassigned.size() << endl;
  return false;
}


int main(){
  int tStart=clock();
  ifstream inFile;
  inFile.open("maze.txt");
  vector<string> maze;
  string temp = "";
  while(inFile.good()){
    getline(inFile,temp);
    if(temp.compare("") == 0) break;
    for(int i = 0; i < temp.length(); i++){
      node* tempnode = new node();
      tempnode->wid = i;
      tempnode->hei = maze.size();
      if(isalpha(temp[i])){
        tempnode->source = true;
        tempnode->unassign = false;
        bool flag = true;
        for(int j = 0; j < domain.size(); j++){
          if(domain[j] == temp[i]) {
            flag = false;
            break;
          }
        }
        if(flag)
          domain.push_back(temp[i]);
      }
      else {
        tempnode->source = false;
        tempnode->unassign = true;
        unassigned.push_back(tempnode);
      }
      maze_vec.push_back(tempnode);
    }
    maze.push_back(temp);
  }
  length = maze[0].length();
  height = maze.size();
  for(int m = 0; m < unassigned.size();m++){
    unassigned[m]->own_domain.resize(domain.size());
    for(int n = 0; n < domain.size();n++){
      unassigned[m]->own_domain[n] = 1;
    }
    update_domain(maze,unassigned[m]);
  }
  //cout << maze_vec[2]->remained << endl;
  ofstream myfile;
  myfile.open ("10*10_1.txt");
  smart_recurse_function(maze);
  for(int i = 0; i < height;i++){
    myfile << maze[i] << endl;
  }
  myfile << "total attemp node: " << attempt << endl;
  myfile << "total time: " << (double)(clock() - tStart)/CLOCKS_PER_SEC;
}
