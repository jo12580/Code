.ALIASES
V_V1            V1(+=IN -=0 ) CN @DIGITAL SIGNAL SOURCE.Digital_analog_mixing(sch_1):INS32@SOURCE.VSIN.Normal(chips)
V_V2            V2(+=V+ -=0 ) CN @DIGITAL SIGNAL SOURCE.Digital_analog_mixing(sch_1):INS69@SOURCE.VDC.Normal(chips)
V_V3            V3(+=V- -=0 ) CN @DIGITAL SIGNAL SOURCE.Digital_analog_mixing(sch_1):INS85@SOURCE.VDC.Normal(chips)
X_U1            U1(+=N00347 -=IN V+=V+ V-=V- OUT=COMP G=V- ) CN @DIGITAL SIGNAL
+SOURCE.Digital_analog_mixing(sch_1):INS122@OPAMP.LM311.Normal(chips)
X_U2A           U2A(A=COMP B=CLK Y=OUT VCC=$G_DPWR GND=$G_DGND ) CN @DIGITAL SIGNAL
+SOURCE.Digital_analog_mixing(sch_1):INS169@74HC.74HC00.Normal(chips)
R_R1            R1(1=N00347 2=COMP ) CN @DIGITAL SIGNAL SOURCE.Digital_analog_mixing(sch_1):INS219@ANALOG.R.Normal(chips)
R_R2            R2(1=0 2=N00347 ) CN @DIGITAL SIGNAL SOURCE.Digital_analog_mixing(sch_1):INS235@ANALOG.R.Normal(chips)
R_R3            R3(1=COMP 2=V+ ) CN @DIGITAL SIGNAL SOURCE.Digital_analog_mixing(sch_1):INS251@ANALOG.R.Normal(chips)
U_DSTM1          DSTM1(VCC=$G_DPWR GND=$G_DGND 1=CLK ) CN @DIGITAL SIGNAL
+SOURCE.Digital_analog_mixing(sch_1):INS285@SOURCE.DigClock.Normal(chips)
_    _(CLK=CLK)
_    _(comp=COMP)
_    _(IN=IN)
_    _(OUT=OUT)
_    _(V+=V+)
_    _(V-=V-)
.ENDALIASES
