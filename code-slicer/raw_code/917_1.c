static PGconn * get_db_conn ( ClusterInfo * cluster , const char * db_name ) {
 char conn_opts [ 2 * NAMEDATALEN + MAXPGPATH + 100 ] ;
 if ( cluster -> sockdir ) snprintf ( conn_opts , sizeof ( conn_opts ) , "dbname = '%s' user = '%s' host = '%s' port = %d" , db_name , os_info . user , cluster -> sockdir , cluster -> port ) ;
 else snprintf ( conn_opts , sizeof ( conn_opts ) , "dbname = '%s' user = '%s' port = %d" , db_name , os_info . user , cluster -> port ) ;
 return PQconnectdb ( conn_opts ) ;
 }