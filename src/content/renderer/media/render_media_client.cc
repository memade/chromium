// Copyright 2014 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/renderer/media/render_media_client.h"

#include "base/command_line.h"
#include "base/time/default_tick_clock.h"
#include "content/public/common/content_client.h"
#include "content/public/renderer/content_renderer_client.h"
#include "content/renderer/render_thread_impl.h"
#include "media/base/audio_parameters.h"
#include "media/base/media_switches.h"
#include "media/base/video_color_space.h"
#include "ui/display/display_switches.h"

namespace {

// At present, HEVC is the only codec which has optional platform support.
// Some clients need this knowledge synchronously, so we try to populate
// it asynchronously ahead of time, but can fallback to a blocking call
// when it's needed synchronously.
#if BUILDFLAG(ENABLE_PLATFORM_HEVC) &&                                     \
    (BUILDFLAG(IS_WIN) || BUILDFLAG(IS_CHROMEOS) || BUILDFLAG(IS_LINUX) || \
     BUILDFLAG(IS_MAC))
#define NEEDS_PROFILE_UPDATER 1
#else
#define NEEDS_PROFILE_UPDATER 0
#endif

#if NEEDS_PROFILE_UPDATER
void UpdateVideoProfilesInternal(
    const media::SupportedVideoDecoderConfigs& supported_configs) {
  base::flat_set<media::VideoCodecProfile> media_profiles;
  for (const auto& config : supported_configs) {
    for (int profile = config.profile_min; profile <= config.profile_max;
         profile++) {
      media_profiles.insert(static_cast<media::VideoCodecProfile>(profile));
    }
  }
  media::UpdateDefaultSupportedVideoProfiles(media_profiles);
}
#endif

}  // namespace

namespace content {

void RenderMediaClient::Initialize() {
  static RenderMediaClient* client = new RenderMediaClient();
  media::SetMediaClient(client);
}

RenderMediaClient::RenderMediaClient()
    : main_task_runner_(base::SingleThreadTaskRunner::GetCurrentDefault()),
      io_task_runner_(RenderThreadImpl::current()->GetIOTaskRunner()) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(main_thread_sequence_checker_);
#if NEEDS_PROFILE_UPDATER
  // We'll first try to query the supported video decoder configurations
  // asynchronously. If IsSupportedVideoType() is called before we get a
  // response, that method will fall back to querying the video decoder
  // configurations synchronously.
  //
  // The base::Unretained()s here are safe here since the MediaClient is never
  // destructed.
  RenderThreadImpl::current()->BindHostReceiver(
      interface_factory_for_supported_profiles_.BindNewPipeAndPassReceiver());
  interface_factory_for_supported_profiles_.set_disconnect_handler(
      base::BindOnce(&RenderMediaClient::OnGetSupportedVideoDecoderConfigs,
                     base::Unretained(this),
                     media::SupportedVideoDecoderConfigs(),
                     media::VideoDecoderType::kUnknown));

  interface_factory_for_supported_profiles_->CreateVideoDecoder(
      video_decoder_for_supported_profiles_.BindNewPipeAndPassReceiver(),
      /*dst_video_decoder=*/{});
  video_decoder_for_supported_profiles_.set_disconnect_handler(
      base::BindOnce(&RenderMediaClient::OnGetSupportedVideoDecoderConfigs,
                     base::Unretained(this),
                     media::SupportedVideoDecoderConfigs(),
                     media::VideoDecoderType::kUnknown),
      main_task_runner_);

  video_decoder_for_supported_profiles_->GetSupportedConfigs(
      base::BindOnce(&RenderMediaClient::OnGetSupportedVideoDecoderConfigs,
                     base::Unretained(this)));
#endif
}

RenderMediaClient::~RenderMediaClient() = default;

void RenderMediaClient::GetSupportedKeySystems(
    media::GetSupportedKeySystemsCB cb) {
  GetContentClient()->renderer()->GetSupportedKeySystems(std::move(cb));
}

bool RenderMediaClient::IsSupportedAudioType(const media::AudioType& type) {
  return GetContentClient()->renderer()->IsSupportedAudioType(type);
}

bool RenderMediaClient::IsSupportedVideoType(const media::VideoType& type) {
#if NEEDS_PROFILE_UPDATER
  // This method should not run on the IO thread: we don't want to make the sync
  // mojo call below on that thread.
  DCHECK(!io_task_runner_->RunsTasksInCurrentSequence());
  {
    base::AutoLock lock(supported_video_decoder_profiles_lock_);
    if (!supported_video_decoder_profiles_are_known_) {
      // We didn't get the response for the asynchronous query in time. Let's
      // fall back to a synchronous query.
      media::SupportedVideoDecoderConfigs configs;
      media::VideoDecoderType video_decoder_type;
      if (!video_decoder_for_supported_profiles_->GetSupportedConfigs(
              &configs, &video_decoder_type)) {
        configs.clear();
      }
      UpdateVideoProfilesInternal(configs);
      supported_video_decoder_profiles_are_known_ = true;

      // The base::Unretained() here is safe because the MediaClient is never
      // destructed.
      main_task_runner_->PostTask(
          FROM_HERE,
          base::BindOnce(
              &RenderMediaClient::
                  ResetConnectionForSupportedProfilesQueryOnMainThread,
              base::Unretained(this)));
    }
  }
#endif

  return GetContentClient()->renderer()->IsSupportedVideoType(type);
}

bool RenderMediaClient::IsSupportedBitstreamAudioCodec(
    media::AudioCodec codec) {
  return GetContentClient()->renderer()->IsSupportedBitstreamAudioCodec(codec);
}

absl::optional<::media::AudioRendererAlgorithmParameters>
RenderMediaClient::GetAudioRendererAlgorithmParameters(
    media::AudioParameters audio_parameters) {
  return GetContentClient()->renderer()->GetAudioRendererAlgorithmParameters(
      audio_parameters);
}

void RenderMediaClient::OnGetSupportedVideoDecoderConfigs(
    const media::SupportedVideoDecoderConfigs& configs,
    media::VideoDecoderType type) {
#if NEEDS_PROFILE_UPDATER
  base::AutoLock lock(supported_video_decoder_profiles_lock_);
  if (!supported_video_decoder_profiles_are_known_) {
    UpdateVideoProfilesInternal(configs);
    supported_video_decoder_profiles_are_known_ = true;

    // The base::Unretained() here is safe because the MediaClient is never
    // destructed.
    main_task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce(&RenderMediaClient::
                           ResetConnectionForSupportedProfilesQueryOnMainThread,
                       base::Unretained(this)));
  }
#endif
}

void RenderMediaClient::ResetConnectionForSupportedProfilesQueryOnMainThread() {
#if NEEDS_PROFILE_UPDATER
  video_decoder_for_supported_profiles_.reset();
  interface_factory_for_supported_profiles_.reset();
#endif
}

}  // namespace content
