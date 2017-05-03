//
//  WordCloud.cpp
//
//  Created by Zach Dischner on 5/1/2017
// 
//  Takes string/filename input, then gives as an output the words and count of occurances
//  of said word in the string or parsed file input. Punctuation is removed from all words,
//  words are all made lowercase, and are validated against some words that we know we 
//  want to ignore from output "word cloud" statitstics. 
//
// For usage instructions, call with the --help option
// 
// Simplest build instructions if you have scons and a python2 environment (NOT 3, sadly)
//      `scons`
// 
// Otherwise, direct command line g++ call is simple enough
//      `g++ -std=c++11 WordCloud.cpp -o WordCloud`             

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Imports //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <map>
#include <cctype>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <getopt.h>

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Program definitions /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

// Debug helper
# define DEBUG 0
#if DEBUG > 0
# define DEBUG_PRINT(x) printf("\n\t>>"); printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

// For optional argument parsing
#define no_argument 0
#define required_argument 1 
#define optional_argument 2

const char *IGNORES[] = {"a","an","am","and","the","you","me","my","us","be","it","us","he","she","him","her","his","hers",\
                        "ye","your","so","or","is","isnt","not","mr","mrs","ms","dr","sr","they","thy","we","of","to","its",\
                        "i","on","oh","if","as","by","them","our","in","for","do","dont","does","doesnt","did","at","are","did"};


////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Utility Functions //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Attempt to load a file's contents by name
 *
 * Basic function, just takes a filename string and attempts to load the thing. If the filenam
 * isn't available or can't be opened, exit the program. 
 * 
 * @param filename Character array indicating filename relative to executable to load
 * @param theFile pointer to FILE pointer. Passed by reference for use in calling function.
 */
