#include "if.h"
#include "cache.h"
#include "controller.h"
#include "dram.h"
#include "instrDTO.h"
#include <catch2/catch_test_macros.hpp>

class IFPipeFixture
{
  public:
	IFPipeFixture()
	{
		Dram *d;

		d = new Dram(this->m_delay);
		// 0xC00 is a nop
		p = {0xC000, 0xC001, 0xC002, 0xC003};
		d->load(p);

		this->c = new Cache(d, this->c_delay);
		this->f = new IF(nullptr);
		this->ct = new Controller(this->f, this->c, true);
	}
	~IFPipeFixture()
	{
		delete this->ct;
		delete this->c;
	};

	/**
	 * Fetch a clean line not in cache.
	 */
	InstrDTO *fetch_through()
	{
		InstrDTO *r;
		int i;

		for (i = 0; i < this->m_delay + 1; ++i) {
			r = this->ct->advance(OK);
			// check response
			CHECK(r == nullptr);
		}
		return this->fetch_cache();
	}

	/**
	 * Fetch a line in cache.
	 */
	InstrDTO *fetch_cache()
	{
		InstrDTO *r;
		int i;

		for (i = 0; i < this->c_delay; ++i) {
			r = this->ct->advance(OK);
			// check response
			CHECK(r == nullptr);
		}
		r = this->ct->advance(OK);
		// check response
		CHECK(r != nullptr);
		return r;
	}

	int m_delay = 3;
	int c_delay = 1;
	std::vector<signed int> p;
	Cache *c;
	IF *f;
	Controller *ct;
};

TEST_CASE_METHOD(IFPipeFixture, "fetch returns single instuction", "[if_pipe]")
{
	InstrDTO *i;
	int expected_cycles;

	expected_cycles = this->m_delay + this->c_delay + 2;
	i = this->fetch_through();
	CHECK(i->get_time_of(FETCH) == expected_cycles);
	REQUIRE(i->get_instr_bits() == this->p[0]);

	delete i;
}

TEST_CASE_METHOD(IFPipeFixture, "fetch returns two instuctions", "[if_pipe]")
{
	InstrDTO *i;
	int expected_cycles;

	expected_cycles = this->m_delay + this->c_delay + 2;
	i = this->fetch_through();

	CHECK(i->get_time_of(FETCH) == expected_cycles);
	REQUIRE(i->get_instr_bits() == this->p[0]);
	delete i;

	expected_cycles += this->c_delay + 1;
	i = this->fetch_cache();

	CHECK(i->get_time_of(FETCH) == expected_cycles);
	REQUIRE(i->get_instr_bits() == this->p[1]);
	delete i;
}

TEST_CASE_METHOD(IFPipeFixture, "fetch waits with old instruction",
"[if_pipe]")
{
	InstrDTO *i;
	int j, expected_cycles, fetch_cycles;

	fetch_cycles = this->m_delay + this->c_delay + 2;
	expected_cycles = this->m_delay + (this->c_delay * 2) + 1;

	for (j = 0; j < this->m_delay + 1; ++j) {
		i = this->ct->advance(STALLED);
		// check response
		CHECK(i == nullptr);
	}
	for (j = 0; j < this->c_delay; ++j) {
		i = this->ct->advance(STALLED);
		// check response
		CHECK(i == nullptr);
	}
	for (j = 0; j < expected_cycles - fetch_cycles; ++j) {
		i = this->ct->advance(STALLED);
		// check response
		CHECK(i != nullptr);
	}

	i = this->ct->advance(OK);
	CHECK(i->get_time_of(FETCH) == expected_cycles);
	REQUIRE(i->get_instr_bits() == this->p[0]);

	delete i;
}
