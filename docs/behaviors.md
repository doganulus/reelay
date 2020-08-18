# Temporal Behaviors

A temporal behavior of a system is a time-ordered sequence of observations regarding the system. There are two primary models of temporal behaviors we consider in Reelay:

* [Discrete time behaviors]() 
* [Dense time behaviors]() 
    
The main distinction between two models for the user is how time progresses between two observations. For discrete time behaviors, time value increases exactly 1 time unit between two observations. For dense time behaviors, however, time can increase at arbitrary amounts including fractional values. In that case, an explicit timing information such as a timestamp is added to the observation data.

In the following we use multiline JSON files to represent temporal behaviors to give examples and explain some important concepts. 

## Discrete Time Behaviors and Persistence

The multiline JSON document below represents a discrete time behavior where each JSON object containing three variables regarding a system. For discrete time behaviors the progression of time is implicit as time value increases one by one for each line starting from zero.

```json
{"lights_on": false, "speed": 21.23, "mode": "Start"} // time: 0 
{"lights_on": true,  "speed": 20.01, "mode": "Start"} // time: 1
{"lights_on": true,  "speed": 19.12, "mode": "Start"} // time: 2 
{"lights_on": false, "speed": 19.18, "mode": "Sport"} // time: 3
{"lights_on": false, "speed": 19.18, "mode": "Sport"} // time: 4
...
```
Normally each temporal variable of the system is recorded in such JSON objects (or other equivalent representation) for each time point. Usually temporal variables do not change their value for every time point. Indeed they can have the same value for long time periods. This brings the concept of persistence for temporal behaviors.

[Persistence]() means that we interpret the value of temporal variables from the last time when they are set in the behavior. In other words, once a temporal variable set in the behavior, its value persists until it is set again. For example, the discrete time behavior above can be equivalently represented under persistance interpreation as below. 

```json
{"lights_on": false, "speed": 21.23, "mode": "Start"} // time: 0
{"lights_on": true,  "speed": 20.01                 } // time: 1
{                    "speed": 19.12                 } // time: 2
{"lights_on": false, "speed": 19.18, "mode": "Sport"} // time: 3
{}                                                    // time: 4
```

In Reelay we support persistent discrete time behaviors and indeed it is encouraged to use persistent behaviors for efficiency reasons.

Finally notice that discrete time behaviors would contain sequences of empty JSON objects if no value changed from the previous time point (again a common case in real life systems). Monitors will be updated at each time point for the same values for a long time period, which is not very efficient. Here it is better to switch dense time model as we need to process many time points having the same value at once. 

## Dense Time Behaviors

Dense time behaviors contain extra timing information next to the observation data. In Reelay we reserve top-level `time` keyword for this purpose. This allows to represent temporal behaviors changing at irregular intervals and jump many time points at once. Dense time behaviors are naturally persistent, meaning that temporal variables retain their values for skipped times as in the discrete case. For example, suppose we have a dense time behavior as below:

```json
{"time": 0,  "lights_on": false, "speed": 21.23} 
{"time": 11, "lights_on": true,  "speed": 20.01}
{"time": 32,                     "speed": 19.12} 
{"time": 65, "lights_on": false, "speed": 19.18} 
{"time": 100} 
```

When a dense timed Reelay monitor processes such a temporal behavior, it calls its update method 5 times (1 for each JSON object) whereas a discrete timed monitor would need 100 calls. An update step of dense timed monitors is more expensive than that of discrete timed monitors but there is a point using dense time model becomes advantegous. 

>**Therefore, we provide both discrete and dense timed monitors in Reelay where the discrete model is better suited for fast and regularly updated systems and the dense model for slow and irregularly ones.**

Finally, since we no longer need to progress one-by-one, we can also progress smaller steps or fractional steps. Below is a dense time behavior using floating point numbers for time field.

```json
{"time": 0.0,   "lights_on": false, "speed": 21.23} 
{"time": 11.3,  "lights_on": true,  "speed": 20.01}
{"time": 32.1,                      "speed": 19.12} 
{"time": 65.13, "lights_on": false, "speed": 19.18} 
{"time": 100.0} 
```

And the user can select the base time type (currently only primitive types) for Reelay monitors depending on their system. 

## Final remark

This document aimed to give a practical view of temporal behaviors for the user rather than precise mathematical definitions of discrete and dense time behaviors. These definitions can be found at [our paper](https://arxiv.org/abs/1901.00175).