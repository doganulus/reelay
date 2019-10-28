# Benchmark Results

We use the benchmark generator [timescales](https://github.com/doganulus/timescales) to benchmark the performance of reelay MTL monitoring app.  For measurements, we use [multitime](https://github.com/ltratt/multitime) utility. Tests are performed using a laptop computer (Intel Core i5-7200U CPU @2.50 Ghz, 8GB RAM) on Ubuntu 18.04 on WSL (Build 18362.449).

## Discrete Time MTL Application 

Test command:

    make test_mtl_performance_discrete

Performance results (real, min) of for monitoring common temporal properties over traces with a length of 1 million rows:

| Property/Scale | 10x | 100x | 1000x |
| ---------- | ----- | ----- | ----- |
| AbsentAQ   | 0.834 | 0.828 | 0.830 |
| AbsentBR   | 0.809 | 0.833 | 0.843 |
| AbsentBQR  | 0.985 | 0.952 | 0.957 |
| AlwaysAQ   | 0.827 | 0.815 | 0.814 |
| AlwaysBR   | 0.785 | 0.790 | 0.793 |
| AlwaysBQR  | 0.973 | 0.951 | 0.955 |
| RecurGLB   | 0.754 | 0.698 | 0.702 |
| RecurBQR   | 1.102 | 1.068 | 1.068 |
| RespondGLB | 1.008 | 0.927 | 0.943 |
| RespondBQR | 1.285 | 1.237 | 1.215 |

Multitime batch files for these tests can be found [here](https://github.com/doganulus/reelay/tree/master/test/timescales/discrete/multitime).
