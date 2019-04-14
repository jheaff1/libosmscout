/*
 This source is part of the libosmscout library
 Copyright (C) 2019  Lukas Karas

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include <osmscout/navigation/SpeedAgent.h>
#include <osmscout/FeatureReader.h>

namespace osmscout {

CurrentSpeedMessage::CurrentSpeedMessage(const Timestamp& timestamp,
                                         double speed):
  NavigationMessage(timestamp),
  speed(speed)
{}

MaxAllowedSpeedMessage::MaxAllowedSpeedMessage(const Timestamp& timestamp,
                                               double maxAllowedSpeed,
                                               bool defined):
  NavigationMessage(timestamp),
  maxAllowedSpeed(maxAllowedSpeed),
  defined(defined)
{
}

SpeedAgent::SpeedAgent()
{
}

std::list<NavigationMessageRef> SpeedAgent::Process(const NavigationMessageRef &message)
{
  std::list<NavigationMessageRef> result;
  auto positionMsg = dynamic_cast<osmscout::PositionAgent::PositionMessage *>(message.get());
  if (positionMsg==nullptr) {
    return result;
  }

  using namespace std::chrono;
  // compute actual speed
  if (positionMsg->position.state==PositionAgent::PositionState::OnRoute ||
      positionMsg->position.state==PositionAgent::PositionState::OffRoute){

    if (lastPosition){
      segmentFifo.push_back({GetEllipsoidalDistance(lastPosition.coord,positionMsg->position.coord),
                             positionMsg->timestamp-lastPosition.time});
      Timestamp::duration fifoDuration{Timestamp::duration::zero()};
      Distance fifoDistance;
      for (const auto &s:segmentFifo){
        fifoDuration+=s.duration;
        fifoDistance+=s.distance;
      }
      auto sec=duration_cast<duration<double>>(fifoDuration);
      if (sec.count()>0){
        double speed=(fifoDistance.AsMeter()/sec.count())*3.6;
        result.push_back(std::make_shared<CurrentSpeedMessage>(positionMsg->timestamp,speed));
      }
      // pop fifo
      while (!segmentFifo.empty() && fifoDuration>seconds(5)){
        fifoDuration-=segmentFifo.front().duration;
        segmentFifo.pop_front();
      }
    }
    lastPosition={positionMsg->position.coord, positionMsg->timestamp};
  }

  // obtain max speed
  MaxSpeedFeatureValue *maxSpeedValue=nullptr;
  if (positionMsg->position.typeConfig) {
    MaxSpeedFeatureValueReader reader(*(positionMsg->position.typeConfig));
    if (positionMsg->position.way) {
      maxSpeedValue = reader.GetValue(positionMsg->position.way->GetFeatureValueBuffer());
    }
    if (maxSpeedValue == nullptr && positionMsg->position.area) {
      maxSpeedValue = reader.GetValue(positionMsg->position.area->GetFeatureValueBuffer());
    }
  }
  double maxSpeed=-1;
  if (maxSpeedValue){
    maxSpeed=maxSpeedValue->GetMaxSpeed();
  }
  if (lastReportedMaxSpeed!=maxSpeed) {
    result.push_back(std::make_shared<MaxAllowedSpeedMessage>(positionMsg->timestamp,
                                                              maxSpeed, maxSpeed > 0));
    lastReportedMaxSpeed=maxSpeed;
  }

  return result;
}

}