#!/bin/bash

EXECUTABLE="./multi_bfs"
OUTPUT_FILE="execution_time.txt"

for ((i = 1; i <= 10; i++)); do
    TEST_CASE="${i}.in"

    # Record start time
    start_time=$(date +%s.%N)

    # Run the executable with input from the test case and redirect output
    ${EXECUTABLE} < ${TEST_CASE} #> /dev/null

    # Record end time
    end_time=$(date +%s.%N)

    # Calculate execution time
    execution_time=$(echo "${end_time} - ${start_time}" | bc)

    # Print and redirect execution time to file with two newline characters
    echo -e "Test Case ${i}: Execution Time = 0${execution_time} seconds\n\n" >> ${OUTPUT_FILE}
done
