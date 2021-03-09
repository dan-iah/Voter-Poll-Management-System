OS Project 1
mvote system
NetID: dh2741
14.02.2021

The mvote system allows a poll manager to store, manipulate and access voter information.

To run the program:
1. Open terminal 
2.1 Change the directory to where the file is stored 
2.2 Ensure that csv files you want to load are located in the file in which the make, header, function, and main files are loacted.
3. Type: make
4.1 Type: ./mvote -f filename -m hashtablesize
4.2 OR Type: ./mvote -f filename
Note on 4: the -m flag declaring hash table size is not neccessary as the alternative is that the program increments a num_lines counter whilst getting/going through all the lines in the file, using the total number of lines as the hash table capacity. If -m flag is used, a reasonable size (value following -m flag) is to be selected to allow for sufficient space for voter details to be input into hash table. For example for voters50.csv, -m 10 works however, -m 1 does not work.

There are two main strcutures that allow for the voter management:
------------------------------------------------------------------
1. VoterHash: a hash table hashing participants RINs and saving their name, zip code, and default voter status of "N". Each index (RIN hash code) is a bucket that is a pointer to a linked list, where participants that hash at the same index value are all located. This hash table allows for O(1) look ups for the l command and for the deletion d command.

2. ZipHash: a hash table hashing registered voters zip codes, saving their zip code and RIN. This hash table connects zip codes' linked lists to Y voters. For the z <zipcode> command, this hash table allows for O(1) time information access to print the Y voted participants whom belong to that zip code. The nodes in ZipHash's linked lists are added upon voter registration, i.e. the r and bv commands, creating a hash table of only Y voters hashed at their zip code values.

The choices for a hash table in the cases above is to allow for O(1) time command execution.

Other voter management methods:
- Commands v and perc are managed through counters. A num_voted counter increments upon the r and bv commands once existant users are registered as having voted (Y). The perc command uses the num_voted counter value and a num_participants counter (incrememented when reading the number of lines in the input file) to calculate the percentage of Y voters. (num_voted/num_participants)*100.
- Command o is implemented using a dictionary in which the key-value pairs are the zip code and the frequency in which this zip code appears for Y voters (how many Y voters exist at this zip code). The dictionary is then looped through to find the maximum value (voter frequency) to print that zip code first, with all the other zip codes with lower frequency of voters following. To avoid reselection, the key-value pair is deleted after selection from the dictionary. This continues until there are no more values in the dictionary to loop through (dictionary.size()>0).


