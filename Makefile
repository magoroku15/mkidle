All:inflp mkidle

inflp: inflp.c
	cc inflp.c -o inflp

mkidle: mkidle.c
	cc mkidle.c -lrt -o mkidle

clean:
	rm mkidle inflp
