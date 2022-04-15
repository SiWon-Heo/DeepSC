int zrestore ( i_ctx_t * i_ctx_p ) {
 os_ptr op = osp ;
 alloc_save_t * asave ;
 bool last ;
 vm_save_t * vmsave ;
 int code = restore_check_operand ( op , & asave , idmemory ) ;
 if ( code < 0 ) return code ;
 if_debug2m ( 'u' , imemory , "[u]vmrestore 0x%lx, id = %lu\n" , ( ulong ) alloc_save_client_data ( asave ) , ( ulong ) op -> value . saveid ) ;
 if ( I_VALIDATE_BEFORE_RESTORE ) ivalidate_clean_spaces ( i_ctx_p ) ;
 osp -- ;
 {
 int code ;
 if ( ( code = restore_check_stack ( i_ctx_p , & o_stack , asave , false ) ) < 0 || ( code = restore_check_stack ( i_ctx_p , & e_stack , asave , true ) ) < 0 || ( code = restore_check_stack ( i_ctx_p , & d_stack , asave , false ) ) < 0 ) {
 osp ++ ;
 return code ;
 }
 }
 restore_fix_stack ( i_ctx_p , & o_stack , asave , false ) ;
 restore_fix_stack ( i_ctx_p , & e_stack , asave , true ) ;
 restore_fix_stack ( i_ctx_p , & d_stack , asave , false ) ;
 do {
 vmsave = alloc_save_client_data ( alloc_save_current ( idmemory ) ) ;
 gs_grestoreall_for_restore ( igs , vmsave -> gsave ) ;
 vmsave -> gsave = 0 ;
 code = alloc_restore_step_in ( idmemory , asave ) ;
 if ( code < 0 ) return code ;
 last = code ;
 }
 while ( ! last ) ;
 {
 uint space = icurrent_space ;
 ialloc_set_space ( idmemory , avm_local ) ;
 ifree_object ( vmsave , "zrestore" ) ;
 ialloc_set_space ( idmemory , space ) ;
 }
 dict_set_top ( ) ;
 if ( I_VALIDATE_AFTER_RESTORE ) ivalidate_clean_spaces ( i_ctx_p ) ;
 i_ctx_p -> LockFilePermissions = false ;
 return 0 ;
 }