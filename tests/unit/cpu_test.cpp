#include "tmbl/cpu/cpu.h"
#include <gmock/gmock.h>

namespace {
using namespace tmbl;

class MockedCPU : public cpu {
public:
  MOCK_METHOD(void, run, (), (override));
  MOCK_METHOD(void, enableDoubleSpeedMode, (), (override));
};

}

