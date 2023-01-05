# socket-t3
tic tac toe using sockets

### HOW TO RUN
Prerequisites: `Docker`, `Make`

To play, you need to create the environment to run this program. On your first
execution, run:

```bash
make create && make env
```

On future executions run:

```bash
make docker-clean && make create && make env
```

Inside the environment, launch the server with
```bash
./build/src/game --server
```

In another terminal window, access the same environment with `make env` and run
```bash
./build/src/game --client
```

To play, just enter the co-ordinates of the box you want to fill. The game will
accept input in the following formats: `(0,1)`, `0,1`, `0, 1`, `0 1`
