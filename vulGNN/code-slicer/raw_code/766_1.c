Selectivity scalararraysel_containment ( PlannerInfo * root , Node * leftop , Node * rightop , Oid elemtype , bool isEquality , bool useOr , int varRelid ) {
 Selectivity selec ;
 VariableStatData vardata ;
 Datum constval ;
 TypeCacheEntry * typentry ;
 FmgrInfo * cmpfunc ;
 examine_variable ( root , rightop , varRelid , & vardata ) ;
 if ( ! vardata . rel ) {
 ReleaseVariableStats ( vardata ) ;
 return - 1.0 ;
 }
 if ( ! IsA ( leftop , Const ) ) {
 ReleaseVariableStats ( vardata ) ;
 return - 1.0 ;
 }
 if ( ( ( Const * ) leftop ) -> constisnull ) {
 ReleaseVariableStats ( vardata ) ;
 return ( Selectivity ) 0.0 ;
 }
 constval = ( ( Const * ) leftop ) -> constvalue ;
 typentry = lookup_type_cache ( elemtype , TYPECACHE_CMP_PROC_FINFO ) ;
 if ( ! OidIsValid ( typentry -> cmp_proc_finfo . fn_oid ) ) {
 ReleaseVariableStats ( vardata ) ;
 return - 1.0 ;
 }
 cmpfunc = & typentry -> cmp_proc_finfo ;
 if ( ! isEquality ) useOr = ! useOr ;
 if ( HeapTupleIsValid ( vardata . statsTuple ) ) {
 Form_pg_statistic stats ;
 Datum * values ;
 int nvalues ;
 float4 * numbers ;
 int nnumbers ;
 float4 * hist ;
 int nhist ;
 stats = ( Form_pg_statistic ) GETSTRUCT ( vardata . statsTuple ) ;
 if ( get_attstatsslot ( vardata . statsTuple , elemtype , vardata . atttypmod , STATISTIC_KIND_MCELEM , InvalidOid , NULL , & values , & nvalues , & numbers , & nnumbers ) ) {
 if ( useOr || ! get_attstatsslot ( vardata . statsTuple , elemtype , vardata . atttypmod , STATISTIC_KIND_DECHIST , InvalidOid , NULL , NULL , NULL , & hist , & nhist ) ) {
 hist = NULL ;
 nhist = 0 ;
 }
 if ( useOr ) selec = mcelem_array_contain_overlap_selec ( values , nvalues , numbers , nnumbers , & constval , 1 , OID_ARRAY_CONTAINS_OP , cmpfunc ) ;
 else selec = mcelem_array_contained_selec ( values , nvalues , numbers , nnumbers , & constval , 1 , hist , nhist , OID_ARRAY_CONTAINED_OP , cmpfunc ) ;
 if ( hist ) free_attstatsslot ( elemtype , NULL , 0 , hist , nhist ) ;
 free_attstatsslot ( elemtype , values , nvalues , numbers , nnumbers ) ;
 }
 else {
 if ( useOr ) selec = mcelem_array_contain_overlap_selec ( NULL , 0 , NULL , 0 , & constval , 1 , OID_ARRAY_CONTAINS_OP , cmpfunc ) ;
 else selec = mcelem_array_contained_selec ( NULL , 0 , NULL , 0 , & constval , 1 , NULL , 0 , OID_ARRAY_CONTAINED_OP , cmpfunc ) ;
 }
 selec *= ( 1.0 - stats -> stanullfrac ) ;
 }
 else {
 if ( useOr ) selec = mcelem_array_contain_overlap_selec ( NULL , 0 , NULL , 0 , & constval , 1 , OID_ARRAY_CONTAINS_OP , cmpfunc ) ;
 else selec = mcelem_array_contained_selec ( NULL , 0 , NULL , 0 , & constval , 1 , NULL , 0 , OID_ARRAY_CONTAINED_OP , cmpfunc ) ;
 }
 ReleaseVariableStats ( vardata ) ;
 if ( ! isEquality ) selec = 1.0 - selec ;
 CLAMP_PROBABILITY ( selec ) ;
 return selec ;
 }