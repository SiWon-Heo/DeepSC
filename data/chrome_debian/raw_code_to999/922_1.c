static PGconn * connectDatabase ( const char * dbname , const char * connection_string , const char * pghost , const char * pgport , const char * pguser , trivalue prompt_password , bool fail_on_error ) {
 PGconn * conn ;
 bool new_pass ;
 const char * remoteversion_str ;
 int my_version ;
 static char * password = NULL ;
 const char * * keywords = NULL ;
 const char * * values = NULL ;
 PQconninfoOption * conn_opts = NULL ;
 if ( prompt_password == TRI_YES && ! password ) password = simple_prompt ( "Password: " , 100 , false ) ;
 do {
 int argcount = 6 ;
 PQconninfoOption * conn_opt ;
 char * err_msg = NULL ;
 int i = 0 ;
 if ( keywords ) free ( keywords ) ;
 if ( values ) free ( values ) ;
 if ( conn_opts ) PQconninfoFree ( conn_opts ) ;
 if ( connection_string ) {
 conn_opts = PQconninfoParse ( connection_string , & err_msg ) ;
 if ( conn_opts == NULL ) {
 fprintf ( stderr , "%s: %s" , progname , err_msg ) ;
 exit_nicely ( 1 ) ;
 }
 for ( conn_opt = conn_opts ;
 conn_opt -> keyword != NULL ;
 conn_opt ++ ) {
 if ( conn_opt -> val != NULL && conn_opt -> val [ 0 ] != '\0' ) argcount ++ ;
 }
 keywords = pg_malloc0 ( ( argcount + 1 ) * sizeof ( * keywords ) ) ;
 values = pg_malloc0 ( ( argcount + 1 ) * sizeof ( * values ) ) ;
 for ( conn_opt = conn_opts ;
 conn_opt -> keyword != NULL ;
 conn_opt ++ ) {
 if ( conn_opt -> val != NULL && conn_opt -> val [ 0 ] != '\0' ) {
 keywords [ i ] = conn_opt -> keyword ;
 values [ i ] = conn_opt -> val ;
 i ++ ;
 }
 }
 }
 else {
 keywords = pg_malloc0 ( ( argcount + 1 ) * sizeof ( * keywords ) ) ;
 values = pg_malloc0 ( ( argcount + 1 ) * sizeof ( * values ) ) ;
 }
 if ( pghost ) {
 keywords [ i ] = "host" ;
 values [ i ] = pghost ;
 i ++ ;
 }
 if ( pgport ) {
 keywords [ i ] = "port" ;
 values [ i ] = pgport ;
 i ++ ;
 }
 if ( pguser ) {
 keywords [ i ] = "user" ;
 values [ i ] = pguser ;
 i ++ ;
 }
 if ( password ) {
 keywords [ i ] = "password" ;
 values [ i ] = password ;
 i ++ ;
 }
 if ( dbname ) {
 keywords [ i ] = "dbname" ;
 values [ i ] = dbname ;
 i ++ ;
 }
 keywords [ i ] = "fallback_application_name" ;
 values [ i ] = progname ;
 i ++ ;
 new_pass = false ;
 conn = PQconnectdbParams ( keywords , values , true ) ;
 if ( ! conn ) {
 fprintf ( stderr , _ ( "%s: could not connect to database \"%s\"\n" ) , progname , dbname ) ;
 exit_nicely ( 1 ) ;
 }
 if ( PQstatus ( conn ) == CONNECTION_BAD && PQconnectionNeedsPassword ( conn ) && password == NULL && prompt_password != TRI_NO ) {
 PQfinish ( conn ) ;
 password = simple_prompt ( "Password: " , 100 , false ) ;
 new_pass = true ;
 }
 }
 while ( new_pass ) ;
 if ( PQstatus ( conn ) == CONNECTION_BAD ) {
 if ( fail_on_error ) {
 fprintf ( stderr , _ ( "%s: could not connect to database \"%s\": %s\n" ) , progname , dbname , PQerrorMessage ( conn ) ) ;
 exit_nicely ( 1 ) ;
 }
 else {
 PQfinish ( conn ) ;
 free ( keywords ) ;
 free ( values ) ;
 PQconninfoFree ( conn_opts ) ;
 return NULL ;
 }
 }
 connstr = constructConnStr ( keywords , values ) ;
 free ( keywords ) ;
 free ( values ) ;
 PQconninfoFree ( conn_opts ) ;
 remoteversion_str = PQparameterStatus ( conn , "server_version" ) ;
 if ( ! remoteversion_str ) {
 fprintf ( stderr , _ ( "%s: could not get server version\n" ) , progname ) ;
 exit_nicely ( 1 ) ;
 }
 server_version = PQserverVersion ( conn ) ;
 if ( server_version == 0 ) {
 fprintf ( stderr , _ ( "%s: could not parse server version \"%s\"\n" ) , progname , remoteversion_str ) ;
 exit_nicely ( 1 ) ;
 }
 my_version = PG_VERSION_NUM ;
 if ( my_version != server_version && ( server_version < 70000 || ( server_version / 100 ) > ( my_version / 100 ) ) ) {
 fprintf ( stderr , _ ( "server version: %s;
 %s version: %s\n" ) , remoteversion_str , progname , PG_VERSION ) ;
 fprintf ( stderr , _ ( "aborting because of server version mismatch\n" ) ) ;
 exit_nicely ( 1 ) ;
 }
 if ( server_version >= 70300 ) executeCommand ( conn , "SET search_path = pg_catalog" ) ;
 return conn ;
 }