static Selectivity calc_arraycontsel ( VariableStatData * vardata , Datum constval , Oid elemtype , Oid operator ) {
 Selectivity selec ;
 TypeCacheEntry * typentry ;
 FmgrInfo * cmpfunc ;
 ArrayType * array ;
 typentry = lookup_type_cache ( elemtype , TYPECACHE_CMP_PROC_FINFO ) ;
 if ( ! OidIsValid ( typentry -> cmp_proc_finfo . fn_oid ) ) return DEFAULT_SEL ( operator ) ;
 cmpfunc = & typentry -> cmp_proc_finfo ;
 array = DatumGetArrayTypeP ( constval ) ;
 if ( HeapTupleIsValid ( vardata -> statsTuple ) ) {
 Form_pg_statistic stats ;
 Datum * values ;
 int nvalues ;
 float4 * numbers ;
 int nnumbers ;
 float4 * hist ;
 int nhist ;
 stats = ( Form_pg_statistic ) GETSTRUCT ( vardata -> statsTuple ) ;
 if ( get_attstatsslot ( vardata -> statsTuple , elemtype , vardata -> atttypmod , STATISTIC_KIND_MCELEM , InvalidOid , NULL , & values , & nvalues , & numbers , & nnumbers ) ) {
 if ( operator != OID_ARRAY_CONTAINED_OP || ! get_attstatsslot ( vardata -> statsTuple , elemtype , vardata -> atttypmod , STATISTIC_KIND_DECHIST , InvalidOid , NULL , NULL , NULL , & hist , & nhist ) ) {
 hist = NULL ;
 nhist = 0 ;
 }
 selec = mcelem_array_selec ( array , typentry , values , nvalues , numbers , nnumbers , hist , nhist , operator , cmpfunc ) ;
 if ( hist ) free_attstatsslot ( elemtype , NULL , 0 , hist , nhist ) ;
 free_attstatsslot ( elemtype , values , nvalues , numbers , nnumbers ) ;
 }
 else {
 selec = mcelem_array_selec ( array , typentry , NULL , 0 , NULL , 0 , NULL , 0 , operator , cmpfunc ) ;
 }
 selec *= ( 1.0 - stats -> stanullfrac ) ;
 }
 else {
 selec = mcelem_array_selec ( array , typentry , NULL , 0 , NULL , 0 , NULL , 0 , operator , cmpfunc ) ;
 }
 if ( PointerGetDatum ( array ) != constval ) pfree ( array ) ;
 return selec ;
 }