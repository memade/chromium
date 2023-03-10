#!/usr/bin/env python
# Copyright 2022 The Chromium Authors
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
"""Detect flakiness in the Skia Gold based pixel tests.

This script runs the specified Skia Gold pixel tests multiple times and compares
screenshots generated by test runs. The flakiness is detected if pixel test
code generates different screenshots in different iterations.

Because screenshots are compared through MD5, this script should only check
the pixel tests that use precise matching.

This script only checks whether the screenshots under the same names change in
different iterations. This script does NOT check whether screenshots are
expected. Therefore, please ensure the screenshots are correct before running
this script.

During execution, this script creates directories for temporary data. Those
directories' names contain special characters to ensure uniqueness. This
script guarantees to delete those directories at the end of execution.
Users can control the paths of those temporary directories via the option
--root_dir.

* Example usages:

./tools/pixel_test/check_pixel_test_flakiness.py --gtest_filter=\
DemoAshPixelDiffTest.VerifyTopLevelWidgets --test_target=out/debug\
/ash_pixeltests --output_dir=../var

The command above should be executed at the chromium source directory whose
absolute file path looks like .../chromium/src. This command checks
DemoAshPixelDiffTest.VerifyTopLevelWidgets by running ash_pixeltests under the
directory .../chromium/src/out/debug. If flakiness is detected, the flaky test's
screenshots are saved under .../chromium/var. If the directory specified by
--output_dir does not exist and meanwhile the flakiness is detected, the script
will create one. If the --output_dir is not specified, the flaky test's
screenshots are not saved.

./tools/pixel_test/check_pixel_test_flakiness.py --gtest_filter=\
DemoAshPixelDiffTest.VerifyTopLevelWidgets --test_target=out/debug/\
ash_pixeltests --root_dir=../.. --output_dir=var

The command above is similar to the previous one. But difference is that this
command uses the option --root_dir to designate the root path for outputs
(including the temporary data and the saved screenshots when flakiness is
detected). In this example, the absolute path of the output directory is
.../chromium/../var rather than .../chromium/var.

* options:

--test_target: it specifies the path to the executable file of pixel tests. It
is a relative file path from the current working directory. The test target can
be any test executable based on Skia Gold.

--root_dir: it specifies the root path for outputs (including the temporary data
 and the saved screenshots when flakiness is detected). It is a relative file
 path from the current working directory.

--log_mode: its value can only be 'none', 'error_only' and 'all'. 'none' means
that the log generated by gTest runs does not show; 'error_only' means that
only error messages from gTest runs are printed; 'all' shows all logs.
'none' is used by default.

--gtest_repeat: it specifies the count of repeated runs. Use ten by default.
"""


import argparse
import hashlib
import os
import shutil
import subprocess

# Constants used for color print.
_OK_GREEN = '\033[92m'
_FAIL_RED = '\033[91m'
_ENDC = '\033[0m'

# Used by the directory to host screenshots generated in each iteration. Add
# some special characters to make this name unique.
_TEMP_DIRECTORY_NAME_BASE = '@@check_pixel_test_flakiness!#'


def _get_md5(absolute_file_path):
    """Returns the Md5 digest of the specified file."""
    with open(absolute_file_path, 'rb') as target_file:
        return hashlib.md5(target_file.read()).hexdigest()


