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

#include "IOBuffer.hpp"

namespace oatpp { namespace data { namespace buffer {

const v_buff_size IOBuffer::BUFFER_SIZE = 4096;

IOBuffer::IOBuffer()
  : m_entry(getBufferPool().obtain())
{}

std::shared_ptr<IOBuffer> IOBuffer::createShared(){
  return Shared_IOBuffer_Pool::allocateShared();
}

IOBuffer::~IOBuffer() {
  oatpp::base::memory::MemoryPool::free(m_entry);
}

void* IOBuffer::getData(){
  return m_entry;
}

v_buff_size IOBuffer::getSize(){
  return BUFFER_SIZE;
}
  
}}}
