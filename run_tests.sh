#!/bin/bash

# Create a JSON file to store the results
echo "{" > results.json

# Loop over test cases
for i in {1..10}; do
    input_file="CPP_input/${i}.in"

    # Run the C++ code and capture the output
    output=$(
        ./heuristic < $input_file
    )

    # Extract information from the output
    avg_time=$(echo "$output" | awk '/Average Time:/ {print $3}')
    avg_distance=$(echo "$output" | awk '/Average Distance:/ {print $3}')
    mean=$(echo "$output" | awk '/Mean:/ {print $2}')

    # Append results to the JSON file
    echo -e "\"Test$i\": {" >> results.json
    echo -e "\t\"Avg Time\": $avg_time," >> results.json
    echo -e "\t\"Avg Distance\": $avg_distance," >> results.json
    echo -e "\t\"Mean\": $mean" >> results.json
    echo -e "}," >> results.json
done

# Remove the trailing comma from the last entry
sed -i '$ s/,$//' results.json

# Close the JSON file
echo "}" >> results.json

# Print the JSON file
cat results.json
