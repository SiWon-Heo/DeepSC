static double ineq_histogram_selectivity ( PlannerInfo * root , VariableStatData * vardata , FmgrInfo * opproc , bool isgt , Datum constval , Oid consttype ) {
 double hist_selec ;
 Oid hist_op ;
 Datum * values ;
 int nvalues ;
 hist_selec = - 1.0 ;
 if ( HeapTupleIsValid ( vardata -> statsTuple ) && get_attstatsslot ( vardata -> statsTuple , vardata -> atttype , vardata -> atttypmod , STATISTIC_KIND_HISTOGRAM , InvalidOid , & hist_op , & values , & nvalues , NULL , NULL ) ) {
 if ( nvalues > 1 ) {
 double histfrac ;
 int lobound = 0 ;
 int hibound = nvalues ;
 bool have_end = false ;
 if ( nvalues == 2 ) have_end = get_actual_variable_range ( root , vardata , hist_op , & values [ 0 ] , & values [ 1 ] ) ;
 while ( lobound < hibound ) {
 int probe = ( lobound + hibound ) / 2 ;
 bool ltcmp ;
 if ( probe == 0 && nvalues > 2 ) have_end = get_actual_variable_range ( root , vardata , hist_op , & values [ 0 ] , NULL ) ;
 else if ( probe == nvalues - 1 && nvalues > 2 ) have_end = get_actual_variable_range ( root , vardata , hist_op , NULL , & values [ probe ] ) ;
 ltcmp = DatumGetBool ( FunctionCall2Coll ( opproc , DEFAULT_COLLATION_OID , values [ probe ] , constval ) ) ;
 if ( isgt ) ltcmp = ! ltcmp ;
 if ( ltcmp ) lobound = probe + 1 ;
 else hibound = probe ;
 }
 if ( lobound <= 0 ) {
 histfrac = 0.0 ;
 }
 else if ( lobound >= nvalues ) {
 histfrac = 1.0 ;
 }
 else {
 int i = lobound ;
 double val , high , low ;
 double binfrac ;
 if ( convert_to_scalar ( constval , consttype , & val , values [ i - 1 ] , values [ i ] , vardata -> vartype , & low , & high ) ) {
 if ( high <= low ) {
 binfrac = 0.5 ;
 }
 else if ( val <= low ) binfrac = 0.0 ;
 else if ( val >= high ) binfrac = 1.0 ;
 else {
 binfrac = ( val - low ) / ( high - low ) ;
 if ( isnan ( binfrac ) || binfrac < 0.0 || binfrac > 1.0 ) binfrac = 0.5 ;
 }
 }
 else {
 binfrac = 0.5 ;
 }
 histfrac = ( double ) ( i - 1 ) + binfrac ;
 histfrac /= ( double ) ( nvalues - 1 ) ;
 }
 hist_selec = isgt ? ( 1.0 - histfrac ) : histfrac ;
 if ( have_end ) CLAMP_PROBABILITY ( hist_selec ) ;
 else {
 if ( hist_selec < 0.0001 ) hist_selec = 0.0001 ;
 else if ( hist_selec > 0.9999 ) hist_selec = 0.9999 ;
 }
 }
 free_attstatsslot ( vardata -> atttype , values , nvalues , NULL , 0 ) ;
 }
 return hist_selec ;
 }