.ALIASES
V_V1            V1(+=IN1 -=0 ) CN @AC SWEEP.SCHEMATIC1(sch_1):INS42@SOURCE.VAC.Normal(chips)
V_V2            V2(+=IN2 -=0 ) CN @AC SWEEP.SCHEMATIC1(sch_1):INS62@SOURCE.VAC.Normal(chips)
C_C1            C1(1=0 2=OUT1 ) CN @AC SWEEP.SCHEMATIC1(sch_1):INS91@ANALOG.C.Normal(chips)
R_R1            R1(1=IN1 2=OUT1 ) CN @AC SWEEP.SCHEMATIC1(sch_1):INS116@ANALOG.R.Normal(chips)
C_C2            C2(1=IN2 2=OUT2 ) CN @AC SWEEP.SCHEMATIC1(sch_1):INS141@ANALOG.C.Normal(chips)
R_R2            R2(1=0 2=OUT2 ) CN @AC SWEEP.SCHEMATIC1(sch_1):INS166@ANALOG.R.Normal(chips)
_    _(in1=IN1)
_    _(in2=IN2)
_    _(out1=OUT1)
_    _(out2=OUT2)
.ENDALIASES
