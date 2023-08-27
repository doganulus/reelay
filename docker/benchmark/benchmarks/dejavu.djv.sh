#!/bin/bash
hyperfine \
    --warmup 2 \
    --runs 10 \
    --export-json "/results/dejavu.djv.discrete.1M.$(date +%s).json" \
    --command-name Z \
        "dejavu /resources/dejavu/Z.qtl /dejavu/access/log4.csv > /dev/null 2>&1" \
    --command-name Access \
        "dejavu /dejavu/access/prop1.qtl /dejavu/access/log4.csv > /dev/null 2>&1" \
    --command-name File \
        "dejavu /dejavu/file/prop.qtl /dejavu/file/log4.csv > /dev/null 2>&1" \
    --command-name LocksBasic \
        "dejavu /dejavu/locks/basic/prop.qtl /dejavu/locks/basic/log4.csv > /dev/null 2>&1" \
    --command-name LocksDataraces \
        "dejavu /dejavu/locks/dataraces/prop.qtl /dejavu/locks/dataraces/log4.csv > /dev/null 2>&1" \
    --command-name LocksDeadlocks \
        "dejavu /dejavu/locks/deadlocks/prop.qtl /dejavu/locks/deadlocks/log4.csv > /dev/null 2>&1" \
    --command-name TimedAccess \
        "dejavu /dejavu/timed/access/spec.timed.txt /dejavu/timed/access/log.1000k.timed.csv > /dev/null 2>&1" \
    --command-name TimedCommands \
        "dejavu /dejavu/timed/commands/spec.timed.txt /dejavu/timed/commands/log.1000k.timed.csv > /dev/null 2>&1"
