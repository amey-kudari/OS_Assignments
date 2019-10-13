# CONCURRENT QUICKSORT

**Assumptions**

- unoptimized quick sort is okay ,when I choose last element as pivolt every time, (worst case complexity for sorted array: `O(n*n)`.
- Shared memory can be used.

# CODE LOGIC:


## Quick sort logic

- take segment of array `(l,r)`
- keep last element as pivolt and rearrange array such that all elements greater than pivolt are on left and all greater are on right
- this is done using function
- recursively call same process on `(l,p-1)` and `(p+1,r)` if the ranges have more than one element


## Multi process quick sort

- call 2 processes for last step of quick sort logic, wait till the processes get over before returning

## Multi threaded quick sort

- create 2 threads and run process on both steps simultaniously, wait till both threads terminate before returning

----

# Thanks! :D

> Amey Kudari
> 2018101046