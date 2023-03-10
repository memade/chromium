// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/attribution_reporting/attribution_interop_parser.h"

#include <stddef.h>
#include <stdint.h>

#include <memory>
#include <string>
#include <utility>

#include "base/check.h"
#include "base/functional/function_ref.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_piece.h"
#include "base/time/time.h"
#include "base/types/expected.h"
#include "content/browser/attribution_reporting/attribution_config.h"
#include "content/browser/attribution_reporting/attribution_parser_test_utils.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/gurl.h"
#include "url/origin.h"

namespace content {
namespace {

class AttributionInteropParser {
 public:
  AttributionInteropParser() = default;

  // Converts interop test input to simulator input format.
  base::expected<base::Value::Dict, std::string> SimulatorInputFromInteropInput(
      base::Value::Dict) &&;

  // Converts simulator output to interop test output format.
  base::expected<base::Value::Dict, std::string>
  InteropOutputFromSimulatorOutput(base::Value::Dict) &&;

  [[nodiscard]] std::string ParseConfig(const base::Value::Dict&,
                                        AttributionConfig&,
                                        bool required) &&;

 private:
  bool has_error() const { return error_manager_.has_error(); }

  [[nodiscard]] std::unique_ptr<AttributionParserErrorManager::ScopedContext>
  PushContext(AttributionParserErrorManager::Context context);

  AttributionParserErrorManager::ErrorWriter Error();

  void MoveDictValues(base::Value::Dict& in, base::Value::Dict& out);

  void MoveValue(base::Value::Dict& in,
                 base::StringPiece in_key,
                 base::Value::Dict& out,
                 absl::optional<base::StringPiece> out_key_opt = absl::nullopt);

  bool EnsureDictionary(const base::Value* value);

  absl::optional<std::string> ExtractString(base::Value::Dict& dict,
                                            base::StringPiece key);

  void ParseList(base::Value* values,
                 base::FunctionRef<void(base::Value)> callback,
                 size_t expected_size = 0);

  // Returns `attribution_src_url` in the request if exists.
  absl::optional<std::string> ParseRequest(base::Value::Dict& in,
                                           base::Value::Dict& out);

  void ParseResponse(base::Value::Dict& in,
                     base::Value::Dict& out,
                     const std::string& attribution_src_url);

  base::Value::List ParseEvents(base::Value::Dict& dict, base::StringPiece key);

  base::Value::List ParseEventLevelReports(base::Value::Dict& output);

  base::Value::List ParseAggregatableReports(base::Value::Dict& output);

  base::Value::List ParseVerboseDebugReports(base::Value::Dict& output);

  // Returns true if `key` is present in `dict` and the integer is parsed
  // successfully.
  template <typename T>
  bool ParseInteger(const base::Value::Dict& dict,
                    base::StringPiece key,
                    T& result,
                    bool (*convert_func)(base::StringPiece, T*),
                    bool required,
                    bool allow_zero) {
    auto context = PushContext(key);

    const base::Value* value = dict.Find(key);
    if (value) {
      const std::string* s = value->GetIfString();
      if (s && convert_func(*s, &result) &&
          (result > 0 || (result == 0 && allow_zero))) {
        return true;
      }
    } else if (!required) {
      return false;
    }

    if (allow_zero) {
      *Error() << "must be a non-negative integer formatted as base-10 string";
    } else {
      *Error() << "must be a positive integer formatted as base-10 string";
    }

    return false;
  }

  bool ParseInt(const base::Value::Dict& dict,
                base::StringPiece key,
                int& result,
                bool required,
                bool allow_zero = false) {
    return ParseInteger(dict, key, result, &base::StringToInt, required,
                        allow_zero);
  }

  bool ParseUint64(const base::Value::Dict& dict,
                   base::StringPiece key,
                   uint64_t& result,
                   bool required,
                   bool allow_zero = false) {
    return ParseInteger(dict, key, result, &base::StringToUint64, required,
                        allow_zero);
  }

