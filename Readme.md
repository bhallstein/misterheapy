# About MisterHeapy

MisterHeapy is a templated C++ binary heap class.
 
Objects stored in a MisterHeapy can be “updated” (i.e. resorted after update) by index or by reference, both in O(1) time on average.

For simplicity and performance, MisterHeapy places the following restrictions on objects:

- They must all be memory contiguous.
- The first item pushed to the heap must be the first item in memory.

For further details e.g. fast initialisation, see the notes at the top of MisterHeapy.hpp.

MisterHeapy is published under the MIT license.

Ben Hallstein, May 2012
http://ben.am
