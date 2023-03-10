// Copyright 2019 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/autofill/core/common/autofill_internals/logging_scope.h"

#include "base/json/json_writer.h"
#include "components/autofill/core/common/logging/log_buffer.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace autofill {

TEST(LoggingScope, Serialization) {
  LogBuffer buffer;
  buffer << LoggingScope::kContext;
  std::string json;
  EXPECT_TRUE(base::JSONWriter::Write(*buffer.RetrieveResult(), &json));
  EXPECT_EQ(R"({"attributes":{"class":"log-entry","scope":"Context"},)"
            R"("type":"element","value":"div"})",
            json);
}

}  // namespace autofill
