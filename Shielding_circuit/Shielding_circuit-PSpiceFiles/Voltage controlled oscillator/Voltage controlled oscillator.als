.ALIASES
X_U1            U1(GND=0 TRIGGER=N00198 OUTPUT=OUT1 RESET=VCC CONTROL=N00106 THRESHOLD=N00198 DISCHARGE=N00277 VCC=VCC ) CN
+@SHIELDING_CIRCUIT.Voltage controlled oscillator(sch_1):INS52@ANL_MISC.555B.Normal(chips)
C_C3            C3(1=0 2=N00106 ) CN @SHIELDING_CIRCUIT.Voltage controlled oscillator(sch_1):INS119@ANALOG.C.Normal(chips)
R_R3            R3(1=N00277 2=N00198 ) CN @SHIELDING_CIRCUIT.Voltage controlled oscillator(sch_1):INS238@ANALOG.R.Normal(chips)
C_C1            C1(1=0 2=N00277 ) CN @SHIELDING_CIRCUIT.Voltage controlled oscillator(sch_1):INS331@ANALOG.C.Normal(chips)
R_R4            R4(1=N01818 2=N00277 ) CN @SHIELDING_CIRCUIT.Voltage controlled oscillator(sch_1):INS407@ANALOG.R.Normal(chips)
R_R2            R2(1=N00538 2=N00277 ) CN @SHIELDING_CIRCUIT.Voltage controlled oscillator(sch_1):INS462@ANALOG.R.Normal(chips)
R_R1            R1(1=N00538 2=VCC ) CN @SHIELDING_CIRCUIT.Voltage controlled oscillator(sch_1):INS520@ANALOG.R.Normal(chips)
C_C2            C2(1=OUT2 2=N00538 ) CN @SHIELDING_CIRCUIT.Voltage controlled oscillator(sch_1):INS551@ANALOG.C.Normal(chips)
Q_Q1            Q1(c=VCC b=N01818 e=OUT2 ) CN @SHIELDING_CIRCUIT.Voltage controlled
+oscillator(sch_1):INS594@BIPOLAR.Q2N3414.Normal(chips)
R_R5            R5(1=OUT2 2=0 ) CN @SHIELDING_CIRCUIT.Voltage controlled oscillator(sch_1):INS710@ANALOG.R.Normal(chips)
V_V1            V1(+=VCC -=0 ) CN @SHIELDING_CIRCUIT.Voltage controlled oscillator(sch_1):INS1016@SOURCE.VDC.Normal(chips)
_    _(out1=OUT1)
_    _(out2=OUT2)
_    _(VCC=VCC)
.ENDALIASES
