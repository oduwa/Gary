# Gary
Simple programming language for general purpose scripts

# Getting Started

## Variable Types and Assignment
Values are assigned to variables using the << operator. Gary is a weakly-typed language. Basically, this means that you do not need to specify the type of a variable when you declare it. Its type is inferred from the value it is assigned.

```Python
@firstName << "John";
@lastName << "Doe";
@age << 100;
@height << 6.1;
```

## Output
Use the ```Python disp``` keyword to print to the console.

```Python
disp "Hello World!";
```

## Conditionals
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

## Iteration
Gary supports iteration via while-loops. An expression is specified as a condition after the *while* keyword. As long as this expression evalueates to 1, the code within the while loop will be executed.

``` Python
@varA << 0;
while 10 >= @varA then
	increment @varA;
	disp @varA;
endwhile
```

(*The ```increment``` and ```decrement``` keywords are the Gary equivalent of ++ and -- in other mainstream languages*)



