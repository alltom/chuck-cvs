// ********************* chuck_type.cpp
t_CKBOOL type_engine_check_func_def_import( Chuck_Env * env, a_Func_Def func_def );
t_CKBOOL type_engine_check_ugen_def_import( Chuck_Env * env, Chuck_UGen_Info * ugen );
t_CKBOOL type_engine_add_dll( Chuck_Env * env, Chuck_DLL * dll, const char * name );
t_CKBOOL type_engine_check_value_import( Chuck_Env * env, const string & name, const string & type, void * addr );




//-----------------------------------------------------------------------------
// name: type_engine_check_func_def_import()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_func_def_import( Chuck_Env * env, a_Func_Def f )
{
    assert( f->code == NULL );

    // builtin
    f->s_type = ae_func_builtin;
    // type check it
    if( !type_engine_check_func_def( env, f ) )
    {
        EM_error2( 0, "...in imported function '%s'", S_name(f->name) );
        return FALSE;
    }

    return TRUE;
}




// ctrl op
UGEN_CTRL ugen_ctrl_op( t_CKTIME now, void * data, void * value ) { }
UGEN_CGET ugen_cget_op( t_CKTIME now, void * data, void * out ) { }
// ctrl gain
UGEN_CTRL ugen_ctrl_gain( t_CKTIME now, void * data, void * value ) { }
UGEN_CGET ugen_cget_gain( t_CKTIME now, void * data, void * out ) { }
// cget last
UGEN_CGET ugen_cget_last( t_CKTIME now, void * data, void * out ) { }
// ctrl next
UGEN_CTRL ugen_ctrl_next( t_CKTIME now, void * data, void * out ) { }


