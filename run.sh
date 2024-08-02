start=$SECONDS
file=$(find -E . -regex '.*\.(cc|cpp)' | xargs ls -t | head -1)
echo "compiling $file"
g++ -std=c++17 -O3 -Wall $file -g -fsanitize=address -fsanitize=undefined || exit 1
echo "compiled $file in $((SECONDS - start)) seconds"
start=$SECONDS
./a.out
echo "finished in $((SECONDS - start)) seconds"
