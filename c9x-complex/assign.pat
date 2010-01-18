
	* c-typeck.c (convert_for_assignment): Warn about assignments
	  of complex to non-complex types.

*** 1.1	1998/02/02 21:33:11
--- c-typeck.c	1998/02/21 21:37:55
*************** convert_for_assignment (type, rhs, errty
*** 4078,4083 ****
--- 4078,4086 ----
        error ("void value not ignored as it ought to be");
        return error_mark_node;
      }
+   /* Complex type assigns only to complex.  */
+   if (coder == COMPLEX_TYPE && codel != coder)
+     warning ("incompatible complex type");
    /* Arithmetic types all interconvert, and enum is treated like int.  */
    if ((codel == INTEGER_TYPE || codel == REAL_TYPE || codel == ENUMERAL_TYPE
         || codel == COMPLEX_TYPE)

