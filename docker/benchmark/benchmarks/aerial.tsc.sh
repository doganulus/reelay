hyperfine \
    --warmup 1 \
    --runs 2 \
    --export-json "results/aerial.tsc.discrete.1M.$(date +%s).json" \
    --command-name Z \
        "aerial -fmla /resources/monpoly/Z.mtl -log /timescales/fullsuite/AbsentAQ/Discrete/1M/AbsentAQ10.monpoly.log >> /dev/null 2>&1  " \
    --command-name AbsentAQ10 \
        "aerial -fmla /resources/monpoly/AbsentAQ10.mtl -log /timescales/fullsuite/AbsentAQ/Discrete/1M/AbsentAQ10.monpoly.log >> /dev/null 2>&1  " \
    --command-name AbsentAQ100 \
        "aerial -fmla /resources/monpoly/AbsentAQ100.mtl -log /timescales/fullsuite/AbsentAQ/Discrete/1M/AbsentAQ100.monpoly.log >> /dev/null 2>&1  " \
    --command-name AbsentAQ1000 \
        "aerial -fmla /resources/monpoly/AbsentAQ1000.mtl -log /timescales/fullsuite/AbsentAQ/Discrete/1M/AbsentAQ1000.monpoly.log >> /dev/null 2>&1  " \
    --command-name AbsentBQR10 \
        "aerial -fmla /resources/monpoly/AbsentBQR10.mtl -log /timescales/fullsuite/AbsentBQR/Discrete/1M/AbsentBQR10.monpoly.log >> /dev/null 2>&1  " \
    --command-name AbsentBQR100 \
        "aerial -fmla /resources/monpoly/AbsentBQR100.mtl -log /timescales/fullsuite/AbsentBQR/Discrete/1M/AbsentBQR100.monpoly.log >> /dev/null 2>&1  " \
    --command-name AbsentBQR1000 \
        "aerial -fmla /resources/monpoly/AbsentBQR1000.mtl -log /timescales/fullsuite/AbsentBQR/Discrete/1M/AbsentBQR1000.monpoly.log >> /dev/null 2>&1  " \
    --command-name AbsentBR10 \
        "aerial -fmla /resources/monpoly/AbsentBR10.mtl -log /timescales/fullsuite/AbsentBR/Discrete/1M/AbsentBR10.monpoly.log >> /dev/null 2>&1  " \
    --command-name AbsentBR100 \
        "aerial -fmla /resources/monpoly/AbsentBR100.mtl -log /timescales/fullsuite/AbsentBR/Discrete/1M/AbsentBR100.monpoly.log >> /dev/null 2>&1  " \
    --command-name AlwaysBR1000 \
        "aerial -fmla /resources/monpoly/AlwaysBR1000.mtl -log /timescales/fullsuite/AlwaysBR/Discrete/1M/AlwaysBR1000.monpoly.log >> /dev/null 2>&1  " \
    --command-name AlwaysAQ10 \
        "aerial -fmla /resources/monpoly/AlwaysAQ10.mtl -log /timescales/fullsuite/AlwaysAQ/Discrete/1M/AlwaysAQ10.monpoly.log >> /dev/null 2>&1  " \
    --command-name AlwaysAQ100 \
        "aerial -fmla /resources/monpoly/AlwaysAQ100.mtl -log /timescales/fullsuite/AlwaysAQ/Discrete/1M/AlwaysAQ100.monpoly.log >> /dev/null 2>&1  " \
    --command-name AlwaysAQ1000 \
        "aerial -fmla /resources/monpoly/AlwaysAQ1000.mtl -log /timescales/fullsuite/AlwaysAQ/Discrete/1M/AlwaysAQ1000.monpoly.log >> /dev/null 2>&1  " \
    --command-name AlwaysBQR10 \
        "aerial -fmla /resources/monpoly/AlwaysBQR10.mtl -log /timescales/fullsuite/AlwaysBQR/Discrete/1M/AlwaysBQR10.monpoly.log >> /dev/null 2>&1  " \
    --command-name AlwaysBQR100 \
        "aerial -fmla /resources/monpoly/AlwaysBQR100.mtl -log /timescales/fullsuite/AlwaysBQR/Discrete/1M/AlwaysBQR100.monpoly.log >> /dev/null 2>&1  " \
    --command-name AlwaysBQR1000 \
        "aerial -fmla /resources/monpoly/AlwaysBQR1000.mtl -log /timescales/fullsuite/AlwaysBQR/Discrete/1M/AlwaysBQR1000.monpoly.log >> /dev/null 2>&1  " \
    --command-name AlwaysBR10 \
        "aerial -fmla /resources/monpoly/AlwaysBR10.mtl -log /timescales/fullsuite/AlwaysBR/Discrete/1M/AlwaysBR10.monpoly.log >> /dev/null 2>&1  " \
    --command-name AlwaysBR100 \
        "aerial -fmla /resources/monpoly/AlwaysBR100.mtl -log /timescales/fullsuite/AlwaysBR/Discrete/1M/AlwaysBR100.monpoly.log >> /dev/null 2>&1  " \
    --command-name AlwaysBR1000 \
        "aerial -fmla /resources/monpoly/AlwaysBR1000.mtl -log /timescales/fullsuite/AlwaysBR/Discrete/1M/AlwaysBR1000.monpoly.log >> /dev/null 2>&1  " \
    --command-name RecurBQR10 \
        "aerial -fmla /resources/monpoly/RecurBQR10.mtl -log /timescales/fullsuite/RecurBQR/Discrete/1M/RecurBQR10.monpoly.log >> /dev/null 2>&1  " \
    --command-name RecurBQR100 \
        "aerial -fmla /resources/monpoly/RecurBQR100.mtl -log /timescales/fullsuite/RecurBQR/Discrete/1M/RecurBQR100.monpoly.log >> /dev/null 2>&1  " \
    --command-name RecurBQR1000 \
        "aerial -fmla /resources/monpoly/RecurBQR1000.mtl -log /timescales/fullsuite/RecurBQR/Discrete/1M/RecurBQR1000.monpoly.log >> /dev/null 2>&1  " \
    --command-name RecurGLB10 \
        "aerial -fmla /resources/monpoly/RecurGLB10.mtl -log /timescales/fullsuite/RecurGLB/Discrete/1M/RecurGLB10.monpoly.log >> /dev/null 2>&1  " \
    --command-name RecurGLB100 \
        "aerial -fmla /resources/monpoly/RecurGLB100.mtl -log /timescales/fullsuite/RecurGLB/Discrete/1M/RecurGLB100.monpoly.log >> /dev/null 2>&1  " \
    --command-name RecurGLB1000 \
        "aerial -fmla /resources/monpoly/RecurGLB1000.mtl -log /timescales/fullsuite/RecurGLB/Discrete/1M/RecurGLB1000.monpoly.log >> /dev/null 2>&1  " \
    --command-name RespondBQR10 \
        "aerial -fmla /resources/monpoly/RespondBQR10.mtl -log /timescales/fullsuite/RespondBQR/Discrete/1M/RespondBQR10.monpoly.log >> /dev/null 2>&1  " \
    --command-name RespondBQR100 \
        "aerial -fmla /resources/monpoly/RespondBQR100.mtl -log /timescales/fullsuite/RespondBQR/Discrete/1M/RespondBQR100.monpoly.log >> /dev/null 2>&1  " \
    --command-name RespondBQR1000 \
        "aerial -fmla /resources/monpoly/RespondBQR1000.mtl -log /timescales/fullsuite/RespondBQR/Discrete/1M/RespondBQR1000.monpoly.log >> /dev/null 2>&1  " \
    --command-name RespondGLB10 \
        "aerial -fmla /resources/monpoly/RespondGLB10.mtl -log /timescales/fullsuite/RespondGLB/Discrete/1M/RespondGLB10.monpoly.log >> /dev/null 2>&1  " \
    --command-name RespondGLB100 \
        "aerial -fmla /resources/monpoly/RespondGLB100.mtl -log /timescales/fullsuite/RespondGLB/Discrete/1M/RespondGLB100.monpoly.log >> /dev/null 2>&1  " \
    --command-name RespondGLB1000 \
        "aerial -fmla /resources/monpoly/RespondGLB1000.mtl -log /timescales/fullsuite/RespondGLB/Discrete/1M/RespondGLB1000.monpoly.log >> /dev/null 2>&1  " \
