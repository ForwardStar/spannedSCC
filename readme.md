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