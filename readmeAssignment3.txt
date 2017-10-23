Q:Now, change the module so that init_module() returns -1, recompile, 
and retest. What happens? Why?
A: The init_module() function is not allowed to return a value other than 0.
If this functions returns a nunmber that is not 0 such as -1, then it signals
when you are trying to install the module to the Kernel that your module
cannot be loaded.
