.ALIASES
R_R2            R2(1=N00169 2=VCC ) CN @13_4.NE555(sch_1):INS145@ANALOG.R.Normal(chips)
R_R1            R1(1=VC 2=N00169 ) CN @13_4.NE555(sch_1):INS129@ANALOG.R.Normal(chips)
X_X1            X1(GND=0 TRIGGER=VC OUTPUT=OUT RESET=VCC CONTROL=N00214 THRESHOLD=VC DISCHARGE=N00169 VCC=VCC ) CN
+@13_4.NE555(sch_1):INS38@EVAL.555D.Normal(chips)
C_C2            C2(1=0 2=VC ) CN @13_4.NE555(sch_1):INS104@ANALOG.C.Normal(chips)
C_C1            C1(1=0 2=N00214 ) CN @13_4.NE555(sch_1):INS88@ANALOG.C.Normal(chips)
V_V1            V1(+=VCC -=0 ) CN @13_4.NE555(sch_1):INS277@SOURCE.VDC.Normal(chips)
_    _(OUT=OUT)
_    _(VC=VC)
_    _(VCC=VCC)
.ENDALIASES
