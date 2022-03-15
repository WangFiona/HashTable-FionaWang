#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

struct Student {
  char fname[15];
  char lname[15];
  int id;
  float gpa;
};

class HNode {
  Student* data;
  HNode* next;
  
public:
  HNode(Student* newData){
    data = newData;
    next = NULL;
  }

  void setNext(HNode* newNext){
    next = newNext;
  }

  HNode* getNext(){
    return next;
  }

  void setData(Student* newStudent){
    data = newStudent;
  }

  Student* getStudent(){
    return data;
  }
};

void add(HNode** &hTable, int size);
void rehash(HNode** &hTable, int size);
void search(HNode** hTable, int size);
void print(HNode** hTable, int size);
void generate(int number, int &idRand, int size, HNode** &hTable);
int hashFunction(int data, int size);

int main(){
  int size = 10;
  int id = 0;
  HNode** hTable = new HNode*[size];
  //Student* student = new Student();

  for(int i=0; i<size; i++){
    hTable[i] = NULL;
  }

  bool running = true;
  bool found=false;
  char command[10];
  char ADD[] = "ADD";
  char PRINT[] = "PRINT";
  char DELETE[] = "DELETE";
  char GENERATE[] = "GENERATE";
  char QUIT[] = "QUIT";
  char AVERAGE[]="AVERAGE";
  bool ifRehash = false;
  
  //Ask the user for a command
  cout << "Enter a command (ADD, PRINT, DELETE, AVERAGE, or QUIT):" << endl;
  int x=0;
  while (running == true) {
    cin.get(command,10);
    cin.get();
    //Allow both upper and lower case
    for(int i=0; i < strlen(command); i++) {
      command[i] = toupper(command[i]);
    }
    cout << command << endl;

    if(x>0){
      cout << "test" << endl;
      return 0;
    }
    if(strcmp(command, ADD) == false){
      add(hTable, size);
      rehash(hTable, size);
      cout<< "Enter a command (ADD, PRINT, DELETE, AVERAGE, or QUIT):" << endl;
    }
    else if(strcmp(command, PRINT) == false){
      print(hTable, size);
      cout<< "Enter a command (ADD, PRINT, DELETE, AVERAGE, or QUIT):" << endl;

    }
    else if(strcmp(command, DELETE) == false){
      cout<< "Enter a command (ADD, PRINT, DELETE, AVERAGE, or QUIT):" << endl;
    }
    else if(strcmp(command, GENERATE) == false){
      generate(0, id, size, hTable);
      print(hTable, size);
      rehash(hTable, size);
      cout<< "Enter a command (ADD, PRINT, DELETE, AVERAGE, or QUIT):" << endl;
    }
    else if(strcmp(command, QUIT) == false){
      running = false;
    }
    else{
      cout << "Please enter a valid command:" << endl;
    }
    x++;
  }
  return 0;
}

void rehash(HNode** &hTable, int size){
  bool needRehash=false;
  for(int i=0; i<size; i++){
    int x=0;
    while(hTable[i] != NULL){
      hTable[i] = hTable[i]->getNext();
      x++;
    }
    if(x>3){
      needRehash = true;
      cout << "need rehash" << endl;
    }
  }

  if(needRehash){
    HNode** newHTable = new HNode*[size*2];
    for(int i=0; i < size; i++){
      while(hTable[i] != NULL){
	newHTable[hashFunction(hTable[i]->getStudent()->id, size*2)]
	  = new HNode(hTable[i]->getStudent());
	
	hTable[i] = hTable[i]->getNext();
      }
    }
    for(int i=0; i<size; i++){
      hTable[i]->~HNode();
    }
    size = size*2;
    //HNode** hTable = new HNode*[size];
    hTable = newHTable;
    delete[] newHTable;
    cout << "Rehashed!" << endl;
  }
}

void add(HNode** &hTable, int size){
  Student* student = new Student();

  cout << "Enter a first name:" << endl;
  cin.get(student->fname, 15);
  cin.get();

  cout << "Enter a last name:" << endl;
  cin.get(student->lname, 15);
  cin.get();

  cout << "Enter a unique ID:" << endl;
  cin >> student->id;

  cout << "Enter a GPA:" << endl;
  cin >> student->gpa;

  double hashed = hashFunction(student->id, size);
  cout << hashed << endl;
  for(int i=0; i<size; i++){
    if(i == hashed && hTable[i] == NULL){
      hTable[i] = new HNode(student);
    }
    else if(hTable[i] != NULL){
      while(hTable[i]->getNext() != NULL){	
        hTable[i] = hTable[i]->getNext();
      }
      hTable[i]->setNext(new HNode(student));
    }
  }
  hTable[hashFunction(student->id,size)] = new HNode(student);
  cout << "Added!" << endl;
}

void search(HNode** hTable, int size){
  cout << "Enter the ID of the student you would like to print:" << endl;
  int idPrint;
  cin >> idPrint;
  idPrint = hashFunction(idPrint, size);
  //cout << idPrint << endl;

  for(int i=0; i<size; i++){
    if(i == idPrint){
      //cout << i << endl;
      cout << "Name: " << hTable[i]->getStudent()->fname;
      cout << " " << hTable[i]->getStudent()->lname << endl;
    }
  }
}

void print(HNode** hTable, int size){
  for(int i=0; i<size; i++){
    if(hTable[i] != NULL){
      while(hTable[i] != NULL){
	cout << hTable[i]->getStudent()->fname << " "
	     << hTable[i]->getStudent()->lname
	     << ", GPA: " << hTable[i]->getStudent()->gpa << ", ID: "
	     << hTable[i]->getStudent()->id << endl;
	hTable[i] = hTable[i]->getNext();
      }
    }
  }
}

void generate(int number, int &idRand, int size, HNode** &hTable){
  fstream FNfile;
  fstream LNfile;
  FNfile.open("fname.txt");
  LNfile.open("lname.txt");
  
  cout << "How many students do you want to generate?" << endl;
  cin >> number;
  
  for(int i=0; i<number; i++){
    Student* randStudent = new Student();
    char* firstN = new char[50];
    char* lastN = new char[50];
    FNfile >> randStudent->fname;
    LNfile >> randStudent->lname;
    idRand++;
    randStudent->id = idRand;
    double gpaRand = (double)(rand()%400-0)/100;
    randStudent->gpa = gpaRand;
    if(hTable[hashFunction(randStudent->id,size)] == NULL){
      hTable[hashFunction(randStudent->id,size)] = new HNode(randStudent);
    }
    else if(hTable[hashFunction(randStudent->id,size)] != NULL){
      while(hTable[hashFunction(randStudent->id,size)]->getNext() != NULL){
        hTable[hashFunction(randStudent->id,size)]
	  = hTable[hashFunction(randStudent->id,size)]->getNext();
      }
      hTable[hashFunction(randStudent->id,size)]->setNext(new HNode(randStudent));
    }
    //hTable[hashFunction(randStudent->id,size)] = new HNode(randStudent);
    //cout << "Name: " << randStudent->fname << " " <<  randStudent->lname << ", ID:"
    //<< randStudent->id << ", GPA:" << randStudent->gpa << endl;
    //cout << (double)rand()/(RAND_MAX + 1)+(rand()%4) << endl;
    //srand (time(NULL));
    //double seed = ((double)rand()) / ((double)RAND_MAX) * 4;
    //printf("%.2lf\n", seed);
    //cout << (float(rand())/float((RAND_MAX)) * 0.4) << endl;
  }
}

int hashFunction(int data, int size){
  return data % size;
}
