# Copyright 2018 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================
# pylint: disable=line-too-long
"""Script for updating tensorflow/tools/compatibility/reorders_v2.py.

To update reorders_v2.py, run:
  bazel run tensorflow/tools/compatibility/update:generate_v2_reorders_map
"""
# pylint: enable=line-too-long

from absl import app
import tensorflow as tf

from tensorflow import python as tf_python  # pylint: disable=unused-import
from tensorflow.python.lib.io import file_io
from tensorflow.python.util import tf_decorator
from tensorflow.python.util import tf_export
from tensorflow.python.util import tf_inspect
from tensorflow.tools.common import public_api
from tensorflow.tools.common import traverse
from tensorflow.tools.compatibility import tf_upgrade_v2

# This import is needed so that TensorFlow python modules are in sys.modules.


_OUTPUT_FILE_PATH = 'third_party/tensorflow/tools/compatibility/reorders_v2.py'
_FILE_HEADER = """# Copyright 2018 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================
# pylint: disable=line-too-long
\"\"\"List of renames to apply when converting from TF 1.0 to TF 2.0.

THIS FILE IS AUTOGENERATED: To update, please run:
  bazel run tensorflow/tools/compatibility/update:generate_v2_reorders_map
This file should be updated whenever a function is added to
self.reordered_function_names in tf_upgrade_v2.py.
\"\"\"
"""


def collect_function_arg_names(function_names):
  """Determines argument names for reordered function signatures.

  Args:
    function_names: Functions to collect arguments for.

  Returns:
    Dictionary mapping function name to its arguments.
  """
  # Map from reordered function name to its arguments.
  function_to_args = {}

  def visit(unused_path, unused_parent, children):
    """Visitor that collects arguments for reordered functions."""
    for child in children:
      _, attr = tf_decorator.unwrap(child[1])
      api_names_v1 = tf_export.get_v1_names(attr)
      api_names_v1 = ['tf.%s' % name for name in api_names_v1]
      matches_function_names = any(
          name in function_names for name in api_names_v1)
      if matches_function_names:
        if tf_inspect.isclass(attr):
          # Get constructor arguments if attr is a class
          arg_list = tf_inspect.getargspec(
              getattr(attr, '__init__'))[0]
          arg_list = arg_list[1:]  # skip 'self' argument
        else:
          # Get function arguments.
          # getargspec returns a tuple of (args, varargs, keywords, defaults)
          # we just look at args.
          arg_list = tf_inspect.getargspec(attr)[0]
        for name in api_names_v1:
          function_to_args[name] = arg_list

  visitor = public_api.PublicAPIVisitor(visit)
  visitor.do_not_descend_map['tf'].append('contrib')
  visitor.private_map['tf.compat'] = ['v1', 'v2']
  traverse.traverse(tf.compat.v1, visitor)
  traverse.traverse(tf.compat.v1.estimator, visitor)

  return function_to_args


def get_reorder_line(name, arg_list):
  return '    \'%s\': %s' % (name, str(arg_list))


def update_reorders_v2(output_file_path):
  """Writes a Python dictionary mapping function name to argument order.

  Args:
    output_file_path: File path to write output to. Any existing contents
      would be replaced.
  """
  reordered_function_names = (
      tf_upgrade_v2.TFAPIChangeSpec().reordered_function_names)

  all_reorders = collect_function_arg_names(reordered_function_names)

  # List of reorder lines to write to output file in the form:
  #   'tf.function_name': ['arg1', 'arg2', ...]
  rename_lines = [
      get_reorder_line(name, arg_names)
      for name, arg_names in all_reorders.items()]
  renames_file_text = '%sreorders = {\n%s\n}\n' % (
      _FILE_HEADER, ',\n'.join(sorted(rename_lines)))
  file_io.write_string_to_file(output_file_path, renames_file_text)


def main(unused_argv):
  update_reorders_v2(_OUTPUT_FILE_PATH)


if __name__ == '__main__':
  app.run(main=main)
