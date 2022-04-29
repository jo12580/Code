.ALIASES
X_INTEG1    INTEG1(in=IN_ABM out=OUT_M ) CN @BEHAVIOR MODEL CREATION.Expression_element(sch_1):INS57@ABM.INTEG.Normal(chips)
E_ABM1          ABM1(OUT=OUT_ABM ) CN @BEHAVIOR MODEL CREATION.Expression_element(sch_1):INS78@ABM.ABM.Normal(chips)
V_V1            V1(+=IN_ABM -=0 ) CN @BEHAVIOR MODEL CREATION.Expression_element(sch_1):INS141@SOURCE.VSIN.Normal(chips)
_    _(IN_ABM=IN_ABM)
_    _(OUT_ABM=OUT_ABM)
_    _(OUT_M=OUT_M)
.ENDALIASES
