import os

def execute_command(code):
    commands = {
        '0': 'cmake -GNinja',
        '1': 'git pull --recurse-submodules origin master',
        '2': 'ninja RmpGTests && ctest -V -R AbcTest.TestFlow',        
        '3': 'ninja RmpGTests && ctest -V -R AbcTest.DeepSynthesisFlow'
    }

    command = commands.get(code)
    if command:
        os.system(command)
    else:
        print("Invalid code. Please enter 0, 1,2....")

if __name__ == "__main__":
    code = input("Enter the code (0, 1, 2, ...): ")
    execute_command(code)
