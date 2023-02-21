static bool get_variable_range ( PlannerInfo * root , VariableStatData * vardata , Oid sortop , Datum * min , Datum * max ) {
 Datum tmin = 0 ;
 Datum tmax = 0 ;
 bool have_data = false ;
 int16 typLen ;
 bool typByVal ;
 Datum * values ;
 int nvalues ;
 int i ;
 # ifdef NOT_USED if ( get_actual_variable_range ( root , vardata , sortop , min , max ) ) return true ;
 # endif if ( ! HeapTupleIsValid ( vardata -> statsTuple ) ) {
 return false ;
 }
 get_typlenbyval ( vardata -> atttype , & typLen , & typByVal ) ;
 if ( get_attstatsslot ( vardata -> statsTuple , vardata -> atttype , vardata -> atttypmod , STATISTIC_KIND_HISTOGRAM , sortop , NULL , & values , & nvalues , NULL , NULL ) ) {
 if ( nvalues > 0 ) {
 tmin = datumCopy ( values [ 0 ] , typByVal , typLen ) ;
 tmax = datumCopy ( values [ nvalues - 1 ] , typByVal , typLen ) ;
 have_data = true ;
 }
 free_attstatsslot ( vardata -> atttype , values , nvalues , NULL , 0 ) ;
 }
 else if ( get_attstatsslot ( vardata -> statsTuple , vardata -> atttype , vardata -> atttypmod , STATISTIC_KIND_HISTOGRAM , InvalidOid , NULL , & values , & nvalues , NULL , NULL ) ) {
 free_attstatsslot ( vardata -> atttype , values , nvalues , NULL , 0 ) ;
 return false ;
 }
 if ( get_attstatsslot ( vardata -> statsTuple , vardata -> atttype , vardata -> atttypmod , STATISTIC_KIND_MCV , InvalidOid , NULL , & values , & nvalues , NULL , NULL ) ) {
 bool tmin_is_mcv = false ;
 bool tmax_is_mcv = false ;
 FmgrInfo opproc ;
 fmgr_info ( get_opcode ( sortop ) , & opproc ) ;
 for ( i = 0 ;
 i < nvalues ;
 i ++ ) {
 if ( ! have_data ) {
 tmin = tmax = values [ i ] ;
 tmin_is_mcv = tmax_is_mcv = have_data = true ;
 continue ;
 }
 if ( DatumGetBool ( FunctionCall2Coll ( & opproc , DEFAULT_COLLATION_OID , values [ i ] , tmin ) ) ) {
 tmin = values [ i ] ;
 tmin_is_mcv = true ;
 }
 if ( DatumGetBool ( FunctionCall2Coll ( & opproc , DEFAULT_COLLATION_OID , tmax , values [ i ] ) ) ) {
 tmax = values [ i ] ;
 tmax_is_mcv = true ;
 }
 }
 if ( tmin_is_mcv ) tmin = datumCopy ( tmin , typByVal , typLen ) ;
 if ( tmax_is_mcv ) tmax = datumCopy ( tmax , typByVal , typLen ) ;
 free_attstatsslot ( vardata -> atttype , values , nvalues , NULL , 0 ) ;
 }
 * min = tmin ;
 * max = tmax ;
 return have_data ;
 }