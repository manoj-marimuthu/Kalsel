## Kalsel

In a world full of modern and advanced compilers, meet Kalsel.
A Programming Language written from scratch in C++ that compiles
to x86 assembly and produces an reusable executable file (elf64) by 
invoking the NASM and LD linker. Currently it works only on Linux, 
uses the NASM assembler along with LD GNU linker.

It is buggy, needs optimization and more features. I hope to remove the
previous statement in the later versions.

## Language Features and Syntax

### Arithemetic Operations

```
+ , - , *, /
```

nested unary operations (+,-) are also supported.

```
io << --1 << -10 + +-1;
```
The above gives the output "1 -11".

### Variable Declaration and Initialization

```
int x = 10;
int y = 20 + x;
int z = 30 / y;
```
You can also declare a variable first and reuse it.
```
int x;
io << x;
x = 20;
io << x;
```
For the above snippet you will get 0 first, and 20 next, since I
dont intend to store garbage value (inspired by int x{} in C++).
It currently uses 64 bits for ```int``` type.

### io

Can write to standard output using:

```
io << 10;
io << x << y + 2;
```

The '<<' chain, ensures there is a whitespace between each term just like C++.

### meta

The compiler accepts flags as command line arguments.

<table>
    <tr>
        <td>Command</td>
        <td>Purpose</td>
    </tr>
    <tr>
        <td>--version or -v</td>
        <td>Displays Compiler Version</td>
    </tr>
    <tr>
        <td>--target or -t</td>
        <td>Displays target architecture</td>
    </tr>
    <tr>
        <td>--ir</td>
        <td>Displays Kalsel IR before compilation. Raises error if file name not given for compilation</td>
    </tr>
    <tr>
        <td><filename>.kal</td>
        <td>A filename is detected using .kal extension to read source code</td>
    </tr>
    <tr>
        <td>-o</td>
        <td>Lets user to set final executable's name. Needs a filename after -o else raises error.</td>
    </tr>

</table>

## SETUP

1) Clone the repository

```
git clone https://www.github.com/manoj-marimuthu/Kalsel.git
```

2) install the nasm assmebler and ld linker. Ex - For Debian/ Ubuntu

```
sudo apt install nasm ld
```

3) run the make file

```
make
```

4) An elf file will now be produced, name 'kalsel'. 

5) to remove object files in build and the kalsel elf file in case you want
to rebuild or see your new changes run:

```
make clean
```  

6) To run the test file ```test.kal```, do:

```
./kalsel test.kal -o test --ir 
```

## Author

Manoj K M
Written with C++ and Curiosity 

#### Happy Compiling !
