Word Cloud Code Challenge
===============================================
Zach Dischner
May 2 2017

## Build
If you have `scons` installed and an active python 2 environment, compile like so (easy right?):

```
scons
```

Otherwise, compliation direct from the command line is pretty simple!

```
g++ -std=c++11 WordCloud.cpp -o WordCloud
```

## Examples

You can always call with the `--help` kwarg to get command line usage and examples

```
./WordCloud --help
Usage:
                ./WordCloud (input) (--options)

                input:
                    _/--input:  String or path to filename to load and generate word cloud for

                options:
                    --mincounts:    Specify minimum occurances of certain words to be allowed to be output
                    --maxcounts:    Specify maximum occurances of certain words to be allowed to be output
                    --startswith:   String specifying what words must start with in order to be output in

                Examples:
                    # Generate unconstrained wordcloud from file 'test.txt'm
                    ./WordCloud test.txt

                    # Set minimum and maximum occurances for word cloud from file 'test.txt'
                    ./WordCloud test.txt --mincounts=5 --maxcounts=10

                    # Set min occurances and specify starting letters for a string direct from the command line
                    ./WordCloud 'my dog is lazy, labeled like larry, larry likes larry and labeled larry' --mincounts=2 --startswith='la'
```

Some nice examples of use:

### Raw Text Input

```
./WordCloud "I am testing this function. Tests are important. So is a function. Good old tests. Test tesT teST TEST"

function 2
good 1
important 1
old 1
test 4
testing 1
tests 2
this 1
```

```
./WordCloud "I am testing this function. Tests are important. So is a function. Good old tests. Test tesT teST TEST tests" --startswith="test" --mincount=2 --maxcount=5

test 4
tests 3
```

### File Input
Same kwargs apply when reading a file

```
./WordCloud randomWords.txt --startswith='ab'

able 2
absolute 1
```


## Tests

### Debugging
Simple debug statemts can be enabled by changing line 43 such that `DEBUG > 0`. Otherwise, debug pring statements will be suppressed. 

### Test script
Simple test cases are provided in the script: `test.py`. It validates general behavior and some specific situation cases. Call it from the command line with a basically equipped python 2 interpreter. The program will tell you what tests are being ran and will fail loudly if an assertment is missed. 

It is assumed that the program has already been compiled with `DEBUG = 0`

```
python test.py
```

## Discussion
Always room for improvement, however this is a first cut at proving functionality. 

Biggest area for concern that I can see is for large files. The program as of now will load the entire file's contents into memory for analysis. Better would be an incremental file reader which constantly reads chunks and updates the main data structure, the `wordCloud` `map` object, since the update calls can be done over and over again on incremental pieces of text. 

Otherwise, more safety, try-excepts, and error handling would be the first thing I'd like to add. And learn more about C++ functional behaviors, I have ideas therein...

## Prompt
Your program word cloud program should be written in Java, C#, or C++ aned be able to take both a line of text and a path to a text file to generate a word cloud.  The parser in your inner libraries should try not to include words useless to word clouds (like articles [a, an, the], or minor words like "it", "us", "them"); this is just notional since this is a programming example just show the principle of the filtering. As an output your program, and therefore the inner libraries, need to support the ability to return the word cloud sorted by counts with a min/max threshold option and it needs to be able to return the word cloud alphabetically with the ability to choose the starting characters.  For example, one run may ask for all words that occur more than 10 times but less than 20, and another may ask for the top ten words.  Another run may ask for all words alphabetically that start with the phrase "sa".  Have a driver program as well as unit tests that stimulate the library and the code with reasonable test cases. 

