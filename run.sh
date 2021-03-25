gcc -pthread -std=c99 -o thread thread.c
echo; ./thread shakespeare.txt 1
echo; ./thread shakespeare.txt 2
echo; ./thread shakespeare.txt 4
echo; ./thread hamlet.txt 1
echo; ./thread hamlet.txt 2
echo; ./thread hamlet.txt 4
