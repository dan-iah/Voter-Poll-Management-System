#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<vector>
#include<sstream>
#include<ctype.h>
#include<map>

#include"header.h"

using namespace std;

//CITATIONS:
//Data Structures & Algorithms by Michael T. GoodRich, Roberto Tamassia and David Mount

VNode::VNode(VNode* n){next=n;}
VNode::VNode(string id,string s,string cN,string v,VNode* n){
	RIN=id;
	zip_code=s;
	name=cN;
	next=n;
	v_status=v;
}
string VNode::getRIN() {return RIN;}
string VNode::getZipCode(){return zip_code; }
string VNode::getVotingStatus(){return v_status; }

void VNode::register_vote(){v_status="Y";}

void VNode::print(){cout<<RIN<<"     "<<name<<"     "<<zip_code<<"    "<<v_status<<endl;}
void VNode::printRIN(){cout<<RIN<<endl;}

LinkedList::LinkedList(){ //initialising linked list (linked list contructor) with head node
	head=new VNode;
	size=0;
}
LinkedList::~LinkedList(){
	while(!empty()){
		removeFront();
	}
	delete head;
}
void LinkedList::removeFront(){ //for linked list class destructor, this function deletes the linked list nodes
	VNode* oldhead=head; 
	head=oldhead->next; //removing the pointers of first node in list to delete it
	delete oldhead;
}
bool LinkedList::empty(){
	return (head==NULL);
}
int LinkedList::getzipcount(){ //returning num of voters per zip
	return zipcount;
}
bool LinkedList::NodeExists(string RIN){ //used in hash table addnode function below to check if voter already exists
	VNode* cursor;
	cursor=new VNode;
	cursor=head;
	bool exists=false;
	while(cursor!=NULL){
		if((*cursor).getRIN()==RIN){ 
			exists=true;
			break;
		}
		cursor=cursor->next; //loop through linked list till end node (NULL)
	}
	return exists;
}
bool LinkedList::NodeVoted(string RIN){ //function used to register vote (updates to Y) used with update_vote hash table function below
	VNode* cursor;
	cursor=new VNode;
	cursor=head;
	bool NV_exists=false;
	while(cursor!=NULL){
		if((*cursor).getRIN()==RIN){ //if RIN matches
			(*cursor).register_vote(); //register that voter v_status->Y
			NV_exists=true;
			break;
		}
		cursor=cursor->next;
	}
	return NV_exists;
}

string LinkedList::ZipCode(string RIN){ //function used to retreive zip code for retrieve_zip_code funtion below, used during r command in main file to add voter to zip hash
	VNode* cursor;
	cursor=new VNode;
	cursor=head;
	while(cursor!=NULL){
		if((*cursor).getRIN()==RIN){ 
			return (*cursor).getZipCode();
		}
		cursor=cursor->next;
	}
	return "None";
}

bool LinkedList::AddNode(VNode* voter){ //used for i command
	bool exists=false;
	exists=NodeExists((*voter).getRIN()); //checking if voter already exists in hash table
	if(exists){
		cout<<"-> Voter  with RIN "<<(*voter).getRIN()<<" in "<<(*voter).getZipCode()<<" already exists"<<endl;
	}
	else{
		voter->next=head;
		head=voter;
	}
	return exists;
}

void LinkedList::FindNode(string RIN){ //used for l command
	VNode* cursor;
	cursor=new VNode;
	cursor=head;
	bool found=false;
	while(cursor!=NULL){
		if((*cursor).getRIN()==RIN){ //if node of cursor matcides HOTEL name being searcided for, print contents
			cout<<endl<<"RIN"<<"      "<<"  NAME"<<"            "<<"ZIP"<<"    "<<"VOTING STATUS"<<endl;
			cout<<"____________________________________________________________________"<<endl<<endl;
			(*cursor).print();
			found=true;
			break;
		}
		cursor=cursor->next; //if node not found, move to next node
	}
	if(!found){
		cout<<endl<<"-> Voter RIN "<<RIN<<" not found"<<endl;}
}
bool LinkedList::ExistZipNode(string zip, string RIN){ //for zip hash hash table, checking that added node does not already exist
	VNode* cursor;
	cursor=new VNode;
	cursor=head;
	bool found=false;
	while(cursor!=NULL){
		if((*cursor).getZipCode()==zip && (*cursor).getRIN()==RIN){ //if node of cursor matcides HOTEL name being searcided for, print contents
			found=true;
			break;
		}
		cursor=cursor->next; //if node not found, move to next node
	}
	return found;
}
bool LinkedList::CheckVoted(string searchName){ //function to return whther user registering to vote has already voted (currently Y)
	VNode* cursor;
	cursor=new VNode;
	cursor=head;
	bool found=false;
	while(cursor!=NULL){
		if((*cursor).getRIN()==searchName){ 
			if((*cursor).getVotingStatus()=="Y"){
				found=true;
				break;
			}
		}
		cursor=cursor->next; //if node not found, move to next node
	}
	return found;
}
string LinkedList::PrintNodes(){
	VNode* cursor;
	cursor=new VNode;
	cursor=head;
	string zc="";
	bool found=false;	
	if(((*cursor).getZipCode()).size()!=0){
		zc=(*cursor).getZipCode();
		found=true;
	}
	else{cursor=cursor->next;}
	if(found){return zc;}
}

