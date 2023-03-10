#!/usr/bin/env python3
# Copyright (C) 2023 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License a
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from python.generators.diff_tests.testing import Path, DataPath, Metric
from python.generators.diff_tests.testing import Csv, Json, TextProto
from python.generators.diff_tests.testing import DiffTestBlueprint
from python.generators.diff_tests.testing import DiffTestModule


class CrosGeneral(DiffTestModule):

  def test_cros_ec_sensorhub_data(self):
    return DiffTestBlueprint(
        trace=Path('cros_ec_sensorhub_data.textproto'),
        query="""
SELECT
  t.name,
  c.ts,
  c.value,
  EXTRACT_ARG(c.arg_set_id, 'ec_num') AS ec_num,
  EXTRACT_ARG(c.arg_set_id, 'ec_delta') AS ec_delta,
  EXTRACT_ARG(c.arg_set_id, 'sample_ts') AS sample_ts
FROM counter c
JOIN track t
  ON c.track_id = t.id
WHERE t.name = 'cros_ec.cros_ec_sensorhub_data.0';
""",
        out=Path('cros_ec_sensorhub_data.out'))
