// Copyright 2014 The PDFium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CORE_FXGE_SKIA_FX_SKIA_DEVICE_H_
#define CORE_FXGE_SKIA_FX_SKIA_DEVICE_H_

#include <memory>

#include "core/fxcrt/retain_ptr.h"
#include "core/fxge/cfx_fillrenderoptions.h"
#include "core/fxge/cfx_path.h"
#include "core/fxge/renderdevicedriver_iface.h"

class SkCanvas;
class SkMatrix;
class SkPaint;
class SkPictureRecorder;
class SkiaState;
class TextCharPos;

class CFX_SkiaDeviceDriver final : public RenderDeviceDriverIface {
 public:
  static std::unique_ptr<CFX_SkiaDeviceDriver> Create(
      RetainPtr<CFX_DIBitmap> pBitmap,
      bool bRgbByteOrder,
      RetainPtr<CFX_DIBitmap> pBackdropBitmap,
      bool bGroupKnockout);

  explicit CFX_SkiaDeviceDriver(SkPictureRecorder* recorder);
  ~CFX_SkiaDeviceDriver() override;

  /** Options */
  DeviceType GetDeviceType() const override;
  int GetDeviceCaps(int caps_id) const override;

  /** Save and restore all graphic states */
  void SaveState() override;
  void RestoreState(bool bKeepSaved) override;

  /** Set clipping path using filled region */
  bool SetClip_PathFill(
      const CFX_Path& path,                       // path info
      const CFX_Matrix* pObject2Device,           // optional transformation
      const CFX_FillRenderOptions& fill_options)  // fill options
      override;

  /** Set clipping path using stroked region */
  bool SetClip_PathStroke(
      const CFX_Path& path,              // path info
      const CFX_Matrix* pObject2Device,  // required transformation
      const CFX_GraphStateData*
          pGraphState)  // graphic state, for pen attributes
      override;

  /** Draw a path */
  bool DrawPath(const CFX_Path& path,
                const CFX_Matrix* pObject2Device,
                const CFX_GraphStateData* pGraphState,
                uint32_t fill_color,
                uint32_t stroke_color,
                const CFX_FillRenderOptions& fill_options,
                BlendMode blend_type) override;

  bool FillRectWithBlend(const FX_RECT& rect,
                         uint32_t fill_color,
                         BlendMode blend_type) override;

  /** Draw a single pixel (device dependant) line */
  bool DrawCosmeticLine(const CFX_PointF& ptMoveTo,
                        const CFX_PointF& ptLineTo,
                        uint32_t color,
                        BlendMode blend_type) override;

  bool GetClipBox(FX_RECT* pRect) override;

  /** Load device buffer into a DIB */
  bool GetDIBits(const RetainPtr<CFX_DIBitmap>& pBitmap,
                 int left,
                 int top) override;

  RetainPtr<CFX_DIBitmap> GetBackDrop() override;

  bool SetDIBits(const RetainPtr<CFX_DIBBase>& pBitmap,
                 uint32_t color,
                 const FX_RECT& src_rect,
                 int dest_left,
                 int dest_top,
                 BlendMode blend_type) override;
  bool SetBitsWithMask(const RetainPtr<CFX_DIBBase>& pBitmap,
                       const RetainPtr<CFX_DIBBase>& pMask,
                       int dest_left,
                       int dest_top,
                       int bitmap_alpha,
                       BlendMode blend_type) override;
  void SetGroupKnockout(bool group_knockout) override;

  bool StretchDIBits(const RetainPtr<CFX_DIBBase>& pBitmap,
                     uint32_t color,
                     int dest_left,
                     int dest_top,
                     int dest_width,
                     int dest_height,
                     const FX_RECT* pClipRect,
                     const FXDIB_ResampleOptions& options,
                     BlendMode blend_type) override;

  bool StartDIBits(const RetainPtr<CFX_DIBBase>& pBitmap,
                   int bitmap_alpha,
                   uint32_t color,
                   const CFX_Matrix& matrix,
                   const FXDIB_ResampleOptions& options,
                   std::unique_ptr<CFX_ImageRenderer>* handle,
                   BlendMode blend_type) override;

  bool ContinueDIBits(CFX_ImageRenderer* handle,
                      PauseIndicatorIface* pPause) override;

  bool DrawBitsWithMask(const RetainPtr<CFX_DIBBase>& pBitmap,
                        const RetainPtr<CFX_DIBBase>& pMask,
                        int bitmap_alpha,
                        const CFX_Matrix& matrix,
                        BlendMode blend_type);

  bool DrawDeviceText(pdfium::span<const TextCharPos> pCharPos,
                      CFX_Font* pFont,
                      const CFX_Matrix& mtObject2Device,
                      float font_size,
                      uint32_t color,
                      const CFX_TextRenderOptions& options) override;

  int GetDriverType() const override;

  bool DrawShading(const CPDF_ShadingPattern* pPattern,
                   const CFX_Matrix* pMatrix,
                   const FX_RECT& clip_rect,
                   int alpha,
                   bool bAlphaMode) override;

  virtual uint8_t* GetBuffer() const;

  void PaintStroke(SkPaint* spaint,
                   const CFX_GraphStateData* pGraphState,
                   const SkMatrix& matrix);
  void Clear(uint32_t color);
  void Flush() override;
  SkCanvas* SkiaCanvas() { return m_pCanvas; }
  void DebugVerifyBitmapIsPreMultiplied() const;
  void Dump() const;

  bool GetGroupKnockout() const { return m_bGroupKnockout; }

 private:
  CFX_SkiaDeviceDriver(RetainPtr<CFX_DIBitmap> pBitmap,
                       bool bRgbByteOrder,
                       RetainPtr<CFX_DIBitmap> pBackdropBitmap,
                       bool bGroupKnockout);

  bool StartDIBitsSkia(const RetainPtr<CFX_DIBBase>& pBitmap,
                       int bitmap_alpha,
                       uint32_t color,
                       const CFX_Matrix& matrix,
                       const FXDIB_ResampleOptions& options,
                       BlendMode blend_type);

  RetainPtr<CFX_DIBitmap> m_pBitmap;
  RetainPtr<CFX_DIBitmap> m_pBackdropBitmap;

  // The input bitmap passed by the render device. Only used when the input
  // bitmap is 24 bpp and cannot be directly used as the back of a SkCanvas.
  RetainPtr<CFX_DIBitmap> m_pOriginalBitmap;

  SkCanvas* m_pCanvas;
  SkPictureRecorder* const m_pRecorder;
  std::unique_ptr<SkiaState> m_pCache;
  CFX_FillRenderOptions m_FillOptions;
  bool m_bRgbByteOrder;
  bool m_bGroupKnockout;
};

#endif  // CORE_FXGE_SKIA_FX_SKIA_DEVICE_H_
