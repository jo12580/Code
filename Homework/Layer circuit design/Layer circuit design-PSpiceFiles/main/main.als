.ALIASES
R_R1            R1(1=N00385 2=IN1+ ) CN @LAYER CIRCUIT DESIGN.main(sch_1):INS82@ANALOG.R.Normal(chips)
R_R2            R2(1=0 2=IN1- ) CN @LAYER CIRCUIT DESIGN.main(sch_1):INS98@ANALOG.R.Normal(chips)
R_R3            R3(1=OUT1 2=N00229 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):INS114@ANALOG.R.Normal(chips)
R_R4            R4(1=IN2- 2=N00389 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):INS130@ANALOG.R.Normal(chips)
R_R5            R5(1=OUT2 2=N00262 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):INS146@ANALOG.R.Normal(chips)
C_C1            C1(1=IN1- 2=N00229 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):INS171@ANALOG.C.Normal(chips)
C_C2            C2(1=IN2- 2=N00262 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):INS187@ANALOG.C.Normal(chips)
V_V1            V1(+=VCC -=0 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):INS302@SOURCE.VDC.Normal(chips)
V_V2            V2(+=VEE -=0 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):INS318@SOURCE.VDC.Normal(chips)
V_V3            V3(+=N00385 -=0 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):INS345@SOURCE.VAC.Normal(chips)
V_V4            V4(+=N00389 -=0 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):INS365@SOURCE.VAC.Normal(chips)
_    A1(Vin+=IN1+ Vin-=IN1- Vbias=0 Vout=OUT1 VEE=VEE VCC=VCC ) CN 
+@LAYER CIRCUIT DESIGN.main(sch_1):A1@LAYER CIRCUIT DESIGN.Circuit_module(sch_1)
M_A1_M2          A1.M2(d=A1_N03029 g=A1_N03075 s=A1_N03161 s=A1_N03161 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A1@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS3101@JPWRMOS.M2SK1044.Normal(chips)
R_A1_R5          A1.R5(1=VEE 2=OUT1 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A1@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS3171@ANALOG.R.Normal(chips)
R_A1_R6          A1.R6(1=A1_N02857 2=VCC ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A1@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS2815@ANALOG.R.Normal(chips)
R_A1_R3          A1.R3(1=VEE 2=A1_N03161 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A1@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS3191@ANALOG.R.Normal(chips)
R_A1_R2          A1.R2(1=A1_OUT+ 2=VCC ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A1@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS2835@ANALOG.R.Normal(chips)
R_A1_R7          A1.R7(1=0 2=A1_N03075 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A1@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS3041@ANALOG.R.Normal(chips)
M_A1_M3          A1.M3(d=A1_N03075 g=A1_N03075 s=A1_N03167 s=A1_N03167 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A1@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS3125@JPWRMOS.M2SK1044.Normal(chips)
M_A1_M1          A1.M1(d=A1_OUT- g=IN1+ s=A1_N03029 s=A1_N03029 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A1@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS2957@JPWRMOS.M2SK1044.Normal(chips)
R_A1_R4          A1.R4(1=VEE 2=A1_N03167 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A1@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS3215@ANALOG.R.Normal(chips)
C_A1_C1          A1.C1(1=A1_N02857 2=VCC ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A1@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS2859@ANALOG.C.Normal(chips)
M_A1_M5          A1.M5(d=OUT1 g=A1_OUT- s=A1_N02857 s=A1_N02857 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A1@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS2909@JPWRMOS.M2SJ114.Normal(chips)
M_A1_M6          A1.M6(d=A1_OUT+ g=IN1- s=A1_N03029 s=A1_N03029 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A1@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS4066@JPWRMOS.M2SK1044.Normal(chips)
R_A1_R1          A1.R1(1=A1_OUT- 2=VCC ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A1@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS2879@ANALOG.R.Normal(chips)
_    _(A1.out+=A1_OUT+)
_    _(A1.out-=A1_OUT-)
_    _(A1.Vbias=0)
_    _(A1.VCC=VCC)
_    _(A1.VEE=VEE)
_    _(A1.Vin+=IN1+)
_    _(A1.Vin-=IN1-)
_    _(A1.Vout=OUT1)
_    A2(Vin+=0 Vin-=IN2- Vbias=0 Vout=OUT2 VEE=VEE VCC=VCC ) CN 
+@LAYER CIRCUIT DESIGN.main(sch_1):A2@LAYER CIRCUIT DESIGN.Circuit_module(sch_1)
M_A2_M2          A2.M2(d=A2_N03029 g=A2_N03075 s=A2_N03161 s=A2_N03161 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A2@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS3101@JPWRMOS.M2SK1044.Normal(chips)
R_A2_R5          A2.R5(1=VEE 2=OUT2 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A2@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS3171@ANALOG.R.Normal(chips)
R_A2_R6          A2.R6(1=A2_N02857 2=VCC ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A2@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS2815@ANALOG.R.Normal(chips)
R_A2_R3          A2.R3(1=VEE 2=A2_N03161 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A2@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS3191@ANALOG.R.Normal(chips)
R_A2_R2          A2.R2(1=A2_OUT+ 2=VCC ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A2@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS2835@ANALOG.R.Normal(chips)
R_A2_R7          A2.R7(1=0 2=A2_N03075 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A2@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS3041@ANALOG.R.Normal(chips)
M_A2_M3          A2.M3(d=A2_N03075 g=A2_N03075 s=A2_N03167 s=A2_N03167 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A2@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS3125@JPWRMOS.M2SK1044.Normal(chips)
M_A2_M1          A2.M1(d=A2_OUT- g=0 s=A2_N03029 s=A2_N03029 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A2@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS2957@JPWRMOS.M2SK1044.Normal(chips)
R_A2_R4          A2.R4(1=VEE 2=A2_N03167 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A2@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS3215@ANALOG.R.Normal(chips)
C_A2_C1          A2.C1(1=A2_N02857 2=VCC ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A2@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS2859@ANALOG.C.Normal(chips)
M_A2_M5          A2.M5(d=OUT2 g=A2_OUT- s=A2_N02857 s=A2_N02857 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A2@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS2909@JPWRMOS.M2SJ114.Normal(chips)
M_A2_M6          A2.M6(d=A2_OUT+ g=IN2- s=A2_N03029 s=A2_N03029 ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A2@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS4066@JPWRMOS.M2SK1044.Normal(chips)
R_A2_R1          A2.R1(1=A2_OUT- 2=VCC ) CN @LAYER CIRCUIT DESIGN.main(sch_1):A2@LAYER CIRCUIT
+DESIGN.Circuit_module(sch_1):INS2879@ANALOG.R.Normal(chips)
_    _(A2.out+=A2_OUT+)
_    _(A2.out-=A2_OUT-)
_    _(A2.Vbias=0)
_    _(A2.VCC=VCC)
_    _(A2.VEE=VEE)
_    _(A2.Vin+=0)
_    _(A2.Vin-=IN2-)
_    _(A2.Vout=OUT2)
_    _(in2+=0)
_    _(in1+=IN1+)
_    _(in1-=IN1-)
_    _(in2-=IN2-)
_    _(out1=OUT1)
_    _(out2=OUT2)
_    _(VCC=VCC)
_    _(VEE=VEE)
.ENDALIASES
