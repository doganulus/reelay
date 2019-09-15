#include "iostream"
#include "vector"

#include "catch.hpp"

#include "reelay/intervals.hpp"
#include "reelay/monitors.hpp"

using time_t = int64_t;
using input_t = std::map<std::string, std::string>;
using interval = reelay::interval<time_t>;
using interval_set = reelay::interval_set<time_t>;

TEST_CASE( "Atoms" ) {
}

TEST_CASE( "Boolean Operations" ) {

    SECTION( "Disjunction" ) {

        std::vector<input_t> sequence = std::vector<input_t>();
       
        sequence.push_back( input_t{{"time", "10"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "30"}, {"p1", "0"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "40"}, {"p1", "1"}, {"p2", "1"}} );


        auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("p1 or p2");

        auto result = interval_set();

        for (const auto &s : sequence)
        {
            net1->update(s);
            result = result | net1->output();
        }

        auto expected = interval_set();
        expected.add(interval::left_open(10,40));

        CHECK(result == expected);
    }

    SECTION( "Conjuction" ) {

        std::vector<input_t> sequence = std::vector<input_t>();
       
        sequence.push_back( input_t{{"time", "10"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "30"}, {"p1", "0"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "40"}, {"p1", "1"}, {"p2", "1"}} );

        auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("p1 and p2");

        auto result = interval_set();

        for (const auto &s : sequence)
        {
            net1->update(s);
            result = result | net1->output();
        }

        auto expected = interval_set();
        expected.add(interval::left_open(30,40));

        CHECK(result == expected);
    }

    SECTION( "Implication" ) {

        std::vector<input_t> sequence = std::vector<input_t>();
       
        sequence.push_back( input_t{{"time", "10"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "30"}, {"p1", "0"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "40"}, {"p1", "1"}, {"p2", "1"}} );


        auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("p1 -> p2");

        auto result = interval_set();

        for (const auto &s : sequence)
        {
            net1->update(s);
            result = result | net1->output();
        }

        auto expected = interval_set();
        expected.add(interval::left_open(0,10));
        expected.add(interval::left_open(20,40));

        CHECK(result == expected);
    }

    SECTION( "Negation" ) {

        std::vector<input_t> sequence = std::vector<input_t>();
       
        sequence.push_back( input_t{{"time", "10"}, {"p1", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "1"}} );


        auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("not p1");

        auto result = interval_set();

        for (const auto &s : sequence)
        {
            net1->update(s);
            result = result | net1->output();
        }

        auto expected = interval_set();
        expected.add(interval::left_open(0,10));

        CHECK(result == expected);
    }

}

TEST_CASE( "Untimed Temporal Operations" ) {

    SECTION( "Past Always" ) {

        std::vector<input_t> sequence = std::vector<input_t>();
       
        sequence.push_back( input_t{{"time", "10"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "30"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "60"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "70"}, {"p1", "0"}, {"p2", "0"}} );

        auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("historically p1");

        auto result1 = interval_set();

        for (const auto &s : sequence)
        {
            net1->update(s);
            result1 = result1 | net1->output();
        }

        auto expected1 = interval_set();
        expected1.add(interval::left_open(0,60));

        CHECK(result1 == expected1);
    }

    SECTION( "Past Sometime" ) {

        std::vector<input_t> sequence = std::vector<input_t>();
       
        sequence.push_back( input_t{{"time", "10"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "0"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "30"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "60"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "70"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "90"}, {"p1", "1"}, {"p2", "0"}} );

        auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("once p1");

        auto result1 = interval_set();

        for (const auto &s : sequence)
        {
            net1->update(s);
            result1 = result1 | net1->output();
        }

        auto expected1 = interval_set();
        expected1.add(interval::left_open(60,90));

        CHECK(result1 == expected1);
    }

    SECTION( "Since" ) {

        std::vector<input_t> sequence = std::vector<input_t>();
       
        sequence.push_back( input_t{{"time", "10"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "0"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "30"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "60"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "70"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "90"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "115"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "156"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "175"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "201"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "248"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "287"}, {"p1", "0"}, {"p2", "0"}} );        
        sequence.push_back( input_t{{"time", "300"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "315"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "444"}, {"p1", "1"}, {"p2", "0"}} );

        auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("p1 since p2");

        auto result1 = interval_set();

        for (const auto &s : sequence)
        {
            net1->update(s);
            result1 = result1 | net1->output();
        }

        auto expected1 = interval_set();
        expected1.add(interval::left_open(115,201));

        CHECK(result1 == expected1);
    }
}

