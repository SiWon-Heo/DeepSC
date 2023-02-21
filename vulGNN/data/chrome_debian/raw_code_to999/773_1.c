static double eqjoinsel_semi ( Oid operator , VariableStatData * vardata1 , VariableStatData * vardata2 , RelOptInfo * inner_rel ) {
 double selec ;
 double nd1 ;
 double nd2 ;
 bool isdefault1 ;
 bool isdefault2 ;
 Form_pg_statistic stats1 = NULL ;
 bool have_mcvs1 = false ;
 Datum * values1 = NULL ;
 int nvalues1 = 0 ;
 float4 * numbers1 = NULL ;
 int nnumbers1 = 0 ;
 bool have_mcvs2 = false ;
 Datum * values2 = NULL ;
 int nvalues2 = 0 ;
 float4 * numbers2 = NULL ;
 int nnumbers2 = 0 ;
 nd1 = get_variable_numdistinct ( vardata1 , & isdefault1 ) ;
 nd2 = get_variable_numdistinct ( vardata2 , & isdefault2 ) ;
 if ( vardata2 -> rel ) nd2 = Min ( nd2 , vardata2 -> rel -> rows ) ;
 nd2 = Min ( nd2 , inner_rel -> rows ) ;
 if ( HeapTupleIsValid ( vardata1 -> statsTuple ) ) {
 stats1 = ( Form_pg_statistic ) GETSTRUCT ( vardata1 -> statsTuple ) ;
 have_mcvs1 = get_attstatsslot ( vardata1 -> statsTuple , vardata1 -> atttype , vardata1 -> atttypmod , STATISTIC_KIND_MCV , InvalidOid , NULL , & values1 , & nvalues1 , & numbers1 , & nnumbers1 ) ;
 }
 if ( HeapTupleIsValid ( vardata2 -> statsTuple ) ) {
 have_mcvs2 = get_attstatsslot ( vardata2 -> statsTuple , vardata2 -> atttype , vardata2 -> atttypmod , STATISTIC_KIND_MCV , InvalidOid , NULL , & values2 , & nvalues2 , & numbers2 , & nnumbers2 ) ;
 }
 if ( have_mcvs1 && have_mcvs2 && OidIsValid ( operator ) ) {
 FmgrInfo eqproc ;
 bool * hasmatch1 ;
 bool * hasmatch2 ;
 double nullfrac1 = stats1 -> stanullfrac ;
 double matchfreq1 , uncertainfrac , uncertain ;
 int i , nmatches , clamped_nvalues2 ;
 clamped_nvalues2 = Min ( nvalues2 , nd2 ) ;
 fmgr_info ( get_opcode ( operator ) , & eqproc ) ;
 hasmatch1 = ( bool * ) palloc0 ( nvalues1 * sizeof ( bool ) ) ;
 hasmatch2 = ( bool * ) palloc0 ( clamped_nvalues2 * sizeof ( bool ) ) ;
 nmatches = 0 ;
 for ( i = 0 ;
 i < nvalues1 ;
 i ++ ) {
 int j ;
 for ( j = 0 ;
 j < clamped_nvalues2 ;
 j ++ ) {
 if ( hasmatch2 [ j ] ) continue ;
 if ( DatumGetBool ( FunctionCall2Coll ( & eqproc , DEFAULT_COLLATION_OID , values1 [ i ] , values2 [ j ] ) ) ) {
 hasmatch1 [ i ] = hasmatch2 [ j ] = true ;
 nmatches ++ ;
 break ;
 }
 }
 }
 matchfreq1 = 0.0 ;
 for ( i = 0 ;
 i < nvalues1 ;
 i ++ ) {
 if ( hasmatch1 [ i ] ) matchfreq1 += numbers1 [ i ] ;
 }
 CLAMP_PROBABILITY ( matchfreq1 ) ;
 pfree ( hasmatch1 ) ;
 pfree ( hasmatch2 ) ;
 if ( ! isdefault1 && ! isdefault2 ) {
 nd1 -= nmatches ;
 nd2 -= nmatches ;
 if ( nd1 <= nd2 || nd2 < 0 ) uncertainfrac = 1.0 ;
 else uncertainfrac = nd2 / nd1 ;
 }
 else uncertainfrac = 0.5 ;
 uncertain = 1.0 - matchfreq1 - nullfrac1 ;
 CLAMP_PROBABILITY ( uncertain ) ;
 selec = matchfreq1 + uncertainfrac * uncertain ;
 }
 else {
 double nullfrac1 = stats1 ? stats1 -> stanullfrac : 0.0 ;
 if ( ! isdefault1 && ! isdefault2 ) {
 if ( nd1 <= nd2 || nd2 < 0 ) selec = 1.0 - nullfrac1 ;
 else selec = ( nd2 / nd1 ) * ( 1.0 - nullfrac1 ) ;
 }
 else selec = 0.5 * ( 1.0 - nullfrac1 ) ;
 }
 if ( have_mcvs1 ) free_attstatsslot ( vardata1 -> atttype , values1 , nvalues1 , numbers1 , nnumbers1 ) ;
 if ( have_mcvs2 ) free_attstatsslot ( vardata2 -> atttype , values2 , nvalues2 , numbers2 , nnumbers2 ) ;
 return selec ;
 }