/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi <lganzzzo@gmail.com>
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

#include "UnorderedMapTest.hpp"

#include "oatpp/core/Types.hpp"

namespace oatpp { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

void UnorderedMapTest::onRun() {

  {
    OATPP_LOGI(TAG, "test default constructor...");
    oatpp::UnorderedFields<String> map;

    OATPP_ASSERT(!map);
    OATPP_ASSERT(map == nullptr);

    OATPP_ASSERT(map.get() == nullptr);
    OATPP_ASSERT(map.getValueType()->classId.id == oatpp::data::mapping::type::__class::AbstractUnorderedMap::CLASS_ID.id);
    OATPP_ASSERT(map.getValueType()->params.size() == 2);
    auto it = map.getValueType()->params.begin();
    OATPP_ASSERT(*it++ == oatpp::String::Class::getType());
    OATPP_ASSERT(*it++ == oatpp::String::Class::getType());
    OATPP_LOGI(TAG, "OK");
  }

  {
    OATPP_LOGI(TAG, "test empty ilist constructor...");
    oatpp::UnorderedFields<String> map({});

    OATPP_ASSERT(map);
    OATPP_ASSERT(map != nullptr);
    OATPP_ASSERT(map->size() == 0);

    OATPP_ASSERT(map.get() != nullptr);
    OATPP_ASSERT(map.getValueType()->classId.id == oatpp::data::mapping::type::__class::AbstractUnorderedMap::CLASS_ID.id);
    OATPP_ASSERT(map.getValueType()->params.size() == 2);
    auto it = map.getValueType()->params.begin();
    OATPP_ASSERT(*it++ == oatpp::String::Class::getType());
    OATPP_ASSERT(*it++ == oatpp::String::Class::getType());
    OATPP_LOGI(TAG, "OK");
  }

  {
    OATPP_LOGI(TAG, "test createShared()...");
    oatpp::UnorderedFields<String> map = oatpp::UnorderedFields<String>::createShared();

    OATPP_ASSERT(map);
    OATPP_ASSERT(map != nullptr);
    OATPP_ASSERT(map->size() == 0);

    OATPP_ASSERT(map.get() != nullptr);
    OATPP_ASSERT(map.getValueType()->classId.id == oatpp::data::mapping::type::__class::AbstractUnorderedMap::CLASS_ID.id);
    OATPP_LOGI(TAG, "OK");
  }

  {
    OATPP_LOGI(TAG, "test copy-assignment operator...");
    oatpp::UnorderedFields<String> map1({});
    oatpp::UnorderedFields<String> map2;

    map2 = map1;

    OATPP_ASSERT(map1);
    OATPP_ASSERT(map2);

    OATPP_ASSERT(map1->size() == 0);
    OATPP_ASSERT(map2->size() == 0);

    OATPP_ASSERT(map1.get() == map2.get());

    map2->insert({"key", "a"});

    OATPP_ASSERT(map1->size() == 1);
    OATPP_ASSERT(map2->size() == 1);

    map2 = {{"key1", "b"}, {"key2", "c"}};

    OATPP_ASSERT(map1->size() == 1);
    OATPP_ASSERT(map2->size() == 2);

    OATPP_ASSERT(map2["key1"] == "b");
    OATPP_ASSERT(map2["key2"] == "c");
    OATPP_LOGI(TAG, "OK");
  }

  {
    OATPP_LOGI(TAG, "test move-assignment operator...");
    oatpp::UnorderedFields<String> map1({});
    oatpp::UnorderedFields<String> map2;

    map2 = std::move(map1);

    OATPP_ASSERT(!map1);
    OATPP_ASSERT(map2);
    OATPP_LOGI(TAG, "OK");
  }

  {
    OATPP_LOGI(TAG, "test get element by index...");
    oatpp::UnorderedFields<String> map = {{"key1", "a"}, {"key2", "b"}, {"key3", "c"}};

    OATPP_ASSERT(map);
    OATPP_ASSERT(map != nullptr);
    OATPP_ASSERT(map->size() == 3);

    OATPP_ASSERT(map["key1"] == "a");
    OATPP_ASSERT(map["key2"] == "b");
    OATPP_ASSERT(map["key3"] == "c");

    map["key2"] = "Hello!";

    OATPP_ASSERT(map->size() == 3);

    OATPP_ASSERT(map["key1"] == "a");
    OATPP_ASSERT(map["key2"] == "Hello!");
    OATPP_ASSERT(map["key3"] == "c");
    OATPP_LOGI(TAG, "OK");
  }

  {
    OATPP_LOGI(TAG, "test polymorphicDispatcher...");
    oatpp::UnorderedFields<String> map = {{"key1", "a"}, {"key2", "b"}, {"key3", "c"}};

    auto polymorphicDispatcher = static_cast<const typename oatpp::UnorderedFields<String>::Class::PolymorphicDispatcher*>(
      map.getValueType()->polymorphicDispatcher
    );

    polymorphicDispatcher->addPolymorphicItem(map, oatpp::String("key1"), oatpp::String("d"));

    OATPP_ASSERT(map->size() == 3);

    OATPP_ASSERT(map["key1"] == "d");
    OATPP_ASSERT(map["key2"] == "b");
    OATPP_ASSERT(map["key3"] == "c");
    OATPP_LOGI(TAG, "OK");
  }

}

}}}}}}
