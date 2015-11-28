# Gary
Simple procedural programming language for general purpose scripts.

_**Gary**_ is case insensitive. This means that keywords can be upper case, lower case or even a mix of both and still work. Also, note that semicolons are not necessary at the end of each line but are optional. Programming statements are logically separated by new lines. However, if you want multiple programming statements on the same line, a semicolon is necessary at the end of each line. Below are the features of the language and how they are used by default. The keywords can be customised to use other words but the idea is the same. 

_**Note: While some features (docmented below) are available, Gary is still a work in progress. Please stay tuned for a stable release.**_

### New! Gary mini IDE ###
![Screenshot](https://github.com/oduwa/Gary/blob/master/ReadmeResources/screen1.png)

### Variable Types and Assignment
Values are assigned to variables using the << operator. Gary is a weakly-typed language. Basically, this means that you do not need to specify the type of a variable when you declare it. Its type is inferred from the value it is assigned.

```Python
@firstName << "John";
@lastName << "Doe";
@age << 100;
@height << 6.1;
```

### Output
Use the ```disp``` keyword to print to the console.

```Python
disp "Hello World!";
```

### Comments
Use the "#"  character to denote that a line contains comments and should not be processed. Note that this character must be at the start of the line and no other programming statements can be on that line.

```Python
# This is a comment
disp "There's a comment above!";
```

### Conditionals
Gary provides if, else and elseif-statements as shown below

```Python
if @varA == @varB then
  disp 1
elif 1 > @varA then
  disp 2
else
  disp 3
endif;
```

### Iteration
Gary supports iteration via while-loops. An expression is specified as a condition after the *while* keyword. As long as this expression evalueates to 1, the code within the while loop will be executed.

``` Python
@varA << 0;
while 10 >= @varA then
	increment @varA;
	disp @varA;
endwhile
```

(*The ```increment``` and ```decrement``` keywords are the Gary equivalent of ++ and -- in other mainstream languages*)


### Functions
With Gary, functions are declared using the ```func``` keyword, followed by the function name and a list of arguments between brackets and the the function return type. After the function is defined, its scope should be closed using the ```endfunc``` keyword.

``` Python
func myFunc() void:
	disp "printing from my function"; 
endfunc

myFunc();
```

Functions don’t need to be declared or defined before they’re called. They just need to be defined at some point.

### Lists
Lists are created using the built-in ```CREATE_LIST(TEXT type, NUMBER size)``` function. Lists can be manipulated with add and remove operations provided by the built-in ```ADD_TO_LIST(VAR list, VAR value)```, ```PUT_IN_LIST(VAR list, NUMBER index, VAR value)``` and ```REMOVE_FROM_POSITION_IN_LIST(VAR list, NUMBER index)``` functions. *Note that lists are indexed starting from 1 and not 0.*

``` Python
# Create list of strings
@words << CREATE_LIST(string, 5);

# Manipulate lists
ADD_TO_LIST(@words, "pie");
ADD_TO_LIST(@words, "sky");
ADD_TO_LIST(@words, "tie");
REMOVE_FROM_POSITION_IN_LIST(@words, 3);
PUT_IN_LIST(@words, 2, "sky");

# Print list
disp @words;
```

