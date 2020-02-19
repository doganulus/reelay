# Reelay Expression Format

Reelay reads executable specifications written in plain text and verifies that the system does what those specifications say at runtime. To construct runtime monitors automatically, those specifications must follow some basic syntax rules, called Rye format in short.

## What is Rye Format?

Rye format uses a set of special constructs (that is, keywords and punctuation) to describe desired system behavior over temporal data streams expressed in JSON Lines or CSV-like formats.

These constructs are divided into four categories:

1. Atomic expressions
2. Boolean logic expressions
3. Temporal logic expressions
4. Regular expressions (soon)

Let's start with atomic expressions.

#### Atomic Expressions

Atomic expressions are the basic building blocks of Reelay expressions. Atomic expressions in Reelay come in two flavors: (1) curly-bracketed atoms `{...}` for streams of key-value pairs (JSON, CSV with header, etc.) and (2) square-bracketed atoms `[...]` for streams of list-like (CSV without header, space-delimited logs, etc.). The choice depends on the streaming format and these two flavors should not be mixed naturally. 

Each atom describes a set of constraints over data elements. For example, a curly-bracketed atom is written such that

```rye
{lights_on: true, speed > 20, mode: "Sport XL"}
```

and will be evaluated to `true` for time points `101` and `102` below over a temporal behavior given as a JSON object such that

```json
...
{"time": 101, "lights_on": true, "speed": 21.23, "mode": "Sport XL"}
{"time": 102, "lights_on": true, "speed": 20.01, "mode": "Sport XL"}
{"time": 103, "lights_on": true, "speed": 19.12, "mode": "Sport XL"}
...
```

Currently Reelay supports Booleans (`true`, `false`), numerical comparisons (`<`, `<=`, `>`, `>=`, `==`, `!=`), string equivalences inside curly atoms over flat data objects. The support for hierarchical (dotted) keys will be added in a future release for hierarchical data objects.

Over list-like streams, we may write the same requirement,

```rye
[true, * > 20, "Sport XL"]
```

where the order of elements and the length must match the specification in this case except that the first element reserved for timestamp information. For example, this expression would be `true` only for the time point `101` below:

```csv
...
101, true, 21.23, "Sport XL"
102, true, 20.01, "Sport XL", 121
103, true, 19.12, "Sport XL"
...
```

Timing aspects and specification has a special focus in Reelay. Currently we exclusively use `time` field for dict-like objects and the first element of a list-like object for timestamp information. This behavior can be generalized in future releases.

Reelay atomic expressions accepts `*` as a dummy placeholder for field values. For example, the last two column is ignored in the specification below. It will be evalutated to `true` for time points `101` adn `103` as it still need to match the length.

```rye
[true, *, *]
```

For curly atoms, this construct allows you to describe the case where you require the key exist in the object but don't care about its value.

```rye
{lights_on: true, speed: *, mode: *}
```

Finally Reelay allows to declare categorical variable references inside atoms using `*ref_name` syntax such that 

```rye
{lights_on: true, speed > 20, mode: *m}
```

and

```rye
[true, speed > 20, mode: *myref]
```

This reference called `myref` than can be used elsewhere in the expression and quantified by `exists` and `forall` operators. More details regarding these operators are given in the following section.

#### Boolean Logic Expressions

This section describle Boolean logic operations over atomic Reelay expressions.
we mainly use curly atoms in the examples but these constructs are equally applicable unless anything noted.

##### Negation

The negation of Reelay expressions is defined by using keywords `not` and `!`.

```rye
not {key1: value1, key2: value2}
```

##### Conjunction

The conjuction of Reelay expressions is defined by using keywords `and` and `&&`.

One can see that atomic expression syntax is a shortcut for conjunction between atomic constraints. It is true that two Reelay expressions below are functionally equivalent:

```rye
{key1: value1, key2: value2}
```

and

```
{key1: value1} and {key2: value2}
```

For square atoms, however, one should be careful about lengths. Then two Reelay expressions below are functionally equivalent.

```
[value1, value2]  
```

and

