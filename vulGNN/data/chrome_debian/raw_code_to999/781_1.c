static double eqjoinsel_inner ( Oid operator , VariableStatData * vardata1 , VariableStatData * vardata2 ) {
 double selec ;
 double nd1 ;
 double nd2 ;
 bool isdefault1 ;
 bool isdefault2 ;
 Form_pg_statistic stats1 = NULL ;
 Form_pg_statistic stats2 = NULL ;
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
 if ( HeapTupleIsValid ( vardata1 -> statsTuple ) ) {
 stats1 = ( Form_pg_statistic ) GETSTRUCT ( vardata1 -> statsTuple ) ;
 have_mcvs1 = get_attstatsslot ( vardata1 -> statsTuple , vardata1 -> atttype , vardata1 -> atttypmod , STATISTIC_KIND_MCV , InvalidOid , NULL , & values1 , & nvalues1 , & numbers1 , & nnumbers1 ) ;
 }
 if ( HeapTupleIsValid ( vardata2 -> statsTuple ) ) {
 stats2 = ( Form_pg_statistic ) GETSTRUCT ( vardata2 -> statsTuple ) ;
 have_mcvs2 = get_attstatsslot ( vardata2 -> statsTuple , vardata2 -> atttype , vardata2 -> atttypmod , STATISTIC_KIND_MCV , InvalidOid , NULL , & values2 , & nvalues2 , & numbers2 , & nnumbers2 ) ;
 }
 if ( have_mcvs1 && have_mcvs2 ) {
 FmgrInfo eqproc ;
 bool * hasmatch1 ;
 bool * hasmatch2 ;
 double nullfrac1 = stats1 -> stanullfrac ;
 double nullfrac2 = stats2 -> stanullfrac ;
 double matchprodfreq , matchfreq1 , matchfreq2 , unmatchfreq1 , unmatchfreq2 , otherfreq1 , otherfreq2 , totalsel1 , totalsel2 ;
 int i , nmatches ;
 fmgr_info ( get_opcode ( operator ) , & eqproc ) ;
 hasmatch1 = ( bool * ) palloc0 ( nvalues1 * sizeof ( bool ) ) ;
 hasmatch2 = ( bool * ) palloc0 ( nvalues2 * sizeof ( bool ) ) ;
 matchprodfreq = 0.0 ;
 nmatches = 0 ;
 for ( i = 0 ;
 i < nvalues1 ;
 i ++ ) {
 int j ;
 for ( j = 0 ;
 j < nvalues2 ;
 j ++ ) {
 if ( hasmatch2 [ j ] ) continue ;
 if ( DatumGetBool ( FunctionCall2Coll ( & eqproc , DEFAULT_COLLATION_OID , values1 [ i ] , values2 [ j ] ) ) ) {
 hasmatch1 [ i ] = hasmatch2 [ j ] = true ;
 matchprodfreq += numbers1 [ i ] * numbers2 [ j ] ;
 nmatches ++ ;
 break ;
 }
 }
 }
 CLAMP_PROBABILITY ( matchprodfreq ) ;
 matchfreq1 = unmatchfreq1 = 0.0 ;
 for ( i = 0 ;
 i < nvalues1 ;
 i ++ ) {
 if ( hasmatch1 [ i ] ) matchfreq1 += numbers1 [ i ] ;
 else unmatchfreq1 += numbers1 [ i ] ;
 }
 CLAMP_PROBABILITY ( matchfreq1 ) ;
 CLAMP_PROBABILITY ( unmatchfreq1 ) ;
 matchfreq2 = unmatchfreq2 = 0.0 ;
 for ( i = 0 ;
 i < nvalues2 ;
 i ++ ) {
 if ( hasmatch2 [ i ] ) matchfreq2 += numbers2 [ i ] ;
 else unmatchfreq2 += numbers2 [ i ] ;
 }
 CLAMP_PROBABILITY ( matchfreq2 ) ;
 CLAMP_PROBABILITY ( unmatchfreq2 ) ;
 pfree ( hasmatch1 ) ;
 pfree ( hasmatch2 ) ;
 otherfreq1 = 1.0 - nullfrac1 - matchfreq1 - unmatchfreq1 ;
 otherfreq2 = 1.0 - nullfrac2 - matchfreq2 - unmatchfreq2 ;
 CLAMP_PROBABILITY ( otherfreq1 ) ;
 CLAMP_PROBABILITY ( otherfreq2 ) ;
 totalsel1 = matchprodfreq ;
 if ( nd2 > nvalues2 ) totalsel1 += unmatchfreq1 * otherfreq2 / ( nd2 - nvalues2 ) ;
 if ( nd2 > nmatches ) totalsel1 += otherfreq1 * ( otherfreq2 + unmatchfreq2 ) / ( nd2 - nmatches ) ;
 totalsel2 = matchprodfreq ;
 if ( nd1 > nvalues1 ) totalsel2 += unmatchfreq2 * otherfreq1 / ( nd1 - nvalues1 ) ;
 if ( nd1 > nmatches ) totalsel2 += otherfreq2 * ( otherfreq1 + unmatchfreq1 ) / ( nd1 - nmatches ) ;
 selec = ( totalsel1 < totalsel2 ) ? totalsel1 : totalsel2 ;
 }
 else {
 double nullfrac1 = stats1 ? stats1 -> stanullfrac : 0.0 ;
 double nullfrac2 = stats2 ? stats2 -> stanullfrac : 0.0 ;
 selec = ( 1.0 - nullfrac1 ) * ( 1.0 - nullfrac2 ) ;
 if ( nd1 > nd2 ) selec /= nd1 ;
 else selec /= nd2 ;
 }
 if ( have_mcvs1 ) free_attstatsslot ( vardata1 -> atttype , values1 , nvalues1 , numbers1 , nnumbers1 ) ;
 if ( have_mcvs2 ) free_attstatsslot ( vardata2 -> atttype , values2 , nvalues2 , numbers2 , nnumbers2 ) ;
 return selec ;
 }