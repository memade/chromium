// Copyright 2017 The PDFium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef XFA_FXFA_PARSER_CXFA_INSTANCEMANAGER_H_
#define XFA_FXFA_PARSER_CXFA_INSTANCEMANAGER_H_

#include "xfa/fxfa/parser/cxfa_node.h"

class CXFA_InstanceManager final : public CXFA_Node {
 public:
  CONSTRUCT_VIA_MAKE_GARBAGE_COLLECTED;
  ~CXFA_InstanceManager() override;

 private:
  CXFA_InstanceManager(CXFA_Document* doc, XFA_PacketType packet);
};

#endif  // XFA_FXFA_PARSER_CXFA_INSTANCEMANAGER_H_