```
[value1, *] and [*, value2]  
```

##### Disjunction

The disjunction of constraints has to defined explicitly using keywords `or` and `||`.

```rye
{key1: value1} or {key2: value2}
```

##### Logical Implication

Logical implication is a popular Boolean operation when writing conditional specifications. The logical implication of constraints has to defined explicitly using keywords `implies` and `->`.

```rye
{key1: value1} -> {key2: value2}
```

##### References and Quantifiers

The ability to refer to data values (unknown at compile time) is one of the advanced features of Reelay. Roughly speaking, a reference variable acts a memory location where many data values (from a key) can be stored and can be checked later (or with another key) for the equality. Then these data values can quantified for existence (`exists`) and universality (`forall`). Note that this feature is currently supported for categorical variables (e.g. strings) only.

The syntax for references and quantifiers  is as follows:
```rye
exists[ref1, ref2, ...]. RYE(ref1, ref2, ...)
```

```rye
forall[ref1, ref2, ...]. RYE(ref1, ref2, ...)
```
where `RYE(ref1, ref2, ...)` is an arbitrary Reelay expression that contains reference declarations `ref1, ref2, ...` inside its atoms.

For example, we may want to check the equality of data values for two fields but don't know or cannot write all possible cases at compile time.
```rye
exists[refname]. {key1: *refname, key2: *refname}
```

This expression above will be evaluated to `true` for the data object

```json
{"key1": "hello", "key2": "hello"}
```

but `false` for the data object

```json
{"key1": "hello", "key2": "world"}
```

as two string values are not equal in the latter. More advanced use of references involve temporal expressions explained in the following sections, which allow us checking data values from different time points.

#### Temporal Logic Expressions

##### Previously

The unary operation of `previously` is defined using keywords `pre` and `Y`. For example, consider the Reelay expression

```rye
pre{lights_on: true}
```

which is true at the current time point if lights were on in the previous time point.

Note that `previously` operation is only meaningful for untimed and discrete timed settings but dense timed settings.


##### Sometime in the Past, Once

The unary operation of `once` is defined using keywords `once` and `P`. For example, consider the Reelay expression

```rye
once{lights_on: true}
```

which is true at the current time point if lights were on in sometime in the past. It is possible to add timing constraints to the specification such that

```rye
once[a:b]{lights_on: true}
```

which is true at the current time point if lights were always on in sometime between `b` and `a` time units before now. We use the syntax `[a:]` and `[:b]` if there is no constraints on upper and lower bounds, respectively.

##### Always in the Past, Historically

The unary operation of `historically` is defined using keywords `historically` and `H`. For example, consider the Reelay expression

```rye
historically{lights_on: true}
```

which is true at the current time point if lights were on in some time in the past. It is possible to add timing constraints to the specification such that 

```rye
historically[a:b]{lights_on: true}
```

which is true at the current time point if lights were always on between `b` and `a` time units before now. We use the syntax `[a:]` and `[:b]` if there is no constraints on upper and lower bounds, respectively.

##### Since

The binary operation of `since` is defined using keywords `since` and `S`. For example, consider the Reelay expression

```rye
{speed < 30} since {lights_on: true}
```

which is true at the current time point if the variable `speed` is always less than 30 units since `lights_on` is true last time. It is possible to add timing constraints to the specification such that 

```rye
{speed < 30} since[a:b] {lights_on: true}
```

which is true at the current time point if the variable `speed` is always less than 30 since `lights_on` is true sometime between `b` and `a` time units before now. We use the syntax `[a:]` and `[:b]` if there is no constraints on upper and lower bounds, respectively.

#### Regular Expressions

##### Union

##### Concatenation

##### Repetition

##### Duration Constraints

##### Intersection

#### A Note regarding Operator Precedence

As a rule of thumb, unary operators bind stronger than binary operators (except quantifiers, better use parantheses) and unary operators are eagerly matching therefore an expression such as

```rye
not {...} since {...}
```

would be parsed as

```rye
(not {...}) since {...}
```

and not as

```rye
not ({...} since {...})
```
