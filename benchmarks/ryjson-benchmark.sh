#!/usr/bin/env bash
set -xeuo pipefail

commit_hash=$(git rev-parse HEAD)
echo "Running time series discrete benchmark for commit ${commit_hash}"

RYJSON_FLAGS=${RYJSON_FLAGS:-"-x"}
TESTDATA_DIR="${TESTDATA_DIR:-$1}"

hyperfine \
    --warmup 3 \
    --runs 25 \
    --export-json "${TESTDATA_DIR}/ryjson.${commit_hash}.dx.results.json" \
    --command-name AbsentAQ10 \
        "ryjson ${RYJSON_FLAGS} 'historically((once[:10]{q}) -> ((not{p}) since {q}))' ${TESTDATA_DIR}/AbsentAQ10.jsonl" \
    --command-name AbsentAQ100 \
        "ryjson ${RYJSON_FLAGS} 'historically((once[:100]{q}) -> ((not{p}) since {q}))' ${TESTDATA_DIR}/AbsentAQ100.jsonl" \
    --command-name AbsentAQ1000 \
        "ryjson ${RYJSON_FLAGS} 'historically((once[:1000]{q}) -> ((not{p}) since {q}))' ${TESTDATA_DIR}/AbsentAQ1000.jsonl" \
    --command-name AbsentBQR10 \
        "ryjson ${RYJSON_FLAGS} 'historically({r} && !{q} && once{q}) -> ((not{p}) since[3:10] {q})' ${TESTDATA_DIR}/AbsentBQR10.jsonl" \
    --command-name AbsentBQR100 \
        "ryjson ${RYJSON_FLAGS} 'historically({r} && !{q} && once{q}) -> ((not{p}) since[30:100] {q})' ${TESTDATA_DIR}/AbsentBQR100.jsonl" \
    --command-name AbsentBQR1000 \
        "ryjson ${RYJSON_FLAGS} 'historically({r} && !{q} && once{q}) -> ((not{p}) since[300:1000] {q})' ${TESTDATA_DIR}/AbsentBQR1000.jsonl" \
    --command-name AbsentBR10 \
        "ryjson ${RYJSON_FLAGS} 'historically({r} -> (historically[:10](not{p})))' ${TESTDATA_DIR}/AbsentBR10.jsonl" \
    --command-name AbsentBR100 \
        "ryjson ${RYJSON_FLAGS} 'historically({r} -> (historically[:100](not{p})))' ${TESTDATA_DIR}/AbsentBR100.jsonl" \
    --command-name AbsentBR1000 \
        "ryjson ${RYJSON_FLAGS} 'historically({r} -> (historically[:1000](not{p})))' ${TESTDATA_DIR}/AbsentBR1000.jsonl" \
    --command-name AlwaysAQ10 \
        "ryjson ${RYJSON_FLAGS} 'historically((once[:10]{q}) -> ({p} since {q}))' ${TESTDATA_DIR}/AlwaysAQ10.jsonl" \
    --command-name AlwaysAQ100 \
        "ryjson ${RYJSON_FLAGS} 'historically((once[:100]{q}) -> ({p} since {q}))' ${TESTDATA_DIR}/AlwaysAQ100.jsonl" \
    --command-name AlwaysAQ1000 \
        "ryjson ${RYJSON_FLAGS} 'historically((once[:1000]{q}) -> ({p} since {q}))' ${TESTDATA_DIR}/AlwaysAQ1000.jsonl" \
    --command-name AlwaysBQR10 \
        "ryjson ${RYJSON_FLAGS} 'historically(({r} && !{q} && once{q}) -> ({p} since[3:10] {q}))' ${TESTDATA_DIR}/AlwaysBQR10.jsonl" \
    --command-name AlwaysBQR100 \
        "ryjson ${RYJSON_FLAGS} 'historically(({r} && !{q} && once{q}) -> ({p} since[30:100] {q}))' ${TESTDATA_DIR}/AlwaysBQR100.jsonl" \
    --command-name AlwaysBQR1000 \
        "ryjson ${RYJSON_FLAGS} 'historically(({r} && !{q} && once{q}) -> ({p} since[300:1000] {q}))' ${TESTDATA_DIR}/AlwaysBQR1000.jsonl" \
    --command-name AlwaysBR10 \
        "ryjson ${RYJSON_FLAGS} 'historically({r} -> (historically[:10]{p}))' ${TESTDATA_DIR}/AlwaysBR10.jsonl" \
    --command-name AlwaysBR100 \
        "ryjson ${RYJSON_FLAGS} 'historically({r} -> (historically[:100]{p}))' ${TESTDATA_DIR}/AlwaysBR100.jsonl" \
    --command-name AlwaysBR1000 \
        "ryjson ${RYJSON_FLAGS} 'historically({r} -> (historically[:1000]{p}))' ${TESTDATA_DIR}/AlwaysBR1000.jsonl" \
    --command-name RecurBQR10 \
        "ryjson ${RYJSON_FLAGS} 'historically(({r} && !{q} && once{q}) -> ((once[:10]({p} or {q})) since {q}))' ${TESTDATA_DIR}/RecurBQR10.jsonl" \
    --command-name RecurBQR100 \
        "ryjson ${RYJSON_FLAGS} 'historically(({r} && !{q} && once{q}) -> ((once[:100]({p} or {q})) since {q}))' ${TESTDATA_DIR}/RecurBQR100.jsonl" \
    --command-name RecurBQR1000 \
        "ryjson ${RYJSON_FLAGS} 'historically(({r} && !{q} && once{q}) -> ((once[:1000]({p} or {q})) since {q}))' ${TESTDATA_DIR}/RecurBQR1000.jsonl" \
    --command-name RecurGLB10 \
        "ryjson ${RYJSON_FLAGS} 'historically(once[:10]{p})' ${TESTDATA_DIR}/RecurGLB10.jsonl" \
    --command-name RecurGLB100 \
        "ryjson ${RYJSON_FLAGS} 'historically(once[:100]{p})' ${TESTDATA_DIR}/RecurGLB100.jsonl" \
    --command-name RecurGLB1000 \
        "ryjson ${RYJSON_FLAGS} 'historically(once[:1000]{p})' ${TESTDATA_DIR}/RecurGLB1000.jsonl" \
    --command-name RespondBQR10 \
        "ryjson ${RYJSON_FLAGS} 'historically(({r} && !{q} && once{q}) -> ( (({s} -> once[3:10]{p}) and not((not {s}) since[10:] {p})) since {q}))' ${TESTDATA_DIR}/RespondBQR10.jsonl" \
    --command-name RespondBQR100 \
        "ryjson ${RYJSON_FLAGS} 'historically(({r} && !{q} && once{q}) -> ( (({s} -> once[30:100]{p}) and not((not {s}) since[100:] {p})) since {q}))' ${TESTDATA_DIR}/RespondBQR100.jsonl" \
    --command-name RespondBQR1000 \
        "ryjson ${RYJSON_FLAGS} 'historically(({r} && !{q} && once{q}) -> ( (({s} -> once[300:1000]{p}) and not((not {s}) since[1000:] {p})) since {q}))' ${TESTDATA_DIR}/RespondBQR1000.jsonl" \
    --command-name RespondGLB10 \
        "ryjson ${RYJSON_FLAGS} 'historically(({s} -> once[3:10]{p}) and not((not {s}) since[10:] {p}))' ${TESTDATA_DIR}/RespondGLB10.jsonl" \
    --command-name RespondGLB100 \
        "ryjson ${RYJSON_FLAGS} 'historically(({s} -> once[30:100]{p}) and not((not {s}) since[100:] {p}))' ${TESTDATA_DIR}/RespondGLB100.jsonl" \
    --command-name RespondGLB1000 \
        "ryjson ${RYJSON_FLAGS} 'historically(({s} -> once[300:1000]{p}) and not((not {s}) since[1000:] {p}))' ${TESTDATA_DIR}/RespondGLB1000.jsonl"
