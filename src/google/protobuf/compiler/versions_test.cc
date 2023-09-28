#include "google/protobuf/compiler/versions.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "absl/memory/memory.h"
#include "google/protobuf/test_textproto.h"

namespace {
using ::google::protobuf::EqualsProto;
using ::google::protobuf::compiler::internal::ParseProtobufVersion;
TEST(GetProtobufVersionTest, EmptyVersionString) {
  EXPECT_DEATH({ ParseProtobufVersion(""); }, "version cannot be empty");
}

TEST(GetProtobufVersionTest, MissingVersionSegment) {
  EXPECT_DEATH({ ParseProtobufVersion("3-rc1"); },
               "version must at least have major and micro numbers.");
}

TEST(GetProtobufVersionTest, FullVersionWithRCSuffix) {
  auto v = absl::WrapUnique(ParseProtobufVersion("3.26.2-rc1"));
  EXPECT_THAT(*v,
              EqualsProto(R"pb(major: 3 minor: 26 patch: 2 suffix: "-rc1")pb"));
}

TEST(GetProtobufVersionTest, FullVersionWithoutSuffix) {
  auto v = absl::WrapUnique(ParseProtobufVersion("3.26.2"));
  EXPECT_THAT(*v, EqualsProto(R"pb(major: 3 minor: 26 patch: 2)pb"));
}

TEST(GetProtobufVersionTest, VersionWithDevSuffixMissingPatch) {
  auto v = absl::WrapUnique(ParseProtobufVersion("3.26-dev"));
  EXPECT_THAT(*v, EqualsProto(R"pb(major: 3 minor: 26 suffix: "-dev")pb"));
}
}  // namespace
