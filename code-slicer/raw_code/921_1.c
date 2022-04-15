static void dumpDatabases ( PGconn * conn ) {
 PGresult * res ;
 int i ;
 if ( server_version >= 70100 ) res = executeQuery ( conn , "SELECT datname FROM pg_database WHERE datallowconn ORDER BY 1" ) ;
 else res = executeQuery ( conn , "SELECT datname FROM pg_database ORDER BY 1" ) ;
 for ( i = 0 ;
 i < PQntuples ( res ) ;
 i ++ ) {
 int ret ;
 char * dbname = PQgetvalue ( res , i , 0 ) ;
 if ( verbose ) fprintf ( stderr , _ ( "%s: dumping database \"%s\"...\n" ) , progname , dbname ) ;
 fprintf ( OPF , "\\connect %s\n\n" , fmtId ( dbname ) ) ;
 fprintf ( OPF , "SET default_transaction_read_only = off;
\n\n" ) ;
 if ( filename ) fclose ( OPF ) ;
 ret = runPgDump ( dbname ) ;
 if ( ret != 0 ) {
 fprintf ( stderr , _ ( "%s: pg_dump failed on database \"%s\", exiting\n" ) , progname , dbname ) ;
 exit_nicely ( 1 ) ;
 }
 if ( filename ) {
 OPF = fopen ( filename , PG_BINARY_A ) ;
 if ( ! OPF ) {
 fprintf ( stderr , _ ( "%s: could not re-open the output file \"%s\": %s\n" ) , progname , filename , strerror ( errno ) ) ;
 exit_nicely ( 1 ) ;
 }
 }
 }
 PQclear ( res ) ;
 }