string loadFile(string filename)
{
    ifstream t(filename.c_str());
    stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

/**
 * @brief Determine if a string is a valid path specifier to a file
 * 
 * @param maybeFilename String path potentially to a readable file
 *
 * @returns _ Boolean indicator of file validity
 */
bool fileExist(string maybeFilename)
{
    ifstream infile(maybeFilename);
    return infile.good();
}

/**
 * @brief See if the word is worth keeping
 *
 * Is it in the list of ignorable words? Is there a number in it? Are there special matching conditions that it has to meet? 
 * 
 * @param word The word you want to validate
 * @param startswith String that a good word should start with
 */
bool goodWord(string word, string startswith=string(""))
{
    // No empty words (should never happen)
    if (word.empty()) {return false;}

    // See if the word is in our explicit ignore list
    for(auto ignore : IGNORES)
    {
        if (word == ignore)
        {
            DEBUG_PRINT(("Word '%s' is in ignore set. Marking as bad word", word.c_str()));
            return false;
        }
    }
    
    // If we have a `startswith` condition, validate that it starts with the correct letters
    if (startswith != "")
    {
        if (strncmp(word.c_str(), startswith.c_str(), startswith.size())!=0) 
        {
            DEBUG_PRINT(("Word '%s' does not start with '%s'. Marking as bad word", word.c_str(), startswith.c_str()));
            return false;
        }
    }

    // See if the word contains bad characters (numbers, punctuation, specials)
    if (word.find_first_not_of("abcdefghijklmnopqrstuvwxyz") != string::npos)
    {
        DEBUG_PRINT(("Word '%s' is in ignore set. Marking as bad word", word.c_str()));
        return false;
    }
    return true;

}

/**
 * @brief Build up a map object. The key is a word, the value is occurances of that word in the input string `words`
 *
 * Words are all converted to lowercase, and punctuation is removed. Then it is checked for validity via `wordGood()
 * 
 * @param words String with many words in it. Space delimited
 * @param wordCloud Referenced map object constructed to hold word cloud mappings
 * @param startswith (optional) String that a good word should start with
 */
void buildWordMap(string words, map<string,int> &wordCloud, string startswith="")
{
   stringstream wordstream(words);
   string word;
   while (wordstream >> word)
   {
       // Make word lowercase, strip punctuation from it
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());

        // Add word to our map if it passes all validity checks
        if (goodWord(word, startswith))
        {
            wordCloud[word]++;  // Values initialized to 0 by default, can just start incrementing
        }
        else
        {
            DEBUG_PRINT(("Not including word '%s' in the wordCloud map", word.c_str()));
        }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Runtime Execution //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

int main (int argc, char **argv)
{
    // Declare/Allocate program variables
    map <string,int> wordCloud;         // Map/Dictionary to hold word=count mappings
    string contents;                    // Content to build word cloud for
    int mincounts = 0;                  // Potential Minimum counts for word cloud
    int maxcounts = 0;                  // ..
    string startswith = "";             // Potential word starting letters for word cloud
    string input = "";                  // Raw input to program. To be parsed into `content`
    int option_index;                   // Optional argument variables
    int c;

    // Specifications for optional command line argument parsing
    const struct option longopts[] =
    {
        {"help",      no_argument,        0, 'h'},
        {"input",     required_argument,  0, 'i'},
        {"startswith",optional_argument,  0, 's'},
        {"mincounts", optional_argument,  0, 'm'},
        {"maxcounts", optional_argument,  0, 'n'},
        {0,0,0,0},
    };

    //Parse optional arguments: https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
    while ((c = getopt_long(argc, argv, "i:smnh:", longopts, &option_index)) != -1)
    {
        switch (c)
            {
            case 'm':
                mincounts = atoi(optarg);
                break;
            case 'n':
                maxcounts = atoi(optarg);
                break;
            case 's':
                startswith = string(optarg);
                break;
            case 'i':
                input = string(optarg);
                break;
            case 'h':
                printf("Usage:\n\
                ./WordCloud (input) (--options)\n\
                \n\
                input:\n\
                    _/--input:  String or path to filename to load and generate word cloud for\n\
                \n\
                options:\n\
                    --mincounts:    Specify minimum occurances of certain words to be allowed to be output\n\
                    --maxcounts:    Specify maximum occurances of certain words to be allowed to be output\n\
                    --startswith:   String specifying what words must start with in order to be output in\n\
                \n\
                Examples: \n\
                    # Generate unconstrained wordcloud from file 'test.txt'm \n\
                    ./WordCloud test.txt \n\
                    \n\
                    # Set minimum and maximum occurances for word cloud from file 'test.txt' \n\
                    ./WordCloud test.txt --mincounts=5 --maxcounts=10 \n\
                    \n\
                    # Set min occurances and specify starting letters for a string direct from the command line\n\
                    ./WordCloud 'my dog is lazy, labeled like larry, larry likes larry and labeled larry' --mincounts=2 --startswith='la' \n\
                    ");
                    
                    abort();
            default:
                printf("\nOptional arguments could not be parsed! Call with --help for usage instructions\n");
                abort ();
            }
    }
    DEBUG_PRINT(("CL Options: mincounts = %d, maxcounts = %d, startswith = '%s', index = %d, argc = %d\n",
          mincounts, maxcounts, startswith.c_str(), option_index, argc));
    
    // Some validity checks
    if (maxcounts < mincounts)
    {
        printf("--maxcounts (%d) cannot be less than --mincounts (%d)! Exiting", maxcounts, mincounts);
        exit(1);
    }
    if (input == "")
    {
        input = string(argv[argc-1]);
    }
    
    // See if input is a valid file. Read into a string for analysis if so, treat directly as a string if not. 
    DEBUG_PRINT(("Input to be read (file) or parsed directly (raw input): '%s'\n", input.c_str()));
    bool isvalid = fileExist(input);
    DEBUG_PRINT(("Is the input a valid filepath? %s", isvalid ? "true":"false"));
    if (isvalid)
    {
        contents = loadFile(input);
    }
    else
    {
        contents = input;
    }

    // Build up our word cloud! 
    buildWordMap(contents, wordCloud, startswith);

    // Display word cloud results     
    // Remember, wordCloud map keys are already in alphabetical order. Handy
    printf("\n");
    for(auto elem : wordCloud)
    {
        // If provided, only print out word counts within min/max counts range
        if (mincounts > 0)
        {
            if (elem.second < mincounts) {continue;}
        }
        if (maxcounts > 0)
        {
            if (elem.second > maxcounts) {continue;}
        }
        printf("%s %d\n",elem.first.c_str(), elem.second);
    }
}


