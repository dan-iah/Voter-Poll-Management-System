#include<iostream>
#include<cstring>
#include<cstdlib>
#include<fstream>
#include<sstream>
#include<ctype.h>
#include<stdio.h>

#include"header.h"

using namespace std;

//CITATIONS:
//Data Structures & Algorithms by Michael T. GoodRich, Roberto Tamassia and David Mount

int main(int argc, char* const argv[]){
	ifstream inFile;
	int capacity=0;
	float num_voted, num_participants, num_lines=0;
	bool input_provided=false;
	string hash_table_size="";
	string v_status="N";
	string line,filename,outfile,info,action;
	vector<string> lineinfo;

		//READING COMMAND LINE INPUT
		for(int i=0;i<argc;i++) //processing command line args
			{
				if (strcmp(argv[i], "-f") == 0){ //if -f is found, check if following arg is a string
					if(argv[i+1]!=""){ 
						filename=argv[i+1]; 
						input_provided=true;
					}
					else{
						input_provided=false; //if no string (filename) after -f flag, -> line 46
					}
				}
				 if(strcmp(argv[i], "-m") == 0){ //if -m found
				 	if(argv[i+1]!=""){ //if arg after -m is provided
						hash_table_size=argv[i+1]; //save it as the size for the hash table strcuture
						input_provided=true;
					}
					capacity=stoi(hash_table_size)*1.4;
				}
			}
		if(!input_provided){ //if input file not provided, program cannot run!
			cout<<endl<<"Please provide an input file else program cannot run"<<endl;
			exit(1);}

		//FINDING HASH TABLE CAPACITY IF -m FLAG NOT PROVIDED
		inFile.open(filename); //after filename provided, open the file
		if(inFile.is_open()){ //if file is open
			while(getline(inFile,line)){num_lines++;} //go through the length of the file till end to get num of lines in the file (i.e. size of the file)
			if(hash_table_size==""){capacity=num_lines*1.4;} //if -m was not provided (size not provided by user), use num_lines for the file size
		}

		//CREATING HASH TABLES
		HashTable VoterHash(capacity); //declaring the hashtables, this calls the HashTable constructor which creates a new linked list strcuture 
		HashTable ZipHash(capacity); //for every index for the hash table size (capacity)

		//RETRIEVING VOTER INFO FROM FILE TO ADD INTO VOTERHASH HASH TABLE
		if(inFile.is_open()){ //
			inFile.clear();
			inFile.seekg(0);
			while(!inFile.eof()){ //Continuing until end of file 
				getline(inFile,line);
				if(line!="") { //dont want to consider blank lines
					stringstream ss; //declare stringstream as new variable at each line to save each line (declaration at each line avoided issues of improper line reset)
					ss.str(string()); //reset string stream to ensure it is empty to receive new lien info
					ss.clear();
					ss<<line; //save line in file into ss stringstream
					while(getline(ss, info,' ')){lineinfo.push_back(info);} 
					string RIN=lineinfo[0]; //first item in line (saved as first elem into lineinfo vector) is the RIN
					string name=lineinfo[1]+ " "+ lineinfo[2];
					string zip_code=lineinfo[3];
					VNode* voter=new VNode(RIN,zip_code,name,v_status); //create new node with line info
					VoterHash.insert(voter); //insert new node into VoterHash hash table (where it will find hash code)
					lineinfo.clear(); //resetting vector for next line	
					num_participants++;
				}
		}
	}
		else{ //if infile not open, terminate program
			cout<<"Provided file is not open"<<endl<<"Program cannot execute"<<endl;
			exit(1);}

	inFile.close();

	//COMMANDS
	do{ 
		cout<<endl<<"_______________________"<<endl;
		cout<<endl<<"Monitering Voting Poll"<<endl<<"_______________________"<<endl;
		cout<<endl<<"Options: "<<endl
		<<"-> l <key>"<<endl
		<<"-> i <rin> <lname> <fname> <zip>"<<endl
		<<"-> d <rin>"<<endl
		<<"-> r <rin>"<<endl
		<<"-> bv <fileofkeys>"<<endl
		<<"-> v"<<endl
		<<"-> perc"<<endl
		<<"-> z <zipcode>"<<endl
		<<"-> o"<<endl
		<<"-> exit"<<endl<<endl
		<<"-> ";

		getline(cin,action); //read user's input and save into action variable
		vector<string> action_tokenized; //vector to receive command details
		istringstream iss(action);
		for(action;iss>>action;)
			action_tokenized.push_back(action);

		//FINDING VOTER WITH RIN
		if(action_tokenized[0]=="l"){	
			string RIN=action_tokenized[1];
			VoterHash.find(RIN);
		}
		//DELETING VOTER WITH RIN
		else if(action_tokenized[0]=="d"){	
			string RIN=action_tokenized[1];
			string zip=VoterHash.retrieve_zip_code(RIN);
			bool exists=ZipHash.exist_zip_node(zip,RIN);
			if (exists){num_voted--;}
			ZipHash.remove2(zip);
			VoterHash.remove(RIN);

		}
		//ADDING VOTER WITH ALL REQUIRED DETAILS: ZIP,NAME,RIN
		else if(action_tokenized[0]=="i"){	
				string RIN=action_tokenized[1]; //saving voter details into vars
				string name=action_tokenized[2]+' '+action_tokenized[3];
				string zip_code=action_tokenized[4];
				string v_status="N";
				VNode* voter=new VNode(RIN,zip_code,name,v_status); //creating node with all inputed data
				VoterHash.insert(voter); //inserted node into hash table linked lists (VoterHash)
				cout<<"Voter "<<RIN<<' '<<name<<' '<<zip_code<<" has been added"<<endl;
		}
		
		//PRINTING ZIP CODE PARTICIPANTS WHO VOTED Y
		else if(action_tokenized[0]=="z"){ 
			string zip=action_tokenized[1];
			cout<<endl<<"Searching for zip code "<<zip<<"..."<<endl;
			ZipHash.voted_zip_code(zip);
		}
		//PRINTING ZIP CODES BASED ON MOST TO LEAST VOTED
		else if(action_tokenized[0]=="o"){
			ZipHash.print_all();
		}
		//REGISTERING VOTER -> Y
		else if(action_tokenized[0]=="r"){
			string RIN=action_tokenized[1];
			bool voted=VoterHash.check_voted(RIN); //check if person already voted or not
			if(!voted){ //if not, register their vote
				string zip=VoterHash.retrieve_zip_code(RIN);
				bool exists=VoterHash.update_vote(RIN); //registeres the voter as Y
				if(!exists){cout<<"Voter with RIN "<<RIN<<" does not exist."<<endl;} //check the voter exists

				//ADDING VOTER TO ZIPHASH HASH TABLE OF Y VOTERS PER ZIP CODE
				else if(exists){
					VNode* voter2=new VNode(RIN,zip,"",""); //name and v_status
					ZipHash.insert2(voter2); //Adds Node to ZipHash
					num_voted++;
					cout<<"Voter with RIN "<<RIN<<" has been marked as voted."<<endl;
				}
			}
			else{cout<<"This person has already been marked as voted."<<endl;}
		}
		//BULK VOTE FOR PARTICIPANTS IN A FILE
		else if(action_tokenized[0]=="bv"){ 
			vector<string> voter;
			bool exists=false;
			string filename=action_tokenized[1];
			inFile.open(filename);
			if(inFile.is_open()){ 
				if(line=="") {}; //if line is blank, skip it
				while(!inFile.eof()){ //Continuing until end of file 
					getline(inFile,line);
					stringstream ss; //declare stringstream as new variable at each line
					ss.str(string()); //reset string stream
					ss.clear();
					ss<<line;
					while(getline(ss, info,' ')){voter.push_back(info);} //saving RIN to check if already voted
				}
				int i=0;
				while(i<voter.size()){
					bool voted=VoterHash.check_voted(voter[i]); //checking if RIN prev voted
					//ADDING VOTER TO ZIPHASH HASH TABLE OF Y VOTERS PER ZIP CODE
					if(!voted){
						bool exists=VoterHash.update_vote(voter[i]);
						string zip=VoterHash.retrieve_zip_code(voter[i]);
						VNode* voter2=new VNode(voter[i],zip,"","");
						ZipHash.insert2(voter2);
						num_voted++;}
					i+=4;
				}
				if(voter.size()>0){cout<<"Voters from file "<<filename<<" have been registered as voted."<<endl;}
				voter.clear(); //resetting vector
			}
				else{cout<<"Provided file is not open. Command cannot be fulfilled."<<endl;}//if infile not open, terminate program after telling user it is closed

			inFile.close();

		}
		//RETURN NUM OF Y VOTERS
		else if(action_tokenized[0]=="v"){ 
			cout<<num_voted<<" voted."<<endl;
		}
		//RETURN PERC OF Y VOTERS OUT OF TOTAL NUM OF PARTICIPANTS
		else if(action_tokenized[0]=="perc"){ 
			cout<<(num_voted/num_participants)*100<<"%"<<" of people voted."<<endl;
		}
		//TERMINATE PROGRAM
		else if(action_tokenized[0]=="exit"){ 
			exit(1);
		}
		//ANY INPUT APART FROM ABOVE -> INVALID
		else{
			cout<<endl<<"Invalid command format"<<endl; //error if one of options not chosen
		}

	}while(action!=""); //while valid option being chosen
	

	return EXIT_SUCCESS;
}