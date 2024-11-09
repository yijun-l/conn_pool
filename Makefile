all: src/client.c
	$(shell mkdir -p build)
	gcc src/client.c src/av_cp.c -Ithird_party/include -Lthird_party/lib -lpq -o build/client -pthread

client: all
	./build/client

clean:
	$(shell rm -rf build)

