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

#include "TemporaryFile.hpp"

#include "oatpp/core/data/stream/BufferStream.hpp"
#include "oatpp/encoding/Hex.hpp"
#include "oatpp/core/utils/Random.hpp"

namespace oatpp { namespace data { namespace share {

TemporaryFile::FileHandle::~FileHandle() {
  if(fileName) {
    std::remove(fileName->c_str());
  }
}

oatpp::String TemporaryFile::concatDirAndName(const oatpp::String& dir, const oatpp::String& filename) {
  if(dir && dir->size() > 0) {
    auto lastChar = dir->data()[dir->size() - 1];
    if(lastChar != '/' && lastChar != '\\') {
      return dir + "/" + filename;
    }
    return dir + filename;
  }
  return filename;
}

oatpp::String TemporaryFile::constructRandomFilename(const oatpp::String& dir, v_int32 randomWordSizeBytes) {

  std::unique_ptr<v_char8[]> buff(new v_char8[randomWordSizeBytes]);
  utils::random::Random::randomBytes(buff.get(), randomWordSizeBytes);
  data::stream::BufferOutputStream s(randomWordSizeBytes * 2 + 4);
  encoding::Hex::encode(&s, buff.get(), randomWordSizeBytes, encoding::Hex::ALPHABET_LOWER);
  s << ".tmp";

  return concatDirAndName(dir, s.toString());

}

TemporaryFile::TemporaryFile(const oatpp::String& tmpDirectory, v_int32 randomWordSizeBytes)
  : m_handle(std::make_shared<FileHandle>(constructRandomFilename(tmpDirectory, randomWordSizeBytes)))
{}

TemporaryFile::TemporaryFile(const oatpp::String& tmpDirectory, const oatpp::String& tmpFileName)
  : m_handle(std::make_shared<FileHandle>(concatDirAndName(tmpDirectory, tmpFileName)))
{}

oatpp::String TemporaryFile::getFullFileName() {
  if(m_handle) {
    return m_handle->fileName;
  }
  return nullptr;
}

data::stream::FileOutputStream TemporaryFile::openOutputStream() {
  if(m_handle) {
    return data::stream::FileOutputStream(m_handle->fileName->c_str(), "wb", m_handle);
  }
  throw std::runtime_error("[oatpp::data::share::TemporaryFile::openOutputStream()]: Error. FileHandle is NOT initialized.");
}

data::stream::FileInputStream TemporaryFile::openInputStream() {
  if(m_handle) {
    return data::stream::FileInputStream(m_handle->fileName->c_str(), m_handle);
  }
  throw std::runtime_error("[oatpp::data::share::TemporaryFile::openInputStream()]: Error. FileHandle is NOT initialized.");
}

std::shared_ptr<data::stream::FileOutputStream> TemporaryFile::openOutputStreamShared() {
  if(m_handle) {
    return std::make_shared<data::stream::FileOutputStream>(m_handle->fileName->c_str(), "wb", m_handle);
  }
  throw std::runtime_error("[oatpp::data::share::TemporaryFile::openOutputStreamShared()]: Error. FileHandle is NOT initialized.");
}

std::shared_ptr<data::stream::FileInputStream> TemporaryFile::openInputStreamShared() {
  if(m_handle) {
    return std::make_shared<data::stream::FileInputStream>(m_handle->fileName->c_str(), m_handle);
  }
  throw std::runtime_error("[oatpp::data::share::TemporaryFile::openInputStreamShared()]: Error. FileHandle is NOT initialized.");
}

bool TemporaryFile::moveFile(const oatpp::String& fullFileName) {
  if(m_handle) {
    return std::rename(m_handle->fileName->c_str(), fullFileName->c_str()) == 0;
  }
  return false;
}

}}}
