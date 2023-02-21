void examine_variable ( PlannerInfo * root , Node * node , int varRelid , VariableStatData * vardata ) {
 Node * basenode ;
 Relids varnos ;
 RelOptInfo * onerel ;
 MemSet ( vardata , 0 , sizeof ( VariableStatData ) ) ;
 vardata -> vartype = exprType ( node ) ;
 if ( IsA ( node , RelabelType ) ) basenode = ( Node * ) ( ( RelabelType * ) node ) -> arg ;
 else basenode = node ;
 if ( IsA ( basenode , Var ) && ( varRelid == 0 || varRelid == ( ( Var * ) basenode ) -> varno ) ) {
 Var * var = ( Var * ) basenode ;
 vardata -> var = basenode ;
 vardata -> rel = find_base_rel ( root , var -> varno ) ;
 vardata -> atttype = var -> vartype ;
 vardata -> atttypmod = var -> vartypmod ;
 vardata -> isunique = has_unique_index ( vardata -> rel , var -> varattno ) ;
 examine_simple_variable ( root , var , vardata ) ;
 return ;
 }
 varnos = pull_varnos ( basenode ) ;
 onerel = NULL ;
 switch ( bms_membership ( varnos ) ) {
 case BMS_EMPTY_SET : break ;
 case BMS_SINGLETON : if ( varRelid == 0 || bms_is_member ( varRelid , varnos ) ) {
 onerel = find_base_rel ( root , ( varRelid ? varRelid : bms_singleton_member ( varnos ) ) ) ;
 vardata -> rel = onerel ;
 node = basenode ;
 }
 break ;
 case BMS_MULTIPLE : if ( varRelid == 0 ) {
 vardata -> rel = find_join_rel ( root , varnos ) ;
 node = basenode ;
 }
 else if ( bms_is_member ( varRelid , varnos ) ) {
 vardata -> rel = find_base_rel ( root , varRelid ) ;
 node = basenode ;
 }
 break ;
 }
 bms_free ( varnos ) ;
 vardata -> var = node ;
 vardata -> atttype = exprType ( node ) ;
 vardata -> atttypmod = exprTypmod ( node ) ;
 if ( onerel ) {
 ListCell * ilist ;
 foreach ( ilist , onerel -> indexlist ) {
 IndexOptInfo * index = ( IndexOptInfo * ) lfirst ( ilist ) ;
 ListCell * indexpr_item ;
 int pos ;
 indexpr_item = list_head ( index -> indexprs ) ;
 if ( indexpr_item == NULL ) continue ;
 for ( pos = 0 ;
 pos < index -> ncolumns ;
 pos ++ ) {
 if ( index -> indexkeys [ pos ] == 0 ) {
 Node * indexkey ;
 if ( indexpr_item == NULL ) elog ( ERROR , "too few entries in indexprs list" ) ;
 indexkey = ( Node * ) lfirst ( indexpr_item ) ;
 if ( indexkey && IsA ( indexkey , RelabelType ) ) indexkey = ( Node * ) ( ( RelabelType * ) indexkey ) -> arg ;
 if ( equal ( node , indexkey ) ) {
 if ( index -> unique && index -> ncolumns == 1 && ( index -> indpred == NIL || index -> predOK ) ) vardata -> isunique = true ;
 if ( get_index_stats_hook && ( * get_index_stats_hook ) ( root , index -> indexoid , pos + 1 , vardata ) ) {
 if ( HeapTupleIsValid ( vardata -> statsTuple ) && ! vardata -> freefunc ) elog ( ERROR , "no function provided to release variable stats with" ) ;
 }
 else if ( index -> indpred == NIL ) {
 vardata -> statsTuple = SearchSysCache3 ( STATRELATTINH , ObjectIdGetDatum ( index -> indexoid ) , Int16GetDatum ( pos + 1 ) , BoolGetDatum ( false ) ) ;
 vardata -> freefunc = ReleaseSysCache ;
 }
 if ( vardata -> statsTuple ) break ;
 }
 indexpr_item = lnext ( indexpr_item ) ;
 }
 }
 if ( vardata -> statsTuple ) break ;
 }
 }
 }