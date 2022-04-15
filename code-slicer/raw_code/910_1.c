static void reindex_system_catalogs ( const char * dbname , const char * host , const char * port , const char * username , enum trivalue prompt_password , const char * progname , bool echo , bool verbose ) {
 PGconn * conn ;
 PQExpBufferData sql ;
 conn = connectDatabase ( dbname , host , port , username , prompt_password , progname , false , false ) ;
 initPQExpBuffer ( & sql ) ;
 appendPQExpBuffer ( & sql , "REINDEX" ) ;
 if ( verbose ) appendPQExpBuffer ( & sql , " (VERBOSE)" ) ;
 appendPQExpBuffer ( & sql , " SYSTEM %s;
" , PQdb ( conn ) ) ;
 if ( ! executeMaintenanceCommand ( conn , sql . data , echo ) ) {
 fprintf ( stderr , _ ( "%s: reindexing of system catalogs failed: %s" ) , progname , PQerrorMessage ( conn ) ) ;
 PQfinish ( conn ) ;
 exit ( 1 ) ;
 }
 PQfinish ( conn ) ;
 termPQExpBuffer ( & sql ) ;
 }