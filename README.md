# file-system in c
This project is a basic implementation of a simple file system in C, allowing users to manage files and folders through a command-line interface. The file system is designed using data structures and algorithms, enabling users to create, edit, delete, and navigate through directories.
## Usage
```c
> ls
___Empty____
> pwd
/
> mkdir hi
Folder 'hi' added
> touch hello
File 'hello' added
> edit hello
Please enter some text to overwrite the file content:
hi this is my file system.
> cat hello
hi this is my file system.
> ls
0 items 01 Aug 07:57    hi
26B     01 Aug 07:58    hello
> cd hi
> ls
___Empty____
> pwd
/hi
> cd ..
> ls
0 items 01 Aug 07:57    hi
26B     01 Aug 07:58    hello
> lsrecursive
0 items 01 Aug 07:57    hi
        |____Empty____
26B     01 Aug 07:58    hello
> mov hello hi
> ls
1 items 01 Aug 07:57    hi
> cd hi
> ls
26B     01 Aug 07:58    hello
> cat hello
hi this is my file system.
> ls
26B     01 Aug 07:58    hello
> rm hello
Do you really want to remove hello and all of its content? (y/n)
y
hello and its all content is removed!
> ls
___Empty____
> cd ..
> ls
0 items 01 Aug 07:57    hi
> rm hi
Do you really want to remove hi and all of its content? (y/n)
y
hi and its all content is removed!
> exit
```
