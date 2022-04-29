.ALIASES
U_DSTM1          DSTM1(VCC=$G_DPWR GND=$G_DGND 1=CLK ) CN @DIGITAL SIGNAL
+SOURCE.digital_circuit(sch_1):INS147@SOURCE.DigClock.Normal(chips)
U_DSTM2          DSTM2(pin1=N00581 ) CN @DIGITAL SIGNAL SOURCE.digital_circuit(sch_1):INS202@SOURCE.STIM1.Normal(chips)
X_U1A           U1A(CLK=CLK CLRbar=N00581 J=$D_HI K=$D_HI Q=Q0 Qbar=M_UN0001 VCC=$G_DPWR GND=$G_DGND ) CN @DIGITAL SIGNAL
+SOURCE.digital_circuit(sch_1):INS239@7400.74107.Normal(chips)
X_U1B           U1B(CLK=Q0 CLRbar=N00581 J=$D_HI K=$D_HI Q=Q1 Qbar=M_UN0002 VCC=$G_DPWR GND=$G_DGND ) CN @DIGITAL SIGNAL
+SOURCE.digital_circuit(sch_1):INS292@7400.74107.Normal(chips)
X_U2A           U2A(CLK=Q1 CLRbar=N00581 J=$D_HI K=$D_HI Q=Q2 Qbar=M_UN0003 VCC=$G_DPWR GND=$G_DGND ) CN @DIGITAL SIGNAL
+SOURCE.digital_circuit(sch_1):INS345@7400.74107.Normal(chips)
X_U2B           U2B(CLK=Q2 CLRbar=N00581 J=$D_HI K=$D_HI Q=Q3 Qbar=M_UN0004 VCC=$G_DPWR GND=$G_DGND ) CN @DIGITAL SIGNAL
+SOURCE.digital_circuit(sch_1):INS398@7400.74107.Normal(chips)
_    _(clk=CLK)
_    _(Q0=Q0)
_    _(Q1=Q1)
_    _(Q2=Q2)
_    _(Q3=Q3)
.ENDALIASES
