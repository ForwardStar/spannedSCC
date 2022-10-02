## Finding Spanned Strongly Connected Component in Temporal Graphs

A fast, elegant index structure to find all spanned strongly connected components in a temporal graph.

How to use it:

- Write your graph data into ``graph.txt``; it contains info about all edges with the format (u, v, t); you can use the ``graph-gen.sh`` to generate graph data automatically:

```sh
sh graph-gen.sh
```

- Write your query data into ``query.txt``; it contains info about queries with the format [ts, te]; you can use ``query-gen.sh`` to generate query data automatically:

```sh
sh query-gen.sh
```

- Run the following command:

```sh
sh run.sh $1
```

where ``$1`` is ``Online``, ``Baseline`` or ``Optimized``, indicating which solution to use.

The output is in the file ``output.txt``.