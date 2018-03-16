/*
Burak Mete
150140131
23.11.17
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cmath>
#include <ctime>
#include <list>

#define M 131071

using namespace std;

class BookCharacter {
	int pageNumber;
	int lineNumber;
	int indexNumber;
	char character;
	unsigned long key;
public:
	BookCharacter(string p="", string l="", string in="", string x=""); // default parameters are added for initialization
	char getChar()const { return character; };
	unsigned long getKey()const { return key; }; 
	int getPage()const{return pageNumber;};
	int getLine()const{return lineNumber;};
	int getIndex()const{return indexNumber;};
	void setChar(char char1){character = char1;}; // for updating character
	void setKey(unsigned long k){key=k;};	// the key is being generated in the constructor, saved me time
};

class Books {
	vector<BookCharacter> books;
public:
	void add(BookCharacter& bObj);
	Books() {;};
	char getObjValue(int i)const { return books[i].getChar(); };
	unsigned long getObjKey(int i)const { return books[i].getKey(); }; 
	int getObjPage(int i)const{return books[i].getPage();};
	int getObjLine(int i) const{return books[i].getLine();};
	int getObjIndex(int i)const{return books[i].getIndex();};
	BookCharacter getObject(int i){ return books[i];};
	void setCharacter(int i , char c){books[i].setChar(c);};
};

class Dictionary {
	BookCharacter* hashTable;
	unsigned long collisions;
	unsigned long collisions1k;
	unsigned long collisions10k;
	unsigned long collisions100k;
public:
	Dictionary();
	~Dictionary(){delete[] hashTable;}; // delete the allocated memory space for hash table
	void blockInsertion(const char* input);
	void blockLookup(const char* input);
	double hashFunction(unsigned long key);
	void probing(unsigned long hashKey , const BookCharacter& obj);
	char lookupProbing(unsigned long hashKey , unsigned long key);// updating character values of the object
};

class List{
	list<BookCharacter> listObject;
	public:
		List(){;};
		void addObject(const BookCharacter& obj);
		void listInsertion(const char* input);
		void listLookup(const char* input);
}; 

void List::addObject(const BookCharacter& obj){
	listObject.push_back(obj);
}

void List::listLookup(const char* input){
	ifstream ii;
	ii.open(input);
	string p, l, in, x; // page-line-index-char	
	Books b2;
	
	while(ii.good()){	
		getline(ii, p,'\t');
		if(p.compare("") == 0) break;
		getline(ii, l,'\t');
		getline(ii, in,'\n');
		BookCharacter bObj(p,l,in,"");
		b2.add(bObj);
	}
	ii.close();
	
	clock_t beginClock = clock();
	for(int i = 0 ; i < M-1  ; ++i){
		for( list<BookCharacter>::iterator it = listObject.begin() ;  it != listObject.end() ; ++it){
			if(b2.getObjKey(i) == (*it).getKey()){	// if correct key is found, update char value for container and break the iterationloop
				b2.setCharacter(i,(*it).getChar());	
				break;
			}
		}
	}
	clock_t endClock = clock();
	const char * output1 = "ds-set-output-list.txt";
	ofstream oo;
	oo.open(output1);
	for(int i = 0 ; i < M-1; ++i){
		oo << b2.getObjPage(i) << "\t" << b2.getObjLine(i) << "\t" << b2.getObjIndex(i) << "\t" << b2.getObjValue(i) << "\n";
	}
	oo.close();
	
	
	cout << "Lookup finished after " << double(endClock-beginClock) / CLOCKS_PER_SEC << " seconds." << endl;
}

void List::listInsertion(const char* input){
	ifstream ii;
	ii.open(input);
	string p, l, in, x; // page-line-index-char
	Books b1;

	while (ii.good()) {
		getline(ii, p, '\t');
		if (p.compare("") == 0) break;
		getline(ii, l, '\t');
		getline(ii, in, '\t');
		getline(ii, x, '\n');

		BookCharacter bObj(p, l, in, x);
		b1.add(bObj);
	}
	ii.close();
	clock_t beginClock = clock();
	for(int i = 0 ; i < M - 1 ; ++i){
		addObject(b1.getObject(i));
	}
	clock_t endClock = clock();
	cout << "Insertion finished after " << double(endClock-beginClock) / CLOCKS_PER_SEC << " seconds." << endl;
}

char Dictionary::lookupProbing(unsigned long hashKey , unsigned long key){
	int i = 0 ;
	hashKey = (hashKey + (i*i * 3) + 7 * i) % M;
	while(hashTable[hashKey].getKey() != key){
		++i;
		hashKey = (hashKey + (i*i * 3) + 7 * i) % M;	// collision occured
	}
	return hashTable[hashKey].getChar();
}

Dictionary::Dictionary() {
	collisions = 0;
	hashTable = new BookCharacter[M];
}

void Dictionary::blockLookup(const char * input){
	ifstream ii;
	ii.open(input);
	string p, l, in, x; // page-line-index-char	
	Books b2;
	
	while(ii.good()){	
		getline(ii, p,'\t');
		if(p.compare("") == 0) break;
		getline(ii, l,'\t');
		getline(ii, in,'\n');
		BookCharacter bObj(p,l,in,"");
		b2.add(bObj);
	}
	ii.close();
	
	clock_t beginClock = clock();
	for(int i = 0 ; i < M-1; ++i){
		unsigned long key = b2.getObjKey(i);
		unsigned long hashK = hashFunction(key);
		b2.setCharacter(i,lookupProbing(hashK,key));// updating character values of the object
	}
	clock_t endClock = clock();
	
	const char * output1 = "ds-set-output-dict.txt";
	ofstream oo;
	oo.open(output1);
	for(int i = 0 ; i < M-1; ++i){
		oo << b2.getObjPage(i) << "\t" << b2.getObjLine(i) << "\t" << b2.getObjIndex(i) << "\t" << b2.getObjValue(i) << "\n";
	}
	oo.close();
	cout << "Lookup finished after " << double(endClock-beginClock) / CLOCKS_PER_SEC << " seconds." << endl;
	cout << endl;
}

void Dictionary::blockInsertion(const char* input) {
	ifstream ii;
	ii.open(input);
	string p, l, in, x; // page-line-index-char
	Books b1;
	
	while (ii.good()) {
		getline(ii, p, '\t');
		if (p.compare("") == 0) break;
		getline(ii, l, '\t');
		getline(ii, in, '\t');
		getline(ii, x, '\n');

		BookCharacter bObj(p, l, in, x);
		b1.add(bObj);
	}
	ii.close();
	clock_t beginClock = clock();
	for (int i = 0; i < M-1; ++i) {
		double hashKey = hashFunction(b1.getObjKey(i)); 
		probing(hashKey, b1.getObject(i)); 
		if(i == 1000) collisions1k = collisions;
		else if(i== 10000) collisions10k = collisions;
		else if(i == 100000)collisions100k = collisions;
		
	}
	clock_t endClock = clock();
	cout << "Insertion finished after " << double(endClock-beginClock) / CLOCKS_PER_SEC << " seconds." << endl;
	cout << "Average number of collisions (first 1,000)\t | " << double(collisions1k)/1000 << endl;
	cout << "Average number of collisions (first 10,000)\t | " << double(collisions10k)/10000 << endl;
	cout << "Average number of collisions (first 100,000)\t | " << double(collisions100k)/100000 << endl;
	cout << "Average number of collisions (overall)\t\t | " << double(collisions)/M<< endl;
	cout << endl;
}

double Dictionary::hashFunction(unsigned long k) {
	double A = (double(sqrt(5)) - 1) / 2;
	double x;
	A*=k;
	double r = modf(A, &x);
	r *= M;
	return r;
}

void Dictionary::probing(unsigned long hashKey , const BookCharacter& obj ) {
	int i = 0;
	hashKey = (hashKey + (i*i * 3) + 7 * i) % M;
	while (hashTable[hashKey].getKey() != 0) {
		++i;
		hashKey = (hashKey + (i*i * 3) + 7 * i) % M;	// collision occured
	}
	collisions += i;
	hashTable[hashKey] = obj;
}

BookCharacter::BookCharacter(string p, string l, string in, string x) {
	key =0;
	if(p.compare("") != 0 && l.compare("") != 0 && in.compare("") != 0){	
		istringstream(p) >> pageNumber;
		istringstream(l) >> lineNumber;
		istringstream(in) >> indexNumber; 
		if (l.length() == 1) {
			l = "0" + l;			// add 0 to if line is just one character integer, to get 7 digit key
		}
		if (in.length() == 1) {
			in = "0" + in;		  // add 0 to if index is just one character integer, to get 7 digit key
		}
		istringstream(p + l + in) >> key;	// generate key accordingly
	}
	if (x.compare("") == 0) {
		character = 0;
	}
	else
		character = x[0];
}

void Books::add(BookCharacter& bObj) {
	books.push_back(bObj);
}

int main() {
	const char * input1 = "ds-set-input.txt";
	const char * input2 = "ds-set-lookup.txt";

	Dictionary dict;
	
	cout << "DICTIONARY"<<endl;
	dict.blockInsertion(input1);
	dict.blockLookup(input2);
	
	cout << "LIST" << endl;
	
	List l1;
	l1.listInsertion(input1);
	l1.listLookup(input2);

	return 0;
}
