static void refresh_connections ( void ) {
 if ( ! ( wdg_connections -> flags & WDG_OBJ_FOCUSED ) ) return ;
 wdg_dynlist_refresh ( wdg_connections ) ;
 }