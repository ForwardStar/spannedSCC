import os
import time
import threading
import tarfile

headers = {'User-Agent':'Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.6) Gecko/20091201 Firefox/3.5.6'}

def download(url, path):
    try:
        from pathlib import Path
        from tqdm import tqdm
    except:
        print("Installing dependencies...")
        from pip._internal import main
        main(['install', 'pathlib'])
        main(['install', 'tqdm'])
        from pathlib import Path
        from tqdm import tqdm
    from urllib.request import urlopen, Request
    print("Fetching from", url + "...")
    path = Path(path)
    blocksize = 1024 * 8
    blocknum = 0
    retry_times = 0
    while True:
        try:
            with urlopen(Request(url, headers=headers), timeout=3) as resp:
                total = resp.info().get("content-length", None)
                with tqdm(
                    unit="B",
                    unit_scale=True,
                    miniters=1,
                    unit_divisor=1024,
                    total=total if total is None else int(total),
                ) as t, path.open("wb") as f:
                    block = resp.read(blocksize)
                    while block:
                        f.write(block)
                        blocknum += 1
                        t.update(len(block))
                        block = resp.read(blocksize)
            break
        except KeyboardInterrupt:
            if path.is_file():
                path.unlink()
            raise
        except:
            retry_times += 1
            if retry_times >= 20:
                break
            print("Timed out, retrying...")
    if retry_times >= 20:
        if path.is_file():
            path.unlink()
        raise ConnectionError("bad internet connection, check it and retry.")

def showProcess():
    print(waiting_message, end="  ")
    while is_finished is False:
        print('\b-', end='')
        time.sleep(0.05)
        print('\b\\', end='')
        time.sleep(0.05)
        print('\b|', end='')
        time.sleep(0.05)
        print('\b/', end='')
        time.sleep(0.05)
    if is_finished is True:
        print('\bdone')
    else:
        print('\berror!')
    
def takeThird(triple):
    return triple[2]

def move_data_file(source, destination):
    source = open(os.path.join(os.path.join('datasets', source), "out." + source), "r")
    lines = source.readlines()
    destination = open(destination, "w")
    destination.writelines(lines)

def normalize(filename):
    lines = open(filename, "r").readlines()
    contents = list()

    for line in lines:
        # omit the comments
        if '%' in line:
            continue
        
        # omit the multiplicity of edges
        line = line.split()
        contents.append([line[0], line[1], int(line[len(line) - 1])])
    
    # normalize
    contents.sort(key=takeThird)
    contents[0].append(0)
    for i in range(1, len(contents)):
        if contents[i][2] == contents[i - 1][2]:
            contents[i].append(contents[i - 1][3])
        else:
            contents[i].append(contents[i - 1][3] + 1)

    # wrap up
    text = ""
    for line in contents:
        text += line[0] + " " + line[1] + " " + str(line[3]) + "\n"
    open(filename, "w").write(text)

if __name__ == "__main__":
    # download datasets
    DATASETS_URL = ["http://konect.cc/files/download.tsv.dblp-cite.tar.bz2",
                    "http://konect.cc/files/download.tsv.flickr-growth.tar.bz2",
                    "http://konect.cc/files/download.tsv.digg-friends.tar.bz2"]
    if os.path.isdir("datasets") is False or len(os.listdir("datasets")) < len(DATASETS_URL):
        print("Downloading datasets...")
        if os.path.isdir("datasets") is False:
            os.mkdir("datasets")
        for url in DATASETS_URL:
            path = os.path.join("datasets", url.split('/')[-1])
            if not os.path.exists(os.path.join("datasets", path.split('.')[2])):
                download(url, path)

    # extract all datasets
    waiting_message = "Extracting datasets..."
    is_finished = False
    thread_extract_datasets = threading.Thread(target=showProcess)
    thread_extract_datasets.start()
    file_ls = os.listdir("datasets")
    for file in file_ls:
        if file.endswith(".tar.bz2"):
            archive = tarfile.open(os.path.join("datasets", file), "r:bz2")
            archive.extractall("datasets")
            os.remove(os.path.join("datasets", file))
    is_finished = True
    thread_extract_datasets.join()

    # clear cache
    if os.path.isfile("model"):
        os.remove("model")

    # select a target graph dataset
    file_ls = os.listdir("datasets")
    count = 1
    print("Datasets:")
    print("0. naive")
    for file in file_ls:
        print(str(count) + ".", file)
        count += 1
    user_input = input("Select a graph dataset (0-" + str(count - 1) + "): ")

    # move data file
    if user_input.strip() in [str(i) for i in range(count)]:
        waiting_message = 'Copying dataset to "graph.txt"...'
        is_finished = False
        thread_move_data_file = threading.Thread(target=showProcess)
        thread_move_data_file.start()
        if int(user_input) == 0:
            open("graph.txt", "w").write("0 1 0\n1 2 0\n2 0 0\n2 3 0\n3 2 1\n3 4 0\n4 5 0\n5 3 0")
        else:
            move_data_file(file_ls[int(user_input) - 1], "graph.txt")
        is_finished = True
        thread_move_data_file.join()
    else:
        print("Invalid input! Program terminated.")
        exit()
    
    # normalize the graph
    waiting_message = "Normalizing the graph..."
    is_finished = False
    thread_normalize = threading.Thread(target=showProcess)
    thread_normalize.start()
    if int(user_input) != 0:
        normalize("graph.txt")
    is_finished = True
    thread_normalize.join()
