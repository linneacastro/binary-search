/* A Dictionary Search Program Using Binary Search Written in C */
// NAME: Linnea P. Castro
// DATE: 26 SEP 2023
// COURSE: CS 360
// ASSIGNMENT: 2

/*
The function lineNum takes in 3 arguments: a pointer to the dictionary name,
a pointer to a character array which is the word being searched for, and an integer
representing the width of the dictionary.  The function returns an integer, signifying
either the linenumber the searched word was found on, or the negative of the last line
searched if no match was found  If an error is encoutered while using fopen, fseek, or 
read, an error number is returned.  

The dictionary search algorithm is based on binary search, where the search area is 
essentially havled on each search passthrough.  Using lseek, we establish the number of
lines in the dictionary.  In dividing this number by 2, we can find the middle line, 
read it, and use strcmp to compare the read in buffer to the contents of our truncated
word.  If a match is not found, the return value of strcmp will let us know which "side"
of the middle line to search next, letting us reset either our search floor or ceiling
to zero in on the word.   
*/
