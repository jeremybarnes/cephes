*** 1.1	1999/07/14 23:03:52
--- emit-rtl.c	1999/08/01 21:14:33
*************** gen_realpart (mode, x)
*** 966,976 ****
  {
    if (GET_CODE (x) == CONCAT && GET_MODE (XEXP (x, 0)) == mode)
      return XEXP (x, 0);
-   else if (WORDS_BIG_ENDIAN
- 	   && GET_MODE_BITSIZE (mode) < BITS_PER_WORD
- 	   && REG_P (x)
- 	   && REGNO (x) < FIRST_PSEUDO_REGISTER)
-     fatal ("Unable to access real part of complex value in a hard register on this target");
    else if (WORDS_BIG_ENDIAN)
      return gen_highpart (mode, x);
    else
--- 966,971 ----
*************** gen_imagpart (mode, x)
*** 989,999 ****
      return XEXP (x, 1);
    else if (WORDS_BIG_ENDIAN)
      return gen_lowpart (mode, x);
-   else if (!WORDS_BIG_ENDIAN
- 	   && GET_MODE_BITSIZE (mode) < BITS_PER_WORD
- 	   && REG_P (x)
- 	   && REGNO (x) < FIRST_PSEUDO_REGISTER)
-     fatal ("Unable to access imaginary part of complex value in a hard register on this target");
    else
      return gen_highpart (mode, x);
  }
--- 984,989 ----
