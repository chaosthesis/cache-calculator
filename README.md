# A: Compute the size of RAM

## Description

The objective of this program is to approximates the size of RAM by observing
the memory allocation performance.

## Algorithm

The method is to maintain a block list over which We incrementally allocate a
block of memory. After each malloc() call, we memset() all existing blocks and
meausre the time elapsed. If at one point the time is significant longer than
the previous runs, we assume that page swap is happening due to memory being
exhausted.

## Usage

```
$ make
```

## Example

The program has been tested on machine of 2GB RAM:

```
   ./compute_ram
   last run: 0.00s, this run: 0.00s, 64MB touched
   last run: 0.00s, this run: 0.07s, 128MB touched
   last run: 0.07s, this run: 0.12s, 192MB touched
   last run: 0.12s, this run: 0.09s, 256MB touched
   last run: 0.09s, this run: 0.08s, 320MB touched
   last run: 0.08s, this run: 0.08s, 384MB touched
   last run: 0.08s, this run: 0.09s, 448MB touched
   last run: 0.09s, this run: 0.10s, 512MB touched
   last run: 0.10s, this run: 0.11s, 576MB touched
   last run: 0.11s, this run: 0.12s, 640MB touched
   last run: 0.12s, this run: 0.14s, 704MB touched
   last run: 0.14s, this run: 0.13s, 768MB touched
   last run: 0.13s, this run: 0.16s, 832MB touched
   last run: 0.16s, this run: 0.16s, 896MB touched
   last run: 0.16s, this run: 0.17s, 960MB touched
   last run: 0.17s, this run: 0.18s, 1024MB touched
   last run: 0.18s, this run: 0.20s, 1088MB touched
   last run: 0.20s, this run: 0.21s, 1152MB touched
   last run: 0.21s, this run: 0.21s, 1216MB touched
   last run: 0.21s, this run: 0.23s, 1280MB touched
   last run: 0.23s, this run: 0.24s, 1344MB touched
   last run: 0.24s, this run: 0.27s, 1408MB touched
   last run: 0.27s, this run: 0.26s, 1472MB touched
   last run: 0.26s, this run: 0.27s, 1536MB touched
   last run: 0.27s, this run: 0.28s, 1600MB touched
   last run: 0.28s, this run: 0.30s, 1664MB touched
   last run: 0.30s, this run: 0.32s, 1728MB touched
   last run: 0.32s, this run: 0.30s, 1792MB touched
   last run: 0.30s, this run: 0.36s, 1856MB touched
   last run: 0.36s, this run: 0.43s, 1920MB touched
   last run: 0.43s, this run: 0.95s, 1984MB touched
```

# B: Measure the characteristics of Cache

## Description

This is an implementation of cache characteristics measurement loosly based
on the paper "Measuring Cache and TLB Performance and Their Effect on Benchmark
Runtimes" by Saavedra, R.H., and A.J. Smith.

# Algorithm

Essentially the method is to iterate throgh an array and observe the performance
(e.g. latency) while modifying the array size and stride size.

The authors give a table to summarize what would theroetically happen when
changing the paramemters:

|Regime|Array Size |Stride         |Frequency of Misses    |Time per Iteration|
|------|-----------|---------------|-----------------------|------------------|
|1     |1 <= N <= C|1 <= s <= N/2  |no misses              |T_no_miss         |
|2.a   |C < N      |1 <= s <= b    |1 miss per b/s elements|T_no_miss + Ds/b  |
|2.b   |C < N      |b <= s <= N/a  |1 miss per element     |T_no_miss + D     |
|2.c   |C < N      |N/a <= s <= N/2|no misses              |T_no_miss         |


## Usage

```
$ make
```

## Example

The program has been tested on two machines:

```
1. CCIS machine (*please see attached graph*)
-> Intel(R) Xeon(R) CPU E5-2630 v3 @ 2.40GHz

  -> measuring cache size
  vectorSize = 1KB, avgLAT = 0
  vectorSize = 2KB, avgLAT = 0
  vectorSize = 4KB, avgLAT = 0
  vectorSize = 8KB, avgLAT = 0
  vectorSize = 16KB, avgLAT = 0
  vectorSize = 32KB, avgLAT = 1 <- cache size: 32KB
  vectorSize = 64KB, avgLAT = 2
  vectorSize = 128KB, avgLAT = 6
  vectorSize = 256KB, avgLAT = 13
  vectorSize = 512KB, avgLAT = 28
  vectorSize = 1024KB, avgLAT = 62
    
  -> measuring cache line size
  strideSize = 8B, avgLAT = 219
  strideSize = 16B, avgLAT = 266
  strideSize = 32B, avgLAT = 468
  strideSize = 64B, avgLAT = 930 <- cache line size: 64B
  strideSize = 128B, avgLAT = 1064
  strideSize = 256B, avgLAT = 1157
  strideSize = 512B, avgLAT = 1070
  strideSize = 1024B, avgLAT = 2245
    
  -> measuring associativity
  strideSize = 4KB, avgLAT = 225 (N /s = 256)
  strideSize = 8KB, avgLAT = 271 (N /s = 128)
  strideSize = 16KB, avgLAT = 473 (N /s = 64)
  strideSize = 32KB, avgLAT = 936 (N /s = 32)
  strideSize = 64KB, avgLAT = 1073 (N /s = 16)
  strideSize = 128KB, avgLAT = 1160 (N /s = 8) <- associativity: 8
  strideSize = 256KB, avgLAT = 1082 (N /s = 4)
  strideSize = 512KB, avgLAT = 2245 (N /s = 2)

2. MacBook Pro (Retina, 15-inch, Late 2013)
-> Intel(R) Core(TM) i7-4750HQ CPU @ 2.00GHz

  -> measuring cache size
  vectorSize = 1KB, avgLAT = 0
  vectorSize = 2KB, avgLAT = 0
  vectorSize = 4KB, avgLAT = 0
  vectorSize = 8KB, avgLAT = 0
  vectorSize = 16KB, avgLAT = 0
  vectorSize = 32KB, avgLAT = 6 <- cache size: 32KB
  vectorSize = 64KB, avgLAT = 3
  vectorSize = 128KB, avgLAT = 7
  vectorSize = 256KB, avgLAT = 16
  vectorSize = 512KB, avgLAT = 39
  vectorSize = 1024KB, avgLAT = 86
     
  -> measuring cache line size
  strideSize = 8B, avgLAT = 119
  strideSize = 16B, avgLAT = 146
  strideSize = 32B, avgLAT = 267
  strideSize = 64B, avgLAT = 550 <- cache line size: 64B
  strideSize = 128B, avgLAT = 749
  strideSize = 256B, avgLAT = 884
  strideSize = 512B, avgLAT = 704
  strideSize = 1024B, avgLAT = 1664
     
  -> measuring associativity
  strideSize = 4KB, avgLAT = 121 (N /s = 256)
  strideSize = 8KB, avgLAT = 149 (N /s = 128)
  strideSize = 16KB, avgLAT = 269 (N /s = 64)
  strideSize = 32KB, avgLAT = 552 (N /s = 32)
  strideSize = 64KB, avgLAT = 753 (N /s = 16)
  strideSize = 128KB, avgLAT = 890 (N /s = 8) <- associativity: 8
  strideSize = 256KB, avgLAT = 704 (N /s = 4)
  strideSize = 512KB, avgLAT = 1664 (N /s = 2)
```
