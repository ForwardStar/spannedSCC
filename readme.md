## Span-SCC solutions

The repository includes three solutions to solve the span-SCC queries: ``D-online``, ``D-baseline`` and ``RES-index``.

How to use it (you need to run the scripts on a Linux platform):

- Run ``graph-gen.sh`` to generate graph data automatically, which would download datasets and process the data into ``graph.txt``:

```sh
sh graph-gen.sh
```

- Run ``query-gen.sh`` to generate query data automatically, which would write queries into ``query.txt``:

```sh
sh query-gen.sh
```

- Run the following command to run the solutions:

```sh
sh run.sh $1
```

where ``$1`` is ``Online``, ``Baseline`` or ``RES``, corresponding to ``D-online``, ``D-baseline``, ``RES-index`` solutions, respectively.

The output is in the file ``output.txt``.

## Why do I fail to execute the program with the instruction?
If you fail to execute the step:

```sh
sh graph-gen.sh
```

This may be caused by several reasons:

(1) User's environment is incompatible with the program, and our automatic dependency solver crashes;

Possible solutions: check whether your Python interpreter is installed correctly, and is newer than version 3.5; run the aforementioned command again; or install the dependencies manually by:

```sh
pip install tqdm pathlib
```

(2) User's Internet condition is poor and fails to download the datasets from KONECT and SNAP websites;

Possible solutions: check your Internet; run the aforementioned command again; or download the datasets manually by:

```sh
mkdir datasets
cd datasets
wget -c "http://konect.cc/files/download.tsv.dblp-cite.tar.bz2"
wget -c "http://konect.cc/files/download.tsv.flickr-growth.tar.bz2"
wget -c "http://konect.cc/files/download.tsv.soc-sign-bitcoinotc.tar.bz2"
wget -c "https://snap.stanford.edu/data/email-Eu-core-temporal.txt.gz"
wget -c "https://snap.stanford.edu/data/CollegeMsg.txt.gz"
cd ..
sh graph-gen.sh
```

If you fail to execute the step:
```sh
sh run.sh $1
```

This may be caused by several reasons:

(1) Users' environment is incompatible with the program;

Possible solutions: install or update your gcc and g++ compiler;

(2) User replaces ``$1`` with wrong parameters;

Possible solutions: check the parameter format in the former of this README file.

## Very large dataset: amazon-review
The amazon review dataset is so large that it is too slow to process it with our Python program. To use the dataset, you need to download it manually:

```sh
wget -c "https://jmcauley.ucsd.edu/data/amazon_v2/categoryFilesSmall/all_csv_files.csv"
```

Note that you may need to download manually on the website if you fail to execute the command above.

Then process it by:
```sh
g++ amazon_handler.cpp -o amazon_handler -O3
./amazon_handler ./all_csv_files.csv $1 $2
```

where ``$1`` is the output directory, ``$2`` is the number of edges you want to extract.