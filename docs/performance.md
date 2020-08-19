
# Performance

This document present the performance of `ryjson1` application,which checks Rye specifications over multiline JSON logfiles and provide a command line interface to Reelay monitors.

We use `timescales` benchmark generator to measure the performance of `ryjson1`. The benchmark covers a number of common classes of specifications with timing constraints. More information regarding the benchmark generator and motivations can be found in the paper of [Timescales: A Benchmark Generator for MTL Monitoring Tools](https://link.springer.com/chapter/10.1007/978-3-030-32079-9_25). 


## Benchmark Reelay

The benchmark can be replicated by typing commands at the Reelay project directory. First generate some data using `timescales` by typing

    make timescales

This command will clone `timescales` generator and generates discrete and dense time behaviors for benchmarking. The total size of generated benchmarks is about 2.2GB. There are two variant for dense time behaviors with two different degrees of [condensation]().

    make test_performance_discrete
    make test_performance_dense10
    make test_performance_dense100

To execute these commands, you need `multitime` utility for benchmarking with these commands, which can be installed via `apt` or [from source](https://github.com/ltratt/multitime).

## Results

Below we present performance of `ryjson1` aapplication on a laptop computer (Intel Core i5-7200U CPU @2.50 Ghz, 8GB RAM) on Ubuntu 20.04 and using the version 2008 of Reelay. And we report minimum user time of 10 executions. 


### 1) Discrete results

Results over processing discrete time behaviors with a length of 1M.

    | Property   | 10x   | 100x  | 1000x |
    | ---------- | ----- | ----- | ----- |
    | AbsentAQ   | 0.246 | 0.224 | 0.234 |
    | AbsentBR   | 0.265 | 0.268 | 0.277 |
    | AbsentBQR  | 0.376 | 0.347 | 0.339 |
    | AlwaysAQ   | 0.242 | 0.223 | 0.219 |
    | AlwaysBR   | 0.265 | 0.264 | 0.261 |
    | AlwaysBQR  | 0.388 | 0.342 | 0.342 |
    | RecurGLB   | 0.211 | 0.164 | 0.155 |
    | RecurBQR   | 0.473 | 0.412 | 0.411 |
    | RespondGLB | 0.395 | 0.321 | 0.310 |
    | RespondBQR | 0.597 | 0.544 | 0.529 |
    | ---------- | ----- | ----- | ----- |



### 2) Dense10 results

Condensation allows to represent periods of the same JSON object with a single timestamped object. Dense10 behaviors limits this feature maximum 10 successive time points. Consequently the same temporal behavior is expressed a shorter logfile (up to 10x). Dense time monitors may work over such condensed (dense) logfiles.

    | Property   | 10x   | 100x  | 1000x |
    | ---------- | ----- | ----- | ----- |
    | AbsentAQ   | 0.446 | 0.356 | 0.334 |
    | AbsentBR   | 0.398 | 0.344 | 0.335 |
    | AbsentBQR  | 0.678 | 0.189 | 0.124 |
    | AlwaysAQ   | 0.453 | 0.358 | 0.342 |
    | AlwaysBR   | 0.392 | 0.346 | 0.329 |
    | AlwaysBQR  | 0.896 | 0.243 | 0.169 |
    | RecurGLB   | 0.302 | 0.103 | 0.083 |
    | RecurBQR   | 0.676 | 0.222 | 0.169 |
    | RespondGLB | 0.751 | 0.222 | 0.155 |
    | RespondBQR | 1.452 | 0.402 | 0.253 |
    | ---------- | ----- | ----- | ----- |

### Dense100 results

Condensation allows to represent periods of the same JSON object with a single timestamped object. Dense100 behaviors limits this feature maximum 100 successive time points. Consequently the same temporal behavior is expressed a shorter logfile (up to 100x). Dense time monitors may work over such condensed (dense) logfiles.

    | Property   | 10x   | 100x  | 1000x |
    | ---------- | ----- | ----- | ----- |
    | AbsentAQ   | 0.430 | 0.290 | 0.276 |
    | AbsentBR   | 0.387 | 0.301 | 0.294 |
    | AbsentBQR  | 0.627 | 0.103 | 0.033 |
    | AlwaysAQ   | 0.432 | 0.271 | 0.266 |
    | AlwaysBR   | 0.376 | 0.273 | 0.280 |
    | AlwaysBQR  | 0.828 | 0.126 | 0.044 |
    | RecurGLB   | 0.272 | 0.046 | 0.027 |
    | RecurBQR   | 0.671 | 0.097 | 0.037 |
    | RespondGLB | 0.729 | 0.108 | 0.041 |
    | RespondBQR | 1.378 | 0.217 | 0.060 |
    | ---------- | ----- | ----- | ----- |

