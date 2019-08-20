Checking for nested brackets in a text file.

Most programming languages require that brackets (round brackets, square brackets, curly braces) 
are "balanced" and "nested." This means that every opening bracket should have a matching closing 
bracket of the same type and that two matching pairs should completely contain one inside the 
other. Thus, {()} and [() ()] are examples of nested brackets, while {(}) and (())) are not nested. 

These rules hold for C++ too. However, brackets that appear within comments and quoted strings do not count towards nesting. Thus, these are valid: 

int i = m * (5 + n); // comment: this ( does not count 
cout << "Sum of [4, 3, ... = " << (4 + (3 + (2 + 1))) << endl;
