import random

def get_tmax(filename):
    result = None
    lines = open(filename, "r").readlines()
    for line in lines:
        line = line.split()
        if result is None:
            result = int(line[2])
        else:
            result = max(result, int(line[2]))
    return result

if __name__ == "__main__":
    tmax = get_tmax("graph.txt")
    contents = ""
    num_of_queries = input("Enter the number of queries to be generated: ")
    try:
        num_of_queries = int(num_of_queries)
    except:
        print("Invalid input! Program terminated.")
        exit()
    for i in range(num_of_queries):
        ts = random.randint(0, tmax)
        length = random.randint(tmax // 2, tmax)
        contents += str(ts) + " " + str(min(ts + length, tmax)) + "\n"
    open("query.txt", "w").write(contents)