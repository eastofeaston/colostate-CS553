# Week 3
### Towards roofline
At a high level, roofline is defined as the maximum of time to compute, footprint, and accesses.

WIP

see `ex-base.c` and `ex-better.c`

### My target
These tests were done on a Ubuntu 22.04.3 LTS x86_64 host, using a dodeca-core Ryzen 9 3900X (Matisse).

### Optimizing accesses
In `ex-base.c`, the provided code accesses in an unnecessarily complex fashion. On the first loop, if `N >= 1000`, our loop reads 2N floats and writes N floats. In the second loop 2N^2 floats are read with N^2 floats written. 
In the worst case, our accesses are 3N^2 + 3N.
What makes our second loop particularly pernicious is our access pattern. 
We loop through N elements of B, adding a singular element from A.
This portion is flagged by `gcc` in the `ftree-vectorize` optimization:

*$ gcc -O2 -ftree-vectorize -fopt-info-vec-missed ex-base.c*

```
ex-base.c:22:17: missed: couldn't vectorize loop
ex-base.c:24:16: missed: not vectorized: complicated access pattern.
/usr/include/x86_64-linux-gnu/bits/stdio2.h:112:10: missed: statement clobbers memory: __printf_chk (1, "result: %.3f\n", prephitmp_56);
...
```
My solution to this has three parts.
First, always calculate the full sum. If we prioritize this value first, we can reuse it in our code. In our nested loop, we effectively calculate this sum N times.
Second, eliminate the outer `i` loop in the second stage. Because of our prepped sum, looping through A in this stage is moot. We add the value of the sum in its place.
Third, we introduce a ternary in the print statement to support an original edge case. In situations where N is less than 1000, the first loop would not activate and a zero would be printed. A ternary in our print allows us to handle this case.

*$ gcc -O2 -ftree-vectorize -fopt-info-vec-missed ex-better.c*
```
/usr/include/x86_64-linux-gnu/bits/stdio2.h:112:10: missed: statement clobbers memory: __printf_chk (1, "result: %.3f\n", iftmp.0_14);
...
```
Our unvectorized loop is eliminated. Our real time between these loops drops from ~32ms to sub-millisecond runs...

WIP
