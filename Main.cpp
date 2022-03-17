#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

struct Student {
  char fname[15];
  char lname[15];
  int id;
  float gpa;
public:
  Student(){
    
  }
  Student(char* newFName, char* newLName, int newID, float newGPA){
    strcpy(fname, newFName);
    strcpy(lname, newLName);
    id = newID;
    gpa = newGPA;
  }
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

void rehash(HNode** &hTable, int size);
void print(HNode* head);
void generate(int number, int &random, int &idRand, int size, HNode** &hTable);
int hashFunction(int data, int size);
void addStudent(int size, Student* student, HNode** head, HNode** &hTable);
void deleteStudent(int size, int id, HNode** &hTable);

int main(){
  int size = 10;
  int id = 0;
  int random = 0;
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
  bool ifRehash = false;
  
  //Ask the user for a command
  cout << "Enter a command (ADD, PRINT, DELETE, GENERATE, or QUIT):" << endl;
  while (running == true) {
    cin >> command;
    //cin.get(command,10);
    //cin.get();

    //Allow both upper and lower case
    for(int i=0; i < strlen(command); i++) {
      command[i] = toupper(command[i]);
    }
    
    if(strcmp(command, ADD) == false){
      char* newF = new char[15];
      cout << "Enter a first name:" << endl;
      cin >> newF;

      char* newL = new char[15];
      cout << "Enter a last name:" << endl;
      cin >> newL;

      int nID = 0;
      cout << "Enter a unique ID:" << endl;
      cin >> nID;

      float nGPA = 0;
      cout << "Enter a GPA:" << endl;
      cin >> nGPA;

      Student* student = new Student(newF, newL, nID, nGPA);
      addStudent(size, student, &(hTable[hashFunction(student->id, size)]), hTable);
      
      rehash(hTable, size);
      cout << "Enter a command (ADD, PRINT, DELETE, GENERATE, or QUIT):" << endl;
    }
    else if(strcmp(command, PRINT) == false){
      for(int i=0; i<size; i++){
	print(hTable[i]);
      }
      cout << "Enter a command (ADD, PRINT, DELETE, GENERATE, or QUIT):" << endl;

    }
    else if(strcmp(command, DELETE) == false){
      cout << "Enter the ID you want to delete:" << endl;
      int target = 0;
      cin >> target;
      deleteStudent(size, target, hTable);
      cout << "Enter a command (ADD, PRINT, DELETE, GENERATE, or QUIT):" << endl;
    }
    else if(strcmp(command, GENERATE) == false){
      generate(0,random, id, size, hTable);
      //rehash(hTable, size);
      cout << "Enter a command (ADD, PRINT, DELETE, GENERATE, or QUIT):" << endl;
    }
    else if(strcmp(command, QUIT) == false){
      running = false;
    }
    else{
      cout << "Please enter a valid command:" << endl;
    }
  }
  return 0;
}

void rehash(HNode** &hTable, int size){
  /*bool needRehash=false;
  for(int i=0; i<size; i++){
    HNode* y = hTable[i];
    if(y != NULL){
      int x=0;
      HNode** current = hTable;
      while(current[i]->getNext() != NULL){
	x++;
	current[i] = current[i]->getNext();
      }
      if(x>3){
	needRehash = true;
	//cout << "need rehash" << endl;
      }
    }
  }
  for(int i=0; i<size; i++){
    print(hTable[i]);
  }
    
  if(){*/
  //cout << "Rehashing" << endl;
  int count=0;
      HNode** newHTable = new HNode*[size*2];
      for(int i=0; i<size*2; i++){
	newHTable[i] = NULL;
      }
      for(int i=0; i < size; i++){
	//cout << "in" << endl;
	//cout << hTable[i]->getStudent()->fname << endl;
	HNode* temp = hTable[i];
	//cout << temp->getStudent()->fname << endl;
	while(temp != NULL){
	  addStudent(size*2, temp->getStudent(),
		     &(newHTable[hashFunction(temp->getStudent()->id, size*2)]), hTable);
	  temp = temp->getNext();
	  count++;
	  //cout << "added" << endl;
	}
      }
      for(int i=0; i<size; i++){
	hTable[i]->~HNode();
      }
      size = size*2;
      hTable = new HNode*[size];
      hTable = newHTable;
      delete[] newHTable;
      cout << "Rehashed!" << endl;
      cout << count << endl;
      //cout << hTable[1]->getStudent()->fname << endl;
}

void print(HNode* head){
  if(head){
    cout << head->getStudent()->fname << " " << head->getStudent()->lname
	 << ", ID: " << head->getStudent()->id
	 << ", GPA: " << head->getStudent()->gpa << endl;
    print(head->getNext());
  }
}

void generate(int number, int &random,  int &idRand, int size, HNode** &hTable){
  srand(time(NULL));
  cout << "How many students do you want to generate?" << endl;
  cin >> number;
  int	lineNum	= 0;

  for(int i=0; i<number; i++){
    fstream FNfile;
    fstream LNfile;
    FNfile.open("fname.txt");
    LNfile.open("lname.txt");

    lineNum = 0;
    int oldR = random;
    while(oldR == random){
      random = rand() % 1001; 
    }
    Student* randStudent = new Student();
    char* firstN = new char[50];
    char* lastN = new char[50];
    while(FNfile >> randStudent->fname && LNfile >> randStudent->lname){
      if(lineNum == random) break;
      lineNum++;
    }
    idRand++;
    randStudent->id = idRand;
    double gpaRand = (double)(rand()%400-0)/100;
    randStudent->gpa = gpaRand;
    addStudent(size, randStudent, &(hTable[hashFunction(randStudent->id, size)]), hTable); 
  }
  cout << "Generated!" << endl;
}

int hashFunction(int data, int size){
  return data % size;
}

void addStudent(int size, Student* student, HNode** head, HNode** &hTable){
  HNode* temp = NULL;
  temp = new HNode(student);
  temp->setNext(*head);
  *head = temp;
  if((*head)->getNext() != NULL && (*head)->getNext()->getNext() != NULL
     && (*head)->getNext()->getNext()->getNext() != NULL){
    cout << "about to rehash" << endl;
    //rehash(hTable, size);
  }
}

void deleteStudent(int size, int id, HNode** &hTable){
  for(int i=0; i<size; i++){
    if(hTable[i]){
      HNode* prev = NULL;
      HNode* head = hTable[i];
      HNode* first = hTable[i];
      if(first->getStudent()->id == id){
	HNode* temp = first;
	first = first->getNext();
	temp->~HNode();
	hTable[i] = first;
      }
      else{
	while(head->getNext() != NULL && head->getStudent()->id != id){
	  prev = head;
	  head = head->getNext();
	}
	if(id == head->getStudent()->id){
	  prev->setNext(head->getNext());
	  head->~HNode();
	}
      }
    }
  }
  cout << "Deleted!" << endl;
}
