// File: pp2-hint0.cpp
// Author: Yudan(Maggie) Chen, modified from code by Steve Wolfman
//
// Want to use this file?  CHANGE THE AUTHOR AND ACKNOWLEDGE STEVE!  
// 
// This work is licensed under a <a rel="license"
// href="http://creativecommons.org/licenses/by/3.0/">Creative Commons
// Attribution 3.0 Unported License</a>.
//
// Purpose: Generates

#include <cstdio>    // for printf, ..
#include <cstdlib>   // for atoi, exit, ..
#include <stdexcept> // for domain_error, ..
#include <sstream>   // for stringstream, ..
#include <utility>   // for pair, ..

// Why use printf, by the way?  Because it's faster than cout, which
// doesn't much matter usually, but for outputting a couple million
// lines, it might.


using namespace std;

// Output a usage statement.
void usage(char * program) {
  printf("Usage: %s <n>\n", program);
  printf("  <n>   A positive integer, the exact number of lines of\n");
  printf("        output (commands) to produce.\n\n");
}

// Attempt to extract the command line arguments (just the <n>
// argument) from the command line.  Prints usage and exits on
// failure.
int getNArg(int argc, char * argv[]) {
  // Need exactly two elements in argv: the name of the command and
  // <n>.
  if (argc != 2) {
    usage(argv[0]);
    printf("Expected 1 argument, received: %d.\n", (argc-1));
    exit(1);
  }

  // On an error translating the string to an integer, the return
  // value is 0.  In any case, a 0 or negative value is an error (must
  // be positive).
  int n = atoi(argv[1]);
  if (n == 0 || n < 0) {  // two tests emphasize 0 as a special case
    usage(argv[0]);
    printf("Expected a positive integer for <n>, received: %s.\n", argv[1]);
    exit(1);
  }

  return n;
}

// Output a single command in the appropriate format.  Also
// double-checks that the command will be valid, throwing an exception
// if it's not.
void output_command(char command, int key) {
  if (command != 'F' && command != 'I' && command != 'R') {
    stringstream s;
    s << "Command must be one of F, I, or R.  Received " << command << " instead." << endl;
    throw domain_error(s.str());
  }

  if (key <= 0) {
    stringstream s;
    s << "Key must be positive.  Received " << command << " instead." << endl;
    throw domain_error(s.str());
  }
  
  printf("%c %d\n", command, key);
}


// Generate the appropriate command, given the total number of
// commands to produce and the index of the current command.  (It's
// certainly possible to do something interesting here without using
// n, although this example needs it.)
//
// Expects index to be in the range [1, n].  (NOT from 0 to n-1.)
pair<char, int> generate_command(int n, int index) {
  // As an example, we want to generate a series of insert commands, I
  // 1, I 2, .., I k.  Then, a series of finds F 1, F2, .., F k.
  // Finally, a series of removes.  Since we're not allowed to remove
  // something that hasn't been inserted, we do some careful math
  // (figured out by thinking, sketching examples, and a bit of trial
  // and error) to ensure that the roughly n/3 inserts are actually at
  // least as many as the roughly n/3 finds, which are actually at
  // least as many as the roughly n/3 removes.

  //const int I_LIMIT = (n-1)/3 + 1;
  //const int F_LIMIT = (2*n-1)/3 + 1;
  const int I_LIMIT = n;
  const int F_LIMIT = n;


  char command;

  // What might happen if instead of making the first third inserts,
  // the next third finds, and the next third removes, we instead
  // chose based on index % 3 (or just alternated between two of the
  // types by choosing based on index % 2)?
  if (index <= I_LIMIT) {
    command = 'I';
    // do nothing to index.
  }
  else if (index <= F_LIMIT) {
    // What might happen if we always set index to I_LIMIT?  To 1?
    command = 'F';
    index -= I_LIMIT;  // "shift" over to the range 1 .. ~n/3.
  }
  else {
    command = 'R';
    index -= F_LIMIT;  // "shift" over to the range 1 .. ~n/3.
  }

  return pair<char, int>(command, index);
}

int main(int argc, char * argv[]) {
  int n = getNArg(argc, argv);

  for (int i = 1; i <= n; i++) {
    pair<char, int> command = generate_command(n, i);
    output_command(command.first, command.second);
  }

  return 0;
}