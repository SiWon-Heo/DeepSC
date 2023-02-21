double mcv_selectivity ( VariableStatData * vardata , FmgrInfo * opproc , Datum constval , bool varonleft , double * sumcommonp ) {
 double mcv_selec , sumcommon ;
 Datum * values ;
 int nvalues ;
 float4 * numbers ;
 int nnumbers ;
 int i ;
 mcv_selec = 0.0 ;
 sumcommon = 0.0 ;
 if ( HeapTupleIsValid ( vardata -> statsTuple ) && get_attstatsslot ( vardata -> statsTuple , vardata -> atttype , vardata -> atttypmod , STATISTIC_KIND_MCV , InvalidOid , NULL , & values , & nvalues , & numbers , & nnumbers ) ) {
 for ( i = 0 ;
 i < nvalues ;
 i ++ ) {
 if ( varonleft ? DatumGetBool ( FunctionCall2Coll ( opproc , DEFAULT_COLLATION_OID , values [ i ] , constval ) ) : DatumGetBool ( FunctionCall2Coll ( opproc , DEFAULT_COLLATION_OID , constval , values [ i ] ) ) ) mcv_selec += numbers [ i ] ;
 sumcommon += numbers [ i ] ;
 }
 free_attstatsslot ( vardata -> atttype , values , nvalues , numbers , nnumbers ) ;
 }
 * sumcommonp = sumcommon ;
 return mcv_selec ;
 }