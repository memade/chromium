# This file is auto-generated by the Perl DateTime Suite time zone
# code generator (0.08) This code generator comes with the
# DateTime::TimeZone module distribution in the tools/ directory

#
# Generated from /tmp/4pktg_OhFc/australasia.  Olson data version 2019a
#
# Do not edit this file directly.
#
package DateTime::TimeZone::Pacific::Majuro;

use strict;
use warnings;
use namespace::autoclean;

our $VERSION = '2.35';

use Class::Singleton 1.03;
use DateTime::TimeZone;
use DateTime::TimeZone::OlsonDB;

@DateTime::TimeZone::Pacific::Majuro::ISA = ( 'Class::Singleton', 'DateTime::TimeZone' );

my $spans =
[
    [
DateTime::TimeZone::NEG_INFINITY, #    utc_start
59958189312, #      utc_end 1900-12-31 12:35:12 (Mon)
DateTime::TimeZone::NEG_INFINITY, #  local_start
59958230400, #    local_end 1901-01-01 00:00:00 (Tue)
41088,
0,
'LMT',
    ],
    [
59958189312, #    utc_start 1900-12-31 12:35:12 (Mon)
60392005200, #      utc_end 1914-09-30 13:00:00 (Wed)
59958228912, #  local_start 1900-12-31 23:35:12 (Mon)
60392044800, #    local_end 1914-10-01 00:00:00 (Thu)
39600,
0,
'+11',
    ],
    [
60392005200, #    utc_start 1914-09-30 13:00:00 (Wed)
60528870000, #      utc_end 1919-01-31 15:00:00 (Fri)
60392037600, #  local_start 1914-09-30 22:00:00 (Wed)
60528902400, #    local_end 1919-02-01 00:00:00 (Sat)
32400,
0,
'+09',
    ],
    [
60528870000, #    utc_start 1919-01-31 15:00:00 (Fri)
61094264400, #      utc_end 1936-12-31 13:00:00 (Thu)
60528909600, #  local_start 1919-02-01 02:00:00 (Sat)
61094304000, #    local_end 1937-01-01 00:00:00 (Fri)
39600,
0,
'+11',
    ],
    [
61094264400, #    utc_start 1936-12-31 13:00:00 (Thu)
61228274400, #      utc_end 1941-03-31 14:00:00 (Mon)
61094300400, #  local_start 1936-12-31 23:00:00 (Thu)
61228310400, #    local_end 1941-04-01 00:00:00 (Tue)
36000,
0,
'+10',
    ],
    [
61228274400, #    utc_start 1941-03-31 14:00:00 (Mon)
61317615600, #      utc_end 1944-01-29 15:00:00 (Sat)
61228306800, #  local_start 1941-03-31 23:00:00 (Mon)
61317648000, #    local_end 1944-01-30 00:00:00 (Sun)
32400,
0,
'+09',
    ],
    [
61317615600, #    utc_start 1944-01-29 15:00:00 (Sat)
62127694800, #      utc_end 1969-09-30 13:00:00 (Tue)
61317655200, #  local_start 1944-01-30 02:00:00 (Sun)
62127734400, #    local_end 1969-10-01 00:00:00 (Wed)
39600,
0,
'+11',
    ],
    [
62127694800, #    utc_start 1969-09-30 13:00:00 (Tue)
DateTime::TimeZone::INFINITY, #      utc_end
62127738000, #  local_start 1969-10-01 01:00:00 (Wed)
DateTime::TimeZone::INFINITY, #    local_end
43200,
0,
'+12',
    ],
];

sub olson_version {'2019a'}

sub has_dst_changes {0}

sub _max_year {2029}

sub _new_instance {
    return shift->_init( @_, spans => $spans );
}



1;

