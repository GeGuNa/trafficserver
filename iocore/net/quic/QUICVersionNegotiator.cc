/** @file
 *
 *  A brief file description
 *
 *  @section license License
 *
 *  Licensed to the Apache Software Foundation (ASF) under one
 *  or more contributor license agreements.  See the NOTICE file
 *  distributed with this work for additional information
 *  regarding copyright ownership.  The ASF licenses this file
 *  to you under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in compliance
 *  with the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "QUICVersionNegotiator.h"
#include "QUICTransportParameters.h"

QUICVersionNegotiationStatus
QUICVersionNegotiator::status() const
{
  return this->_status;
}

QUICVersionNegotiationStatus
QUICVersionNegotiator::negotiate(const QUICPacket &packet)
{
  switch (packet.type()) {
  case QUICPacketType::INITIAL: {
    const QUICInitialPacketR &initial_packet = static_cast<const QUICInitialPacketR &>(packet);
    if (QUICTypeUtil::is_supported_version(initial_packet.version())) {
      this->_status             = QUICVersionNegotiationStatus::NEGOTIATED;
      this->_negotiated_version = initial_packet.version();
    }

    break;
  }
  case QUICPacketType::VERSION_NEGOTIATION: {
    const QUICVersionNegotiationPacketR &vn_packet = static_cast<const QUICVersionNegotiationPacketR &>(packet);
    uint16_t n_supported_version                   = vn_packet.nversions();

    for (int i = 0; i < n_supported_version; ++i) {
      QUICVersion version = vn_packet.supported_version(i);

      if (QUICTypeUtil::is_supported_version(version)) {
        this->_status             = QUICVersionNegotiationStatus::NEGOTIATED;
        this->_negotiated_version = version;
        break;
      }
    }

    break;
  }
  default:
    ink_assert(false);
    break;
  }

  return this->_status;
}

QUICVersionNegotiationStatus
QUICVersionNegotiator::validate() const
{
  return this->_status;
}

QUICVersion
QUICVersionNegotiator::negotiated_version() const
{
  return this->_negotiated_version;
}
