# rvsdg-xmlparser

This program parses the rvsdg output in xml format from `jlm-print --j2rx` and
produces a corresponding dotfile (graphviz) for each region in the graph.

Each edge in the dotfile points to a argument/result or input/output of a containing region or node.
For simple nodes this is modeled by edges going into or out of the node, and we thus print an edge to and from
the node that the edge input/output corresponds to, i.e. its parent.

If the edge is in the regions arguments or results, this is modeled by a single node for each input/output *and*
a common entry/exit node for the region such that each argument is the child of the regions entry node and
all result nodes have the regions exit node as a child.