  bool ParseInt64(const base::Value::Dict& dict,
                  base::StringPiece key,
                  int64_t& result,
                  bool required,
                  bool allow_zero = false) {
    return ParseInteger(dict, key, result, &base::StringToInt64, required,
                        allow_zero);
  }

  void ParseRandomizedResponseRate(const base::Value::Dict& dict,
                                   base::StringPiece key,
                                   double& result,
                                   bool required);

  AttributionParserErrorManager error_manager_;
};

std::unique_ptr<AttributionParserErrorManager::ScopedContext>
AttributionInteropParser::PushContext(
    AttributionParserErrorManager::Context context) {
  return error_manager_.PushContext(context);
}

AttributionParserErrorManager::ErrorWriter AttributionInteropParser::Error() {
  return error_manager_.Error();
}

void AttributionInteropParser::MoveDictValues(base::Value::Dict& in,
                                              base::Value::Dict& out) {
  for (auto [key, value] : in) {
    auto context = PushContext(key);
    if (out.contains(key)) {
      *Error() << "must not be present";
      return;
    }
    out.Set(key, std::move(value));
  }
}

void AttributionInteropParser::MoveValue(
    base::Value::Dict& in,
    base::StringPiece in_key,
    base::Value::Dict& out,
    absl::optional<base::StringPiece> out_key_opt) {
  auto context = PushContext(in_key);

  base::Value* value = in.Find(in_key);
  if (!value) {
    *Error() << "must be present";
    return;
  }

  base::StringPiece out_key = out_key_opt.value_or(in_key);
  DCHECK(!out.contains(out_key));
  out.Set(out_key, std::move(*value));
}

bool AttributionInteropParser::EnsureDictionary(const base::Value* value) {
  if (!value) {
    *Error() << "must be present";
    return false;
  }

  if (!value->is_dict()) {
    *Error() << "must be a dictionary";
    return false;
  }

  return true;
}

absl::optional<std::string> AttributionInteropParser::ExtractString(
    base::Value::Dict& dict,
    base::StringPiece key) {
  auto context = PushContext(key);

  absl::optional<base::Value> value = dict.Extract(key);
  if (!value) {
    *Error() << "must be present";
    return absl::nullopt;
  }

  if (std::string* str = value->GetIfString()) {
    return std::move(*str);
  }

  *Error() << "must be a string";
  return absl::nullopt;
}

void AttributionInteropParser::ParseList(
    base::Value* values,
    base::FunctionRef<void(base::Value)> parse_element,
    size_t expected_size) {
  if (!values) {
    *Error() << "must be present";
    return;
  }

  base::Value::List* list = values->GetIfList();
  if (!list) {
    *Error() << "must be a list";
    return;
  }

  if (expected_size > 0 && list->size() != expected_size) {
    *Error() << "must have size " << expected_size;
    return;
  }

  size_t index = 0;
  for (auto& value : values->GetList()) {
    auto context = PushContext(index);
    parse_element(std::move(value));
    index++;
  }
}

absl::optional<std::string> AttributionInteropParser::ParseRequest(
    base::Value::Dict& in,
    base::Value::Dict& out) {
  static constexpr char kKey[] = "registration_request";

  auto context = PushContext(kKey);

  base::Value* request = in.Find(kKey);
  if (!EnsureDictionary(request)) {
    return absl::nullopt;
  }

  absl::optional<std::string> str =
      ExtractString(request->GetDict(), "attribution_src_url");

  MoveDictValues(request->GetDict(), out);

  return str;
}

void AttributionInteropParser::ParseResponse(
    base::Value::Dict& in,
    base::Value::Dict& out,
    const std::string& attribution_src_url) {
  static constexpr char kKey[] = "responses";

  auto context = PushContext(kKey);

  ParseList(
      in.Find(kKey),
      [&](base::Value value) {
        if (!EnsureDictionary(&value)) {
          return;
        }

        static constexpr char kKeyUrl[] = "url";
        if (absl::optional<std::string> url =
                ExtractString(value.GetDict(), kKeyUrl);
            url && *url != attribution_src_url) {
          auto inner_context = PushContext(kKeyUrl);
          *Error() << "must match " << attribution_src_url;
        }

        static constexpr char kKeyDebugPermission[] = "debug_permission";
        if (value.GetDict().contains(kKeyDebugPermission)) {
          MoveValue(value.GetDict(), kKeyDebugPermission, out);
        }

        static constexpr char kKeyResponse[] = "response";
        auto inner_context = PushContext(kKeyResponse);
        base::Value* response = value.GetDict().Find(kKeyResponse);
        if (!EnsureDictionary(response)) {
          return;
        }

        MoveDictValues(response->GetDict(), out);
      },
      /*expected_size=*/1);
}

base::Value::List AttributionInteropParser::ParseEvents(base::Value::Dict& dict,
                                                        base::StringPiece key) {
  auto context = PushContext(key);

  base::Value::List results;

  ParseList(dict.Find(key),
            [&](base::Value value) {
              if (!EnsureDictionary(&value)) {
                return;
              }

              static constexpr char kKeyReportingOrigin[] = "reporting_origin";

              base::Value::Dict dict;
              MoveValue(value.GetDict(), "timestamp", dict);

              // Placeholder so that it errors out if request or response
              // contains this field.
              dict.Set(kKeyReportingOrigin, "");

              absl::optional<std::string> attribution_src_url =
                  ParseRequest(value.GetDict(), dict);

              if (has_error()) {
                return;
              }

              DCHECK(attribution_src_url);

              ParseResponse(value.GetDict(), dict, *attribution_src_url);

              if (has_error()) {
                return;
              }

              dict.Set(
                  kKeyReportingOrigin,
                  url::Origin::Create(GURL(std::move(*attribution_src_url)))
                      .Serialize());

              results.Append(std::move(dict));
            });

  return results;
}

base::expected<base::Value::Dict, std::string>
AttributionInteropParser::SimulatorInputFromInteropInput(
    base::Value::Dict input) && {
  static constexpr char kKey[] = "input";

  auto context = PushContext(kKey);

  base::Value* dict = input.Find(kKey);
  if (!EnsureDictionary(dict)) {
    return base::unexpected(std::move(error_manager_).TakeError());
  }

  base::Value::List sources = ParseEvents(dict->GetDict(), "sources");
  base::Value::List triggers = ParseEvents(dict->GetDict(), "triggers");

  if (has_error()) {
    return base::unexpected(std::move(error_manager_).TakeError());
  }

  base::Value::Dict result;
  result.Set("sources", std::move(sources));
  result.Set("triggers", std::move(triggers));
  return result;
}

base::Value::List AttributionInteropParser::ParseEventLevelReports(
    base::Value::Dict& output) {
  static constexpr char kKey[] = "event_level_reports";

  base::Value::List event_level_results;

  base::Value* value = output.Find(kKey);
  if (!value) {
    return event_level_results;
  }

  auto context = PushContext(kKey);
  ParseList(output.Find(kKey), [&](base::Value value) {
    if (!EnsureDictionary(&value)) {
      return;
    }

    base::Value::Dict result;

    base::Value::Dict& value_dict = value.GetDict();
    MoveValue(value_dict, "report", result, "payload");
    MoveValue(value_dict, "report_url", result);
    MoveValue(value_dict, "intended_report_time", result, "report_time");

    if (has_error()) {
      return;
    }

    event_level_results.Append(std::move(result));
  });

  return event_level_results;
}

base::Value::List AttributionInteropParser::ParseAggregatableReports(
    base::Value::Dict& output) {
  static constexpr char kKey[] = "aggregatable_reports";

  base::Value::List aggregatable_results;

  base::Value* value = output.Find(kKey);
  if (!value) {
    return aggregatable_results;
  }

  auto context = PushContext(kKey);
  ParseList(output.Find(kKey), [&](base::Value value) {
    if (!EnsureDictionary(&value)) {
      return;
    }

    base::Value::Dict result;

    base::Value::Dict& value_dict = value.GetDict();
    MoveValue(value_dict, "report_url", result);
    MoveValue(value_dict, "intended_report_time", result, "report_time");

    static constexpr char kKeyTestInfo[] = "test_info";
    base::Value* test_info;
    {
      auto test_info_context = PushContext(kKeyTestInfo);
      test_info = value_dict.Find(kKeyTestInfo);
      if (!EnsureDictionary(test_info)) {
        return;
      }
    }

    static constexpr char kKeyReport[] = "report";
    {
      auto report_context = PushContext(kKeyReport);
      base::Value* report = value_dict.Find(kKeyReport);
      if (!EnsureDictionary(report)) {
        return;
      }

      MoveDictValues(test_info->GetDict(), report->GetDict());
    }

    MoveValue(value_dict, "report", result, "payload");

    if (has_error()) {
      return;
    }

    aggregatable_results.Append(std::move(result));
  });

  return aggregatable_results;
}

base::Value::List AttributionInteropParser::ParseVerboseDebugReports(
    base::Value::Dict& output) {
  static constexpr char kKey[] = "verbose_debug_reports";

  base::Value::List reports;

  base::Value* value = output.Find(kKey);
  if (!value) {
    return reports;
  }

  auto context = PushContext(kKey);
  ParseList(output.Find(kKey), [&](base::Value value) {
    if (!EnsureDictionary(&value)) {
      return;
    }

    base::Value::Dict report;

    base::Value::Dict& value_dict = value.GetDict();
    MoveValue(value_dict, "report", report, "payload");
    MoveValue(value_dict, "report_url", report);
    MoveValue(value_dict, "report_time", report);

    if (has_error()) {
      return;
    }

    reports.Append(std::move(report));
  });

  return reports;
}

base::expected<base::Value::Dict, std::string>
AttributionInteropParser::InteropOutputFromSimulatorOutput(
    base::Value::Dict output) && {
  base::Value::List event_level_results = ParseEventLevelReports(output);

  base::Value::List aggregatable_results = ParseAggregatableReports(output);

  base::Value::List verbose_debug_reports = ParseVerboseDebugReports(output);

  if (has_error()) {
    return base::unexpected(std::move(error_manager_).TakeError());
  }

  base::Value::Dict dict;
  if (!event_level_results.empty()) {
    dict.Set("event_level_results", std::move(event_level_results));
  }

  if (!aggregatable_results.empty()) {
    dict.Set("aggregatable_results", std::move(aggregatable_results));
  }

  if (!verbose_debug_reports.empty()) {
    dict.Set("verbose_debug_reports", std::move(verbose_debug_reports));
  }

  return dict;
}

void AttributionInteropParser::ParseRandomizedResponseRate(
    const base::Value::Dict& dict,
    base::StringPiece key,
    double& result,
    bool required) {
  auto context = PushContext(key);

  const base::Value* value = dict.Find(key);

  if (value) {
    absl::optional<double> d = value->GetIfDouble();
    if (d && *d >= 0 && *d <= 1) {
      result = *d;
      return;
    }
  } else if (!required) {
    return;
  }

  *Error() << "must be a double between 0 and 1 formatted as string";
}

std::string AttributionInteropParser::ParseConfig(const base::Value::Dict& dict,
                                                  AttributionConfig& config,
                                                  bool required) && {
  ParseInt(dict, "max_sources_per_origin", config.max_sources_per_origin,
           required);

  ParseInt(dict, "max_destinations_per_source_site_reporting_origin",
           config.max_destinations_per_source_site_reporting_origin, required);

  uint64_t source_event_id_cardinality;
  if (ParseUint64(dict, "source_event_id_cardinality",
                  source_event_id_cardinality, required,
                  /*allow_zero=*/true)) {
    if (source_event_id_cardinality == 0u) {
      config.source_event_id_cardinality = absl::nullopt;
    } else {
      config.source_event_id_cardinality = source_event_id_cardinality;
    }
  }

  int rate_limit_time_window;
  if (ParseInt(dict, "rate_limit_time_window", rate_limit_time_window,
               required)) {
    config.rate_limit.time_window = base::Days(rate_limit_time_window);
  }

  ParseInt64(dict, "rate_limit_max_source_registration_reporting_origins",
             config.rate_limit.max_source_registration_reporting_origins,
             required);
  ParseInt64(dict, "rate_limit_max_attribution_reporting_origins",
             config.rate_limit.max_attribution_reporting_origins, required);
  ParseInt64(dict, "rate_limit_max_attributions",
             config.rate_limit.max_attributions, required);

  ParseInt(dict, "max_event_level_reports_per_destination",
           config.event_level_limit.max_reports_per_destination, required);
  ParseInt(dict, "max_attributions_per_navigation_source",
           config.event_level_limit.max_attributions_per_navigation_source,
           required);
  ParseInt(dict, "max_attributions_per_event_source",
           config.event_level_limit.max_attributions_per_event_source,
           required);
  ParseUint64(
      dict, "navigation_source_trigger_data_cardinality",
      config.event_level_limit.navigation_source_trigger_data_cardinality,
      required);
  ParseUint64(dict, "event_source_trigger_data_cardinality",
              config.event_level_limit.event_source_trigger_data_cardinality,
              required);
  ParseRandomizedResponseRate(
      dict, "navigation_source_randomized_response_rate",
      config.event_level_limit.navigation_source_randomized_response_rate,
      required);
  ParseRandomizedResponseRate(
      dict, "event_source_randomized_response_rate",
      config.event_level_limit.event_source_randomized_response_rate, required);

  ParseInt(dict, "max_aggregatable_reports_per_destination",
           config.aggregate_limit.max_reports_per_destination, required);
  ParseInt64(dict, "aggregatable_budget_per_source",
             config.aggregate_limit.aggregatable_budget_per_source, required);

  int aggregatable_report_min_delay;
  if (ParseInt(dict, "aggregatable_report_min_delay",
               aggregatable_report_min_delay, required,
               /*allow_zero=*/true)) {
    config.aggregate_limit.min_delay =
        base::Minutes(aggregatable_report_min_delay);
  }

  int aggregatable_report_delay_span;
  if (ParseInt(dict, "aggregatable_report_delay_span",
               aggregatable_report_delay_span, required,
               /*allow_zero=*/true)) {
    config.aggregate_limit.delay_span =
        base::Minutes(aggregatable_report_delay_span);
  }

  return std::move(error_manager_).TakeError();
}

}  // namespace

base::expected<base::Value::Dict, std::string>
AttributionSimulatorInputFromInteropInput(base::Value::Dict input) {
  return AttributionInteropParser().SimulatorInputFromInteropInput(
      std::move(input));
}

base::expected<base::Value::Dict, std::string>
AttributionInteropOutputFromSimulatorOutput(base::Value::Dict output) {
  return AttributionInteropParser().InteropOutputFromSimulatorOutput(
      std::move(output));
}

base::expected<AttributionConfig, std::string> ParseAttributionConfig(
    const base::Value::Dict& dict) {
  AttributionConfig config;
  std::string error =
      AttributionInteropParser().ParseConfig(dict, config, /*required=*/true);
  if (!error.empty()) {
    return base::unexpected(std::move(error));
  }
  return config;
}

std::string MergeAttributionConfig(const base::Value::Dict& dict,
                                   AttributionConfig& config) {
  return AttributionInteropParser().ParseConfig(dict, config,
                                                /*required=*/false);
}

}  // namespace content
