/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "GetTrainScheduleAgent.hpp"
#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_template_search.hpp>

using namespace std;

ScAddr GetTrainScheduleAgent::GetActionClass() const
{
  return Keynodes::action_get_train_schedule;
}

ScResult GetTrainScheduleAgent::DoProgram(ScAction & action)
{
  auto const & [trainAddr] = action.GetArguments<1>();
  if (!trainAddr.IsValid())
    return action.FinishWithError();

  ScTemplate searchTemplate;
  searchTemplate(
    ScTemplate::Triple(
      trainAddr,
      Keynodes::nrel_schedule,
      ScType::NodeConst
    )
  );
  ScTemplateSearchResult result;
  if (!m_context.HelperSearchTemplate(searchTemplate, result) || result.Size() == 0)
    return action.FinishWithError();
  ScAddr scheduleAddr = result[0][2];

  ScStructure answer = m_context.GenerateStructure();
  answer << trainAddr << Keynodes::nrel_schedule << scheduleAddr;
  action.SetResult(answer);
  return action.FinishSuccessfully();
}