int LinkedList::PrintZipCode(string zip){ //for z command, printing voted participants per specified zip code
	VNode* cursor;
	cursor=new VNode;
	cursor=head;
	zipcount=0;
	string zc=""; //this var is to aid with printing all RINs below each other for same zip code
	while(cursor!=NULL){
		if(((*cursor).getZipCode()).size()!=0){
			if((*cursor).getZipCode()==zip){
				if(zc!=(*cursor).getZipCode()){ //checking if new zip code being printed for, only print RIN
						zc=(*cursor).getZipCode();
						cout<<endl<<"Zip Code: "<<(*cursor).getZipCode()<<endl;
						cout<<(*cursor).getRIN()<<endl;
						cursor=cursor->next; //if node not found, move to next node
						zipcount++;}
				else{
					cout<<(*cursor).getRIN()<<endl; //if same zip code is still printing RINs, only print RIN (above format prints Zip Code: too)
					cursor=cursor->next; //move to next node after printing
					zc=(*cursor).getZipCode(); //update current zip code to next node's zip code
					zipcount++;}
			}	
			else{cursor=cursor->next;}
		}
		else{cursor=cursor->next;}
		if(cursor==NULL){break;}	
	}
	return zipcount;
}
void LinkedList::RemoveNode2(string zip){ //for d command, deleting from zip hash hash table
	bool deleted=false;
	VNode* p;
	p=new VNode;
	VNode* q;
	q=new VNode;
	q=head;
	p=head->next;
	if((*q).getZipCode()==zip){ //if voter exists as first node
		head=p;
		q->next=NULL;
		delete q;
		deleted=true;
		size--;
	}
	else{ //if voter exists as any node apart from starting node in linked list
		if (p == NULL) return ;
		int counter=0;
		while(!deleted && counter<=size && p!=NULL){
			if((*p).getZipCode()==zip){
				deleted=true;
				break;
			}
			p=p->next;
			q=q->next;
			counter++;
		}
		if (p == NULL) return ;
		q->next=p->next;
		p->next=NULL;
		delete p;
		deleted=true;
	}

	if(!deleted){cout<<endl<<"-> Voter(s) with zip code "<<zip<<" does not exist in records"<<endl;
	}
}

void LinkedList::RemoveNode(string RIN){ //for d command, deleting voter from voter hash hash table
	bool deleted=false;
	VNode* p;
	p=new VNode;
	VNode* q;
	q=new VNode;
	q=head;
	p=head->next;
	if((*q).getRIN()==RIN){ //if first node
		head=p;
		q->next=NULL;
		delete q;
		deleted=true;
		cout<<endl<<"-> Voter with RIN "<<RIN<<" has been deleted"<<endl;
	}
	else{ //if not start node
		if (p == NULL) return ;
		int counter=0;
		while(!deleted && counter<=size && p!=NULL){
			if((*p).getRIN()==RIN){
				deleted=true;
				break;
			}
			p=p->next;
			q=q->next;
			counter++;
		}
		if (p == NULL) return ;
		q->next=p->next;
		p->next=NULL;
		delete p;
		deleted=true;
		cout<<endl<<"-> Voter with RIN "<<RIN<<" has been deleted"<<endl;
	}

	if(!deleted){cout<<endl<<"-> RIN "<<RIN<<" does not exist in records"<<endl;
	}
}

