static PGconn * _connectDB ( ArchiveHandle * AH , const char * reqdb , const char * requser ) {
 PGconn * newConn ;
 const char * newdb ;
 const char * newuser ;
 char * password ;
 bool new_pass ;
 if ( ! reqdb ) newdb = PQdb ( AH -> connection ) ;
 else newdb = reqdb ;
 if ( ! requser || strlen ( requser ) == 0 ) newuser = PQuser ( AH -> connection ) ;
 else newuser = requser ;
 ahlog ( AH , 1 , "connecting to database \"%s\" as user \"%s\"\n" , newdb , newuser ) ;
 password = AH -> savedPassword ? pg_strdup ( AH -> savedPassword ) : NULL ;
 if ( AH -> promptPassword == TRI_YES && password == NULL ) {
 password = simple_prompt ( "Password: " , 100 , false ) ;
 if ( password == NULL ) exit_horribly ( modulename , "out of memory\n" ) ;
 }
 do {
 const char * keywords [ 7 ] ;
 const char * values [ 7 ] ;
 keywords [ 0 ] = "host" ;
 values [ 0 ] = PQhost ( AH -> connection ) ;
 keywords [ 1 ] = "port" ;
 values [ 1 ] = PQport ( AH -> connection ) ;
 keywords [ 2 ] = "user" ;
 values [ 2 ] = newuser ;
 keywords [ 3 ] = "password" ;
 values [ 3 ] = password ;
 keywords [ 4 ] = "dbname" ;
 values [ 4 ] = newdb ;
 keywords [ 5 ] = "fallback_application_name" ;
 values [ 5 ] = progname ;
 keywords [ 6 ] = NULL ;
 values [ 6 ] = NULL ;
 new_pass = false ;
 newConn = PQconnectdbParams ( keywords , values , true ) ;
 if ( ! newConn ) exit_horribly ( modulename , "failed to reconnect to database\n" ) ;
 if ( PQstatus ( newConn ) == CONNECTION_BAD ) {
 if ( ! PQconnectionNeedsPassword ( newConn ) ) exit_horribly ( modulename , "could not reconnect to database: %s" , PQerrorMessage ( newConn ) ) ;
 PQfinish ( newConn ) ;
 if ( password ) fprintf ( stderr , "Password incorrect\n" ) ;
 fprintf ( stderr , "Connecting to %s as %s\n" , newdb , newuser ) ;
 if ( password ) free ( password ) ;
 if ( AH -> promptPassword != TRI_NO ) password = simple_prompt ( "Password: " , 100 , false ) ;
 else exit_horribly ( modulename , "connection needs password\n" ) ;
 if ( password == NULL ) exit_horribly ( modulename , "out of memory\n" ) ;
 new_pass = true ;
 }
 }
 while ( new_pass ) ;
 if ( PQconnectionUsedPassword ( newConn ) ) {
 if ( AH -> savedPassword ) free ( AH -> savedPassword ) ;
 AH -> savedPassword = pg_strdup ( PQpass ( newConn ) ) ;
 }
 if ( password ) free ( password ) ;
 _check_database_version ( AH ) ;
 PQsetNoticeProcessor ( newConn , notice_processor , NULL ) ;
 return newConn ;
 }