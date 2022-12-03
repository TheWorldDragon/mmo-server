/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: game.proto */

#ifndef PROTOBUF_C_game_2eproto__INCLUDED
#define PROTOBUF_C_game_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _NetCommand NetCommand;
typedef struct _Login Login;
typedef struct _NetReturn NetReturn;
typedef struct _ReturnPublicKey ReturnPublicKey;
typedef struct _ReturnIsLoginSuccess ReturnIsLoginSuccess;


/* --- enums --- */

typedef enum _NetCommand__Command {
  /*
   *不需要参数
   */
  NET_COMMAND__COMMAND__GET_PUBLIC_KEY = 0,
  NET_COMMAND__COMMAND__LOGIN = 1
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(NET_COMMAND__COMMAND)
} NetCommand__Command;
typedef enum _NetReturn__CommandReturn {
  NET_RETURN__COMMAND_RETURN__RETURN_PUBLIC_KEY = 0,
  NET_RETURN__COMMAND_RETURN__RETURN_IS_LOGIN_SUCCESS = 1
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(NET_RETURN__COMMAND_RETURN)
} NetReturn__CommandReturn;

/* --- messages --- */

/*
 *客户端到服务器的包
 */
struct  _NetCommand
{
  ProtobufCMessage base;
  NetCommand__Command command;
  protobuf_c_boolean hasparameter;
  /*
   *指定后面参数message的字节长度, 无表示没有
   */
  uint64_t parameterlength;
};
#define NET_COMMAND__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&net_command__descriptor) \
    , NET_COMMAND__COMMAND__GET_PUBLIC_KEY, 0, 0 }


struct  _Login
{
  ProtobufCMessage base;
  char *account;
  ProtobufCBinaryData password;
};
#define LOGIN__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&login__descriptor) \
    , (char *)protobuf_c_empty_string, {0,NULL} }


/*
 *服务器到客户端的包
 */
struct  _NetReturn
{
  ProtobufCMessage base;
  NetReturn__CommandReturn commandreturn;
  protobuf_c_boolean hasreturnmessage;
  uint64_t returnmessagelength;
};
#define NET_RETURN__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&net_return__descriptor) \
    , NET_RETURN__COMMAND_RETURN__RETURN_PUBLIC_KEY, 0, 0 }


struct  _ReturnPublicKey
{
  ProtobufCMessage base;
  char *publickey;
};
#define RETURN_PUBLIC_KEY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&return_public_key__descriptor) \
    , (char *)protobuf_c_empty_string }


struct  _ReturnIsLoginSuccess
{
  ProtobufCMessage base;
  protobuf_c_boolean success;
};
#define RETURN_IS_LOGIN_SUCCESS__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&return_is_login_success__descriptor) \
    , 0 }


/* NetCommand methods */
void   net_command__init
                     (NetCommand         *message);
size_t net_command__get_packed_size
                     (const NetCommand   *message);
size_t net_command__pack
                     (const NetCommand   *message,
                      uint8_t             *out);
size_t net_command__pack_to_buffer
                     (const NetCommand   *message,
                      ProtobufCBuffer     *buffer);
NetCommand *
       net_command__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   net_command__free_unpacked
                     (NetCommand *message,
                      ProtobufCAllocator *allocator);
/* Login methods */
void   login__init
                     (Login         *message);
size_t login__get_packed_size
                     (const Login   *message);
size_t login__pack
                     (const Login   *message,
                      uint8_t             *out);
size_t login__pack_to_buffer
                     (const Login   *message,
                      ProtobufCBuffer     *buffer);
Login *
       login__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   login__free_unpacked
                     (Login *message,
                      ProtobufCAllocator *allocator);
/* NetReturn methods */
void   net_return__init
                     (NetReturn         *message);
size_t net_return__get_packed_size
                     (const NetReturn   *message);
size_t net_return__pack
                     (const NetReturn   *message,
                      uint8_t             *out);
size_t net_return__pack_to_buffer
                     (const NetReturn   *message,
                      ProtobufCBuffer     *buffer);
NetReturn *
       net_return__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   net_return__free_unpacked
                     (NetReturn *message,
                      ProtobufCAllocator *allocator);
/* ReturnPublicKey methods */
void   return_public_key__init
                     (ReturnPublicKey         *message);
size_t return_public_key__get_packed_size
                     (const ReturnPublicKey   *message);
size_t return_public_key__pack
                     (const ReturnPublicKey   *message,
                      uint8_t             *out);
size_t return_public_key__pack_to_buffer
                     (const ReturnPublicKey   *message,
                      ProtobufCBuffer     *buffer);
ReturnPublicKey *
       return_public_key__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   return_public_key__free_unpacked
                     (ReturnPublicKey *message,
                      ProtobufCAllocator *allocator);
/* ReturnIsLoginSuccess methods */
void   return_is_login_success__init
                     (ReturnIsLoginSuccess         *message);
size_t return_is_login_success__get_packed_size
                     (const ReturnIsLoginSuccess   *message);
size_t return_is_login_success__pack
                     (const ReturnIsLoginSuccess   *message,
                      uint8_t             *out);
size_t return_is_login_success__pack_to_buffer
                     (const ReturnIsLoginSuccess   *message,
                      ProtobufCBuffer     *buffer);
ReturnIsLoginSuccess *
       return_is_login_success__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   return_is_login_success__free_unpacked
                     (ReturnIsLoginSuccess *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*NetCommand_Closure)
                 (const NetCommand *message,
                  void *closure_data);
typedef void (*Login_Closure)
                 (const Login *message,
                  void *closure_data);
typedef void (*NetReturn_Closure)
                 (const NetReturn *message,
                  void *closure_data);
typedef void (*ReturnPublicKey_Closure)
                 (const ReturnPublicKey *message,
                  void *closure_data);
typedef void (*ReturnIsLoginSuccess_Closure)
                 (const ReturnIsLoginSuccess *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor net_command__descriptor;
extern const ProtobufCEnumDescriptor    net_command__command__descriptor;
extern const ProtobufCMessageDescriptor login__descriptor;
extern const ProtobufCMessageDescriptor net_return__descriptor;
extern const ProtobufCEnumDescriptor    net_return__command_return__descriptor;
extern const ProtobufCMessageDescriptor return_public_key__descriptor;
extern const ProtobufCMessageDescriptor return_is_login_success__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_game_2eproto__INCLUDED */