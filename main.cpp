#include <iostream>
#include <map>
#include <string>
#include <memory>
#include <tr1/memory>
#include <fstream>
std::ifstream fin ("file.txt"); //read the local file for the text
typedef struct node node; //typedef for shorter writing
typedef std::shared_ptr<node> node_ptr; //typedef for shorter writing, I am using smart pointers but raw pointers could be used too
using namespace std;


struct node{
    std::map<char,node_ptr> characters; //the pair <chair, node_ptr> is actually the a node, the char key is their own character and the node_ptr is the corresponding node data for the character
    bool isEnd; //it shows if it is the last character of a word
    // string info; //all sorts of data that could be added for each word

};


void createTrie(node_ptr root)
{
    root->isEnd=false; //the root->isEnd is always false
}


void insertWord(node_ptr root,string word)//insertion function of a given word in the trie
{
    std::map<char,node_ptr>::iterator it; //iterator for node's map
    node_ptr current=root; //current node_ptr iterates the trie, it should always start from the root


    for(int i=0;i<word.length();i++)
    {
        it=current->characters.find(word[i]); //searching for the key to know if it already exists in the current node_ptr. Find returns the iterator of the serched key or std::map::end
        if(!isalpha(word[i])) //if it is not a alphabetical character we ignore it e.g. ,".'?! etc.
            continue;
        if(it==current->characters.end()) //if it does not exists we insert it to the trie
        {
            node_ptr newNode(new node);//allocate memory
            newNode->isEnd=false; //isEnd is false by default

            current->characters.insert(std::pair<char,node_ptr>(word[i],newNode));//we insert it to the current node_ptr's map of <char,node_ptr>
            current=newNode;//then current becomes the new inserted node

        }
        else //if the key already exists, it is the pair with the searched key and current goes further
        {
            current=it->second;//current becomes the node of from the pair which was found
        }


    }
    current->isEnd=true;//the last inserted/found pair has the isEnd=true to show the end of the word

}

void searchFor(node_ptr root, string searchWord) //search function for a given word in the trie
{
    std::map<char,node_ptr>::iterator it; //iterator for node's map
    node_ptr current=root; //current node_ptr iterates the trie, it should always start from the root, the same we do for the insertion
    string foundWord="";//we add the key characters one by one in this string
    for(int i=0;i<searchWord.length();i++)
    {

        it=current->characters.find(searchWord[i]); //searching for the key, the same as we do for insertions

        if(it!=current->characters.end())// if the char key exists
        {

            foundWord += it->first;//adds the key to the string
            current=it->second;//current becomes it->second node_ptr and goes further
        }
        else//if the char key does not exists the function stops and a message is displayed
        {
            cout<<searchWord<<" it doesn't exists in the dictionary"<<endl;
            return;
        }

    }
    //if all the char keys exists in the trie in the exact order, but the word was not actually inserted it means that isEnd is false so it does not exists
    if(current->isEnd) //check if isEnd is true
    cout<<foundWord<<" FOUND"<<endl; //display the word and found message if it is true
    else
    cout<<searchWord<<" it doesn't exists in the dictionary"<<endl;//if it is false display the searched word and a message
}

void readFromFile(node_ptr root){//function for reading from a file the words that will be inserted in the trie
    string temporaryWord;
    while(true)
    {
     fin>>temporaryWord; //read string word from file
     if(fin.eof()) //check if is end of file
            break; //stops while loop if is end of file
     insertWord(root,temporaryWord);//inserts the read word from the file
    }

}

void deleteFromTrie(node_ptr root, string deleteWord){ //function for the deletion of a given word from the trie
    std::map<char,node_ptr>::iterator it;//iterator for node's map
    node_ptr current=root; //current node_ptr iterates the trie, it should always start from the root, the same we do for the insertion
    std::map<char,node_ptr>::iterator previous; //iterator to hold the  previous node_ptr from the iteration


    if(deleteWord.length()==1) //if the word is just 1 character long
    {
        root->characters.erase(deleteWord[0]);// erase the character
        return; //stops the function
    }
    for(int i=0;i<deleteWord.length();i++)//goes through all characters until the last character of the word
    {
        previous=it; //holds the iterator before it change because of the std::map::find() function
        it=current->characters.find(deleteWord[i]);// it becomes std::map::end if it is not found or the iterator of the key if it is found
         if(it!=current->characters.end())//check if the key was found
            current=it->second; //
        else
        {
            break; //if it is not found the for loop stops
        }

    }

    if(it==current->characters.end()) //if the characters does not exists
    {
        cout<<"the word "<<deleteWord<<" it can't be deleted: does not exists"<<endl;
        return;
    }
    else
    {

        if(!it->second->characters.empty())//check if the map iterator's map is empty
            it->second->isEnd=false; //if it is not empty, you cannot erase the node_ptr, you need to assign the last character with false

        else
        {

            previous->second->characters.erase(it); //if it is empty, we can delete it from the the map
            deleteWord=deleteWord.substr(0,deleteWord.size()-1); //we delete the last char from the string deleteWord

            deleteFromTrie(root,deleteWord); //we call deleteFromTrie again for the word with the deleted last char
        }
    }

}


int main()
{
    cout << "Hello world!" << endl;
    node_ptr root(new node);
    createTrie(root);

    /*
	// Insert words by yourself
	insertWord(root,"abc");
    insertWord(root,"ac");
    insertWord(root,"abc");
    insertWord(root,"abcd");
    insertWord(root,"abgd");
    insertWord(root,"aabc");
    insertWord(root,"baba");
    searchFor(root,"baba");//Search for the word you have inserted in the trie
    searchFor(root,"abc");
    searchFor(root,"abcd");
    searchFor(root,"abcde");
    searchFor(root,"abc");
    searchFor(root,"bab");
    deleteFromTrie(root,"abcd"); //Delete the word from the trie
    searchFor(root,"abcd");
    deleteFromTrie(root,"baba");
    searchFor(root,"baba");
    searchFor(root,"bab");

	*/



    readFromFile(root); //Insert words to trie from the file which was declared on line 7
    searchFor(root,"Lorem");//Search for a word in the trie
    searchFor(root,"sit");
    searchFor(root,"facilisis");
    searchFor(root,"facilisis");
    searchFor(root,"Robert");
    searchFor(root,"RIUF");
    searchFor(root,"Cezara");
    searchFor(root,"lorem");
    searchFor(root,"sIt");

	deleteFromTrie(root,"Lorem");//Delete the word from the trie
	searchFor(root,"Lorem");

    return 0;
}
