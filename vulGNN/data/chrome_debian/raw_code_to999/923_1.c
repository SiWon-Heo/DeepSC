static void vacuum_all_databases ( vacuumingOptions * vacopts , bool analyze_in_stages , const char * maintenance_db , const char * host , const char * port , const char * username , enum trivalue prompt_password , int concurrentCons , const char * progname , bool echo , bool quiet ) {
 PGconn * conn ;
 PGresult * result ;
 int stage ;
 int i ;
 conn = connectMaintenanceDatabase ( maintenance_db , host , port , username , prompt_password , progname ) ;
 result = executeQuery ( conn , "SELECT datname FROM pg_database WHERE datallowconn ORDER BY 1;
" , progname , echo ) ;
 PQfinish ( conn ) ;
 if ( analyze_in_stages ) {
 for ( stage = 0 ;
 stage < ANALYZE_NUM_STAGES ;
 stage ++ ) {
 for ( i = 0 ;
 i < PQntuples ( result ) ;
 i ++ ) {
 const char * dbname ;
 dbname = PQgetvalue ( result , i , 0 ) ;
 vacuum_one_database ( dbname , vacopts , stage , NULL , host , port , username , prompt_password , concurrentCons , progname , echo , quiet ) ;
 }
 }
 }
 else {
 for ( i = 0 ;
 i < PQntuples ( result ) ;
 i ++ ) {
 const char * dbname ;
 dbname = PQgetvalue ( result , i , 0 ) ;
 vacuum_one_database ( dbname , vacopts , ANALYZE_NO_STAGE , NULL , host , port , username , prompt_password , concurrentCons , progname , echo , quiet ) ;
 }
 }
 PQclear ( result ) ;
 }