.PHONY: rm rerun

rm:
	rm main

main:
	g++ main.cpp -o main

rerun: rm main
	./main