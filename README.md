# librtsgtree

[![build status](https://legrandgroup.githost.io/ci/projects/1/status.png?ref=master)](https://legrandgroup.githost.io/ci/projects/1?ref=master)

A library to handle RouTing in a Self Generated manner for binary trees.

See the details of the algorithm and documentation of this project on the Confluence webpage:
http://sirius.limousin.fr.grpleg.com/confluence/display/A2/IP+over+RS485+addressing+and+routing

# Binary-tree distributed routing

This page discusses a technical architecture for addressing and routing inside a binary-tree.

We assume that this binary consists of nodes that are linked point-to-point (interconnecting segments)

The addressing described here is done so that the routing rules can be generated locally.

## Integer case

Let's build a tree with 7 nodes N1, N2, N3, N4, N5, N6, N7 (the number of nodes will be represented by a value B, that must be a power of 2 **minus 1**: B=2^Rmax - 1)

We illustrate below the small binary tree made of these 7 nodes, interconnected with 7 network segments (the top-most segment being the entry into our tree)

```
          |
          S4
          |
      +---N---+        rank 1
      |   4   |
      |       |
      |S2   S6|
      |       |
   +--N--+ +--N--+     rank 2
   |  2  | |  6  |
   |     | |     |
   |S1 S3| |S5 S7|
   |     | |     |
   N     N N     N     rank 3
   1     3 5     7
```

On this figure, interconnecting networks segments are noted S*n*, and nodes (addressable and routing) are noted N*n*

Node numbering follows the uplink segment numbering (N*n* always has segment S*n* as uplink)

Let's call:

* N*n*: the identifier of each node *n* (with 1>=*n*>=B), each node having an ID that is unique (inside the tree)
* S*n*: network segment between node N*n* and its parent node
* LC(*n*): left child network segment for node N*n* (and thus also left child node ID)
* RC(*n*): right child network segment for node N*n* (and thus also right child node ID)
* R<sub>max</sub>: the maximum rank of a node in the tree (aka the tree depth)
* B: the total number of nodes addressed in the tree (must be a power of 2 minus 1: B=2^R<sub>max</sub> - 1)
* R: the rank of a node in the tree (root node has R=1, bottom nodes have R=R<sub>max</sub>)
* P: the prefix length (not used for the integer case, but used later on for IPv4 and IPv6 routing)
* A: for IPv4 trees this is the number of host bits of the network segments (not used for the integer case, but used later on for IPv4 routing)
  for IPv6 trees, this is the number of bits allocated for local networks attached to each node

> Warning:
> A has thus different meanings between IPv4 trees and IPv6 trees.
> In IPv4, it is a provision of bits to address interconnecting network segments inside the addressing space of the tree
> In IPv6, we don't need to use the addressing space of the tree, we use link-local IPv6, however, we can then also benefit from local network attached to tree nodes. Each node thus route to a local network which is allocated a prefix of /A (A=64 recommended)
>
> In the small example above, B = 7 = ( 2^3 ) - 1, thus R<sub>max</sub> = 3 (the bottom rank is rank 3)

### Root node properties

We can get the identity of the root node ID:

n = (B+1)/2 = 2^(R<sub>max</sub>-1) = 2^2 = 4

(N4 is the root node and has segment S4 as uplink).

The root can thus self-generate its ID by knowing B (or R<sub>max</sub>).


Once the root is determined, we can number all subnodes in the tree.

### Determination of children nodes properties

Based on the value of its parent node number *p*, and parent node rank R*p*, and whether its is the left of right child of its parent node, every node can then define its own properties (ID and network segment)

#### Rank determination

Practically, R*p* can even be calculated if we know *p* and R<sub>max</sub>:

We count the right most 0 bits from the right of our node ID p, this count will be called b

R*p* = R(p) = R<sub>max</sub> - b

So let say we have b = 2 bits set to 0 (third least significant bit is 1), R(p) = R<sub>max</sub> - 2
Icon

If there is no bit set to 1, we assume b=R<sub>max</sub> (higher values for b should not be used, they would lead to a negative rank)

In practice, we are not using the subnetwork with all values set to 0 (there is no node N0), so the case with b = R<sub>max</sub> should never occur.


#### Left children

As a left child, a node will get its node number:

*n* = LC(p) = *p* - 2^(R<sub>max</sub> - 1 - R*p*)

#### Right children

As a left child, a node will get its node number:

*n* = RC(p) = *p* + 2^(R<sub>max</sub> - 1 - R*p*)

### Example

For example, for the left child of N4 (the root node in our example):

* *p* = 4
* R*p* = 1

LC(4) = 4 - 2^(3-1-1) = 4 - 2 = 2

Left child for N4 is thus N2

The right child of N4 will have an ID: RC(4) = 4 + 2^(3-1-1) = 6, thus this will be node N6

The left child of N6 being: LC(6) = 6 - 2^(3-1-2) = 5

The right child of N6 being: RC(6) = 6 + 2^(3-1-2) = 7
