#ifndef FUNCTION_H
#define FUNCTION_H

#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<vector>
#include<sstream>
#include<ctype.h>
#include<map>

using namespace std;

class VNode{
private:
	string RIN,zip_code,name,v_status;
	VNode* next;
public:
	VNode(VNode* n=NULL);
	VNode(string id,string s,string cN,string v_status,VNode* n=NULL);
	string getRIN();
	string getZipCode();
	string getVotingStatus();
	void register_vote();
	void print();
	void printRIN();
	friend class LinkedList; 
};

class LinkedList{ 
private:
	VNode* head;
	int zipcount;
	int size;
public:
	LinkedList();
	~LinkedList();
	void removeFront();
	bool empty();
	int getzipcount();
	bool NodeExists(string RIN);
	bool AddNode(VNode* voter);
	void RemoveNode(string RIN);
	void RemoveNode2(string zip);
	void FindNode(string RIN);
	bool ExistZipNode(string zip, string RIN);
	string PrintNodes();
	bool NodeVoted(string RIN);
	bool CheckVoted(string searchName);
	int PrintZipCode(string searchName);
	string ZipCode(string searchName);
	friend class HashTable;
};

class HashTable{ 
private:
	LinkedList** VoterList;
	map<string, int> indices;
	int size;					
	int capacity;
public:
	HashTable(int capacity);
	~HashTable();
	long hashCode(const string key);
	void insert(VNode* voter);
	void insert2(VNode* voter);
	void find(string htlname);
	void voted_zip_code(string zip);
	void print_all();
	string retrieve_zip_code(string RIN);
	bool update_vote(string RIN);
	bool exist_zip_node(string zip,string RIN);
	bool check_voted(string RIN);
	void remove(string RIN);
	void remove2(string RIN);

};

#endif