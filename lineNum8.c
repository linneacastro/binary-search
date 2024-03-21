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

// LIBRARIES TO INCLUDE //
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define DEBUG 0

// lineNum FUNCTION TEMPLATE
int lineNum(char *dictionaryName, char *word, int dictWidth){

// VARIABLES
int fd; // fd = file descriptor
int totalbytesinfile;
char *truncatedword;
int numberoflines;
int lengthofword;
int middleoffilelinenum;
int lowestlinenum;
int highestlinenum;
int currentoffset;
char buffer[dictWidth]; // Read in the whole line, including the newline
int numbytesread;
int stillsearching; // Flag variable
int result;
int lastlinesearched;
int lengthoftruncatedword;
int numspacestoadd;

// OPEN THE FILE
fd = open (dictionaryName, O_RDONLY);

if (fd < 0){
  fprintf(stderr, "Error: Can't open file for reading -- %s\n", strerror(errno)); 
  return errno;
}

// PROCESS INCOMING WORD
lengthofword = 0; // Initialize length of word passed in to 0
for (int i = 0 ; word[i] != '\0' ; i++){ // Find length of word passed in, NOT including null terminator
  lengthofword++;
}
if (DEBUG) printf("length of word passed in (not incl. null terminator) is: %d\n", lengthofword);

if (lengthofword >= dictWidth){ // If length of word is greater than or equal to dictWidth (aardvarks if tiny_9)
  truncatedword = malloc(sizeof(lengthofword+1));
  for (int i = 0 ; word[i] != '\0' ; i++){ // Copy word into truncatedword
    truncatedword[i] = word[i];
  }
  truncatedword[dictWidth-1] = '\0'; // Then truncate the word, making the last char the null terminator
}

else{ // Length of word is less than dictWidth
  truncatedword = malloc(sizeof(dictWidth+1));
  for (int i = 0 ; word[i] != '\0' ; i++){ // Copy word into truncatedword
    truncatedword[i] = word[i];
  }
  numspacestoadd = ((dictWidth - lengthofword)-1); // Pad out with spaces
  for (int i = lengthofword ; truncatedword[i] < (dictWidth - 2) ; i++){
    truncatedword[i] = ' ';
  }
 truncatedword[dictWidth-1] = '\0'; // Append null terminator to end of word, so its length including null terminator matches dictWidth 
}

lengthoftruncatedword = 0; // Init. lengthoftruncatedword to 0
for (int i = 0 ; truncatedword[i] != '\0' ; i++){ // Find length of truncated word
  lengthoftruncatedword++;
}

if (DEBUG) printf("word passed in is: '%s'\n", word);
if (DEBUG) printf("truncated word is: '%s'\n", truncatedword);
if (DEBUG) printf("number of chars in truncated word is: %d (not including null terminator)\n", lengthoftruncatedword);

// FIND NUMBER OF LINES IN FILE
totalbytesinfile = lseek(fd, 0, SEEK_END);
if (totalbytesinfile == -1){ // Error check after using LSEEK
  fprintf(stderr, "Error: %s\n", strerror(errno));
  return errno;
}
  
numberoflines = (totalbytesinfile/dictWidth);
if (DEBUG) printf("total bytes in file: %d\n", totalbytesinfile);
if (DEBUG) printf("number of lines in file: %d\n", numberoflines);

// FIND THE MIDDLE OF THE FILE
middleoffilelinenum = (numberoflines/2);
if (DEBUG) printf("middle line number is: %d\n", middleoffilelinenum);

// SET UP BEFORE SEARCH
lowestlinenum = 0; 
highestlinenum = numberoflines; // Set highestlinenum to the number of lines to start out 
currentoffset = lseek(fd, 0, SEEK_SET); // Move file offset to beginning of file
if(currentoffset == -1){ // Error check after using LSEEK
  fprintf(stderr, "Error: %s\n", strerror(errno));
  return errno;
}

// BEGIN WHILE LOOP/SEARCH
stillsearching = 1;
while (lowestlinenum <= highestlinenum){ // Remain in while loop as long as lowestlinenum is <= highestlinenum 

  // FIND MIDDLE OF FILE
  middleoffilelinenum = ((lowestlinenum+highestlinenum)/2);
  if (DEBUG) printf("middle line number is: %d\n", middleoffilelinenum);
  
  if (middleoffilelinenum == 0){
    break;
  }

  if (middleoffilelinenum > numberoflines){
    break;
  }

  // GO TO BEG. OF MIDDLE LINE
  currentoffset = lseek(fd, ((middleoffilelinenum-1) * dictWidth), SEEK_CUR); // Go to the BEGINNING of the middle line
  if (currentoffset == -1){ // Error check after using LSEEK
    fprintf(stderr, "Error: %s\n", strerror(errno));
    return errno;
  }
  // READ IN MIDDLE LINE
  numbytesread = read(fd, buffer, dictWidth); // Read in as many characters as the dictionary is wide and store in buffer
  buffer[dictWidth-1] = '\0'; // Append null terminator in place of newline
  if (DEBUG) printf("contents of buffer: '%s'\n", buffer);
  if (DEBUG) printf("contents of truncatedword: '%s'\n", truncatedword);  

  if (numbytesread < 0){ // If there was an error (dictWidth bytes NOT read or NOT at EOF) then return that error
    fprintf(stderr, "Error: %s\n", strerror(errno));
    return errno;
  }
  
  // MOVE CURSOR BACK TO BEGINNING
  currentoffset = lseek(fd, 0, SEEK_SET); // Move file offet to beginning of file
  if(currentoffset == -1){ // Error check after using LSEEK
    fprintf(stderr, "Error: %s\n", strerror(errno));
    return errno;
  }
  
  // COMPARE WORD W MIDDLE LINE IN BUFFER
  result = (strcmp(truncatedword, buffer)); // Use strcmp to compare word with middle line stored in buffer
  if (DEBUG) printf("result of strcmp: %d\n", result);

  if (result == 0){ // word and buffer are identical, word match found
    if (DEBUG) printf("Found word!\n");
    stillsearching = 0; // Change flag, word found
    return (middleoffilelinenum);
  }

  if (result < 0){ // word is earlier in dictionary than word in buffer/middle line
    highestlinenum = (middleoffilelinenum - 1); // Recalibrate highestlinenum to be middle line - 1
  }

  if (result > 0){ // word is later in dictionary than word in buffer/middle line
    lowestlinenum = (middleoffilelinenum + 1); // Recalibrate lowestlinenum to be middle line + 1
  }
}

free(truncatedword); // Free memory we malloced earlier
close(fd); // Close the file we opened

if (middleoffilelinenum >= numberoflines){
  if (DEBUG) printf("Beyond last word in dictionary\n");
  return ((middleoffilelinenum) * -1); // Word not found, word beyond last word in current dictionary
}

else{
return ((middleoffilelinenum + 1) * -1); // Word not found, return negative of last line found
}

};
