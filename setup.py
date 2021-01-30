import os

print("What is your OS, Window for 1, Linux for 2")
n = int(input())
if n == 1:
    print("No need to compile the main.cpp, the main.exe is exist in default")
elif n == 2:
    os.system("gcc -o main main.cpp")
