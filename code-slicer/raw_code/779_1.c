static double var_eq_const ( VariableStatData * vardata , Oid operator , Datum constval , bool constisnull , bool varonleft ) {
 double selec ;
 bool isdefault ;
 if ( constisnull ) return 0.0 ;
 if ( vardata -> isunique && vardata -> rel && vardata -> rel -> tuples >= 1.0 ) return 1.0 / vardata -> rel -> tuples ;
 if ( HeapTupleIsValid ( vardata -> statsTuple ) ) {
 Form_pg_statistic stats ;
 Datum * values ;
 int nvalues ;
 float4 * numbers ;
 int nnumbers ;
 bool match = false ;
 int i ;
 stats = ( Form_pg_statistic ) GETSTRUCT ( vardata -> statsTuple ) ;
 if ( get_attstatsslot ( vardata -> statsTuple , vardata -> atttype , vardata -> atttypmod , STATISTIC_KIND_MCV , InvalidOid , NULL , & values , & nvalues , & numbers , & nnumbers ) ) {
 FmgrInfo eqproc ;
 fmgr_info ( get_opcode ( operator ) , & eqproc ) ;
 for ( i = 0 ;
 i < nvalues ;
 i ++ ) {
 if ( varonleft ) match = DatumGetBool ( FunctionCall2Coll ( & eqproc , DEFAULT_COLLATION_OID , values [ i ] , constval ) ) ;
 else match = DatumGetBool ( FunctionCall2Coll ( & eqproc , DEFAULT_COLLATION_OID , constval , values [ i ] ) ) ;
 if ( match ) break ;
 }
 }
 else {
 values = NULL ;
 numbers = NULL ;
 i = nvalues = nnumbers = 0 ;
 }
 if ( match ) {
 selec = numbers [ i ] ;
 }
 else {
 double sumcommon = 0.0 ;
 double otherdistinct ;
 for ( i = 0 ;
 i < nnumbers ;
 i ++ ) sumcommon += numbers [ i ] ;
 selec = 1.0 - sumcommon - stats -> stanullfrac ;
 CLAMP_PROBABILITY ( selec ) ;
 otherdistinct = get_variable_numdistinct ( vardata , & isdefault ) - nnumbers ;
 if ( otherdistinct > 1 ) selec /= otherdistinct ;
 if ( nnumbers > 0 && selec > numbers [ nnumbers - 1 ] ) selec = numbers [ nnumbers - 1 ] ;
 }
 free_attstatsslot ( vardata -> atttype , values , nvalues , numbers , nnumbers ) ;
 }
 else {
 selec = 1.0 / get_variable_numdistinct ( vardata , & isdefault ) ;
 }
 CLAMP_PROBABILITY ( selec ) ;
 return selec ;
 }