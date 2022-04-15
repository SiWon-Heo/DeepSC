static int _setup_resv_limits ( slurmdb_reservation_rec_t * resv , char * * cols , char * * vals , char * * extra ) {
 if ( resv -> assocs ) {
 int start = 0 ;
 int len = strlen ( resv -> assocs ) - 1 ;
 if ( strchr ( resv -> assocs , '-' ) ) {
 int i = 0 , i2 = 0 ;
 char * assocs = xmalloc ( sizeof ( char ) * len ) ;
 while ( i < len ) {
 if ( resv -> assocs [ i ] == ',' && resv -> assocs [ i + 1 ] == '-' ) {
 i += 2 ;
 while ( i < len ) {
 i ++ ;
 if ( resv -> assocs [ i ] == ',' ) break ;
 }
 continue ;
 }
 assocs [ i2 ++ ] = resv -> assocs [ i ++ ] ;
 }
 xfree ( resv -> assocs ) ;
 len = i2 - 1 ;
 resv -> assocs = assocs ;
 assocs = NULL ;
 }
 if ( resv -> assocs [ 0 ] == ',' ) start = 1 ;
 if ( resv -> assocs [ len ] == ',' ) resv -> assocs [ len ] = '\0' ;
 xstrcat ( * cols , ", assoclist" ) ;
 xstrfmtcat ( * vals , ", '%s'" , resv -> assocs + start ) ;
 xstrfmtcat ( * extra , ", assoclist='%s'" , resv -> assocs + start ) ;
 }
 if ( resv -> flags != NO_VAL ) {
 xstrcat ( * cols , ", flags" ) ;
 xstrfmtcat ( * vals , ", %u" , resv -> flags ) ;
 xstrfmtcat ( * extra , ", flags=%u" , resv -> flags ) ;
 }
 if ( resv -> name ) {
 char * tmp_char = slurm_add_slash_to_quotes ( resv -> name ) ;
 xstrcat ( * cols , ", resv_name" ) ;
 xstrfmtcat ( * vals , ", '%s'" , tmp_char ) ;
 xstrfmtcat ( * extra , ", resv_name='%s'" , tmp_char ) ;
 xfree ( tmp_char ) ;
 }
 if ( resv -> nodes ) {
 xstrcat ( * cols , ", nodelist" ) ;
 xstrfmtcat ( * vals , ", '%s'" , resv -> nodes ) ;
 xstrfmtcat ( * extra , ", nodelist='%s'" , resv -> nodes ) ;
 }
 if ( resv -> node_inx ) {
 xstrcat ( * cols , ", node_inx" ) ;
 xstrfmtcat ( * vals , ", '%s'" , resv -> node_inx ) ;
 xstrfmtcat ( * extra , ", node_inx='%s'" , resv -> node_inx ) ;
 }
 if ( resv -> time_end ) {
 xstrcat ( * cols , ", time_end" ) ;
 xstrfmtcat ( * vals , ", %ld" , resv -> time_end ) ;
 xstrfmtcat ( * extra , ", time_end=%ld" , resv -> time_end ) ;
 }
 if ( resv -> time_start ) {
 xstrcat ( * cols , ", time_start" ) ;
 xstrfmtcat ( * vals , ", %ld" , resv -> time_start ) ;
 xstrfmtcat ( * extra , ", time_start=%ld" , resv -> time_start ) ;
 }
 if ( resv -> tres_str ) {
 xstrcat ( * cols , ", tres" ) ;
 xstrfmtcat ( * vals , ", '%s'" , resv -> tres_str ) ;
 xstrfmtcat ( * extra , ", tres='%s'" , resv -> tres_str ) ;
 }
 return SLURM_SUCCESS ;
 }