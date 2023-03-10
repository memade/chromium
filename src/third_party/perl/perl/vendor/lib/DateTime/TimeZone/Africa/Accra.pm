# This file is auto-generated by the Perl DateTime Suite time zone
# code generator (0.08) This code generator comes with the
# DateTime::TimeZone module distribution in the tools/ directory

#
# Generated from /tmp/4pktg_OhFc/africa.  Olson data version 2019a
#
# Do not edit this file directly.
#
package DateTime::TimeZone::Africa::Accra;

use strict;
use warnings;
use namespace::autoclean;

our $VERSION = '2.35';

use Class::Singleton 1.03;
use DateTime::TimeZone;
use DateTime::TimeZone::OlsonDB;

@DateTime::TimeZone::Africa::Accra::ISA = ( 'Class::Singleton', 'DateTime::TimeZone' );

my $spans =
[
    [
DateTime::TimeZone::NEG_INFINITY, #    utc_start
60494688052, #      utc_end 1918-01-01 00:00:52 (Tue)
DateTime::TimeZone::NEG_INFINITY, #  local_start
60494688000, #    local_end 1918-01-01 00:00:00 (Tue)
-52,
0,
'LMT',
    ],
    [
60494688052, #    utc_start 1918-01-01 00:00:52 (Tue)
60578841600, #      utc_end 1920-09-01 00:00:00 (Wed)
60494688052, #  local_start 1918-01-01 00:00:52 (Tue)
60578841600, #    local_end 1920-09-01 00:00:00 (Wed)
0,
0,
'GMT',
    ],
    [
60578841600, #    utc_start 1920-09-01 00:00:00 (Wed)
60589294800, #      utc_end 1920-12-30 23:40:00 (Thu)
60578842800, #  local_start 1920-09-01 00:20:00 (Wed)
60589296000, #    local_end 1920-12-31 00:00:00 (Fri)
1200,
1,
'+0020',
    ],
    [
60589294800, #    utc_start 1920-12-30 23:40:00 (Thu)
60610377600, #      utc_end 1921-09-01 00:00:00 (Thu)
60589294800, #  local_start 1920-12-30 23:40:00 (Thu)
60610377600, #    local_end 1921-09-01 00:00:00 (Thu)
0,
0,
'GMT',
    ],
    [
60610377600, #    utc_start 1921-09-01 00:00:00 (Thu)
60620830800, #      utc_end 1921-12-30 23:40:00 (Fri)
60610378800, #  local_start 1921-09-01 00:20:00 (Thu)
60620832000, #    local_end 1921-12-31 00:00:00 (Sat)
1200,
1,
'+0020',
    ],
    [
60620830800, #    utc_start 1921-12-30 23:40:00 (Fri)
60641913600, #      utc_end 1922-09-01 00:00:00 (Fri)
60620830800, #  local_start 1921-12-30 23:40:00 (Fri)
60641913600, #    local_end 1922-09-01 00:00:00 (Fri)
0,
0,
'GMT',
    ],
    [
60641913600, #    utc_start 1922-09-01 00:00:00 (Fri)
60652366800, #      utc_end 1922-12-30 23:40:00 (Sat)
60641914800, #  local_start 1922-09-01 00:20:00 (Fri)
60652368000, #    local_end 1922-12-31 00:00:00 (Sun)
1200,
1,
'+0020',
    ],
    [
60652366800, #    utc_start 1922-12-30 23:40:00 (Sat)
60673449600, #      utc_end 1923-09-01 00:00:00 (Sat)
60652366800, #  local_start 1922-12-30 23:40:00 (Sat)
60673449600, #    local_end 1923-09-01 00:00:00 (Sat)
0,
0,
'GMT',
    ],
    [
60673449600, #    utc_start 1923-09-01 00:00:00 (Sat)
60683902800, #      utc_end 1923-12-30 23:40:00 (Sun)
60673450800, #  local_start 1923-09-01 00:20:00 (Sat)
60683904000, #    local_end 1923-12-31 00:00:00 (Mon)
1200,
1,
'+0020',
    ],
    [
60683902800, #    utc_start 1923-12-30 23:40:00 (Sun)
60705072000, #      utc_end 1924-09-01 00:00:00 (Mon)
60683902800, #  local_start 1923-12-30 23:40:00 (Sun)
60705072000, #    local_end 1924-09-01 00:00:00 (Mon)
0,
0,
'GMT',
    ],
    [
60705072000, #    utc_start 1924-09-01 00:00:00 (Mon)
60715525200, #      utc_end 1924-12-30 23:40:00 (Tue)
60705073200, #  local_start 1924-09-01 00:20:00 (Mon)
60715526400, #    local_end 1924-12-31 00:00:00 (Wed)
1200,
1,
'+0020',
    ],
    [
60715525200, #    utc_start 1924-12-30 23:40:00 (Tue)
60736608000, #      utc_end 1925-09-01 00:00:00 (Tue)
60715525200, #  local_start 1924-12-30 23:40:00 (Tue)
60736608000, #    local_end 1925-09-01 00:00:00 (Tue)
0,
0,
'GMT',
    ],
    [
60736608000, #    utc_start 1925-09-01 00:00:00 (Tue)
60747061200, #      utc_end 1925-12-30 23:40:00 (Wed)
60736609200, #  local_start 1925-09-01 00:20:00 (Tue)
60747062400, #    local_end 1925-12-31 00:00:00 (Thu)
1200,
1,
'+0020',
    ],
    [
60747061200, #    utc_start 1925-12-30 23:40:00 (Wed)
60768144000, #      utc_end 1926-09-01 00:00:00 (Wed)
60747061200, #  local_start 1925-12-30 23:40:00 (Wed)
60768144000, #    local_end 1926-09-01 00:00:00 (Wed)
0,
0,
'GMT',
    ],
    [
60768144000, #    utc_start 1926-09-01 00:00:00 (Wed)
60778597200, #      utc_end 1926-12-30 23:40:00 (Thu)
60768145200, #  local_start 1926-09-01 00:20:00 (Wed)
60778598400, #    local_end 1926-12-31 00:00:00 (Fri)
1200,
1,
'+0020',
    ],
    [
60778597200, #    utc_start 1926-12-30 23:40:00 (Thu)
60799680000, #      utc_end 1927-09-01 00:00:00 (Thu)
60778597200, #  local_start 1926-12-30 23:40:00 (Thu)
60799680000, #    local_end 1927-09-01 00:00:00 (Thu)
0,
0,
'GMT',
    ],
    [
60799680000, #    utc_start 1927-09-01 00:00:00 (Thu)
60810133200, #      utc_end 1927-12-30 23:40:00 (Fri)
60799681200, #  local_start 1927-09-01 00:20:00 (Thu)
60810134400, #    local_end 1927-12-31 00:00:00 (Sat)
1200,
1,
'+0020',
    ],
    [
60810133200, #    utc_start 1927-12-30 23:40:00 (Fri)
60831302400, #      utc_end 1928-09-01 00:00:00 (Sat)
60810133200, #  local_start 1927-12-30 23:40:00 (Fri)
60831302400, #    local_end 1928-09-01 00:00:00 (Sat)
0,
0,
'GMT',
    ],
    [
60831302400, #    utc_start 1928-09-01 00:00:00 (Sat)
60841755600, #      utc_end 1928-12-30 23:40:00 (Sun)
60831303600, #  local_start 1928-09-01 00:20:00 (Sat)
60841756800, #    local_end 1928-12-31 00:00:00 (Mon)
1200,
1,
'+0020',
    ],
    [
60841755600, #    utc_start 1928-12-30 23:40:00 (Sun)
60862838400, #      utc_end 1929-09-01 00:00:00 (Sun)
60841755600, #  local_start 1928-12-30 23:40:00 (Sun)
60862838400, #    local_end 1929-09-01 00:00:00 (Sun)
0,
0,
'GMT',
    ],
    [
60862838400, #    utc_start 1929-09-01 00:00:00 (Sun)
60873291600, #      utc_end 1929-12-30 23:40:00 (Mon)
60862839600, #  local_start 1929-09-01 00:20:00 (Sun)
60873292800, #    local_end 1929-12-31 00:00:00 (Tue)
1200,
1,
'+0020',
    ],
    [
60873291600, #    utc_start 1929-12-30 23:40:00 (Mon)
60894374400, #      utc_end 1930-09-01 00:00:00 (Mon)
60873291600, #  local_start 1929-12-30 23:40:00 (Mon)
60894374400, #    local_end 1930-09-01 00:00:00 (Mon)
0,
0,
'GMT',
    ],
    [
60894374400, #    utc_start 1930-09-01 00:00:00 (Mon)
60904827600, #      utc_end 1930-12-30 23:40:00 (Tue)
60894375600, #  local_start 1930-09-01 00:20:00 (Mon)
60904828800, #    local_end 1930-12-31 00:00:00 (Wed)
1200,
1,
'+0020',
    ],
    [
60904827600, #    utc_start 1930-12-30 23:40:00 (Tue)
60925910400, #      utc_end 1931-09-01 00:00:00 (Tue)
60904827600, #  local_start 1930-12-30 23:40:00 (Tue)
60925910400, #    local_end 1931-09-01 00:00:00 (Tue)
0,
0,
'GMT',
    ],
    [
60925910400, #    utc_start 1931-09-01 00:00:00 (Tue)
60936363600, #      utc_end 1931-12-30 23:40:00 (Wed)
60925911600, #  local_start 1931-09-01 00:20:00 (Tue)
60936364800, #    local_end 1931-12-31 00:00:00 (Thu)
1200,
1,
'+0020',
    ],
    [
60936363600, #    utc_start 1931-12-30 23:40:00 (Wed)
60957532800, #      utc_end 1932-09-01 00:00:00 (Thu)
60936363600, #  local_start 1931-12-30 23:40:00 (Wed)
60957532800, #    local_end 1932-09-01 00:00:00 (Thu)
0,
0,
'GMT',
    ],
    [
60957532800, #    utc_start 1932-09-01 00:00:00 (Thu)
60967986000, #      utc_end 1932-12-30 23:40:00 (Fri)
60957534000, #  local_start 1932-09-01 00:20:00 (Thu)
60967987200, #    local_end 1932-12-31 00:00:00 (Sat)
1200,
1,
'+0020',
    ],
    [
60967986000, #    utc_start 1932-12-30 23:40:00 (Fri)
60989068800, #      utc_end 1933-09-01 00:00:00 (Fri)
60967986000, #  local_start 1932-12-30 23:40:00 (Fri)
60989068800, #    local_end 1933-09-01 00:00:00 (Fri)
0,
0,
'GMT',
    ],
    [
60989068800, #    utc_start 1933-09-01 00:00:00 (Fri)
60999522000, #      utc_end 1933-12-30 23:40:00 (Sat)
60989070000, #  local_start 1933-09-01 00:20:00 (Fri)
60999523200, #    local_end 1933-12-31 00:00:00 (Sun)
1200,
1,
'+0020',
    ],
    [
60999522000, #    utc_start 1933-12-30 23:40:00 (Sat)
61020604800, #      utc_end 1934-09-01 00:00:00 (Sat)
60999522000, #  local_start 1933-12-30 23:40:00 (Sat)
61020604800, #    local_end 1934-09-01 00:00:00 (Sat)
0,
0,
'GMT',
    ],
    [
61020604800, #    utc_start 1934-09-01 00:00:00 (Sat)
61031058000, #      utc_end 1934-12-30 23:40:00 (Sun)
61020606000, #  local_start 1934-09-01 00:20:00 (Sat)
61031059200, #    local_end 1934-12-31 00:00:00 (Mon)
1200,
1,
'+0020',
    ],
    [
61031058000, #    utc_start 1934-12-30 23:40:00 (Sun)
61052140800, #      utc_end 1935-09-01 00:00:00 (Sun)
61031058000, #  local_start 1934-12-30 23:40:00 (Sun)
61052140800, #    local_end 1935-09-01 00:00:00 (Sun)
0,
0,
'GMT',
    ],
    [
61052140800, #    utc_start 1935-09-01 00:00:00 (Sun)
61062594000, #      utc_end 1935-12-30 23:40:00 (Mon)
61052142000, #  local_start 1935-09-01 00:20:00 (Sun)
61062595200, #    local_end 1935-12-31 00:00:00 (Tue)
1200,
1,
'+0020',
    ],
    [
61062594000, #    utc_start 1935-12-30 23:40:00 (Mon)
61083763200, #      utc_end 1936-09-01 00:00:00 (Tue)
61062594000, #  local_start 1935-12-30 23:40:00 (Mon)
61083763200, #    local_end 1936-09-01 00:00:00 (Tue)
0,
0,
'GMT',
    ],
    [
61083763200, #    utc_start 1936-09-01 00:00:00 (Tue)
61094216400, #      utc_end 1936-12-30 23:40:00 (Wed)
61083764400, #  local_start 1936-09-01 00:20:00 (Tue)
61094217600, #    local_end 1936-12-31 00:00:00 (Thu)
1200,
1,
'+0020',
    ],
    [
61094216400, #    utc_start 1936-12-30 23:40:00 (Wed)
61115299200, #      utc_end 1937-09-01 00:00:00 (Wed)
61094216400, #  local_start 1936-12-30 23:40:00 (Wed)
61115299200, #    local_end 1937-09-01 00:00:00 (Wed)
0,
0,
'GMT',
    ],
    [
61115299200, #    utc_start 1937-09-01 00:00:00 (Wed)
61125752400, #      utc_end 1937-12-30 23:40:00 (Thu)
61115300400, #  local_start 1937-09-01 00:20:00 (Wed)
61125753600, #    local_end 1937-12-31 00:00:00 (Fri)
1200,
1,
'+0020',
    ],
    [
61125752400, #    utc_start 1937-12-30 23:40:00 (Thu)
61146835200, #      utc_end 1938-09-01 00:00:00 (Thu)
61125752400, #  local_start 1937-12-30 23:40:00 (Thu)
61146835200, #    local_end 1938-09-01 00:00:00 (Thu)
0,
0,
'GMT',
    ],
    [
61146835200, #    utc_start 1938-09-01 00:00:00 (Thu)
61157288400, #      utc_end 1938-12-30 23:40:00 (Fri)
61146836400, #  local_start 1938-09-01 00:20:00 (Thu)
61157289600, #    local_end 1938-12-31 00:00:00 (Sat)
1200,
1,
'+0020',
    ],
    [
61157288400, #    utc_start 1938-12-30 23:40:00 (Fri)
61178371200, #      utc_end 1939-09-01 00:00:00 (Fri)
61157288400, #  local_start 1938-12-30 23:40:00 (Fri)
61178371200, #    local_end 1939-09-01 00:00:00 (Fri)
0,
0,
'GMT',
    ],
    [
61178371200, #    utc_start 1939-09-01 00:00:00 (Fri)
61188824400, #      utc_end 1939-12-30 23:40:00 (Sat)
61178372400, #  local_start 1939-09-01 00:20:00 (Fri)
61188825600, #    local_end 1939-12-31 00:00:00 (Sun)
1200,
1,
'+0020',
    ],
    [
61188824400, #    utc_start 1939-12-30 23:40:00 (Sat)
61209993600, #      utc_end 1940-09-01 00:00:00 (Sun)
61188824400, #  local_start 1939-12-30 23:40:00 (Sat)
61209993600, #    local_end 1940-09-01 00:00:00 (Sun)
0,
0,
'GMT',
    ],
    [
61209993600, #    utc_start 1940-09-01 00:00:00 (Sun)
61220446800, #      utc_end 1940-12-30 23:40:00 (Mon)
61209994800, #  local_start 1940-09-01 00:20:00 (Sun)
61220448000, #    local_end 1940-12-31 00:00:00 (Tue)
1200,
1,
'+0020',
    ],
    [
61220446800, #    utc_start 1940-12-30 23:40:00 (Mon)
61241529600, #      utc_end 1941-09-01 00:00:00 (Mon)
61220446800, #  local_start 1940-12-30 23:40:00 (Mon)
61241529600, #    local_end 1941-09-01 00:00:00 (Mon)
0,
0,
'GMT',
    ],
    [
61241529600, #    utc_start 1941-09-01 00:00:00 (Mon)
61251982800, #      utc_end 1941-12-30 23:40:00 (Tue)
61241530800, #  local_start 1941-09-01 00:20:00 (Mon)
61251984000, #    local_end 1941-12-31 00:00:00 (Wed)
1200,
1,
'+0020',
    ],
    [
61251982800, #    utc_start 1941-12-30 23:40:00 (Tue)
61273065600, #      utc_end 1942-09-01 00:00:00 (Tue)
61251982800, #  local_start 1941-12-30 23:40:00 (Tue)
61273065600, #    local_end 1942-09-01 00:00:00 (Tue)
0,
0,
'GMT',
    ],
    [
61273065600, #    utc_start 1942-09-01 00:00:00 (Tue)
61283518800, #      utc_end 1942-12-30 23:40:00 (Wed)
61273066800, #  local_start 1942-09-01 00:20:00 (Tue)
61283520000, #    local_end 1942-12-31 00:00:00 (Thu)
1200,
1,
'+0020',
    ],
    [
61283518800, #    utc_start 1942-12-30 23:40:00 (Wed)
DateTime::TimeZone::INFINITY, #      utc_end
61283518800, #  local_start 1942-12-30 23:40:00 (Wed)
DateTime::TimeZone::INFINITY, #    local_end
0,
0,
'GMT',
    ],
];

sub olson_version {'2019a'}

sub has_dst_changes {23}

sub _max_year {2029}

sub _new_instance {
    return shift->_init( @_, spans => $spans );
}



1;

