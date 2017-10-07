/*
    main.cpp
        
    Implements the user interface for hangman.
    
    assignment: CSCI 262 Project 2 - Evil Hangman        

    author: Lane Starritt

    last modified: 2017-10-06
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <map>
#include <set>
#include <fstream>

//#include "hangman.h"

using namespace std;

// helper function prototypes
int get_integer(string prompt); // return a positive integer
char get_letter(string prompt); // return a valid lowercase letter
bool get_yesno(string prompt);  // return true == yes / false == no
string trim(string s);          // remove leading/trailing whitespace

int main() {
    cout << "Welcome to Hangman!" << endl;
    cout << "===================" << endl << endl;

    ifstream dic ("dictionary.txt", ifstream::in);
    if (dic.fail()){
        cout << "Error opening dictionary file." << endl;
        return 1;
    }else{
        cout << "Initializing dictionary..." << endl;
    }

    map <int, vector <string>> dict;

    while (!dic.eof()){
        string temp;
        dic >> temp;
        dict[temp.size()].push_back(temp);
    }
    
    // Keep playing the game until the user decides otherwise
    while (true) {
        
        int num_guesses = get_integer("How many guesses would you like?");
        cout << endl;
        int word_length = get_integer("How long should the word be?");
        cout << endl;

        set <int> w_lens_set;
        for(int i = 0; i < 30; i++){
            if(dict.find(i) != dict.end()){
                w_lens_set.insert(i);
            }
        }
        
        while(w_lens_set.find(word_length) == w_lens_set.end()){
            cout << "No words of that length." << endl;
            word_length = get_integer("How long should the word be?");
            cout << endl;
        }
                
        map <string, string> families;
        for(string s: dict[word_length]){
            string temp_string;
            for (int i = 0; i < s.length(); i++){
                temp_string += '_';
            }
            families.emplace(s, temp_string);
            //cout << s << endl;
        }
      
        //hangman game;
        //game.start_new_game(num_guesses, word_length);


        string the_word;
        for (int i = 0; i < word_length; i++){
            the_word += '_';
        }
        set <char> guessed;
        
        bool won = false, lost = false;
        while (!won && !lost) {
            cout << "Your word is: ";
            for(char c: the_word){
                cout << c;
            }
            cout << endl << endl;


            if (guessed.size() == 0) {
                cout << "You have not yet guessed any letters." << endl;
            } else {
                cout << "You have already guessed these letters: ";
                for(char c: guessed){
                    cout << c;
                }
                cout << endl;
            }

            cout << "You have " << num_guesses;
            cout << " guesses remaining." << endl << endl;

            char guess = get_letter("What is your next guess?");
            while (guessed.find(guess) != guessed.end()) {
                cout << endl << "You already guessed that!" << endl;
                guess = get_letter("What is your next guess?");
            }
            cout << endl;
            guessed.emplace(guess);

            //proccess the guess.
            bool good_guess;
            map <string, int> cur_families;
            for (pair <string, string>  p: families){
                for(int i = 0; i < word_length; i++){
                    p.second.clear();
                    if(p.first[i] == guess){
                        p.second.push_back(guess);
                    }else{
                        p.second.append("_");
                    }
                }
            }
            for (map <string, string>::iterator p=families.begin(); p!=families.end(); p++){
                if (cur_families.count(p->second) > 0){//familiy paracing isn't working...
                    cur_families.at(p->second)++;
                    cout << p->second << cur_families[p->second] << endl;
                }else{
                    cur_families.insert(pair <string, int> (p->second, 1));
                }
            }
            


//here be the nice outputs.
            if (good_guess){
                cout << "Good guess!" << endl;
            }
            if (!good_guess){
                cout << "Sorry, that letter isn't in the word." << endl;
                num_guesses--;
            }
//impliment win code later
            cout << "Congratulations! You won the game!" << endl;
            if(num_guesses == 0){
                cout << "Oh no! You lost!!!" << endl;
                lost = true;
            }
        }

        cout << endl;
        if (!get_yesno("Would you like to play again (y/n)?")) break;
    }

    cout << endl << "Thank you for playing Hangman." << endl;

    return 0;
}

// Prompt for a positive integer response, re-prompting if invalid
// input is given. This is not super-robust - it really should work
// harder to filter out responses like "123foo", but oh well.
int get_integer(string msg) {
    while (true) {
        string input;    
        int result = 0;

        cout << msg << endl;
        getline(cin, input);

        result = atoi(input.c_str());
        if (result > 0) return result;

        cout << "I didn't understand that. Please enter a positive integer.";
        cout << endl;
    }
}
    
// Prompt for a letter of the alphabet, re-prompting if invalid
// input is given.
char get_letter(string msg) {
    while (true) {
        string input;    
 
        cout << msg << endl;
        getline(cin, input);

        input = trim(input);

        if (input.size() == 1) {
            char result = tolower(input[0]);
            if (result >= 'a' && result <= 'z') return result;
        }
        
        cout << "I didn't understand that. ";
        cout << "Please enter a letter of the alphabet.";
        cout << endl;
    }
}


// Prompt for a yes/no response, re-prompting if invalid
// input is given.
bool get_yesno(string msg) {
    while (true) {
        string input;    
 
        cout << msg << endl;
        getline(cin, input);

        input = trim(input);
        for (int i = 0; i < input.size(); i++) {
            input[i] = tolower(input[i]);
        }

        if (input == "y" || input == "yes") return true;
        if (input == "n" || input == "no") return false;
        
        cout << "I didn't understand that. ";
        cout << "Please enter y(es) or n(o).";
        cout << endl;
    }
}

string trim(string s) {
    int a, b;

    for (a = 0; a < s.size() && isspace(s[a]); a++);
    for (b = s.size() - 1; b >= a && isspace(s[b]); b--);
    
    return s.substr(a, b - a + 1);
}


