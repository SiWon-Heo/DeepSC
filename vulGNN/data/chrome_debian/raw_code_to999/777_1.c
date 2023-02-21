static void examine_simple_variable ( PlannerInfo * root , Var * var , VariableStatData * vardata ) {
 RangeTblEntry * rte = root -> simple_rte_array [ var -> varno ] ;
 Assert ( IsA ( rte , RangeTblEntry ) ) ;
 if ( get_relation_stats_hook && ( * get_relation_stats_hook ) ( root , rte , var -> varattno , vardata ) ) {
 if ( HeapTupleIsValid ( vardata -> statsTuple ) && ! vardata -> freefunc ) elog ( ERROR , "no function provided to release variable stats with" ) ;
 }
 else if ( rte -> rtekind == RTE_RELATION ) {
 vardata -> statsTuple = SearchSysCache3 ( STATRELATTINH , ObjectIdGetDatum ( rte -> relid ) , Int16GetDatum ( var -> varattno ) , BoolGetDatum ( rte -> inh ) ) ;
 vardata -> freefunc = ReleaseSysCache ;
 }
 else if ( rte -> rtekind == RTE_SUBQUERY && ! rte -> inh ) {
 Query * subquery = rte -> subquery ;
 RelOptInfo * rel ;
 TargetEntry * ste ;
 if ( var -> varattno == InvalidAttrNumber ) return ;
 if ( subquery -> setOperations || subquery -> groupClause ) return ;
 rel = find_base_rel ( root , var -> varno ) ;
 if ( rel -> subroot == NULL ) return ;
 Assert ( IsA ( rel -> subroot , PlannerInfo ) ) ;
 subquery = rel -> subroot -> parse ;
 Assert ( IsA ( subquery , Query ) ) ;
 ste = get_tle_by_resno ( subquery -> targetList , var -> varattno ) ;
 if ( ste == NULL || ste -> resjunk ) elog ( ERROR , "subquery %s does not have attribute %d" , rte -> eref -> aliasname , var -> varattno ) ;
 var = ( Var * ) ste -> expr ;
 if ( subquery -> distinctClause ) {
 if ( list_length ( subquery -> distinctClause ) == 1 && targetIsInSortList ( ste , InvalidOid , subquery -> distinctClause ) ) vardata -> isunique = true ;
 return ;
 }
 if ( rte -> security_barrier ) return ;
 if ( var && IsA ( var , Var ) && var -> varlevelsup == 0 ) {
 examine_simple_variable ( rel -> subroot , var , vardata ) ;
 }
 }
 else {
 }
 }