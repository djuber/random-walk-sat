/** CS408 Project 2
    Daniel Uber
    Random Walk Sat Solver
*/

#ifndef CNF_H
#define CNF_H

// define types

// our possible assignment states for each variable
// variables will be randomly assigned TRUE or FALSE by algorithm
// only clauses can have variables unassigned.
enum assignment
{
  FALSE = -1,
  UNASSIGNED = 0,
  TRUE = 1
};

// variable list holds assignments in our problem to this point
typedef assignment *variable_list;
// a clause is an array of assignments
typedef assignment *clause;
// a cnf formula is an array of clauses
typedef clause *form;

// problem_struct is the representation of a cnf form plus the 
// assigned interpretations, and information about the problem
// i.e. the number of variables and clauses
struct problem_struct
{
  variable_list variables;	// current interpretation
  form clauses;			// the cnf formula
  int variable_count;		// variable array bound
  int clause_count;		// clauses, clause_length, and fresh_clause bound
};

// utility typedef
typedef struct problem_struct *problem;

// declarations

// print help information. Argv passed to show program name
void usage (char **args);

// main algorithm function. True if a satisfying assignment was found
bool satisfiable (problem phi);

// test if all clauses have been satisfied 
bool satisfied (problem phi);

// test if a particular clause has been satisfied
bool clause_satisfied (problem phi, int clause_index);

// find some clause not satisfied at present.
clause find_unsatisfied_clause (problem phi);

// pick a literal in a clause to flip
int random_literal_in_clause (clause c, problem phi);



// load form phi from cnf file
problem read_problem_file (const char *filename);

// print assigned variables, assigning TRUE to free variables
void print_assignment (problem phi);

// release allocated resources
void release_problem (problem phi);

// problem creation/allocation utility functions
problem empty_form_of_size (int clauses, int variables);

// used for debugging
void print_form (problem phi);

// calculate needed values for schonings algorithm.
unsigned long int max_starts_for (problem phi);
int max_loops_for (problem phi);



#endif // CNF_H
