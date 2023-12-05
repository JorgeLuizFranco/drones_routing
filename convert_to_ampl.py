import sys

def convert_input_to_params(input_str):
    input_lines = input_str.strip().split('\n')
    N, M, K = map(int, input_lines[0].split())
    i_start, j_start, i_end, j_end = [], [], [], []

    drone_k = 1
    for line in input_lines[1:]:
        i_begin, j_begin, i_end_d, j_end_d = map(int, line.split())
        i_start.append(drone_k)
        i_start.append(i_begin + 1)
        j_start.append(drone_k)
        j_start.append(j_begin + 1)
        i_end.append(drone_k)
        i_end.append(i_end_d + 1)
        j_end.append(drone_k)
        j_end.append(j_end_d + 1)

        drone_k += 1

    x_max = max(i_start + i_end)
    y_max = max(j_start + j_end)

    params_str = (
        f"param N := {N};\n"
        f"param M := {M};\n"
        f"param K := {K};\n"
        f"param x_max := {N};\n"
        f"param y_max := {M};\n"
        f"param a := 1.0;\n"
        f"param b := 1.0;\n"
        f"param T := {(N + M) * K};\n"
        f"param i_start := {' '.join(map(str, i_start))};\n"
        f"param j_start := {' '.join(map(str, j_start))};\n"
        f"param i_end := {' '.join(map(str, i_end))};\n"
        f"param j_end := {' '.join(map(str, j_end))};"
    )

    return params_str

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 convert_to_ampl.py <filename.in>")
        sys.exit(1)

    input_filename = sys.argv[1]

    with open(input_filename, "r") as file:
        input_str = file.read()

    params_str = convert_input_to_params(input_str)

    output_filename = f"/AMPL_input/{input_filename.replace('.in', '.dat')}"
    with open(output_filename, "w") as output_file:
        output_file.write(params_str)

    print(f"Conversion successful. Output written to {output_filename}")

if __name__ == "__main__":
    main()
