	* flow.c (insn_dead_p): Don't kill part of a complex number.
	* emit-rtl.c (gen_lowpart_common): Handle narrow complex reg values.
	  (gen_highpart): Likewise.
	* calls.c (expand_call): Narrow complex variable uses two regs.
	* alpha/alpha.h (ALPHA_ARG_SIZE): Likewise. 
	  (HARD_REGNO_NREGS): Likewise.
*** ../gcc/flow.c	Tue Feb  4 18:34:01 1997
--- flow.c	Sun Mar 16 09:17:52 1997
***************
*** 1819,1824 ****
--- 1819,1828 ----
  		     & ((REGSET_ELT_TYPE) 1
  			<< ((regno + n) % REGSET_ELT_BITS))) != 0)
  		  return 0;
+ 
+ 	      /* Don't allow part of a complex number to be deleted.  */
+ 	      if (GET_MODE_CLASS (GET_MODE (r)) == MODE_COMPLEX_FLOAT)
+ 		return 0;
  	    }
  
  	  return 1;
*** ../gcc/emit-rtl.c	Mon Mar 10 16:09:45 1997
--- emit-rtl.c	Sun Mar 16 09:38:31 1997
***************
*** 686,691 ****
--- 686,702 ----
  	  && ! HARD_REGNO_MODE_OK (REGNO (x) + word, mode)
  	  && HARD_REGNO_MODE_OK (REGNO (x), GET_MODE (x)))
  	return 0;
+ 
+       else if (GET_MODE_CLASS (GET_MODE (x)) == MODE_COMPLEX_FLOAT
+ 	       && GET_MODE_SIZE (GET_MODE (x)) <= UNITS_PER_WORD
+ 	       && REGNO (x) < FIRST_PSEUDO_REGISTER)
+ 	{
+ 	  if (REG_FUNCTION_VALUE_P (x))
+ 	    return gen_rtx (SUBREG, mode, x, 0);
+ 	  else
+ 	    return gen_rtx (REG, mode, REGNO (x));
+ 	}
+ 
        else if (REGNO (x) < FIRST_PSEUDO_REGISTER
  	       /* integrate.c can't handle parts of a return value register. */
  	       && (! REG_FUNCTION_VALUE_P (x)
***************
*** 1006,1011 ****
--- 1017,1032 ----
  	word = ((GET_MODE_SIZE (GET_MODE (x))
  		 - MAX (GET_MODE_SIZE (mode), UNITS_PER_WORD))
  		/ UNITS_PER_WORD);
+ 
+       if (GET_MODE_CLASS (GET_MODE (x)) == MODE_COMPLEX_FLOAT
+ 	  && GET_MODE_SIZE (GET_MODE (x)) <= UNITS_PER_WORD
+ 	  && REGNO (x) < FIRST_PSEUDO_REGISTER)
+ 	{
+ 	  if (REG_FUNCTION_VALUE_P (x))
+ 	    return gen_rtx (SUBREG, mode, x, 1);
+ 	  else
+ 	    return gen_rtx (REG, mode, REGNO (x) + 1);
+ 	}
  
        /*
         * ??? This fails miserably for complex values being passed in registers
*** ../gcc/calls.c	Sat Jan  4 18:43:38 1997
--- calls.c	Sun Mar 16 09:54:39 1997
***************
*** 1893,1898 ****
--- 1893,1903 ----
  			       validize_mem (args[i].value), nregs,
  			       args[i].mode);
  
+ 	  /* Indicate two regs used to hold a short complex arg.  */
+ 	  if (GET_MODE_CLASS (args[i].mode) == MODE_COMPLEX_FLOAT
+ 	      && GET_MODE_SIZE (args[i].mode) <= UNITS_PER_WORD)
+ 	    nregs = 2;
+ 
  	  /* Handle calls that pass values in multiple non-contiguous
  	     locations.  The Irix 6 ABI has examples of this.  */
  	  if (GET_CODE (reg) == PARALLEL)
*** ../gcc/config/alpha/alpha.h	Sun Jan 19 13:17:06 1997
--- alpha.h	Sun Mar 16 14:39:59 1997
***************
*** 493,499 ****
     but can be less for certain modes in special long registers.  */
  
  #define HARD_REGNO_NREGS(REGNO, MODE)   \
!   ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)
  
  /* Value is 1 if hard register REGNO can hold a value of machine-mode MODE.
     On Alpha, the integer registers can hold any mode.  The floating-point
--- 493,502 ----
     but can be less for certain modes in special long registers.  */
  
  #define HARD_REGNO_NREGS(REGNO, MODE)   \
! ((GET_MODE_CLASS (MODE) == MODE_COMPLEX_FLOAT			\
!   && GET_MODE_SIZE (MODE) <= UNITS_PER_WORD)			\
!  ? 2								\
!  : ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD))
  
  /* Value is 1 if hard register REGNO can hold a value of machine-mode MODE.
     On Alpha, the integer registers can hold any mode.  The floating-point
***************
*** 923,934 ****
  #define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME,INDIRECT)  (CUM) = 0
  
  /* Define intermediate macro to compute the size (in registers) of an argument
!    for the Alpha.  */
  
  #define ALPHA_ARG_SIZE(MODE, TYPE, NAMED)				\
! ((MODE) != BLKmode							\
!  ? (GET_MODE_SIZE (MODE) + (UNITS_PER_WORD - 1)) / UNITS_PER_WORD 	\
!  : (int_size_in_bytes (TYPE) + (UNITS_PER_WORD - 1)) / UNITS_PER_WORD)
  
  /* Update the data in CUM to advance over an argument
     of mode MODE and data type TYPE.
--- 926,941 ----
  #define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME,INDIRECT)  (CUM) = 0
  
  /* Define intermediate macro to compute the size (in registers) of an argument
!    for the Alpha.
!    A short complex arg takes two registers.  */
  
  #define ALPHA_ARG_SIZE(MODE, TYPE, NAMED)				\
! ((GET_MODE_CLASS (MODE) == MODE_COMPLEX_FLOAT				\
!   && GET_MODE_SIZE (MODE) <= UNITS_PER_WORD)				\
!  ? 2 									\
!  : ((MODE) != BLKmode 							\
!     ? (GET_MODE_SIZE (MODE) + (UNITS_PER_WORD - 1)) / UNITS_PER_WORD 	\
!     : (int_size_in_bytes (TYPE) + (UNITS_PER_WORD - 1)) / UNITS_PER_WORD))
  
  /* Update the data in CUM to advance over an argument
     of mode MODE and data type TYPE.
