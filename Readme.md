# About MisterHeapy

A C++ binary heap.
 
Entries can be “updated” (resorted after update) by index **or by reference** both in constant time on average.

For simplicity and performance, MisterHeapy places the following restrictions on objects:

- They must be memory contiguous.
- The first item pushed to the heap must be the first item in memory.

For further details e.g. fast initialisation, see the notes at the top of MisterHeapy.hpp.

-- Published under the MIT license
-- BH May 2012

