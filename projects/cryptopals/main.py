import os
import sys
import subprocess


def main(directory: str):

    if not os.path.isdir(directory):
        print(f"The path '{directory}' is not a valid directory.")
        return

    python_files = [file for file in os.listdir(directory) if file.endswith(".py")]

    if not python_files:
        print("No Python files found in the directory.")
        return

    print(f"Found {len(python_files)} Python file(s). Executing them...\n")

    for python_file in python_files:
        file_path = os.path.join(directory, python_file)
        print(f"Running: {python_file}")
        try:
            result = subprocess.run(
                ["python", file_path], capture_output=True, text=True
            )
            print(f"Output of {python_file}:\n\n{result.stdout}")
            if result.stderr:
                print(f"Error in {python_file}:\n{result.stderr}")
        except Exception as e:
            print(f"An error occurred while running {python_file}: {e}")
        print("-" * 50)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python main.py <directory>")
        sys.exit(1)
    main(sys.argv[1])
