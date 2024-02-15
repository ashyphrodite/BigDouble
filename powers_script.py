'''
Generates the hardcoded powers of 10 for the lookup table
'''

DOUBLE_EXP_MIN = -307
DOUBLE_EXP_MAX = 308
ELEMENTS_PER_LINE = 30

with open('output.txt', 'w') as f:
    f.write("static const double Powers[] = {\n")
    for i in range(DOUBLE_EXP_MIN, DOUBLE_EXP_MAX + 1, ELEMENTS_PER_LINE):
        f.write("    " + ", ".join(f"1E{j}" for j in range(i, min(i + ELEMENTS_PER_LINE, DOUBLE_EXP_MAX + 1))) + ",\n")
    f.write("};\n")