def _compare_with_last_iteration(screenshots, prev_temp_dir, temp_dir,
                                 names_hash_mappings, flaky_screenshot_dir):
    """Compares the screenshots generated in the current iteration with those
        from the previous iteration. If flakiness is detected, returns a flaky
        screenshot's name. Otherwise, returns an empty string.

    Args:
      screenshots: A list of screenshot names.
      prev_temp_dir: The absolute file path to the directory that hosts the
        screenshots generated by the previous iteration.
      temp_dir: The absolute file path to the directory that hosts the
        screenshots generated by the current iteration.
      names_hash_mappings: The mappings from screenshot names to hash code.
      flaky_screenshot_dir: The absolute file path to the directory used to host
        flaky screenshots. If it is null, flaky screenshots are not written into
        files.

    Returns:
      A string that indicates the name of the flaky screenshot. If no flakiness
      is detected, the return string is empty.
    """

    for screenshot in screenshots:
        screenshot_path = os.path.join(temp_dir, screenshot)

        # The screenshot hash code does not change so no flakiness is detected
        # on `screenshot`.
        if names_hash_mappings[screenshot] == _get_md5(screenshot_path):
            continue

        # Return if writing flaky screenshots to files is not required.
        if flaky_screenshot_dir is None:
            return screenshot

        # Delete the output directory if it already exists.
        if os.path.exists(flaky_screenshot_dir):
            shutil.rmtree(flaky_screenshot_dir)

        os.mkdir(flaky_screenshot_dir)
        split_image_name = os.path.splitext(screenshot)

        # Move the screenshot generated by the last iteration to the dest
        # directory.
        shutil.move(
            os.path.join(prev_temp_dir, screenshot),
            os.path.join(
                flaky_screenshot_dir,
                split_image_name[0] + '_Version_1' + split_image_name[1]))

        # Move the screenshot generated by the current iteration to the dest
        # directory.
        shutil.move(
            screenshot_path,
            os.path.join(
                flaky_screenshot_dir,
                split_image_name[0] + '_Version_2' + split_image_name[1]))

        return screenshot

    # No flakiness detected.
    return ''


def _analyze_screenshots(iteration_index, prev_temp_dir, temp_dir,
                         names_hash_mappings, flaky_screenshot_dir):
    """Analyzes the screenshots generated by one iteration.

    Args:
      iteration_index: An integer that indicates the iteration index.
      prev_temp_dir: The absolute file path to the directory that hosts the
        screenshots generated by the previous iteration.
      temp_dir: The absolute file path to the directory that hosts the
        screenshots generated by the current iteration.
      names_hash_mappings: The mappings from screenshot names to hash code.
      flaky_screenshot_dir: The absolute file path to the directory used to host
        flaky screenshots. If it is null, flaky screenshots are not written into
        files.

    Returns: A boolean value that indicates the execution result. True if
      flakiness is detected or the specified pixel tests do not generate any
      screenshot.
    """
    screenshots = os.listdir(temp_dir)

    # For the first iteration, nothing to compare with. Therefore, fill
    # `previous_temp_data_dir` and `names_hash_mappings`.
    if iteration_index == 0:
        if not screenshots:
            print(_FAIL_RED + '[Failure]' + _ENDC + ' no screenshots are '
                  'generated in the specified tests: are you using the '
                  'correct test filter?')
            return True

        for screenshot in screenshots:
            screenshot_absolute_path = os.path.join(temp_dir, screenshot)
            names_hash_mappings[screenshot] = _get_md5(
                screenshot_absolute_path)
        print(_OK_GREEN + '[OK]' + _ENDC + ' the iteration ' +
              str(iteration_index) + ' succeeds')
        return False

    flaky_image_name = _compare_with_last_iteration(screenshots, prev_temp_dir,
                                                    temp_dir, names_hash_mappings,
                                                    flaky_screenshot_dir)

    if len(flaky_image_name) > 0:
        print(_FAIL_RED + '[Failure]' + _ENDC + ' Detect flakiness in: ' +
              flaky_image_name)
        return True

    print(_OK_GREEN + '[OK]' + _ENDC + ' the iteration ' +
          str(iteration_index) + ' succeeds')

    return False


