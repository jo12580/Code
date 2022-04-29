.ALIASES
X_U1            U1(IN+=IN IN-=0 OUT=OUT REF=0 RG+=N01913 RG-=N02009 VS+=+VS VS-=-VS ) CN
+@23.SCHEMATIC1(sch_1):INS1811@AD627A.AD627A.Normal(chips)
V_V1            V1(+=+VS -=0 ) CN @23.SCHEMATIC1(sch_1):INS1919@SOURCE.VDC.Normal(chips)
V_V2            V2(+=-VS -=0 ) CN @23.SCHEMATIC1(sch_1):INS1935@SOURCE.VDC.Normal(chips)
R_R1            R1(1=N02009 2=N01913 ) CN @23.SCHEMATIC1(sch_1):INS1979@ANALOG.R.Normal(chips)
C_C1            C1(1=+VS 2=0 ) CN @23.SCHEMATIC1(sch_1):INS1771@ANALOG.C.Normal(chips)
C_C2            C2(1=-VS 2=0 ) CN @23.SCHEMATIC1(sch_1):INS2063@ANALOG.C.Normal(chips)
V_V3            V3(+=IN -=0 ) CN @23.SCHEMATIC1(sch_1):INS2339@SOURCE.VSIN.Normal(chips)
_    _(+VS=+VS)
_    _(-VS=-VS)
_    _(in=IN)
_    _(out=OUT)
.ENDALIASES
