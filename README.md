Use Netbeans to import the project files.

Used languages : JAVA and C++

WHAT: POS tagger. Given a string, gives the parts of speech tags to it. (limited pos tags: Noun, Verb, Adverb, Adjective, Others)
HOW: Trained from test corpus
USES: http://en.wikipedia.org/wiki/Hidden_Markov_model and http://en.wikipedia.org/wiki/Viterbi_algorithm
SLIDES: not yet uploaded
TESTING METHOD: five fold training and testing - corpus is divided into five parts. sentenes in each part are pos tagged using the training data from other four parts and results are compared with the actual data. Total corpus data-500lines. Each training corupsu file provided has 400 each lines.
USAGE:
trainingCorpus.java is used to train from a given already POS tagged corpus
See HMM.java file and change the paths accordingly to train new corpus
The analysed results of five corpus files (each containing 400 POS tagged sentence) are produced in this format.

<---->
total POS tags used: 5
unique words in the corpus: 2721
Total number of lines in the corpus: 500
<----->
//----confusion matrix----// (http://en.wikipedia.org/wiki/Confusion_matrix)
----transition table Columns : V A R N O----
----transition table rows: ^ V A R N O-----

Followed by each word and probability witht which it is tagged in one of the five tags

USAGE2:
use viterbi-pr.cpp
to train the data and send the file witht training data appended with the test sentences 
==========