def main():
    parser = argparse.ArgumentParser(
        description='Detect flakiness in the Skia Gold based pixel tests by '
        'running the specified pixel test executable file multiple iterations '
        'and comparing screenshots generated by neighboring iterations through '
        'file hash code. Warning: this script can only be used to detect '
        'flakiness in the pixel tests that use precise comparison.')
    parser.add_argument('--test_target', type=str, required=True, help='a '
                        'relative file path from the current working directory '
                        'to the test executable based on Skia Gold, such as '
                        'ash_pixeltests')
    parser.add_argument('--gtest_repeat', type=int, default=10, help='the count'
                        ' of the repeated runs. The default value is ten.')
    parser.add_argument('--root_dir', type=str, default='', help='a relative '
                        'file path from the current working directory to the '
                        'root directory that hosts output data including the '
                        'screenshots generated in each iteration and the '
                        'detected flaky screenshots')
    parser.add_argument('--output_dir', type=str, help='a relative path'
                        ' starting from the output root path specified by'
                        ' --root_dir or the current working directory if'
                        ' --root_dir is omitted. It specifies a directory used'
                        ' to host the flaky screenshots if any.')
    parser.add_argument('--log_mode',
                        choices=['none', 'error_only', 'all'],
                        default='none', help='the option to control the log '
                        'output during test runs. `none` means that the log '
                        'generated by test runs does not show; `error_only` '
                        'means that only error logs are printed; `all` shows '
                        'all logs. `none` is used by default.')
    [known_args, unknown_args] = parser.parse_known_args()

    # Calculate the absolute path to the pixel test executable file.
    cwd = os.getcwd()
    executable_full_path = os.path.join(cwd, known_args.test_target)

    # Calculate the absolute path to the directory that hosts output data.
    output_root_path = os.path.join(cwd, known_args.root_dir)

    # Skip the Skia Gold functionality. Because this script compares images
    # through hash code.
    pixel_test_command_base = [
        executable_full_path, '--bypass-skia-gold-functionality'
    ]

    # Pass unknown args to gtest.
    if unknown_args:
        pixel_test_command_base += unknown_args

    # Print the command to run pixel tests.
    print(_OK_GREEN + '[Begin] ' + _ENDC + ' '.join(pixel_test_command_base))

    # Configure log output.
    std_out_mode = subprocess.DEVNULL
    if known_args.log_mode == 'all':
        std_out_mode = None
    std_err_mode = None
    if known_args.log_mode == 'none':
        std_err_mode = subprocess.DEVNULL

    # Cache the screenshot host directory used in the last iteration. It updates
    # at the end of each iteration.
    prev_temp_dir = ''

    # Similar to `prev_temp_dir` but it caches data for the active
    # iteration.
    temp_dir = ''

    # Mappings screenshot names to hash code.
    names_hash_mappings = {}

    # Calculate the directory path for saving flaky screenshots.
    flaky_screenshot_dir = None
    if known_args.output_dir is not None:
        flaky_screenshot_dir = os.path.join(
            output_root_path, known_args.output_dir)

    try:
        for iteration_index in range(known_args.gtest_repeat):
            # Calculate the absolute path to the screenshot host directory used for
            # this iteration. Recreate the host directory if it already exists.
            temp_dir = os.path.join(
                output_root_path, _TEMP_DIRECTORY_NAME_BASE + str(iteration_index))

            # Ensure `temp_dir` is an absolute path. Otherwise, screenshots
            # generated during test runs will fail to be written into files.
            temp_dir = os.path.abspath(temp_dir)

            if os.path.exists(temp_dir):
                shutil.rmtree(temp_dir)
            os.mkdir(temp_dir)

            # Append the option so that the screenshots generated in pixel tests are
            # written into `temp_dir`.
            pixel_test_command = pixel_test_command_base[:]
            pixel_test_command.append('--skia-gold-local-png-write-directory=' +
                                      temp_dir)

            # Run pixel tests.
            subprocess.run(pixel_test_command,
                           stdout=std_out_mode,
                           stderr=std_err_mode,
                           check=True)

            result = _analyze_screenshots(
                iteration_index, prev_temp_dir, temp_dir, names_hash_mappings,
                flaky_screenshot_dir)

            # Get an error so exit the loop.
            if result:
                shutil.rmtree(temp_dir)
                break

            # Delete the temporary data directory used by the previous loop
            # iteration before overwriting it.
            if iteration_index > 0:
                shutil.rmtree(prev_temp_dir)

            prev_temp_dir = temp_dir

            # All iterations end. Print the success message.
            if iteration_index == known_args.gtest_repeat - 1:
                print(_OK_GREEN + '[Success]' + _ENDC +
                      ' no flakiness is detected')

    finally:
        # ensure that temp data are removed.
        if os.path.isdir(prev_temp_dir):
            shutil.rmtree(prev_temp_dir)


if __name__ == '__main__':
    main()
