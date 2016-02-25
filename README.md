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

On this figure, interconnecting networks segments are noted S<i>n</i>, and nodes (addressable and routing) are noted N<i>n</i>

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

>If there is no bit set to 1, we assume b = R<sub>max</sub> (higher values for b should not be used, they would lead to a negative rank)
>
>In practice, we are not using the subnetwork with all values set to 0 (there is no node N0), so the case with b = R<sub>max</sub> should never occur.


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

# IPv4 addressing and routing in a binary tree

The integer example above is now applied to IPv4 addresses.

The specificities of IPv4 are that:

* Nodes will be routing devices, but also addressable endpoints.
* Network segments Sn between two nodes will have their own IPv4 subnet.
* In this example, we only need to reach routing nodes, so addresses are only allocated for nodes.
* Each network segment Sn will thus consist of a point-to-point link with two hosts (the two extremity nodes), these network segments will use /30 subnets.
* By convention, within each /30 subnet, top hosts (path to the root) will take the low IP address inside the subnet, bottom hosts (children) will take the high IP address.
* Also, nodes all have multiple addresses, but their *reference IPv4 address* used for communication should be the top one (the one used for the path to the root), as this is the only interface that must be up to be able to communicate with the outside.

## IPv4 range to binary tree size

We have a whole IPv4 range allocated for the tree, represented by the range's prefix length P.

A /8 network would give P=8

The individual addresses available with this prefix are encoded with (in IPv4) 32-P bits.

For example, on a /8 network, we have 32-8=24 bits for addressing, or 2^24 possible combinations

We will use /30 interconnecting segments (subnets), so host are encoded using A=2 bits within each /30 network.
Icon

> Small subnets (/31) for point to point links is discussed in RFC3021, in a effort to save IP addresses, in that case each interconnection network would use A=1 bit
> 
> /31 subnets would give us an extra rank in the tree (with a /8 (P=8), we would be able to fit a tree with Rmax = 32 - P - A = 23). But because it may require adaptation of the operating system, it is safer to stick to /30 network, at least initially, reserving extreme addresses for network address and broadcast address, as it is done conventionally. This ensures that any implementation (linux kernel, lwip, etc..) will behave as expected.

Using /30 subnets, for a total /8 (P=8) IP network Rmax = 32 - P - A = 22

## Routing applied to IPv4 binary trees

The data can be exchanged within the binary tree from any node to any other.

Nodes can generate their routing table automatically, given their node number n and rank Rn

Indeed:

* n allows us to know the associated top subnet Sn
* n and Rn allow us to calculate child node numbers LC(n) and RC(n)
* LC(n) and RC(n) allow us to know the associated subnets

### Left and right routes (to children)

We will setup a first routing rule to each child and its subchildren.

We take LC(n), or RC(n) represent it in binary and left shift by A = 2 (to take into account the host bits required for interconnecting links addressing).

We then keep only the Rn (rank) most significant bits by applying a "bitwise and" mask, this will be the base network for left and right child routes.

The prefix for left and right child rules being P + R

### Route up

The next rule allows us to reach our parent subnet Sn, we will use n to compute the top network (use n, represent it in binary and left shift by A = 2 (to take host bits into account)).

The prefix for the top rule will be 32 - A (/30)

A last rule will be a default gateway via our parent to reach the part of the tree that is not below us.

## Example using IPv4 private range 192.168.0.0/24

In this example, we are using a /24 range:

P = 24

Interconnection networks are /30 subnets:

A = 2

Therefore our maximum depth is:

Rmax = 32 - 24 - 2 = 6

We thus have 6 bits for nodes numbering.

B = 2^6 - 1 = 63

### Root subnet

Root node ID:

n = 2^(6-1) = 32 = 100000b

Root is thus N32

Its uplink network is S32.


Each node can actually guess its node ID based on the size of the tree (Rmax, that would be harcoded on all devices), and on the top interface subnetwork characteristics (that are provided when using ppp IPv4 configuration via ppp's LCP).

A node can thus build all its configuration when the ppp link is brought up on the top interface:

From the uplink network subnet, we apply a "bitwise and" mask with ((2^Rmax-1 << A), the result will be right shifted of A bits to get the Rmax bits of the network ID (Sn) and thus get the node ID (n)

In our case, the root node will get IPv4 address 192.168.0.130/30 via ppp.

It knows Rmax = 6, and we use /30 interconnecting subnets by convention (A = 2) thus the mask would be:

2^6 - 1 << 2 = 11111100b

We represent the IPv4 address 192.168.0.130 as binary:

uint32(192.168.0.130) = ((192 * 256) + 168) * 65536) + 130 = 11000000 10101000 00000000 10000010b

We apply the mask, then right shift:

(11000000 10101000 00000000 10000010b & 11111100b)>>2 = 100000b = 32

From the node ID, we also can also calculate our rank calculation from node ID. In 100000b, there are 5 right bits set to 0, so b = 5:

R(32) = Rmax - 5 = 1

 

 

S32 subnet prefix is built using the network ID (32) left shifted to give room for the 2 last bits (host ID)

100000 00b

We apply a "binary or" of this ID with the network prefix 192.168.0.0/24 and we will get the IPv4 range for S32:

192.168.0.128/30

As explained above, by convention, within this /30 subnet, N32 being the bottom host of S32, it will take the high IPv4:

N32 will have IPv4 address 192.168.0.130/30 on S32, and this will be the reference IPv4 address for N32

### Root's left child

LC(p) = p - 2^(Rmax - 1 - Rp)

LC(32) = 32 - 2^(6 - 1 - 1) = 16 = 010000b

N32's left child is N16

And the subnet between N32 and N16 is S16

S16 subnet prefix is built using the network ID (16) left shifted to give room for the 2 last bits (host ID)

010000 00b

We apply a "binary or" of this ID with the network prefix 192.168.0.0/24 and we will get the IPv4 range for S16:

192.168.0.64/30

### Root's right child

RC(p) = p + 2^(Rmax - 1 - Rp)

RC(32) = 32 + 2^(6 - 1 - 1) = 48 = 110000b

N32's right child is N48

And the subnet between N32 and N48 is S48

S48 subnet prefix is built using the network ID (48) left shifted to give room for the 2 last bits (host ID)

110000 00b

We apply a "binary or" of this ID with the network prefix 192.168.0.0/24 and we will get the IPv4 range for S48:

192.168.0.192/30

### Root node's routing table

We apply the routing rule calculation described above on the root node of our example tree.

Root node has ID n=32

Node children's route prefix: P + R = 24 + 1 = 25

Most significant bits mask for node's (left and right) children routes (rank R = 1):

100000b

#### Left interface routes

Left child: LC(32) = 16 = 010000b

(010000b & 100000b) << 2 = 00000000b = 0

Node's route with left child as next hop via left interface:

    192.168.0.0/25

#### Right interface routes

Right child: RC(32) = 48 = 110000b

(110000b & 100000b) << 2 = 10000000b = 128

Node's route with right child as next hop via right interface:

    192.168.0.128/25

#### Top interface route

Top route: n = 32 = 100000b

(100000 << 2) = 10000000b = 128

Node's route via parent as next hop via top interface:

    192.168.0.128/30
    default route
