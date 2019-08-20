Project 1, CPSC 131

Template code written in 2016 by Kevin Wortman

To the extent possible under law, the author(s) have dedicated all
copyright and related and neighboring rights to this software to the
public domain worldwide. This software is distributed without any
warranty. You should have received a copy of the CC0 Public Domain
Dedication along with this software. If not, see
<http://creativecommons.org/publicdomain/zero/1.0/>.

Our convention for memory management:

When an object is passed by POINTER, the called function takes ownership
of the object and is responsible for eventually deleting it.

When an object is passed by REFERENCE, ownership is not transferred, so
the called function does not need to delete anything.