TEST_CASE( "Timed Temporal Operations" ) {

    SECTION("Timed Past Always")
    {

        std::vector<input_t> sequence = std::vector<input_t>();

        sequence.push_back( input_t{{"time", "10"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "30"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "60"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "70"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "90"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "115"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "156"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "175"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "201"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "248"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "287"}, {"p1", "1"}, {"p2", "0"}} );        
        sequence.push_back( input_t{{"time", "300"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "315"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "444"}, {"p1", "1"}, {"p2", "0"}} );

        //auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("not (p1 since [12:24] not p2)");
        auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("historically[12:24] p2");

        auto result1 = interval_set();

        for (const auto &s : sequence)
        {
            net1->update(s);
            result1 = result1 | net1->output();
        }

        auto expected1 = interval_set();
        expected1.add(interval::left_open(0,12));
        expected1.add(interval::left_open(139,168));
        expected1.add(interval::left_open(199,260));
        expected1.add(interval::left_open(311,312));

        CHECK(result1 == expected1);
    }
    SECTION("Timed Past Always Zero")
    {

        std::vector<input_t> sequence = std::vector<input_t>();

        sequence.push_back( input_t{{"time", "10"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "30"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "60"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "70"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "90"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "115"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "156"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "175"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "201"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "248"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "287"}, {"p1", "1"}, {"p2", "0"}} );        
        sequence.push_back( input_t{{"time", "300"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "315"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "444"}, {"p1", "1"}, {"p2", "0"}} );

        //auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("not (p1 since [12:24] not p2)");
        auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("historically[0:24] p2");

        auto result1 = interval_set();

        for (const auto &s : sequence)
        {
            net1->update(s);
            result1 = result1 | net1->output();
        }

        auto expected1 = interval_set();
        expected1.add(interval::left_open(139,156));
        expected1.add(interval::left_open(199,248));

        CHECK(result1 == expected1);
    }

    SECTION("Timed Past Always Inf")
    {

        std::vector<input_t> sequence = std::vector<input_t>();

        sequence.push_back( input_t{{"time", "10"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "30"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "60"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "70"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "90"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "115"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "156"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "175"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "201"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "248"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "287"}, {"p1", "1"}, {"p2", "0"}} );        
        sequence.push_back( input_t{{"time", "300"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "315"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "444"}, {"p1", "1"}, {"p2", "0"}} );

        //auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("not (p1 since [12:24] not p2)");
        auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("historically[12:] p2");

        auto result1 = interval_set();

        for (const auto &s : sequence)
        {
            net1->update(s);
            result1 = result1 | net1->output();
        }

        auto expected1 = interval_set();
        expected1.add(interval::left_open(0,12));

        CHECK(result1 == expected1);
    }

    SECTION("Timed Past Sometime")
    {

        std::vector<input_t> sequence = std::vector<input_t>();

        sequence.push_back( input_t{{"time", "10"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "30"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "60"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "70"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "90"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "115"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "156"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "175"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "201"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "248"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "287"}, {"p1", "1"}, {"p2", "0"}} );        
        sequence.push_back( input_t{{"time", "300"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "315"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "444"}, {"p1", "1"}, {"p2", "0"}} );

        // auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("p1 since [12:24] p2");
        auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("once[12:24] p2");

        auto result1 = interval_set();

        for (const auto &s : sequence)
        {
            net1->update(s);
            result1 = result1 | net1->output();
        }

        auto expected1 = interval_set();
        expected1.add(interval::left_open(22,44));
        expected1.add(interval::left_open(127,180));
        expected1.add(interval::left_open(187,272));
        expected1.add(interval::left_open(299,324));

        CHECK(result1 == expected1);
    }

    SECTION("Timed Past Sometime Zero")
    {

        std::vector<input_t> sequence = std::vector<input_t>();

        sequence.push_back( input_t{{"time", "10"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "30"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "60"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "70"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "90"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "115"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "156"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "175"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "201"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "248"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "287"}, {"p1", "1"}, {"p2", "0"}} );        
        sequence.push_back( input_t{{"time", "300"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "315"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "444"}, {"p1", "1"}, {"p2", "0"}} );

        // auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("p1 since [12:24] p2");
        auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("once[:24] p2");

        auto result1 = interval_set();

        for (const auto &s : sequence)
        {
            net1->update(s);
            result1 = result1 | net1->output();
        }

        auto expected1 = interval_set();
        expected1.add(interval::left_open(10,44));
        expected1.add(interval::left_open(115,180));
        expected1.add(interval::left_open(175,272));
        expected1.add(interval::left_open(287,324));

        CHECK(result1 == expected1);
    }

    SECTION("Timed Past Sometime Inf")
    {

        std::vector<input_t> sequence = std::vector<input_t>();

        sequence.push_back( input_t{{"time", "10"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "30"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "60"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "70"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "90"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "115"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "156"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "175"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "201"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "248"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "287"}, {"p1", "1"}, {"p2", "0"}} );        
        sequence.push_back( input_t{{"time", "300"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "315"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "444"}, {"p1", "1"}, {"p2", "0"}} );

        // auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("p1 since [12:24] p2");
        auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("once[12:] p2");

        auto result1 = interval_set();

        for (const auto &s : sequence)
        {
            net1->update(s);
            result1 = result1 | net1->output();
        }

        auto expected1 = interval_set();
        expected1.add(interval::left_open(22,444));

        CHECK(result1 == expected1);
    }


	SECTION("Timed Since")
	{

        std::vector<input_t> sequence = std::vector<input_t>();

        sequence.push_back( input_t{{"time", "10"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "0"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "30"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "60"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "70"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "90"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "115"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "156"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "175"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "201"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "248"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "287"}, {"p1", "0"}, {"p2", "0"}} );        
        sequence.push_back( input_t{{"time", "300"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "315"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "444"}, {"p1", "1"}, {"p2", "0"}} );

		auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("p1 since[18:24] p2");

		auto result1 = interval_set();

		for (const auto &s : sequence)
		{
			net1->update(s);
			result1 = result1 | net1->output();
		}

		auto expected1 = interval_set(
            interval::left_open(133,180)
            );

		CHECK(result1 == expected1);
	}

    SECTION("Timed Since Zero")
    {

        std::vector<input_t> sequence = std::vector<input_t>();

        sequence.push_back( input_t{{"time", "10"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "0"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "30"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "60"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "70"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "90"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "115"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "156"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "175"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "201"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "248"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "287"}, {"p1", "0"}, {"p2", "0"}} );        
        sequence.push_back( input_t{{"time", "300"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "315"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "444"}, {"p1", "1"}, {"p2", "0"}} );

        auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("p1 since[:24] p2");

        auto result1 = interval_set();

        for (const auto &s : sequence)
        {
            net1->update(s);
            result1 = result1 | net1->output();
        }

        auto expected1 = interval_set(
            interval::left_open(115,180)
            );

        CHECK(result1 == expected1);
    }

    SECTION("Timed Since")
    {

        std::vector<input_t> sequence = std::vector<input_t>();

        sequence.push_back( input_t{{"time", "10"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "20"}, {"p1", "0"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "30"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "60"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "70"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "90"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "115"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "156"}, {"p1", "1"}, {"p2", "1"}} );
        sequence.push_back( input_t{{"time", "175"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "201"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "248"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "287"}, {"p1", "0"}, {"p2", "0"}} );        
        sequence.push_back( input_t{{"time", "300"}, {"p1", "0"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "315"}, {"p1", "1"}, {"p2", "0"}} );
        sequence.push_back( input_t{{"time", "444"}, {"p1", "1"}, {"p2", "0"}} );

        auto net1 = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("p1 since[18:] p2");

        auto result1 = interval_set();

        for (const auto &s : sequence)
        {
            net1->update(s);
            result1 = result1 | net1->output();
        }

        auto expected1 = interval_set(
            interval::left_open(133,201)
            );

        CHECK(result1 == expected1);
    }
}
