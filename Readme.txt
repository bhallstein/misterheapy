~~ About MisterHeapy ~~

MisterHeapy is a binary heap for storing pointers to objects.
 
He is bog standard, but has capabilities not provided by the standard library's heap
functions:

  1. Live updating of elements by index in O(logN) time.
  2. By maintaining a lookup table, it enables you to update objects in the heap without
     knowing their index.

For simplicity, MisterHeapy places the following restrictions on your objects:

  - They must all be memory contiguous.
  - The first item pushed to the heap must be the first item in memory.

For further details, such as how to craft objects for use with MisterHeapy, and fast
initialisation, see the notes in MisterHeapy.hpp.

Ben Hallstein, May 2012
http://ben.am