.ALIASES
U_DSTM1          DSTM1(VCC=$G_DPWR GND=$G_DGND 1=CLK ) CN @13_4.SCHEMATIC2(sch_1):INS1573@DATACONV.DigClock.Normal(chips)
X_U1A           U1A(A=CLK CLR=$D_LO QA=D1 QB=D2 QC=D3 QD=D4 VCC=$G_DPWR GND=$G_DGND ) CN
+@13_4.SCHEMATIC2(sch_1):INS38@74HC.74HC393.Normal(chips)
X_U1B           U1B(A=D4 CLR=$D_LO QA=D5 QB=D6 QC=D7 QD=D8 VCC=$G_DPWR GND=$G_DGND ) CN
+@13_4.SCHEMATIC2(sch_1):INS91@74HC.74HC393.Normal(chips)
X_U3            U3(1A1=D1 1A2=D2 1A3=D3 1A4=D4 2A1=D5 2A2=D6 2A3=D7 2A4=D8 1Gbar=$D_LO 2Gbar=$D_LO 1Y1=B1 1Y2=B2 1Y3=B3 1Y4=B4
+2Y1=B5 2Y2=B6 2Y3=B7 2Y4=B8 VCC=$G_DPWR GND=$G_DGND ) CN @13_4.SCHEMATIC2(sch_1):INS290@74HC.74HC240.Normal(chips)
X_U2            U2(1A1=D1 1A2=D2 1A3=D3 1A4=D4 2A1=D5 2A2=D6 2A3=D7 2A4=D8 1Gbar=$D_LO 2Gbar=$D_LO 1Y1=A1 1Y2=A2 1Y3=A3 1Y4=A4
+2Y1=A5 2Y2=A6 2Y3=A7 2Y4=A8 VCC=$G_DPWR GND=$G_DGND ) CN @13_4.SCHEMATIC2(sch_1):INS189@74HC.74HC244.Normal(chips)
_    _(A1=A1)
_    _(A2=A2)
_    _(A3=A3)
_    _(A4=A4)
_    _(A5=A5)
_    _(A6=A6)
_    _(A7=A7)
_    _(A8=A8)
_    _(B1=B1)
_    _(B2=B2)
_    _(B3=B3)
_    _(B4=B4)
_    _(B5=B5)
_    _(B6=B6)
_    _(B7=B7)
_    _(B8=B8)
_    _(CLK=CLK)
_    _(D1=D1)
_    _(D2=D2)
_    _(D3=D3)
_    _(D4=D4)
_    _(D5=D5)
_    _(D6=D6)
_    _(D7=D7)
_    _(D8=D8)
.ENDALIASES