//-----------------------------------------------------------------------------
// name: type_engine_check_ugen_def_import()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_ugen_def_import( Chuck_Env * env, Chuck_UGen_Info * ugen )
{
    map<string, bool> params;
    Chuck_Type * type = NULL;
    Chuck_Type * the_class = NULL, * t_parent = NULL;

    // look up the type
    if( env->curr->lookup_type( ugen->name, FALSE ) )
    {
        EM_error2( ugen->linepos,
            "imported ugen '%s.%s': \n  type identifier '%s' is in use in namespace '%s'",
            env->curr->name.c_str(), ugen->name.c_str(), 
            ugen->name.c_str(), env->curr->name.c_str() );
        return FALSE;
    }

    // check if reserved
    if( type_engine_check_reserved( env, ugen->name, ugen->linepos ) )
    {
        EM_error2( ugen->linepos,
            "...in imported ugen '%s.%s'",
            env->curr->name.c_str(), ugen->name.c_str() );
        return FALSE;
    }
    
    // no return type
    if( !ugen->tick )
    {
        EM_error2( ugen->linepos, 
            "imported ugen '%s.%s': no [tick] method defined",
            env->curr->name.c_str(), ugen->name.c_str() );
        return FALSE;
    }

    // type check parent
    if( ugen->parent != "" )  // trim later
    {
        type = env->curr->lookup_type( ugen->parent, TRUE );
        if( !type )
        {
            EM_error2( ugen->linepos,
                "imported ugen '%s.%s' extends undefined type '%s'",
                env->curr->name.c_str(), ugen->name.c_str(),
                ugen->parent.c_str() );
            return FALSE;
        }

        // make sure we are extending from ugen
        if( !isa( type, &t_ugen ) )
        {
            EM_error2( ugen->linepos,
                "imported ugen '%s.%s' inherits from '%s' -> not a 'ugen'",
                env->curr->name.c_str(), ugen->name.c_str(),
                ugen->parent.c_str() );
            return FALSE;
        }

        t_parent = type;
    }
    else
    {
        t_parent = &t_ugen;
    }
    
    // add default
    ugen->add( ugen_ctrl_op, ugen_cget_op, "int", "op" );
    ugen->add( ugen_ctrl_gain, ugen_cget_gain, "float", "gain" );
    ugen->add( NULL, ugen_cget_last, "float", "last" );
    ugen->add( ugen_ctrl_next, NULL, "float", "next" );

    // loop through ctrl parameters
    for( unsigned int i = 0; i < ugen->param_list.size(); i++ )
    {
        const Chuck_Info_Param * param = &ugen->param_list[i];
        
        // check the type
        type = env->curr->lookup_type( param->type, TRUE );
        if( !type )
        {
            EM_error2( ugen->linepos,
                "imported ugen '%s.%s': unrecognized type '%s' for control parameter '%s'",
                env->curr->name.c_str(), ugen->name.c_str(), 
                param->type.c_str(), param->name.c_str() );
            return FALSE;
        }

        // make sure it's not void
        if( type->size == 0 )
        {
            EM_error2( ugen->linepos,
                "cannot declare variables of size '0' (i.e. 'void')..." );
            return FALSE;
        }

        // check if reserved
        if( type_engine_check_reserved( env, param->name, 0 ) )
        {
            EM_error2( 0, "...in imported ugen '%s.%s'",
                env->curr->name.c_str(), ugen->name.c_str() );
            return FALSE;
        }

        // XXX - pld
        // in general, this just updates the param_map and the
        // older functions are 'forgotten' though still present 
        // in the param list
        if( params[param->name] )
        {
            EM_error2( ugen->linepos, 
                "imported ugen '%s.%s': duplicate control parameter name '%s'",
                env->curr->name.c_str(), ugen->name.c_str(), param->name.c_str() );
            return FALSE;
        }

        // make sure there is a function
        if( !param->ctrl_addr && !param->cget_addr )
        {
            EM_error2( ugen->linepos,
                "imported ugen '%s.%s': no ctrl or cget function defined for param '%s'",
                env->curr->name.c_str(), ugen->name.c_str(), param->name.c_str() );
            return FALSE;
        }
        
        params[param->name] = true;
    }

    // allocate new type
    the_class = env->context->new_Chuck_Type();
    // init as vm object
    // the_class->init();
    // set the fields
    the_class->id = te_user;
    the_class->name = ugen->name;
    the_class->parent = t_parent;
    the_class->size = sizeof(void *);
    the_class->obj_size = 0;  // TODO:
    the_class->self_size = 0;  // TODO:
    the_class->owner = env->curr;
    the_class->array_depth = 0;
    the_class->info = NULL;
    the_class->func = NULL;
    // make reference to the ugen info (for emit)
    the_class->ugen = ugen;

    // add to env
    env->curr->type.add( the_class->name, the_class );
    // add as ugen
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_value_import()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_value_import( Chuck_Env * env, const string & name,
										 const string & type, void * addr )
{
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: do_make_args()
// desc: ...
//-----------------------------------------------------------------------------
a_Arg_List do_make_args( const vector<Chuck_Info_Param> & params, t_CKUINT index )
{
    a_Arg_List args = NULL;
    if( index >= params.size() )
        return NULL;

    if( index == (params.size()-1) )
        args = new_arg_list( new_type_decl( new_id_list((char*)params[index].type.c_str(), 0), 0, 0 ),
                             new_var_decl( (char *)params[index].name.c_str(), NULL, 0 ), 0 );
    else
        args = prepend_arg_list(
            new_type_decl( new_id_list((char*)params[index].type.c_str(), 0), 0, 0 ),
            new_var_decl( (char *)params[index].name.c_str(), NULL, 0 ),
            do_make_args( params, index + 1 ), 0 );
    return args;
}




//-----------------------------------------------------------------------------
// name: type_engine_add_dll()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_add_dll( Chuck_Env * env, Chuck_DLL * dll, const string & name )
{    
    // lookup the namesapce
    Chuck_Namespace * nspc = NULL;
    Chuck_Type * type = NULL;
    
    // go through type - which is what namespaces are in ChucK
    if( type = env->global.lookup_type( name ) )
        nspc = type->info;

    // if already there
    if( env->dlls[dll] )
        return FALSE;

    // if not there
    if( !nspc )
    {
        // allocate new namespace
        nspc = env->context->new_Chuck_Namespace();
        if( !type )
        {
            // add as new type/namespace
            type = env->context->new_Chuck_Type();
            // user defined
            type->id = te_user;
            // the name
            type->name = name;
            // add to global type space
            env->global.type.add( name, type );
        }
        // set the nspc
        type->info = nspc;
        nspc->name = type->name;
        nspc->parent = type->info;
    }

    // add all the prototypes
    Chuck_DL_Query * query = (Chuck_DL_Query *)dll->query();
    for( t_CKUINT i = 0; i < query->dll_exports.size(); i++ )
    {
        // the prototype
        Chuck_DL_Proto * proto = &query->dll_exports[i];

        // add func
        if( proto->is_func )
        {
            // type decl
            a_Type_Decl rtype = new_type_decl( new_id_list((char *)proto->type.c_str(), 0), 0, 0 );
            // arg list
            a_Arg_List args = NULL;
            if( proto->params.size() )
                args = do_make_args( proto->params, 0 );
            // allocate a new function
            a_Func_Def func = new_func_def( 
                ae_key_func, ae_key_public, rtype, (char*)proto->name.c_str(), args, NULL, 0 );
            // set the pointer
            func->builtin = (builtin_func_ptr)proto->addr;
            func->linepos = query->linepos;
            // type check it
            if( !type_engine_check_func_def_import( env, func ) )
            {
                // clean up
                // env->dlls.erase( dll );
                return FALSE;
            }
        }
        else
        {
            // add value
            if( !type_engine_check_value_import( env,
                proto->name, proto->type, (void *)proto->addr ) )
            {
                // info->dlls.erase( dll );
                return FALSE;
            }
        }
    }

    // add the unit generators
    for( t_CKUINT j = 0; j < query->ugen_exports.size(); j++ )
    {
        Chuck_UGen_Info * ugen = new Chuck_UGen_Info( query->ugen_exports[j] );
        
        if( !type_engine_check_ugen_def_import( env, ugen ) )
        {
            // clean up
            // info->dlls.erase( dll );
            return FALSE;
        }
    }

    // flag
    env->dlls[dll] = true;

    return TRUE;
}




// ************* chuck_vm.cpp

//-----------------------------------------------------------------------------
// name: dll_load()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_DLL * Chuck_VM::dll_load( const char * path, const char * id )
{
    Chuck_DLL * dll = m_dlls[path];
    if( !dll )
    {
        dll = new Chuck_DLL(id);
        if( !dll ) return NULL;

        // load the dll
        if( !dll->load( path ) )
        {
            m_last_error = dll->last_error();
            delete dll;
            return NULL;
        }

        m_dlls[path] = dll;
    }

    return dll;
}




//-----------------------------------------------------------------------------
// name: dll_unload()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_VM::dll_unload( Chuck_DLL *& dll )
{
    if( !dll ) return TRUE;

    // deallocate
    dll->unload();
    delete dll;
    dll = NULL;
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: dll_clear()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_VM::dll_clear()
{
    map<string, Chuck_DLL *>::iterator iter;
    for( iter = m_dlls.begin(); iter != m_dlls.end(); iter++ )
    {
        (*iter).second->unload();
        delete (*iter).second;
    }
    
    m_dlls.clear();
}




    Chuck_DLL * dll_load( const char * path, const char * id = NULL );
    t_CKBOOL dll_unload( Chuck_DLL * & dll );
    void dll_clear( );
    map<string, Chuck_DLL *> m_dlls;

// *********** chuck_instr.cpp

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_Ctrl_Op::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    
    pop_( sp, 4 );
    Chuck_UGen * ugen = (Chuck_UGen *)*(sp+1);
    ugen->m_op = *(t_CKINT *)sp;
    // push the new value
    push_( sp, *sp);
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_CGet_Op::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;

    pop_( sp, 2 );
    Chuck_UGen * ugen = (Chuck_UGen *)*(sp);
    // push the new value
    push_( sp, ugen->m_op );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_Ctrl_Gain::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    
    // HACK: this won't work for 64-bit long
    ((Chuck_UGen *)*(sp-3))->m_gain = (float)*(t_CKFLOAT *)(sp-5);
    pop_( sp, 5 );

    // push the new value
    ((t_CKFLOAT *&)shred->reg->sp)++;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_CGet_Gain::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;

    pop_( sp, 2 );
    Chuck_UGen * ugen = (Chuck_UGen *)*(sp);
    // push the new value
    t_CKFLOAT *& sp_double = (t_CKFLOAT *&)shred->reg->sp;
    push_( sp_double, (t_CKFLOAT)ugen->m_gain );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_CGet_Last::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;

    pop_( sp, 2 );
    Chuck_UGen * ugen = (Chuck_UGen *)*(sp);
    // push the new value
    t_CKFLOAT *& sp_double = (t_CKFLOAT *&)shred->reg->sp;
    push_( sp_double, (t_CKFLOAT)ugen->m_current );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_DLL_Load::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    t_CKINT retval = FALSE;
    Chuck_DLL * dll = NULL;
    pop_( sp, 2 );
    
    // load the DLL into the vm
    dll = vm->dll_load( (const char *)(*sp) );
    // load the DLL into the namespace
    if( dll ) retval = type_engine_add_dll( (Chuck_Env *)vm->get_env(), dll,
                                            (const char *)(*(sp+1)) );

    // push the result
    push_( sp, (t_CKUINT)dll );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_DLL_Unload::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    t_CKINT retval = FALSE;
    Chuck_DLL * dll = NULL;
    pop_( sp, 1 );

    // unload the dll
    dll = (Chuck_DLL *)(*sp);
    if( dll ) retval = vm->dll_unload( dll );

    // push the result
    push_( sp, retval );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_Alloc::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    Chuck_UGen_Info * info = NULL;
    Chuck_UGen * ugen = NULL;

    pop_( sp, 1 );
    info = (Chuck_UGen_Info *)*(sp);
    ugen = new Chuck_UGen;
    // copy the info over
    ugen->ctor = info->ctor;
    ugen->dtor = info->dtor;
    ugen->tick = info->tick;
    ugen->pmsg = info->pmsg;
    ugen->m_max_src = info->max_src;
    // call the constructor
    ugen->state = ugen->ctor ? ugen->ctor( shred->now ) : NULL ;

    // setup the reference with the shred
    ugen->shred = shred;
    shred->add( ugen );
    push_( sp, (t_CKUINT)ugen );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_DeAlloc::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
}








//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_Ctrl_Op
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_Ctrl_Op : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_CGet_Op
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_CGet_Op : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_CGet_Gain
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_CGet_Gain : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_Ctrl_Gain
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_Ctrl_Gain : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_CGet_Last
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_CGet_Last : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_DLL_Load
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_DLL_Load : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_DLL_Unload
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_DLL_Unload : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};

//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_Alloc
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_Alloc : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_DeAlloc
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_DeAlloc : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};