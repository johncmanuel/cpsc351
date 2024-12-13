import os
import sys
import subprocess


def main(directory: str | None = None):

    print(os.path.dirname(os.path.abspath(__file__)))

    if not directory:
        directory = os.path.dirname(os.path.abspath(__file__))

    if not os.path.isdir(directory):
        print(f"The path '{directory}' is not a valid directory.")
        return

    python_files = sorted(
        [
            file
            for file in os.listdir(directory)
            if file.endswith(".py") and file != "main.py"
        ]
    )

    if not python_files:
        print("No Python files found in the directory.")
        return

    for python_file in python_files:
        file_path = os.path.join(directory, python_file)
        print(f"Running: {python_file}")
        try:
            result = subprocess.run(
                ["python", file_path],
                capture_output=True,
                text=True,
                cwd=os.path.dirname(os.path.abspath(__file__)),
            )
            print(f"Output of {python_file}:\n\n{result.stdout}")
            if result.stderr:
                print(f"Error in {python_file}:\n{result.stderr}")
        except Exception as e:
            print(f"An error occurred while running {python_file}: {e}")
        print("-" * 50)


if __name__ == "__main__":
    if len(sys.argv) == 1:
        main()
    else:
        main(sys.argv[1])
