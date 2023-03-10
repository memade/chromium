#include "third_party/webrtc_overrides/p2p/base/ice_switch_proposal.h"

#include <algorithm>
#include <iterator>

#include "base/notreached.h"

#include "third_party/webrtc/p2p/base/ice_controller_interface.h"
#include "third_party/webrtc/p2p/base/ice_switch_reason.h"
#include "third_party/webrtc/rtc_base/strings/string_builder.h"
#include "third_party/webrtc_overrides/p2p/base/ice_connection.h"
#include "third_party/webrtc_overrides/p2p/base/ice_proposal.h"

namespace blink {

IceSwitchReason ConvertFromWebrtcIceSwitchReason(
    cricket::IceSwitchReason reason) {
  switch (reason) {
    case cricket::IceSwitchReason::REMOTE_CANDIDATE_GENERATION_CHANGE:
      return IceSwitchReason::kRemoteCandidateGenerationChange;
    case cricket::IceSwitchReason::NETWORK_PREFERENCE_CHANGE:
      return IceSwitchReason::kNetworkPreferenceChange;
    case cricket::IceSwitchReason::NEW_CONNECTION_FROM_LOCAL_CANDIDATE:
      return IceSwitchReason::kNewConnectionFromLocalCandidate;
    case cricket::IceSwitchReason::NEW_CONNECTION_FROM_REMOTE_CANDIDATE:
      return IceSwitchReason::kNewConnectionFromRemoteCandidate;
    case cricket::IceSwitchReason::NEW_CONNECTION_FROM_UNKNOWN_REMOTE_ADDRESS:
      return IceSwitchReason::kNewConnectionFromUnknownRemoteAddress;
    case cricket::IceSwitchReason::NOMINATION_ON_CONTROLLED_SIDE:
      return IceSwitchReason::kNominationOnControlledSide;
    case cricket::IceSwitchReason::DATA_RECEIVED:
      return IceSwitchReason::kDataReceived;
    case cricket::IceSwitchReason::CONNECT_STATE_CHANGE:
      return IceSwitchReason::kConnectStateChange;
    case cricket::IceSwitchReason::SELECTED_CONNECTION_DESTROYED:
      return IceSwitchReason::kSelectedConnectionDestroyed;
    case cricket::IceSwitchReason::ICE_CONTROLLER_RECHECK:
      return IceSwitchReason::kIceControllerRecheck;
    default:
      NOTREACHED();
      return IceSwitchReason::kUnknown;
  }
}

std::string IceSwitchReasonToString(IceSwitchReason reason) {
  switch (reason) {
    case IceSwitchReason::kUnknown:
      return "Unknown";
    case IceSwitchReason::kRemoteCandidateGenerationChange:
      return "RemoteCandidateGenerationChange";
    case IceSwitchReason::kNetworkPreferenceChange:
      return "NetworkPreferenceChange";
    case IceSwitchReason::kNewConnectionFromLocalCandidate:
      return "NewConnectionFromLocalCandidate";
    case IceSwitchReason::kNewConnectionFromRemoteCandidate:
      return "NewConnectionFromRemoteCandidate";
    case IceSwitchReason::kNewConnectionFromUnknownRemoteAddress:
      return "NewConnectionFromUnknownRemoteAddress";
    case IceSwitchReason::kNominationOnControlledSide:
      return "NominationOnControlledSide";
    case IceSwitchReason::kDataReceived:
      return "DataReceived";
    case IceSwitchReason::kConnectStateChange:
      return "ConnectStateChange";
    case IceSwitchReason::kSelectedConnectionDestroyed:
      return "SelectedConnectionDestroyed";
    case IceSwitchReason::kIceControllerRecheck:
      return "IceControllerRecheck";
    default:
      NOTREACHED();
  }
}

IceRecheckEvent::IceRecheckEvent(const cricket::IceRecheckEvent& event)
    : reason(ConvertFromWebrtcIceSwitchReason(event.reason)),
      recheck_delay_ms(event.recheck_delay_ms) {}

IceSwitchProposal::IceSwitchProposal(
    cricket::IceSwitchReason reason,
    const cricket::IceControllerInterface::SwitchResult& switch_result,
    bool reply_expected)
    : IceProposal(reply_expected),
      reason_(ConvertFromWebrtcIceSwitchReason(reason)),
      connection_(switch_result.connection),
      recheck_event_(switch_result.recheck_event) {
  std::transform(
      switch_result.connections_to_forget_state_on.cbegin(),
      switch_result.connections_to_forget_state_on.cend(),
      std::back_inserter(connections_to_forget_state_on_),
      [](const cricket::Connection* conn) { return IceConnection(conn); });
}

std::string IceSwitchProposal::ToString() const {
  rtc::StringBuilder ss;
  ss << "SwitchProposal[" << IceSwitchReasonToString(reason_) << ":"
     << (connection_.has_value() ? connection_->ToString() : "<no connection>")
     << ":";
  if (recheck_event_.has_value()) {
    ss << IceSwitchReasonToString(recheck_event_->reason) << ":"
       << recheck_event_->recheck_delay_ms;
  } else {
    ss << "<no recheck>";
  }
  ss << ":";
  int ctr = 1;
  for (auto conn : connections_to_forget_state_on_) {
    ss << "(" << ctr++ << ":" << conn.ToString() << ")";
  }
  ss << "]";
  return ss.Release();
}

}  // namespace blink
