// Copyright 2016 The PDFium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef FXBARCODE_CBC_PDF417I_H_
#define FXBARCODE_CBC_PDF417I_H_

#include "core/fxcrt/widestring.h"
#include "fxbarcode/cbc_codebase.h"

class CBC_PDF417Writer;
class CFX_Matrix;
class CFX_RenderDevice;

class CBC_PDF417I final : public CBC_CodeBase {
 public:
  CBC_PDF417I();
  ~CBC_PDF417I() override;

  // CBC_CodeBase:
  bool Encode(WideStringView contents) override;
  bool RenderDevice(CFX_RenderDevice* device,
                    const CFX_Matrix& matrix) override;
  BC_TYPE GetType() override;

 private:
  CBC_PDF417Writer* GetPDF417Writer();
};

#endif  // FXBARCODE_CBC_PDF417I_H_
