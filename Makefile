all: src/client.c
	$(shell mkdir -p build)
	gcc src/client.c src/db_connection_pool.c src/task_queue.c -Ithird_party/include -Lthird_party/lib -lpq -o build/client -pthread

client_nopool: src/client_nopool.c
	$(shell mkdir -p build)
	gcc src/client_nopool.c -Ithird_party/include -Lthird_party/lib -lpq -o build/client_nopool

client: all
	./build/client

client1: client_nopool
	./build/client_nopool

clean:
	$(shell rm -rf build)

