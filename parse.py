import pandas as pd
import re

# Define the log file path
log_file_path = "logs/all_runs.log"

# Initialize list to store extracted data
data = []

# Read the log file and extract information
with open(log_file_path, "r") as file:
    lines = file.readlines()
    for i in range(len(lines)):
        line = lines[i].strip()
        if line.startswith("Running:"):
            match = re.search(r"mpirun -np (\d+) ./MyMPIProject --filter_type=(\w+) --filter_number=(\d+) --problem_size=(\d+)", line)
            if match:
                np, filter_type, filter_number, problem_size = match.groups()
                time_taken_line = lines[i + 3].strip()  # Time taken line is always 3 lines below
                time_taken_match = re.search(r"Time taken: ([\d\.]+) seconds", time_taken_line)
                if time_taken_match:
                    time_taken = float(time_taken_match.group(1))
                    data.append([int(np), filter_type, int(filter_number), int(problem_size), time_taken])

# Convert to DataFrame
df = pd.DataFrame(data, columns=["np", "filter_type", "filter_number", "problem_size", "time_taken"])


# Display the DataFrame
df.to_csv('filename.csv', index=False)
