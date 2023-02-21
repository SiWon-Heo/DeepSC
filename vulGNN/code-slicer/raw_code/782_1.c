double histogram_selectivity ( VariableStatData * vardata , FmgrInfo * opproc , Datum constval , bool varonleft , int min_hist_size , int n_skip , int * hist_size ) {
 double result ;
 Datum * values ;
 int nvalues ;
 Assert ( n_skip >= 0 ) ;
 Assert ( min_hist_size > 2 * n_skip ) ;
 if ( HeapTupleIsValid ( vardata -> statsTuple ) && get_attstatsslot ( vardata -> statsTuple , vardata -> atttype , vardata -> atttypmod , STATISTIC_KIND_HISTOGRAM , InvalidOid , NULL , & values , & nvalues , NULL , NULL ) ) {
 * hist_size = nvalues ;
 if ( nvalues >= min_hist_size ) {
 int nmatch = 0 ;
 int i ;
 for ( i = n_skip ;
 i < nvalues - n_skip ;
 i ++ ) {
 if ( varonleft ? DatumGetBool ( FunctionCall2Coll ( opproc , DEFAULT_COLLATION_OID , values [ i ] , constval ) ) : DatumGetBool ( FunctionCall2Coll ( opproc , DEFAULT_COLLATION_OID , constval , values [ i ] ) ) ) nmatch ++ ;
 }
 result = ( ( double ) nmatch ) / ( ( double ) ( nvalues - 2 * n_skip ) ) ;
 }
 else result = - 1 ;
 free_attstatsslot ( vardata -> atttype , values , nvalues , NULL , 0 ) ;
 }
 else {
 * hist_size = 0 ;
 result = - 1 ;
 }
 return result ;
 }