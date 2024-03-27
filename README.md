# pipex

## Overview

Welcome to the Pipex project! The purpose of this project is the discovery in detail UNIX mechanism - `pipe`, processes, and inter-process communication in Unix-like operating systems.
In this README, we'll discuss the key concepts covered in the Pipex project


### Inter-Process Communication:

`Interprocess communication (IPC)` is the transfer of data among processes. For example, a
Web browser may request a Web page from a Web server, which then sends HTML data. This
transfer of data usually uses sockets in a telephone-like connection. In another example, you
may want to print the filenames in a directory using a command such as `ls | lpr`. The shell
creates an ls process and a separate lpr process, connecting the two with a pipe, represented
by the “|” symbol. A pipe permits one-way communication between two related processes.
The ls process writes data into the pipe, and the lpr process reads data from the pipe.

We use the term pipe to mean connecting a data flow from one process to another. Generally
you attach, or pipe, the output of one process to the input of another. Most Linux users will
already be familiar with the idea of a pipeline, linking shell commands together so that the
output of one process is fed straight to the input of another. For shell commands, this is done
using the pipe character to join the commands, such as


![Alt Text](/images/command.png)


#### There are five types of interprocess communication:

- **Shared Memory:**
  - Shared memory permits processes to communicate by simply reading and writing to a specified memory location.

- **Mapped Memory:**
  - Mapped memory is similar to shared memory, except that it is associated with a file in the filesystem.

- **Pipes:**
  - Pipes permit sequential communication from one process to a related process.

- **FIFOs (Named Pipes):**
  - FIFOs are similar to pipes, except that unrelated processes can communicate because the pipe is given a name in the filesystem.

- **Sockets:**
  - Sockets support communication between unrelated processes even on different computers.

as we know in this project we work with only unnamed pipes lets talk a litter bit about it
**Unnamed Pipes**
An unnamed pipe, also known as an anonymous pipe, is a communication mechanism that allows one-way communication between processes running on the same system.
It's typically used to connect the standard output (stdout) of one process to the standard input (stdin) of another process. Unnamed pipes are created using the `pipe()` system call .
pipes typically use a fixed-size `4KB`


#### How Pipes Work in Unix

When you create a pipe, it establishes a unidirectional data flow between processes. For example, in an unnamed pipe scenario, Process A writes data to the write end of the pipe, and Process B reads data from the read end of the pipe. This enables data to be transferred from one process to another seamlessly.

you didn't get it !!. let's make it simple

this the pipe how is look like


![Alt Text](/images/pipe.png)


   - The standard input for `cmd1` comes from the terminal keyboard.
   - The standard output from `cmd1` is connected to the pipe.
   - The standard input for `cmd2` comes from the pipe.
   - The standard output from `cmd2` is connected to the terminal screen.


![Alt Text](/images/pipe_work.png)


### Fork:

The Fork system call is used for creating a new process in Linux, and Unix systems, which is called the child process, which runs concurrently with the process that makes the fork() call (parent process). After a new child process is created, both processes will execute the next instruction following the fork() system call.
The child process uses the same pc(program counter), same CPU registers, and same open files which use in the parent process. It takes no parameters and returns an integer value.


![Alt Text](/images/fork.png)


On success fork returns twice: once in the parent and once in the child. After calling fork,
the program can use the `fork` return value to tell whether executing in the parent or child.


  - If the return value is `0`: The program executes in the new child process.
  - If the return value is greater than zero : The program executes in the parent process, and the return value is the process ID (PID) of the created child process.
  - On failure : `fork()` returns `-1`, indicating an error or failure in creating a new process.

`Fork system call is used to create a separate, duplicate process.`


![Alt Text](/images/fork_work.png)


If you just have a series of N fork statements one after the other, then the
total number of processes formed are 2^N.


![Alt Text](/images/equation.png)


You can visualize this in the form of a full binary tree, where the number of
serial fork statements is the height of the tree and the number of leaves is the
total number of processes formed.


![Alt Text](/images/binary_tree.jpg)


If we continue this fork process with out any control we may be threatened by **zombies process**

#### zombies process :


![Alt Text](/images/zombie.png)


A zombie process is a process that has terminated but has not been cleaned up yet. It is the responsibility of
the parent process to clean up its zombie children. The wait functions do this. If the child process has not
terminated at that point, the parent process will block in the wait call until the child process finishes. If the
child process finishes before the parent process calls wait, the child process becomes a zombie. When the
parent process calls **waitpid**, the zombie child’s termination status is extracted, the child process is deleted, and
the wait call returns immediately.


### dup2

The `dup2()` system call duplicates an existing file descriptor and makes it refer to another file descriptor. It's commonly used for input/output redirection and setting up communication channels between processes.

### execve

The `execve()` system call replaces the current process with a new process. It loads a new program into the current process's memory space and starts executing it.



# Discription of mandatory part
The program `pipex` should repeat the behaviour of the next shell command
```bash
$> < file1 cmd1 | cmd2 > file2
```
and looks like this:
```bash
$> ./pipex file1 cmd1 cmd2 file2
```
All errors like: wrong commands,  permission to files and etc, need be handle.
### Discription of bonus part
Program need handle multiple pipes
```bash
$> < file1 cmd1 | cmd2 | cmd3 | ... | cmdn > file2

$> ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
```
Support `<<` and `>>`, when instead of `file1` is`here_doc`.
```bash
$> cmd1 << LIMITER | cmd2 >> file2

$> ./pipex here_doc LIMITER cmd1 cmd2 file2
```


