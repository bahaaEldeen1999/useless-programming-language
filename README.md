# Useless Programming Language

### what more is needed to be said, this is only usefull for educational purposes, or maybe to troll yourself by learning its syntax

### currently it is an interpeted language

## What is supported

1. multiple datatypes
1. variable and constant declarations
1. mathematical and logical expressions
1. Assignments statements
1. If - else statements
1. while loops
1. for loops
1. repeat until loops (this is completely redundant and can be achieved by using while loops)
1. switch statements
1. custom functions to print and toggle debug output
1. comments

### to get an idea of the syntax you can check the examples folder

## Want a taste of it

### this program calculates the Nth fibonacci, print it, and then check if this langiage is truly useless or not

```c
// toggle_debug;
int x0 = 0;
int x1 = 1;
int N = 10; // Nth fibonacci number to be calculated
int temp = 0;
for(int i=0: N : 1 )
{
    temp = x1;
    x1 = x1 + x0;
    x0 = temp;
};
print("Fib number:",string);
print(x1,int);

// check if this really a useless langiage
int can_it_do_fib = 1;
int can_it_do_functions = 0;

switch(can_it_do_fib+can_it_do_functions)
{
    case(0)
    {
        print("this is completely useless",string);
    };
    case(1)
    {
        if(can_it_do_fib)
        {
            print("at least it can do fibonacci",string);
        }
        else
        {
            print("at least it can do functions",string);
        };

    };
    default
    {
        print("this is the most useful language ever",string);
    };

};

```

#### output

> "Fib number:" <br>
> 89 <br>
> "at least it can do fibonacci"<br>
