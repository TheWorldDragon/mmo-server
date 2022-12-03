lib= -lprotobuf-c
debug=-g
all:hinge client user
obj:
	clang *.c -c $(debug)
hinge:obj
	clang hinge.o user-acceptor.o copy-worker.o $(lib) $(debug) -o hinge 
client:obj
	clang client.o util.o game.pb-c.o $(lib) $(debug) -o client 
user:obj
	clang user.o util.o game.pb-c.o $(lib) $(debug) -o user

