# TODO

- run program from path
- string parsing (`./a.out "string with 'special' characters"`)
- command composition (`./a.out && echo "true" || echo "false"`)
- i/o redirection (`./a.out < in > out`)
- pipes (`./a.out | ./b.out`)
- background processes (`./a.out &`)
- signals
- kill

# actual approach

- in parsing the input line, we construct some "classes" (very nice) and then execute it
