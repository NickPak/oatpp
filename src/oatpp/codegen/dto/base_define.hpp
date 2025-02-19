/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi <lganzzzo@gmail.com>
 *                         Benedikt-Alexander Mokroß <bam@icognize.de>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

// Defaults

/**
 * Codegen macoro to be used in classes extending &id:oatpp::data::mapping::type::Object; to generate required fields/methods/constructors for DTO object.
 * @param TYPE_NAME - name of the DTO class.
 * @param TYPE_EXTEND - name of the parent DTO class. If DTO extends &id:oatpp::data::mapping::type::Object; TYPE_EXETENDS should be `Object`.
 */
#define DTO_INIT(TYPE_NAME, TYPE_EXTEND) \
  template<class __Z__T__PARAM> \
  friend class oatpp::data::mapping::type::__class::Object; \
public: \
  typedef TYPE_NAME Z__CLASS; \
  typedef TYPE_EXTEND Z__CLASS_EXTENDED; \
  typedef oatpp::data::mapping::type::DTOWrapper<Z__CLASS> Wrapper; \
private: \
  static const char* Z__CLASS_TYPE_NAME() { \
    return #TYPE_NAME; \
  } \
\
  static oatpp::data::mapping::type::BaseObject::Properties* Z__CLASS_GET_FIELDS_MAP(){ \
    static oatpp::data::mapping::type::BaseObject::Properties map = oatpp::data::mapping::type::BaseObject::Properties(); \
    return &map; \
  } \
public: \
\
  template<typename ... Args> \
  static Wrapper createShared(Args... args){ \
    return Wrapper(std::make_shared<Z__CLASS>(args...), Wrapper::Class::getType()); \
  }

// Fields

#define OATPP_MACRO_DTO_FIELD_1(TYPE, NAME) \
\
static v_int64 Z__PROPERTY_OFFSET_##NAME() { \
  char buffer[sizeof(Z__CLASS)]; \
  auto obj = static_cast<Z__CLASS*>((void*)buffer); \
  auto ptr = &obj->NAME; \
  return (v_int64) ptr - (v_int64) buffer; \
} \
\
static oatpp::data::mapping::type::BaseObject::Property* Z__PROPERTY_SINGLETON_##NAME() { \
  static oatpp::data::mapping::type::BaseObject::Property* property = \
      new oatpp::data::mapping::type::BaseObject::Property(Z__PROPERTY_OFFSET_##NAME(), \
                                                     #NAME, \
                                                     TYPE::Class::getType()); \
  return property; \
} \
\
static bool Z__PROPERTY_INIT_##NAME(... /* default initializer for all cases */) { \
  Z__CLASS_GET_FIELDS_MAP()->pushBack(Z__PROPERTY_SINGLETON_##NAME()); \
  return true; \
} \
\
static TYPE Z__PROPERTY_INITIALIZER_PROXY_##NAME() { \
  static bool initialized = Z__PROPERTY_INIT_##NAME(1 /* init info if found */); \
  (void)initialized; \
  return TYPE(); \
} \
\
TYPE NAME = Z__PROPERTY_INITIALIZER_PROXY_##NAME()

#define OATPP_MACRO_DTO_FIELD_2(TYPE, NAME, QUALIFIER) \
\
static v_int64 Z__PROPERTY_OFFSET_##NAME() { \
  char buffer[sizeof(Z__CLASS)]; \
  auto obj = static_cast<Z__CLASS*>((void*)buffer); \
  auto ptr = &obj->NAME; \
  return (v_int64) ptr - (v_int64) buffer; \
} \
\
static oatpp::data::mapping::type::BaseObject::Property* Z__PROPERTY_SINGLETON_##NAME() { \
  static oatpp::data::mapping::type::BaseObject::Property* property = \
      new oatpp::data::mapping::type::BaseObject::Property(Z__PROPERTY_OFFSET_##NAME(), \
                                                     QUALIFIER, \
                                                     TYPE::Class::getType()); \
  return property; \
} \
\
static bool Z__PROPERTY_INIT_##NAME(... /* default initializer for all cases */) { \
  Z__CLASS_GET_FIELDS_MAP()->pushBack(Z__PROPERTY_SINGLETON_##NAME()); \
  return true; \
} \
\
static TYPE Z__PROPERTY_INITIALIZER_PROXY_##NAME() { \
  static bool initialized = Z__PROPERTY_INIT_##NAME(1 /* init info if found */); \
  (void)initialized; \
  return TYPE(); \
} \
\
TYPE NAME = Z__PROPERTY_INITIALIZER_PROXY_##NAME()

/**
 * Codegen macro to generate fields of DTO object.
 * @param TYPE - type of the field.
 * @param NAME - name of the field.
 * @param QUALIFIER_NAME - additional (optional) field to specify serialized name of the field. If not specified it will be same as NAME.
 */
#define DTO_FIELD(TYPE, ...) \
OATPP_MACRO_EXPAND(OATPP_MACRO_MACRO_SELECTOR(OATPP_MACRO_DTO_FIELD_, (__VA_ARGS__)) (TYPE, __VA_ARGS__))

// DTO_FIELD_INFO

#define DTO_FIELD_INFO(NAME) \
\
static bool Z__PROPERTY_INIT_##NAME(int) { \
  Z__PROPERTY_INIT_##NAME(); /* call first initialization */ \
  Z__PROPERTY_ADD_INFO_##NAME(&Z__PROPERTY_SINGLETON_##NAME()->info); \
  return true; \
} \
\
static void Z__PROPERTY_ADD_INFO_##NAME(oatpp::data::mapping::type::BaseObject::Property::Info* info)

// FOR EACH

#define OATPP_MACRO_DTO_HC_EQ_PARAM_HC(INDEX, COUNT, X) \
result = ((result << 5) - result) + std::hash<decltype(X)>{}(X);

#define OATPP_MACRO_DTO_HC_EQ_PARAM_EQ(INDEX, COUNT, X) \
&& X == other.X

#define DTO_HASHCODE_AND_EQUALS(...) \
v_uint64 defaultHashCode() const override { \
  return 1; \
} \
\
bool defaultEquals(const DTO&) const override { \
  return true; \
} \
\
v_uint64 hashCode() const { \
  v_uint64 result = 1; \
  result = ((result << 5) - result) + static_cast<const Z__CLASS_EXTENDED&>(*this).hashCode(); \
  OATPP_MACRO_FOREACH(OATPP_MACRO_DTO_HC_EQ_PARAM_HC, __VA_ARGS__) \
  return result; \
} \
\
bool operator==(const Z__CLASS& other) const { \
  return static_cast<const Z__CLASS_EXTENDED&>(*this) == static_cast<const Z__CLASS_EXTENDED&>(other) \
  OATPP_MACRO_FOREACH(OATPP_MACRO_DTO_HC_EQ_PARAM_EQ, __VA_ARGS__) \
  ; \
} \
\
bool operator!=(const Z__CLASS& other) const { \
  return !this->operator==(other); \
}

/**
 * Hashcode and Equals macro. <br>
 * List DTO-fields which should count in hashcode and equals operators.
 */
#define DTO_HC_EQ(...) DTO_HASHCODE_AND_EQUALS(__VA_ARGS__)
