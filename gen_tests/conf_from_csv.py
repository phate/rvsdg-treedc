import sys

def create_test(finput, foutput):
    with open(finput) as f:
        csv = f.read().splitlines()

    print("Converting from csv graph input {} to config in {}".format(finput, foutput))

    with open(foutput, 'w') as f:
        for line in csv:
            if line.startswith("graph;"): continue
            line = line.split(";")
            graph6 = line[0].strip('"')
            lb = line[2]
            ub = line[3]
            submitted = line[5]
            if submitted and int(submitted) > 0: # ignore graphs with no submitted solutions
                f.write("{} {} {}\n\n".format(graph6, ub, lb))

if __name__ == "__main__":
    argc = len(sys.argv) - 1
    if argc == 2:
        create_test(*sys.argv[1:])
    else:
        print("Expected 2 args but got {}".format(argc))
