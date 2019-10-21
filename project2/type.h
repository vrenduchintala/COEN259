# ifndef TYPE_H
# define TYPE_H

#include <vector>

using namespace std;

struct type {
  int specifier;
	int declarator; //scalar array function
  vector<pair<int, int>> args; //function arguments
};

# endif /* TYPE_H */
