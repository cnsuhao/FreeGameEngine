#ifndef H_SCRIPT_TOLUA_CONVERTER_H
#define H_SCRIPT_TOLUA_CONVERTER_H

#include "math/math.hpp"
#include "util/smartptr.h"

#include <vector>
#include <string>
#include <type_traits>

extern "C" {
    #include "lua.h"
    #include "tolua++.h"
}

//////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////

#ifndef TOLUA_RELEASE
void luaval_to_native_err(lua_State* L, const char* msg, tolua_Error* err, const char* funcName = "");
#endif

bool luaval_is_usertype(lua_State* L,int lo,const char* type, int def);

const char * tolua_get_typename(const char * cpp_typename);

bool tolua_register_typename(const char * cpp_typename, const char * lua_typename);

template <typename T>
bool tolua_register_typename(const char * lua_typename);

void tolua_push_ref_object(IReferenceCount *p, const char * type);

//////////////////////////////////////////////////////////////////////
/// to native
//////////////////////////////////////////////////////////////////////
bool luaval_to_long(lua_State* L,int lo, long* outValue, const char* funcName = "");
bool luaval_to_ulong(lua_State* L,int lo, unsigned long* outValue, const char* funcName="");
bool luaval_to_uint16(lua_State* L,int lo,uint16_t* outValue, const char* funcName = "");
bool luaval_to_ushort(lua_State* L, int lo, unsigned short* outValue, const char* funcName = "");
bool luaval_to_int32(lua_State* L,int lo,int* outValue, const char* funcName = "");
bool luaval_to_uint32(lua_State* L, int lo, unsigned int* outValue, const char* funcName = "");
bool luaval_to_boolean(lua_State* L,int lo,bool* outValue, const char* funcName = "");
bool luaval_to_number(lua_State* L,int lo,double* outValue, const char* funcName = "");
bool luaval_to_long_long(lua_State* L,int lo,long long* outValue, const char* funcName = "");
bool luaval_to_string(lua_State* L, int lo, std::string* outValue, const char* funcName = "");
bool luaval_to_ssize(lua_State* L,int lo, ssize_t* outValue, const char* funcName = "");
bool luaval_to_vec2(lua_State* L,int lo, ora::Vector2* outValue, const char* funcName = "");
bool luaval_to_vec3(lua_State* L,int lo, ora::Vector3* outValue, const char* funcName = "");
bool luaval_to_vec4(lua_State* L,int lo, ora::Vector4* outValue, const char* funcName = "");
bool luaval_to_quaternion(lua_State* L,int lo, ora::Quaternion* outValue, const char* funcName = "");
bool luaval_to_matrix(lua_State* L, int lo, ora::Matrix* outValue , const char* funcName = "");
bool luaval_to_vector_string(lua_State* L, int lo, std::vector<std::string>* ret, const char* funcName = "");
bool luaval_to_vector_int(lua_State* L, int lo, std::vector<int>* ret, const char* funcName = "");
bool luaval_to_vector_float(lua_State* L, int lo, std::vector<float>* ret, const char* funcName = "");

template <typename T>
bool luaval_to_object(lua_State* L, int lo, T** ret, const char* type);

//////////////////////////////////////////////////////////////////////
// from native
//////////////////////////////////////////////////////////////////////
template <typename T>
void object_to_luaval(lua_State* L, T* ret, const char* type);

template <typename T>
void object_to_luaval(lua_State* L, const T * ret, const char* type);

void vec2_to_luaval(lua_State* L,const ora::Vector2& vec2);
void vec3_to_luaval(lua_State* L,const ora::Vector3& vec3);
void vec4_to_luaval(lua_State* L,const ora::Vector4& vec4);
void matrix_to_luaval(lua_State* L,const ora::Matrix& mat);
void quaternion_to_luaval(lua_State* L,const ora::Quaternion& inValue);

void vector_string_to_luaval(lua_State* L, const std::vector<std::string>& inValue);
void vector_int_to_luaval(lua_State* L, const std::vector<int>& inValue);
void vector_float_to_luaval(lua_State* L, const std::vector<float>& inValue);
void vector_ushort_to_luaval(lua_State* L, const std::vector<unsigned short>& inValue);

#include "tolua_converter.inl"

#endif //H_SCRIPT_TOLUA_CONVERTER_H
