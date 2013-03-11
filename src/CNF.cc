/** CS408 Project 2
    Random Walk Sat Algorithm
    Daniel Uber
*/

#include "CNF.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
/** 
    main()
    verify an argument was provided.
    read problem from file, and pass to the satisfiable function.
    if problem is not satisfiable, print NO, and exit.
*/
int
main (int argc, char **argv)
{
  if (argc < 2)
    usage (argv);
  else if (!satisfiable (read_problem_file (argv[1])))
    std::cout << "NO" << std::endl;
  return 0;
}

/** 
    usage(args)
    print message reminding user to pass a filename when calling
 */
void
usage (char **supplied_args)
{
  std::cout << "Usage: " << supplied_args[0] << " filename" << std::endl
    << "Where filename is the name of a cnf problem file." << std::endl;
  return;
}

/** 
    clause_satisfied(phi, index)
    a disjunctive clause is satisfied if any of its literals are 
    in the same sense as the current variable assignment
 */
bool
clause_satisfied (problem phi, int clause_index)
{
  for (int i = 0; i < phi->variable_count; i++)
    if (phi->clauses[clause_index][i] == phi->variables[i])
      return true;
  return false;
}


/**
   find_unsatisfied_clause(problem phi)
   just return the first clause in phi not satisfied 
   (no randomization here)
 */
clause
find_unsatisfied_clause (problem phi)
{
  for (int index = 0; index < phi->clause_count; index++)
    if (!clause_satisfied (phi, index))
      return phi->clauses[index];
  return (clause) NULL;
}

/**
   random_literal_in_clause
   select one literal present in clause at random
   first count the number of literals.
   then generate a random number mod the count
   then count the present variables until we reach the random
   number, and return the index of this variable.
 */
int
random_literal_in_clause (clause c, problem phi)
{
  int length = 0;
  int random_index;
  // calculate length of clause
  for (int index = 0; index < phi->variable_count; index++)
    if (c[index])
      length++;
  random_index = rand () % length;
  // count the present variables.
  length = 0;
  for (int index = 0; index < phi->variable_count; index++)
    if (c[index])
      {
	if (length == random_index)
	  return index;
	else
	  length++;
      }
  return 0;			// silence compiler warning
}

/** 
    find_literal_to_change(phi)
    get a random literal from and unsatisfied clause.
*/
int
find_literal_to_change (problem phi)
{
  return random_literal_in_clause (find_unsatisfied_clause (phi), phi);
}

/**
   invert_value_of_literal
   flip the state of the variable with index index in phi
 */
void
invert_value_of_literal (int index, problem phi)
{
  phi->variables[index] = (assignment) (-1 * (int) phi->variables[index]);
  return;
}


/** 
    mutate_assignment
    invert the value of a random literal in an unsatisfied clause
*/
void
mutate_assignment (problem phi)
{
  invert_value_of_literal (find_literal_to_change (phi), phi);
  return;
}

/**
   generate_random_assignment()
   set all variables in phi, randomly
 */
void
generate_random_assignment (problem phi)
{
  for (int index = 0; index < phi->variable_count; index++)
    phi->variables[index] = (rand () % 2) ? TRUE : FALSE;
  return;
}

/**
   max_starts_for
   calculation required for schonings algorithm
 */
unsigned long int
max_starts_for (problem phi)
{
  int n = phi->variable_count;
  // 61.3996 is 20 times sqrt(3*pi)
  return (unsigned long int) ceil (61.3996 * sqrt (n) * pow (1.3333, n));
}

/**
   max_loops_for
   calculation required for schonings algorithm
 */
int
max_loops_for (problem phi)
{
  return 3 * phi->variable_count;
}

/** 
    satisfiable(phi)
    given a non-null phi, implement schonings algorithm
    if phi is satisfied, print satisfying assignment and return true
    if no satisfying assignment is found, return false
 */
bool
satisfiable (problem phi)
{
  unsigned long int max_starts;
  int max_loops;
  unsigned long int current_start = 0;
  srand (time (NULL));
  // start
  if (phi)
    {
      max_starts = max_starts_for (phi);
      max_loops = max_loops_for (phi);
      while (current_start++ < max_starts)
	{
	  int current_loop = 0;
	  generate_random_assignment (phi);
	  while (!satisfied (phi) && current_loop++ < max_loops)
	    mutate_assignment (phi);
	  if (satisfied (phi))
	    {
	      print_assignment (phi);
	      release_problem (phi);
	      return true;
	    }
	}
    }
  // we failed to find an assignment, there likely isn't one
  return false;
}

/**
   satisfied(phi)
   if there are no unsatisfied clauses, then phi has been satisfied.
 */
bool
satisfied (problem phi)
{
  for (int index = 0; index < phi->clause_count; index++)
    if (!clause_satisfied (phi, index))
      return false;
  return true;
}


/**
   print_assignment(phi)
   if phi has been satisfied, we can read of the variables from the variable
   list, and print them as our result.
   if some variables were not assigned in the process of satisfying phi
   then they are free, and we assign them true.
 */
void
print_assignment (problem phi)
{
  for (int index = 0; index < phi->variable_count; index++)
    {
      std::cout << std::setw (4)
	<< (index + 1) * (phi->variables[index] ? phi->variables[index] : 1)
	<< "  ";
      if (!((index + 1) % 10))
	std::cout << std::endl;
    }
  std::cout << std::endl << std::endl;
  return;
}

/**
   empty_form_of_size(clauses, variables)
   allocate a problem structure and its lists
   initialize all values to 0. (unassigned/not-present)
   return pointer to the new problem
 */
problem
empty_form_of_size (int clauses, int variables)
{
  problem phi = (problem) malloc (sizeof (problem_struct));
  phi->variable_count = variables;
  phi->clause_count = clauses;
  phi->clauses = (form) calloc (clauses, sizeof (clause));
  phi->variables = (variable_list) calloc (variables, sizeof (assignment));
  return phi;
}

/**
   release_problem(phi)
   free() allocated resources for the problem struct.
   only free memory we allocated for fresh clauses
   don't clobber parent
 */
void
release_problem (problem phi)
{
  if (phi)
    {				// don't try to look inside a null pointer
      free (phi->variables);
      for (int i = 0; i < phi->clause_count; i++)
	{
	  free (phi->clauses[i]);
	}
      free (phi->clauses);
      free (phi);
    }
  return;
}


/** print_form(phi)
    dumps struct info to standard output
    for debugger inspection
    in gdb: call print_form(phi) where phi is in the local context
*/
void
print_form (problem phi)
{
  std::cout << "variables: ";
  for (int i = 0; i < phi->variable_count; i++)
    std::cout << i + 1 << ":" << (int) phi->variables[i] << " ";
  std::cout << std::endl;

  for (int c = 0; c < phi->clause_count; c++)
    {
      if (phi->clauses[c])
	{
	  for (int v = 0; v < phi->variable_count; v++)
	    if (phi->clauses[c][v])	// skip unassigned
	      std::cout << c + 1 << ":" << v +
		1 << ":" << (int) phi->clauses[c][v] << " ";
	  std::cout << std::endl;
	}
    }
  return;
}
