.PHONY: rm rerun

rm:
	rm main || true

main:
	g++ main.cpp -lfltk -lfltk_images -o main

rerun: rm main
	./main