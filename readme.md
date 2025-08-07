## Window-SCC solutions

This is one of the code repositories of the paper [``On Querying Connected Components in Large Temporal Graphs``](https://dl.acm.org/doi/10.1145/3589315) (SIGMOD 2023).

The repository includes three solutions to solve the window-SCC queries: ``D-online``, ``D-baseline`` and ``RES-index``.

How to use it (you need to run the scripts on a Linux platform):

- Run ``graph-gen.sh`` to generate graph data automatically, which would download datasets from [SNAP](https://snap.stanford.edu/data/index.html) and [KONECT](http://konect.cc/) and process the data into ``graph.txt``:

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

## Index update

Run the following command to simulate an index update process:

```sh
sh run.sh $1 update
```
where ``$1`` corresponds to the solution (``Baseline`` or ``RES``).

Then input the fraction of timestamps to update. For example, if the fraction is ``0.2``, the program would firstly construct an index with edges in ``[0, 0.8*tmax]``. When the index construction is finished, it starts to update the index with remaining edges in ``[0.8*tmax, tmax]``.

## Index scalability test (Subgraph construction)

Build different sizes of subgraphs to test the scalability of indices:

```sh
sh run.sh $1 subgraph
```

where ``$1`` corresponds to the solution (``Baseline`` or ``RES``).

Then input the fraction of timestamps in the subgraph. For example, if the fraction is ``0.8``, then the constructed temporal graph would only involve edges in ``[0, 0.8*tmax]``.

## Environment
- System: ``ubuntu-20.04.1``
- Compiler: ``gcc (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0``, ``Python 3.8.8`` (>= 3.5) with packages ``tqdm`` and ``pathlib``
- Memory: ``128G``
- CPU: ``Intel(R) Xeon(R) Silver 4210R CPU @ 2.40GHz``
  
## Running examples
We provided examples about how to derive the experimental data presented in the paper.

``Figure 14`` shows the query efficiency when the length of query time windows is fixed at ``0.8*tmax``. For example, to test the query efficiency of ``D-online`` algorithm under the ``FK(flickr)`` dataset:

```sh
> sh graph-gen.sh
Downloading datasets...
Fetching from http://konect.cc/files/download.tsv.dblp-cite.tar.bz2...
100%|██████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 162k/162k [00:00<00:00, 217kB/s]
Fetching from http://konect.cc/files/download.tsv.flickr-growth.tar.bz2...
100%|█████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 125M/125M [00:38<00:00, 3.40MB/s]
Fetching from http://konect.cc/files/download.tsv.soc-sign-bitcoinotc.tar.bz2...
100%|██████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 363k/363k [00:00<00:00, 375kB/s]
Fetching from https://snap.stanford.edu/data/email-Eu-core-temporal.txt.gz...
100%|███████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 1.61M/1.61M [02:22<00:00, 11.8kB/s]
Fetching from https://snap.stanford.edu/data/CollegeMsg.txt.gz...
100%|█████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 337k/337k [00:32<00:00, 10.7kB/s]
Extracting datasets... done
Datasets:
0. naive
1. flickr-growth
2. dblp-cite
3. email-Eu-core-temporal
4. CollegeMsg
5. soc-sign-bitcoinotc
Select a graph dataset (0-5): 1
Copying dataset to "graph.txt"... done
Normalizing the graph... done
> sh query-gen.sh
Enter the number of queries to be generated: 1000
Enter the length of the query windows (0 < x < 1): 0.8
> ulimit -s 1024000000
> sh run.sh Online
Compiling...
make: 'main' is up to date.
Running...
Building graph...
Build graph success in 7179261μs (7s)
n = 2302926, m = 33140017, tmax = 133, size = 265120136 bytes
Running online search...
Average: 4438485μs (4s)
Online search completed in 4440462497μs (1h 14min 0s)
Program finished in 4447656291μs (1h 14min 7s)
```

``Table 7`` shows the scalability of indices. For example, to test the scalability with ``f=0.25`` of ``D-baseline`` index under the ``FK (flickr)`` dataset:
```sh
> sh graph-gen.sh
Extracting datasets... done
Datasets:
0. naive
1. flickr-growth
2. dblp-cite
3. email-Eu-core-temporal
4. CollegeMsg
5. soc-sign-bitcoinotc
Select a graph dataset (0-5): 1
Copying dataset to "graph.txt"... done
Normalizing the graph... done
> ulimit -s 1024000000
> sh run.sh Baseline subgraph
Compiling...
make: 'main' is up to date.
Running...
Subgraph mode enabled. Please input the fraction of timestamps in the subgraph (0 < x < 1): 0.25
Building graph...
Build graph success in 7160551μs (7s)
n = 2302926, m = 33140017, tmax = 33, size = 265120136 bytes
Running baseline...
Constructing the index structure...
Index construction completed in 142855509μs (2min 22s)
Index cost 626395872 bytes
```

``Table 5`` shows the average index update time of each edge in ``[0.8*tmax, tmax]``. For example, to test the average index update time of ``D-baseline`` index under the ``FK (flickr)`` dataset:
```sh
> sh graph-gen.sh
Extracting datasets... done
Datasets:
0. naive
1. flickr-growth
2. dblp-cite
3. email-Eu-core-temporal
4. CollegeMsg
5. soc-sign-bitcoinotc
Select a graph dataset (0-5): 1
Copying dataset to "graph.txt"... done
Normalizing the graph... done
> ulimit -s 1024000000
> sh run.sh Baseline update
Compiling...
make: 'main' is up to date.
Running...
Update mode enabled. Please input the fraction of timestamps to update (0 < x < 1): 0.2
Building graph...
Build graph success in 7172339μs (7s)
n = 2302926, m = 33140017, tmax = 133, size = 265120136 bytes
Running baseline...
Constructing the index structure...
Index construction completed in 1219283557μs (20min 19s)
Updating the index structure...
Number of edges to be updated: 2227748
Index update completed in 626815298μs (10min 26s)
Index cost 2468736672 bytes
```

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

Possible solutions: check the parameter format in the former of this README file;

(3) Since our solutions involve some recursive procedures, there may not be enough stack memory (segmentation fault);

Possible solutions: execute ``ulimit -s 1024000000`` to enlarge the stack memory.