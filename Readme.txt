~~ About MisterHeapy ~~

MisterHeapy is a binary heap for storing pointers to objects.
 
He has capabilities not provided by the standard library's heap functions:

  1. Live updating of elements by index in O(logN) time.
  2. By maintaining a lookup table, objects in the heap can be updated without knowing
     their index in advance.

For simplicity, MisterHeapy places the following restrictions on objects:

  - They must all be memory contiguous.
  - The first item pushed to the heap must be the first item in memory.

For further details, such as info on crafting objects for use with MisterHeapy, and fast
initialisation, see the notes in MisterHeapy.hpp.

MisterHeapy is made available under the MIT license.

Ben Hallstein, May 2012
http://ben.am
