def main():
	print("Height: ", end="")
	height = input()
	mario(int(height))

def mario(height):
	for i in range(1, height+1):
		print("{}{} {}".format((height-i) * " ", i * "#", i * "#"))

if __name__ == "__main__":
	main()