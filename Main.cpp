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
void generate(int number);
int hashFunction(Student* data, int size);

int main(){
  int size = 10;
  HNode** hTable = new HNode*[size];
  //Student* student = new Student();

  for(int i=0; i<size; i++){
    hTable[i] = NULL;
  }
  
  add(hTable, 5);
  generate(5);
  
  return 0;
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

  cout << hashFunction(student, 2) << endl;
  hTable[hashFunction(student,size)] = new HNode(student);
}

void generate(int number){
  fstream FNfile;
  fstream LNfile;
  FNfile.open("fname.txt");
  LNfile.open("lname.txt");

  cout << "Random names:" << endl;
  for(int i=0; i<number; i++){
    char* firstN = new char[50];
    char* lastN = new char[50];
    FNfile >> firstN;
    LNfile >> lastN;
    cout << firstN << " " <<  lastN << endl;
  }
}

int hashFunction(Student* data, int size){
  return data->id % size;
}
