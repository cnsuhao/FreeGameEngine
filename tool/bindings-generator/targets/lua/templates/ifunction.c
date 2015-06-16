## ===== instance function implementation template
int ${signature_name}(lua_State* tolua_S)
{
\#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
\#endif
    #set class_script_name = $generator.scriptname_from_native($namespaced_class_name, $namespace_name)
    const char * lua_cls_name = "$class_script_name";
    const char * lua_fun_name = "${class_script_name}:${func_name}";
    ${namespaced_class_name}* self = nullptr;
    int argc = 0;
#if $min_args > 0
    bool ok  = true;
#end if

#if not $is_constructor
\#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
\#endif
    self = (${namespaced_class_name}*)tolua_tousertype(tolua_S,1,0);
\#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function '${class_script_name}'", nullptr);
        return 0;
    }
\#endif
#end if

    argc = lua_gettop(tolua_S)-1;
#if len($arguments) >= $min_args
    #set arg_count = len($arguments)
    #set arg_idx = $min_args
    #while $arg_idx <= $arg_count
    if (argc == ${arg_idx}) 
    {
        #set arg_array = []
        #set $count = 0
        #while $count < $arg_idx
            #set $arg = $arguments[$count]
            #set $arg_array += ["arg" + str(count)]
        ${arg.to_string($generator)} arg${count};
            #set $count = $count + 1
        #end while
        #set $count = 0
        #set format_list = ""
        #if $arg_idx > 0
            #while $count < $arg_idx
                #set $arg = $arguments[$count]
        $arg.to_native({
                    "generator" :       $generator,
                    "in_value" :        "argv[" + str(count) + "]",
                    "out_value" :       "arg" + str(count),
                    "arg_idx" :         $count+2,
                    "class_name" :      $class_name,
                    "lua_namespaced_class_name" :     $class_script_name,
                    "func_name" :       $func_name,
                    "level" :           2,
                    "arg" :             $arg,
                    "ntype" :           $arg.namespaced_name.replace("*", ""),
                    "scriptname" :      $generator.scriptname_from_native($arg.namespaced_name, $arg.namespace_name)
                });
                #set $count = $count + 1
            #end while
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function '${class_script_name}'", nullptr);
            return 0;
        }
        #end if
        #set $arg_list = ", ".join($arg_array)
        #if $is_constructor
        self = new ${namespaced_class_name}($arg_list);
            #if not $generator.script_control_cpp
                #if $is_ref_class
        object_to_luaval(tolua_S, self, lua_cls_name);
                #else
        tolua_pushusertype(tolua_S, (void*)self, lua_cls_name);
        tolua_register_gc(tolua_S, lua_gettop(tolua_S));
                #end if
            #else
        tolua_pushusertype(tolua_S, (void*)self, lua_cls_name);
        tolua_register_gc(tolua_S, lua_gettop(tolua_S));
            #end if
        return 1;
        #else
            #if $ret_type.name != "void"
                #if $ret_type.is_enum
        int ret = (int)self->${func_name}($arg_list);
                #else
        ${ret_type.get_whole_name($generator)} ret = self->${func_name}($arg_list);
                #end if
        ${ret_type.from_native({
            "generator": $generator,
            "in_value": "ret",
            "out_value": "ret",
            "type_name": $ret_type.namespaced_name.replace("*", ""),
            "ntype": $ret_type.get_whole_name($generator),
            "class_name": $class_name,
            "level": 2,
            "scriptname": $generator.scriptname_from_native($ret_type.namespaced_name, $ret_type.namespace_name)})};
        return 1;
            #else
        self->${func_name}($arg_list);
        return 0;
            #end if
        #end if
    }
        #set $arg_idx = $arg_idx + 1
    #end while
#end if
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, ${min_args});
    return 0;

\#ifndef TOLUA_RELEASE
#if not $is_constructor
tolua_lerror:
#end if
    tolua_error(tolua_S,"#ferror in function '${signature_name}'.",&tolua_err);
\#endif
    return 0;
}
