// Copyright 2020 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "weblayer/browser/insecure_form_controller_client.h"

#include "components/security_interstitials/content/insecure_form_tab_storage.h"
#include "components/security_interstitials/content/settings_page_helper.h"
#include "content/public/browser/web_contents.h"
#include "weblayer/browser/i18n_util.h"

namespace weblayer {

// static
std::unique_ptr<security_interstitials::MetricsHelper>
InsecureFormControllerClient::GetMetricsHelper(const GURL& url) {
  security_interstitials::MetricsHelper::ReportDetails settings;
  settings.metric_prefix = "insecure_form";
  return std::make_unique<security_interstitials::MetricsHelper>(url, settings,
                                                                 nullptr);
}

// static
std::unique_ptr<security_interstitials::SettingsPageHelper>
InsecureFormControllerClient::GetSettingsPageHelper() {
  // Return nullptr since there is no enhanced protection message in insecure
  // form interstitials.
  return nullptr;
}

InsecureFormControllerClient::InsecureFormControllerClient(
    content::WebContents* web_contents,
    const GURL& form_target_url)
    : SecurityInterstitialControllerClient(
          web_contents,
          GetMetricsHelper(form_target_url),
          nullptr, /* prefs */
          i18n::GetApplicationLocale(),
          GURL("about:blank") /* default_safe_page */,
          GetSettingsPageHelper()),
      web_contents_(web_contents) {}

InsecureFormControllerClient::~InsecureFormControllerClient() = default;

void InsecureFormControllerClient::GoBack() {
  SecurityInterstitialControllerClient::GoBackAfterNavigationCommitted();
}

void InsecureFormControllerClient::Proceed() {
  // Set the is_proceeding flag on the tab storage so reload doesn't trigger
  // another interstitial.
  security_interstitials::InsecureFormTabStorage* tab_storage =
      security_interstitials::InsecureFormTabStorage::GetOrCreate(
          web_contents_);
  tab_storage->SetIsProceeding(true);
  // We don't check for repost on the proceed reload since the interstitial
  // explains this will submit the form.
  web_contents_->GetController().Reload(content::ReloadType::NORMAL, false);
}

}  // namespace weblayer
