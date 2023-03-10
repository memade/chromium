// Copyright 2016 The PDFium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef CORE_FPDFDOC_CPDF_PAGELABEL_H_
#define CORE_FPDFDOC_CPDF_PAGELABEL_H_

#include "core/fxcrt/unowned_ptr.h"
#include "core/fxcrt/widestring.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

class CPDF_Document;

class CPDF_PageLabel {
 public:
  explicit CPDF_PageLabel(CPDF_Document* pDocument);
  ~CPDF_PageLabel();

  absl::optional<WideString> GetLabel(int nPage) const;

 private:
  UnownedPtr<CPDF_Document> const m_pDocument;
};

#endif  // CORE_FPDFDOC_CPDF_PAGELABEL_H_
