.ALIASES
E_LIMIT1          LIMIT1(OUT=LIMIT IN=IN ) CN @BEHAVIOR MODEL CREATION.Limiter_element(sch_1):INS100@ABM.LIMIT.Normal(chips)
E_GLIMIT1          GLIMIT1(OUT=GLIMIT IN=IN ) CN @BEHAVIOR MODEL CREATION.Limiter_element(sch_1):INS127@ABM.GLIMIT.Normal(chips)
E_SLIMIT1          SLIMIT1(OUT=SOFTLIM IN=IN ) CN @BEHAVIOR MODEL CREATION.Limiter_element(sch_1):INS158@ABM.SOFTLIM.Normal(chips)
V_V1            V1(+=IN -=0 ) CN @BEHAVIOR MODEL CREATION.Limiter_element(sch_1):INS455@SOURCE.VSIN.Normal(chips)
_    _(GLIMIT=GLIMIT)
_    _(IN=IN)
_    _(LIMIT=LIMIT)
_    _(SOFTLIM=SOFTLIM)
.ENDALIASES
