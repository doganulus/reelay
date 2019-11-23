# Past Temporal Logic

## Temporal signals



## Expression syntax

|   | Construct | Syntax | Examples | Explanation |
| - | --------- | ------ | -------- | ----------- | 
| | **Atomic Expressions** | | | 
|  | Propositions | *name* | `lights_on` | `lights_on` is a Boolean variable over time.
|  | Numerical<br>Comparison<br>Predicates | *name* **<** *const*<br>*name* **<=** *const*<br>*name* **>** *const*<br>*name* **>=** *const*<br> |  `speed` **<** 30m/s<br><br>`temperature` **>** 10*C | `speed` and `temperature` are numerical variables over time.
| | Custom<br>Predicates | **$** *fname(args...)*  | **$**`my_predicate`(`speed`) | User defines the Boolean valued function `my_predicate`
|| **Boolean Expressions** |
|  | Negation | **not**, **!**   | **not** `lights_on` |
|  | Conjunction | **and**, **&&**  | `speed` > 10m/s **and** `speed` < 30m/s |
|  | Disjunction | **or**, **\|\|** |  |
|  | Implication | **implies**, **->**   | `lights_on` **implies** (`speed` < 30m/s) |
|| **Temporal Expressions** |
| | Previously | **pre**, **Y** | **pre**(`lights_on`) | It is true at the current time point if lights were on in the previous time point.
| | Sometime<br>in the Past | **once**, **P** | **once**(`lights_on`) | The expression **once**(`lights_on`) is true at the current time point if lights were on in some time in the past.
| | Sometime<br>in the Past<br>(Bounded) | **once[a:b]**,<br>**P[a:b]** | **once[a:b]**(`lights_on`) | The expression **once[a:b]**(`lights_on`) is true at the current time point if lights were on in sometime between **b** and **a** time units before now.
| | Always<br>in the Past | **historically**, **H**| **historically**(`lights_on`)  | The expression **historically**(`lights_on`) is true at the current time point if lights were always on time in the past.
| | Always<br>in the Past<br>(Bounded) | **historically[a:b]**,<br> **H[a:b]**| **historically[a:b]**(`lights_on`)  | The expression **historically[a:b]**(`lights_on`) is true at the current time point if lights were always on between **b** and **a** time units before now.
| | Since | **since**, **S**  |(`speed` < 30) **since** `lights_on`| The expression (`speed` < 30) **since** `lights_on` is true at the current time point if the variable `speed` is always less than 30 since `lights_on` is true last time.
| | Since<br>(Bounded) | **since[a:b]**,<br> **S[a:b]** |(`speed` < 30) **since[a:b]** `lights_on`| The expression (`speed` < 30) **since[a:b]** `lights_on` is true at the current time point if the variable `speed` is always less than 30 since `lights_on` is true sometime between **b** and **a** time units before now.

## Commonly used patterns
| Property | Expression | Explanation |
|-|-|-|
| Absence After | **(once[0:b]`Q`) -> (not`P` since `Q`)**| `P` is false at least for `b` time units after `Q` is true. |
| Absence Before | **`R` -> historically[0:b] (not`P`)**| `P` is false at least for `b` time units before `R` is true. |
| Absence Between | **(`R` && !`Q` && once`Q`) -> (not `P` since `Q`)**| `P` is false between `R` is true and then `Q` is true. |
