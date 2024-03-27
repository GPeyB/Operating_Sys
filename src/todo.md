# TODO

- & operator
- signal handling
- `jobs`: list background processes
- `kill`: send signal to background process

- make a pipeline with & not wait for it to complete before running the next one

when we start a pipeline in the background, we don't want to wait for it.
we add it to the processlist.

when `exit` is called, we can check if there are any background processes running.
if there are, we print a message and return to the shell prompt, returning 2.
when eof is reached, we do exit immediately (without memory leaks).

the exit code of a background process is 0.
