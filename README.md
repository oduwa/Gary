# Gary
Simple procedural programming language for general purpose scripts.

![Screenshot](https://github.com/oduwa/Gary/blob/master/ReadmeResources/screen2.png)

_**Gary**_ is case insensitive. This means that keywords can be upper case, lower case or even a mix of both and still work. Also, note that semicolons are not necessary at the end of each line but are optional. Programming statements are logically separated by new lines. However, if you want multiple programming statements on the same line, a semicolon is necessary at the end of each line. Below are the features of the language and how they are used by default. The keywords can be customised to use other words but the idea is the same. 

_**Note: While some features (docmented below) are available, Gary is still a work in progress. Please stay tuned for a stable release.**_

### Setup and Keyword Customisation ###
Simply download the repository and copy the _dist_ directory to wherever you want to use it. To start writing programs in _**Gary**_, simply run the *GaryEditor.jar* file to start the mini IDE and get cracking! The default setup of syntax and keywords is described in the following sections. To setup your own keywords, use the _KeywordConfig_ command line program distributed with _**Gary**_. Spaces are not allowed in custom keywords. To separate words in a keyword, the underscore character ("\_") may be used instead. The signature of the tool is as follows:

Unix:
```bash
./KeywordConfig [OPTIONS]
```

Windows:
```bash
KeywordConfig.exe [OPTIONS]
```

An example command for how this program can be used is as follows -

```bash
# This command will change the "disp", "increment" and
# "while" keywords to "print", "plusplus" and "round_and_round".
#
# On Windows, replace "./KeywordConfig" with "KeywordConfig.exe"
./KeywordConfig -disp print -pls plusplus -while round\_and\_round
```

The table at the bottom of this document shows a list of the available options for the _KeywordConfig_ tool.


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

### Index

| Option | Description          |
| ------------- | ----------- |
| -if      | Replacement for _**if**_ keyword |
| -then     | Replacement for _**then**_ keyword     |
| -else      | Replacement for _**else**_ keyword |
| -elif      | Replacement for _**elif**_ keyword |
| -endif      | Replacement for _**endif**_ keyword |
| -while      | Replacement for _**while**_ keyword |
| -ewhile      | Replacement for _**endwhile**_ keyword |
| -assgn      | Replacement for _**<<**_, the assignment operator |
| -and      | Replacement for _**AND**_ keyword |
| -or      | Replacement for _**OR**_ keyword |
| -e      | Replacement for _**==**_, the equality operator |
| -disp      | Replacement for _**disp**_ keyword |
| -ret      | Replacement for _**return**_ keyword |
| -func      | Replacement for _**func**_ keyword |
| -efunc      | Replacement for _**endfunc**_ keyword |
| -lmake      | Replacement for _**CREATE\_LIST**_ keyword |
| -ladd      | Replacement for _**ADD\_TO\_LIST**_ keyword |
| -lput      | Replacement for _**PUT\_IN\_LIST**_ keyword |
| -lrmv      | Replacement for _**REMOVE\_FROM\_LIST**_ keyword |
| -pls      | Replacement for _**INCREMENT**_ keyword |
| -mns      | Replacement for _**DECREMENT**_ keyword |