//CONSTRUCTOR
HashTable::HashTable(int capacity)
	{
		VoterList=new LinkedList*[capacity]; //Array oflinked lists created
		for(int i=0; i<(capacity-1);i++){
			VoterList[i]=new LinkedList; //create a lonked list per index according to size
		}
		this->capacity = capacity;
		this->size = 0;
	} 
//DESCTRUCTOR
HashTable::~HashTable(){
	for(int i=0; i<(capacity-1);i++){
		delete[] VoterList[i];}
		delete VoterList;
}

long HashTable::hashCode(const string key) //Using ascii value hashCode
	{
		long val=0;
		for(int i=0;i<key.length();i++){
			val=val+key[i];
		} 
		return abs(val); 
	}
void HashTable::insert(VNode* node){ //adding node to hash table into linked list based on index hash code value
	int index = hashCode((*node).getRIN())%capacity; //Using RIN as key
	(*VoterList[index]).AddNode(node);
}
void HashTable::insert2(VNode* node){ //inserting into zip hash hash table for Y voters per zip code
	string zip=(*node).getZipCode();
	int index = hashCode((*node).getZipCode())%capacity; 
	bool exists=(*VoterList[index]).AddNode(node);
	bool not_added=true;
	//the following lines are used for the o command to keep track of how many Y voters per zip code
	if(exists){}
	else if(!exists){
		if(indices.size()!=0){ //if dictionary "indices" is populated
			for(auto item: indices){ //loop through and either add zip code as new elem or increment count value for where it already exists
				if(zip==item.first){
					item.second++; 
					size++;
					indices[zip]=item.second;
					not_added=false;
					break;
				}	
			}
			if(not_added){indices[zip]=1; size++;}

		}
		else{indices[zip]=1; size++;} //if no items in dictionary yet, add zip code as new item with freq of 1
	}
	
}
void HashTable::find(string RIN){
	int index = hashCode(RIN)%capacity; //using RIN as key 
	(*VoterList[index]).FindNode(RIN); //calls findnode function from linkedlist class
}
bool HashTable::exist_zip_node(string zip,string RIN){
	int index = hashCode(zip)%capacity; //using zip as key 
	bool exists=(*VoterList[index]).ExistZipNode(zip,RIN); //checking if voter already exists in zip hash hash table
	return exists;
}
bool HashTable::check_voted(string RIN){
	int index = hashCode(RIN)%capacity; 
	bool check=(*VoterList[index]).CheckVoted(RIN); //checking if voter trying to register their vote already did so previously
	return check;
}
void HashTable::print_all(){ //o command function
	int count=0;
	int reps=indices.size(); //number of elems for which the below function needs to execute
	int max=0;
	string zip;	
	map<string,int> temp;
	temp=indices; //copying over dict values as they get deleted through looping process to avoid reselection, the copied values will be copied into indices dict after execution
	while(count<reps){ 
		for(auto item: indices){ //loop through dict of zip code keys and num of voters value
			if(item.second>max){ //compare num of voters for each zip code and return the max to print first
				max=item.second; 
				zip=item.first;
			}
		}
		cout<<"Y voters in zip code "<<zip<<" is "<<max<<endl;
			indices.erase(zip);
			count++;
			max=0;
	}
	indices=temp;
}
bool HashTable::update_vote(string RIN){
	int index = hashCode(RIN)%capacity; 
	bool NV_exists=(*VoterList[index]).NodeVoted(RIN); //updating voter to Y
	return NV_exists;
}

void HashTable::voted_zip_code(string zip_code){ //z command
	int index = hashCode(zip_code)%capacity; 
	int num_voters=(*VoterList[index]).PrintZipCode(zip_code);
	cout<<endl<<"-> "<<num_voters<<" voted in this zip code."<<endl;

}
string HashTable::retrieve_zip_code(string RIN){ //used to add voters into zip hash in r and bv commands in main file
	int index = hashCode(RIN)%capacity; 
	string zip= (*VoterList[index]).ZipCode(RIN);
	return zip;
}
void HashTable::remove(string RIN){
	int index = hashCode(RIN)%capacity; //using RIN as key for voter hash hash table
	(*VoterList[index]).RemoveNode(RIN); //calls removenode function from linkedlist class
}
void HashTable::remove2(string zip_code){
	int index = hashCode(zip_code)%capacity; //using zip as key for zip hahs hash table
	(*VoterList[index]).RemoveNode2(zip_code); //calls removenode2 function from linkedlist class
	size--;
}
